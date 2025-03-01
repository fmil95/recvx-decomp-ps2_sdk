[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                   Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                      All Rights Reserved

ベーシックサンプル basic3d VU1版
================================

<サンプルの解説>
	EE CoreやVU0で実装していた透視変換やパケット作成のプロセスを、VU1マイク		ロモードで高速に処理する例を示します。その際に必要なVUMem1のダブルバッフ	ァ処理などのテクニックも実装し、より高速なジオメトリ処理を実現する基礎を	提供します。

<ファイル>
	main.c		メインプログラム
	mathfunc.c	透視変換等の行列演算関数(CPUcore版)
	mathfunc.h	ヘッダファイル
	cube.dsm	オブジェクトデータ(立方体)(VU1用パケット形式)
	torus.dsm	オブジェクトデータ(トーラス データ分割)
			(VU1用パケット形式)
	torus1.dsm	オブジェクトデータ(トーラス データ分割)
			(VU1用パケット形式)
	flower.dsm	テクスチャデータ
	basic.vsm	VU1マイクロモードの実行命令ファイル
	vu1dump.c	VU1ローカルメモリのダンプ関数

<コンパイル方法>
	% make

<起動方法>
	% make run

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run main.elf

<仕様>
	テクスチャ付きTriangle Stripのキューブを表示する。
	テクスチャ付きTriangleのトーラスを表示する。
	(selectボタンで切り替え)

	Zバッファは24bit
	光源は3つ
	テクスチャは32bit

  <コントローラの操作方法>
	↑↓(方向キー) : オブジェクトのx軸回転
	←→           : オブジェクトのy軸回転
	L1L2ボタン     : オブジェクトのz軸回転
	□○ボタン     : ワールド座標系に対するカメラのy軸回転
	△×ボタン     : ワールド座標系に対するカメラのx軸回転
	R1R2ボタン     : カメラの前進後退回転
	SELECTボタン   : オブジェクト切り替え

<処理概略>
	1. テクスチャをロード
	2. パッドオープン
	3. マトリクス、光源マトリクスをVU1へ渡すパケット中にセット
	4. VU1へのDMA kick
	5. 2に戻る

	VU1マイクロプログラムでは以下のような処理を行っています。
	1. 回転 x 透視変換マトリクスを設定
	2. 各種パラメータ（マトリクス等）をVU1レジスタにロード
	3. 法線ベクトル、補間する２つの座標、ST値、色をロード
	4. 座標補間、透視変換、光源計算をして結果をVU1Memにストア
	5. 3. に戻る。頂点数回繰り返す。
	6. ストアされたデータ(Gif Packet)をGSへXGKICK

	VU1Mem上ではダブルバッファリングを行います。


データフロー
--------------------------------------------------------------------------

<初期化過程>
メインRAM                          GS内蔵RAM(4M)
  テクスチャデータ ----------------> テクスチャデータ展開
  オブジェクトデータ展開
  透視変換行列等の展開

<実行処理過程>
メインRAM                            VUMem1(16K)
  オブジェクトデータ ----------------> GS転送用パケットデータ展開
  (各種マトリクス＆       DMA転送      (ジオメトリ演算，ダブルバッファ)
   ジオメトリ前データ)                 (プリミティブデータ(STQ,RGBA,XYZF))
                                                    |
                                                    |
GS内蔵RAM(4M) <-------------------------------------+
 フレーム＆Zバッファ書き込み                 GIFにデータKick
 (レンダリング)

プログラムの説明(詳細)
--------------------------------------------------------------------------

<変数・構造体・関数>　（mathfunc.cの関数を除く）

        My_dma_start            :VU1へ送るパケット群の先頭要素
        My_dma_next             :VU1へ送る2つめのパケットの先頭要素
        My_matrix               :パケット中のlocal_screenマトリクス
        My_rot_trans_matrix     :パケット中のlocal_worldマトリクス
        My_light_matrix         :パケット中のlocal_lightマトリクス
        My_cube_start           :cubeパケットデータの先頭要素
        My_torus_start          :torusパケットデータの先頭要素
        My_texture1             :テクスチャデータの先頭

        TexEnv          :sceGsTexEnv構造体を転送するパケット作成の構造体

        camera_p        :カメラ位置
        camera_zd       :カメラの視線方向ベクトル
        camera_yd       :カメラの下方向ベクトル
        camera_rot      :カメラの回転用ベクトル

        light0          :ライト0
        light1          :ライト1
        light2          :ライト2
        color0          :ライト0の色
        color1          :ライト1の色
        color2          :ライト2の色

        ambient         :環境光

        obj_trans       :オブジェクトの移動ベクトル
        obj_rot         :オブジェクトの回転ベクトル

        local_world     :オブジェクトLocal座標をWorld座標系に変換する行列
        world_view      :World座標系をView座標系に変換する行列
        view_screen     :View座標系をScreen座標系に変換する行列
        local_screen    :Local座標をScreen座標に変換する行列(上３行列の積)

        normal_light    :World座標系における頂点の法線ベクトルとライトの
                         方向ベクトルの内積を取るための行列
                         (ライトの方向ベクトルがセットされる)
        local_light     :normal_light行列にlocal_world行列を掛けて、
                         Local座標系における頂点の法線ベクトルと
                         ライトの方向ベクトルの内積を取れるようにする行列
        light_color     :local_light行列と頂点の法線ベクトルの積により
                         生成された内積ベクトルに各ライトの色(3色＋環境光)
                         を掛けて和をとる行列
        local_color     :local_lightとlight_colorの積を取った行列

        work            :一時的な作業のための行列

        sceGsDBuff      db      :ダブルバッファを実装するための構造体
        scePadData      pad     :パッドデータを読み込むための構造体
        sceGsLoadImage  gs_limage:テクスチャをGSローカルメモリに転送するた
                                 めの構造体
        TexEnv          texenv  :テクスチャデータを転送するためのパケット
                                 を作成する構造体

        void SetVu1PacketMatrix(void)
                        :VU1に送るパケット内にマトリクスをセットする関数

<main関数内の変数>

        fd              :パッドをopenの際返り値を受け取るファイル記述子
        frame           :ダブルバッファでフリップを行うためのスイッチ
        delta           :回転角を設定するための変数
        obj_switch      :オブジェクトを切り替えるためのトグルスイッチ
        toggle          :selectボタンの読み込みをトグルで制御する変数

<mathfunc.cでサポートする変数・関数>

        sceSamp0IVECTOR[4]      :128bit境界で配置するint型の配列(要素4)
        sceSamp0FVECTOR[4]      :128bit境界で配置するfloat型の配列(要素4)
        sceSamp0FMATRIX[4][4]   :128bit境界で配置するfloat型の配列(要素4x4)

　関数についての詳しい説明はmathfunc.cにありますのでそちらを参照してくださ
い。mathfunc.cで実装される関数のほとんどはVU0マクロで実装されており、libvu0
により提供しています。
　VU1版では、行列の計算にCoreで実行するsceSamp0***関数を用いていますが、VU0
版同様に関数を置き換えれば、さらに高速な処理が期待できます。
　なお、Core版とVU0版/VU1版では行列の配列形式が異なりますので注意してくださ
い。Core版では一般的な配列ですが、VU0版/VU1版ではVUの演算機能を有効に利用す
るため、転地形式になっています。


プログラム解説（初期化部分）
---------------------------------------------------------------------------

・オブジェクトデータの読み込み
	VU1版では、オブジェクトデータはコンパイルの時点でリンクされメモリに
	展開されています．*.dsm形式のファイルはVU1に直接送れるパケット形式
	で、そのままMy_dma_startポインタが使えます。

・デバイスの初期化
    sceGsResetPath();//データパスデバイスの初期化(VIF1,VU1,GIF)

	プログラムの先頭で、使用するすべてのデバイスの初期化を行います。
	初期化を行っていないデバイスを利用した場合、期待した動作をしないこ
	とがありますので、必ず実行してください。

・描画環境の設定
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
        //GSを初期化する
    sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, 
             SCREEN_HEIGHT, SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);
        //ダブルバッファの設定をおこなう
    *(u_long *)&db.clear0.rgbaq = SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10,
                                                0x80, 0x3f800000);
    *(u_long *)&db.clear1.rgbaq = SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10,
                                                0x80, 0x3f800000);

	各デバイスの初期化が終わった後に、GSの描画環境の設定を行います。
	この設定を行わないと画面上への表示ができません。また、VSyncで同期
	をとっているコントローラの設定も行えませんので、デバイスの初期化直
	後にこの設定を一度は行うようにしてください。

・テクスチャ用のイメージ転送
    sceGsSetDefLoadImage(&pTex,13340,IMAGE_SIZE / 64,SCE_GS_PSMCT32,
                        0, 0,IMAGE_SIZE, IMAGE_SIZE);
                        //LoadImage情報の設定
    FlushCache(0);
    sceGsExecLoadImage(&pTex, (u_long128 *)My_texture1);
                        //LoadImageの実行
    sceGsSyncPath(0, 0);//データ転送の終了待ち

	テクスチャ用のイメージをGSローカルメモりに転送するためにLoadImage
	の情報を設定します。その後、sceGsExecLoadImage()を実行することで、
	指定されたイメージがGSのローカルメモりに転送されます。この関数は内
	部でDMAを利用しますので、転送前にFlushCache(0)でDキャッシュとメイ
	ンメモり間の整合性を取っています。最後に、sceGsSyncPath()でデータ
	転送の終了待ちをおこないます。

・テクスチャの環境設定用パケットの生成
    FlushCache(0);
    // --- set texture env 1 ---
    sceGsSetDefTexEnv(&texenv.gs_tex, 0, 13340, 
                        IMAGE_SIZE / 64, SCE_GS_PSMCT32, 
                          8, 8, 0, 0, 0, 0, 1);
    SCE_GIF_CLEAR_TAG(&texenv.giftag); 
    texenv.giftag.NLOOP = 9;
    texenv.giftag.EOP = 1;
    texenv.giftag.NREG = 1;
    texenv.giftag.REGS0 = 0xe;
    sceGsSetDefAlphaEnv(&texenv.gs_alpha, 0);
    *(u_long *) &texenv.gs_alpha.alpha1 
                = SCE_GS_SET_ALPHA(0, 2, 0, 1, 0);
    *(u_long *) &texenv.gs_test 
                =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 3, 0x3);
    texenv.gs_test1addr = (long)SCE_GS_TEST_1;
    *(u_long*)&texenv.gs_tex.clamp1 
                = SCE_GS_SET_CLAMP(0, 0, 0, 0, 0, 0); 

	sceGsSetDefTexEnv()を用いて、テクスチャ関連のGS汎用レジスタTEX0_1, 
	TEX0_1, CLAMP_1の設定を行うパケットを作成します。

・テクスチャの環境設定用パケットの転送
    FlushCache(0);
    sceGsPutDrawEnv(&texenv.giftag);  //テクスチャ設定データの転送
    sceGsSyncPath(0, 0);

	生成されたテクスチャ環境設定用のパケットを、sceGsPutDrawEnv()関数
	を用いて転送しています。

・コントローラの初期化
    fd = open("pad:0", 0);

	1P側のコントローラを使うための初期化を行います。これ以後、read()で
	指定したバッファにデータを読み込めます。コントローラのデータは
	VSync毎に更新されます。


プログラム解説（メインループ）
--------------------------------------------------------------------------
　以下、メインループの中で行われる、コントローラの情報取得、マトリクスの生
成、パケット生成(座標変換、透視変換、UV->STQ変換、光源計算)、について説明し
ます。

・View-Screenマトリクスの生成
    sceSamp0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
                2048.0f, 2048.0f,1.0f, 16777215.0f,1.0f,65536.0f);

	sceSamp0ViewScreenMatrix()を用いてView-Screenマトリクスを求めます。
	ここでは、視点から投影スクリーンまでの距離を512、画面のアスペクト
	比を1:0.47、画面の中心をGdPrimitive座標系で(2048,2048)、Zバッファ
	の値を1.0-16777215.0、Zの値を1.0-65536.0としたView-Screenマトリク
	スを求めています。

・コントローラの情報取得
    // --- read pad ---
    read(fd, &pad, sizeof(pad));

    // --- object rotate & change view point ---
    if(pad.button & SCE_PADLdown){ 
                obj_rot[0] += deltta;
                if(obj_rot[0]>PI) obj_rot[0] -= 2.0f*PI; }
    if(pad.button & SCE_PADLup){ 
                obj_rot[0] -= delta;
                if(obj_rot[0]<-PI) obj_rot[0] += 2.0f*PI; }
    if(pad.button & SCE_PADLright){ 
                obj_rot[1] += delta;
                if(obj_rot[1]>PI) obj_rot[1] -= 2.0f*PI; }
    if(pad.button & SCE_PADLleft){ 
                obj_rot[1] -= delta;
                if(obj_rot[1]<-PI) obj_rot[1] += 2.0f*PI; }
    if(pad.button & SCE_PADL1){
                obj_rot[2] += delta;
                if(obj_rot[2]>PI) obj_rot[2] -= 2.0f*PI; }
    if(pad.button & SCE_PADL2){
                obj_rot[2] -= delta;
                if(obj_rot[2]<-PI) obj_rot[2] += 2.0f*PI; }
    if(pad.button & SCE_PADRdown){
                camera_rot[0] += delta;
                if(camera_rot[0]>PI*0.4) camera_rot[0] = 0.4f*PI; }
    if(pad.button & SCE_PADRup){
                camera_rot[0] -= delta;
                if(camera_rot[0]<-PI*0.4) camera_rot[0] = -0.4f*PI; }
    if(pad.button & SCE_PADRright){
                camera_rot[1] += delta; 
                if(camera_rot[1]>PI) camera_rot[1] -= 2.0f*PI; }
    if(pad.button & SCE_PADRleft){
                camera_rot[1] -= delta;
                if(camera_rot[1]<PI) camera_rot[1] += 2.0f*PI; }
    if(pad.button & SCE_PADR1){
                camera_p[2] -= delta*5;
                if(camera_p[2]<-100) camera_p[2] = -100; }
    if(pad.button & SCE_PADR2){
                camera_p[2] += delta*5;
                if(camera_p[2]>-10) camera_p[2] = -10; }
    if(!toggle && (pad.button & SCE_PADselect)){
         obj_switch = (++obj_switch)&0x01; toggle = 1;
    }else if(!(pad.button & SCE_PADselect)){toggle = 0;}

	コントローラのボタン情報は、read()を用いて取得できます。
	ここでは得られたデータをもとに、オブジェクトとカメラの回転角、移動
	量を更新しています。コントローラの左手側がオブジェクト、右手側のボ
	タンがカメラの操作となっています。また、selectボタンでオブジェクト
	の切り替えを行います。

・Local-Worldマトリクス(回転のみ)の生成
    sceSamp0UnitMatrix(work);                   //単位行列
    sceSamp0RotMatrix(local_world, work, rot);  //回転行列

	Local-Worldマトリクスは、Local座標で保持されているオブジェクト毎に
	求める必要があります。回転、平行移動といったアフィン変換が用いられ
	ます。このサンプルでは、回転マトリクスおよび平行移動マトリクスを計
	算して、それらを乗算したものをLocal-Worldマトリクスとして用いてい
	ます。
	ただしこのサンプルでは、3行目の平行移動(obj_trans)を設定する前に
	Local_Lightマトリクスを設定する必要があります。平行移動を加味した
	行列を用いてLocal_Lightマトリクスを生成すると、環境光の計算が正し
	く行えません。これは平行移動、環境光いずれも行列の第4列を用いて計
	算するためです。したがって回転行列を設定した直後に、Local_Lightマ
	トリクスを作ってください。
	もうひとつ注意点として、obj_transの第4要素には必ずゼロを設定してく
	ださい。マトリクスの[4,4]要素には非常にクリティカルな値がセットさ
	れており、ゼロ以外の値はこれを変えてしまうことになるためです。

・ローカルライトマトリクスの生成
    sceSamp0NormalLightMatrix( normal_light, light0,light1,light2);
    sceSamp0MulMatrix(local_light, normal_light,local_world);

	sceSamp0NormalLightMatrix()を用いて、3つの光源からNormal-Lightマト
	リクスを求めます。さらに、Local-Worldマトリクスと掛け合わせて、
	Local-Lightマトリクスを生成します。前述したように、このとき掛ける
        Local_Worldマトリクスは、obj_transがセットされていてはいけません。

・ライトカラーマトリクスの生成
    sceSamp0LightColorMatrix(light_color, color0, color1, color2, ambient);

	光源色と環境色からsceSamp0LightColorMatrix()を用いてLight_Colorマ
	トリクスを求めます。光源計算時に、各光源の影響色が求まった後に、
	Light_Colorマトリックスを乗算して、最終的なスクリーン上での頂点色
	が決定します。

・World-Viewマトリックスの生成
    sceSamp0RotCameraMatrix(world_view, camera_p, camera_zd, camera_yd,
                                                         camera_rot);

	この関数sceSamp0RotCameraMatrix()は、内部でsceSamp0CameraMatrix()
	を呼び出しています。このsceSamp0CameraMatrix()用いて、World-Viewマ
	トリクスを求めます。View座標系は視点(eye)を原点(0,0,0)とし、視線方
	向がZ+、右手方向がX+、足方向がY+となっています(右手座標系）。ここ
	では、Worldに対して視点camera_pを(0,0,-25)、視線camera_zdを(0,0,1)、
	垂直方向camera_ydを(0,1,0)とし、これを基準にWorld-Viewマトリクスを
	生成します。
        sceSamp0RotCameraMatrix()関数内部ではこれらのベクトルをあらかじめ
        camera_rotによって回転し、その結果をsceSamp0CameraMatrix()に渡すこ
	とで、カメラの回転を実現します。

・Local-Worldマトリクス(回転＆平行移動)の生成
    sceSamp0TransMatrix(local_world, local_world, obj_trans);
        //回転＆平行移動

	Local_Lightのマトリクスの生成が終了したので、Local_Worldマトリクス
	に平行移動要素(obj_trans)を加えて、真のLocal_Worldマトリクスを生成
	します。

・Local-Screenマトリクスの生成
    sceSamp0MulMatrix(world_screen, view_screen, world_view);
    sceSamp0MulMatrix(local_screen, world_screen, local_world);

	ここまでに求められたView-Screenマトリクス、World-Viewマトリクスを
	用いて、まずWorld-Screenマトリクスを求めます。さらにLocal-Worldマ
	トリクスを掛け合わせて、Local-Screenマトリクスが求められます。
	この時点で掛け合わせるLocal-Worldマトリクスは、回転と平行移動の要
	素が正しくセットされたものである必要があります。

・VUMem1に転送するパケットに値をセット
    SetVu1PacketMatrix();
    if(obj_switch==0) *((&My_dma_next)+1) = (u_int)(&My_cube_start);
    else if(obj_switch==1) *((&My_dma_next)+1) = (u_int)(&My_torus_start);

	SetVu1PacketMatrix()関数で、VU1に転送するマトリクスに値をセットし
	ます。また、オブジェクトを切り替えるため、パケット内のDMAtagに、
	VU1に転送すべきオブジェクトデータのアドレスをセットしています。

・VUMem1にパケットを転送
    // --- kick dma to draw object ---
    *D1_QWC  = 0x00;
    *D1_TADR = (u_int)&My_dma_start & 0x0fffffff;
    *D_STAT = 2;
    FlushCache(0);
    *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
    sceGsSyncPath(0, 0);

	ここではチャネル1を用いてパケットを転送するため、直接レジスタに値
	をセットしています。DMAtagを転送するので、D1_QWCにはゼロをセットし、
	D1_TADRによりパケット転送アドレスをMy_dma_startにします。
	D_STATによりチャネル1のステータスビットをセットし、転送前にキャッ
	シュをフラッシュしてメモリの整合性をあわせます。その後D1_CHCRのチ
	ャネル制御レジスタに値をセットして、DMA転送をスタートします。
	このときVU1に送るデータには、VU1マイクロコードとVIFコード、オブジ
	ェクトデータが含まれ、これらは一度のDMA転送で処理されます。この詳
	しい処理については、後の「VU1・VIF1データ処理」にて解説します。

・描画環境の切り替え、V-Blank期間の終了待ち
    sceGsSetHalfOffset((frame&1) ? &db.draw1 : &db.draw0, 2048,2048,odev);
    odev = !sceGsSyncV(0);

	フィールド毎に動作環境を切り替えをし、V-Blank期間の開始を待ちます。
	これ以降はループの先頭に戻り、同じ作業を繰り返します。


VU1・VIF1データ処理
--------------------------------------------------------------------------
　VU1に送るデータの流れについて簡単に説明します。

	メインRAM ---> DMA ---> VIF1 ---> MicroMem1 or VUMem1

　簡単にはこのような流れでデータが送られるわけですが、その際必要な処理過程
について解説します。

　まずDMAを介してデータを転送するため、DMA制御を行います。DMA制御はDMAtagを
送ることで行います。このDMAtagの追加はメインループと*.dsmファイルの内部で生
成し追加しています(*.dsmファイル内の「DMAcnt *」「.EndDmaData」など)。

    if(obj_switch==0) *((&My_dma_next)+1) = (u_int)(&My_cube_start);
    else if(obj_switch==1) *((&My_dma_next)+1) = (u_int)(&My_torus_start);

　たとえばこの処理では、オブジェクトの切り替え処理のため、DMAtagにnextを指
定しておき、DMAtag内のアドレスを切り替えることで転送オブジェクトを交換して
います。

　その後データはVIF1を通過します。このVIF1にも多様な機能が実装されており、
そこからVU1を制御することが可能です。VIFを制御するコードはVIFcodeで実装さ
れ、パケットに組み込むことができます。本サンプルでは *.dsmファイル内で使用
しています。

	例:

	MPG    :マイクロプログラムをロード(データをuMemに送る)
	-- data --
	.EndMPG

	unpack 4, 4, V4_32, 0, *    :後続データを展開・VUMemに書き込み
	-- data --
	.EndUnpack

　このサンプルでは、マイクロコードとVIFcodeの機能によって,次のような手順で
ダブルバッファ処理を実現しています。

	MSCAL 0       :マイクロプログラム実行(アドレス0スタート)
	BASE 0        :ダブルバッファの一方の先頭アドレスを0に指定。
                       (vif1 base レジスタ設定)
	OFFSET 512    :ダブルバッファの他方の先頭アドレスを512に指定
                       (vif1 offset レジスタ設定)

	unpack ......
	-- object data --
	.EndUnpack
	MSCNT     :マイクロコードの実行再開(描画処理)

　マイクロコードの処理の流れを簡単に示します。

        前処理 --+-> 透視変換・光源処理(GIFパケット生成・転送) 
                 |                              |
                 |               |              |
                 +<----<----<----|<----<----<---+
                         restart | stop
                               MSCNT
                          (switch buffer)

　マイクロコードは前処理もしくは一回のループ処理をした後、次のMSCNT命令が
くるまで待機しています。そして、次のバッファに値がセットされ、MSCNT命令が
実行されると実行再開（もしくはマイクロプログラムの処理停止までMSCNTがスト
ール）し、データを処理します。
　MSCNT命令が実行されるとき、オフセットアドレスが切り替えられるため、処理
メモリがスイッチしてダブルバッファ処理が可能となります。


Basic3d VU1 マイクロコード
--------------------------------------------------------------------------

<動作の概略>
	1.ワールド座標系での座標変換 
	2.透視変換 
	3.頂点の法線ベクトルと平行光源によるライティング 
	4.パースペクティブコレクトされたテクスチャマッピング 


<VUMem1上のデータ配置>

   アドレス     ストアされたデータ              説明
  ---------------------------------------------------------------------
     0 - 3      - 透視変換マトリクス              モデル全体で共用される
     4 - 7      - 座標変換マトリクス              マトリクスデータ等
     8 - 11     - 平行光源ベクトル
    12 - 15     - 光源色
  ----------------------------------------------------------------------
    0 - 511     [ソースデータ]
                - GIFtag
                - 頂点座標
                - 法線ベクトル
                - 頂点色
                - テクスチャ座標
                                                VU1のダブルバッファ領域 0
                [GIFへXGKICKされるパケット]
                - GIFtag
                - ST
                - RGBAQ
                - XYZF2
  -----------------------------------------------------------------------
   512 - 1023
                [ソースデータ]
                - GIFtag
                - 頂点座標
                - 法線ベクトル
                - 頂点色
                - テクスチャ座標
                                                VU1のダブルバッファ領域 1
                [GIFへXGKICKされるパケット]
                - GIFtag
                - ST
                - RGBAQ
                - XYZF2

  ---------------------------------------------------------------------

<マイクロコード>

;
;
; vu1 micro code for basic sample
;
; [主なレジスタの意味]
;
; VF01 - VF04 : 透視変換マトリクス x   ワールド座標系での回転・移動マトリクス
; VF05 - VF08 : 平行光源の方向ベクトル
; VF09 - VF11 : 各光源の色
; VF19 : gif tag 
; VF20 : 処理する頂点座標
; VF21 : 処理する頂点での法線ベクトル
; VF22 : 処理する頂点のテクスチャ座標
; VF23 : 処理する頂点の色
;
;
; VI02 : VUMEMからGIFへXGKICKで送られるデータの先頭（gif tagが置かれる）
; VI03 : VUMEM上にある処理する頂点データへのリードポインタ
; VI04 : VUMEM上にある処理する法線ベクトルへのリードポインタ
; VI05 : VUMEM上にある処理するテクスチャ座標へのリードポインタ
; VI06 : VUMEM上にある処理する頂点の色へのリードポインタ
; VI07 : GIFに送るRGBAQデータへのストアポインタ
; VI08 : GIFに送るSTデータへのストアポインタ
; VI09 : GIFに送るXYZF2データへのストアポインタ
;
; VI10 : 処理する頂点数（ループカウンタ）
;
;
; ---------------------------------------------------------------------


NOP IADDIU VI01, VI00, 0
NOP IADDIU VI02, VI00, 4
NOP IADDIU VI03, VI00, 16 
NOP BAL VI15, MulMatrix ; 透視変換マトリクス x   ワールド座標系での回転・移動マトリクスを計算する
NOP NOP

NOP IADDIU VI01, VI00, 8 
NOP LQI VF09, (VI01++) ; 平行光源ベクトルのロード (8)
NOP LQI VF10, (VI01++)
NOP LQI VF11, (VI01++)
NOP LQI VF12, (VI01++)
NOP LQI VF05, (VI01++) ; 各光源の色のロード (12) 
NOP LQI VF06, (VI01++)
NOP LQI VF07, (VI01++)
NOP LQI VF08, (VI01++)
NOP LQI VF01, (VI01++) ; 透視変換マトリクス x   ワールド座標系での回転・移動マトリクスのロード (16)
NOP LQI VF02, (VI01++)
NOP LQI VF03, (VI01++)
NOP LQI VF04, (VI01++)

NOP[E] NOP
NOP NOP
START0:
NOP XTOP VI01                 ; ダブルバッファリングのために
                              ; VIFのTOPレジスタの値読み出し
NOP ILWR.x VI10, (VI01)x      ; GIFtagからnloopの値を読み出す
NOP IADDIU VI11, VI00, 0x7fff ; ループカウンタ（頂点数）の値を得るためにマスクを設定
NOP IAND VI10, VI11, VI10     ; マスクして VI10 を頂点数の入ったループカウンタに設定
NOP LQ VF19, 0(VI01)          ; gif tagの読み込み
NOP IADDIU VI03, VI01, 1      ; 読み込む頂点データへのポインタ
NOP IADD VI04, VI03, VI10     ; 読み込む法線ベクトルへのポインタ
NOP IADD VI05, VI04, VI10     ; 読み込む頂点色データへのポインタ
NOP IADD VI06, VI05, VI10     ; 読み込むテクスチャ座標データへのポインタ
NOP IADD VI02, VI06, VI10     ; XGKICKする位置のポインタ
NOP SQ VF19, 0(VI02)          ; giftagをストア
NOP IADDIU VI07, VI02, 2      ; 計算されたRGBAQをストアする位置へのポインタ
NOP IADDIU VI08, VI02, 1      ; 計算されたSTをストアする位置へのポインタ
NOP IADDIU VI09, VI02, 3      ; 計算されたXYZF2をストアする位置へのポインタ

LOOP:
NOP LQI VF20, (VI03++)        ; 頂点データの読み込み
NOP LQI VF21, (VI04++)        ; 法線ベクトルの読み込み
NOP LQI VF22, (VI05++)        ; 頂点色の読み込み
NOP LQI VF23, (VI06++)        ; テクスチャ座標の読み込み

; --- 座標変換 &透視変換 ---
MULAx.xyzw ACC, VF01, VF20x NOP   ; 座標変換と透視変換を行う 
MADDAy.xyzw ACC, VF02, VF20y NOP 
MADDAz.xyzw ACC, VF03, VF20z NOP 
MADDw.xyzw VF25, VF04, VF20w NOP 
NOP NOP
NOP NOP
NOP NOP
NOP DIV Q, VF00w, VF25w          ; 1/wを求める

; --- 平行光源ベクトルと法線ベクトルとの内積 ---
MULAx.xyzw ACC, VF05, VF21x NOP   ; 内積
MADDAy.xyzw ACC, VF06, VF21y NOP 
MADDAz.xyzw ACC, VF07, VF21z NOP 
MADDw.xyzw VF13, VF08, VF21w NOP 
NOP NOP
NOP NOP
NOP NOP
MAXx.xyzw VF13, VF13, VF00x NOP ; 0.0f未満の値を0.0fにする

; --- 1/w をかけて、ストアする ---
MULq VF28, VF25, Q NOP       ; 1/w をかける
NOP NOP
NOP NOP
NOP NOP
FTOI4 VF27, VF28 NOP         ; 固定小数点に変換
NOP NOP
NOP NOP
NOP NOP
NOP SQ VF27, 0(VI09)         ; XYZF2をストア(unpacked mode)
NOP IADDIU VI09, VI09, 3     ; ストアポインタをインクリメント

; stを読み込んでQをかけてストアする(for perspective correction)
MULq.xyz VF27, VF23, Q NOP   ; STに1/wをかけ、Qに1/wを設定する
NOP NOP
NOP NOP
NOP NOP
NOP SQ VF27, 0(VI08)         ; STQをストアする(unpacked mode)
NOP IADDIU VI08, VI08, 3     ; ストアポインタをインクリメントする
NOP NOP
NOP NOP

; --- 照度計算されたマテリアルの色を求めストアする ---
MULAx.xyzw ACC, VF09, VF13x NOP    ; 内積に光源色をかける
MADDAy.xyzw ACC, VF10, VF13y NOP
MADDAz.xyzw ACC, VF11, VF13z NOP
MADDw.xyzw VF14, VF12, VF13w NOP
NOP NOP
NOP NOP
NOP NOP
MUL.xyz VF27, VF22, VF14 NOP       ; 計算されたライトの色をマテリアル色にかける
NOP NOP
NOP NOP
NOP NOP
FTOI0 VF26, VF27 NOP               ; 整数に変換 
NOP NOP
NOP NOP
NOP NOP
NOP SQ VF26, 0(VI07)               ; RGBAQをストアする(unpacked mode)
NOP IADDIU VI07, VI07, 3           ; ストアポインタをインクリメントする

; --- ループ ---
NOP IADDI VI10, VI10, -1           ; ループカウンタをデクリメント
NOP IBNE VI10, VI00, LOOP          ; ループを繰り返し

; --- 計算結果をGIFに送り(XGKICK)、マイクロの実行を停止 ---
NOP NOP 
NOP XGKICK VI02 ; XGKICK 
NOP[E] NOP      ; マイクロの停止
NOP NOP 
NOP B START0    ; 次のマイクロ実行位置へジャンプ
NOP NOP


; -------------------------------------------------------------------
; MulMatrix マトリクスの掛け算 
; VI01 : ソースマトリクス1のアドレス
; VI02 : ソースマトリクス1のアドレス
; VI03 : 計算されたマトリクスをストアするアドレス
;

MulMatrix:
NOP LQI.xyzw VF08, (VI02++)
NOP LQI.xyzw VF04, (VI01++)
NOP LQI.xyzw VF05, (VI01++)
NOP LQI.xyzw VF06, (VI01++)
NOP LQI.xyzw VF07, (VI01++)
MULAx.xyzw ACC, VF04, VF08x LQI.xyzw VF09, (VI02++)
MADDAy.xyzw ACC, VF05, VF08y NOP
MADDAz.xyzw ACC, VF06, VF08z NOP
MADDw.xyzw VF12, VF07, VF08w NOP
MULAx.xyzw ACC, VF04, VF09x LQI.xyzw VF10, (VI02++)
MADDAy.xyzw ACC, VF05, VF09y NOP
MADDAz.xyzw ACC, VF06, VF09z NOP
MADDw.xyzw VF13, VF07, VF09w SQI.xyzw VF12, (VI03++)
MULAx.xyzw ACC, VF04, VF10x LQI.xyzw VF11, (VI02++)
MADDAy.xyzw ACC, VF05, VF10y NOP
MADDAz.xyzw ACC, VF06, VF10z NOP
MADDw.xyzw VF14, VF07, VF10w SQI.xyzw VF13, (VI03++)
MULAx.xyzw ACC, VF04, VF11x NOP
MADDAy.xyzw ACC, VF05, VF11y NOP
MADDAz.xyzw ACC, VF06, VF11z NOP
MADDw.xyzw VF15, VF07, VF11w SQI.xyzw VF14, (VI03++)
NOP NOP
NOP NOP
NOP NOP
NOP SQI.xyzw VF15, (VI03++)
NOP NOP
NOP JR VI15
NOP NOP


パケット説明
--------------------------------------------------------------------------

　VU1マイクロコードを用いて透視変換・平行光源計算を行うためにVU1に送るパケ
ット（.dsm形式）について説明します。
　GIFtagを設定することで、独立三角形、トライアングルストリップともに扱うこ
とができます。多くの頂点を描画する場合には、GIFtag - MSCNTまでのパケットを
複数個ならべて転送します。これによりVU1はダブルバッファ処理を行います。

 <例>トライアングルストリップキューブ------------------------------------
.include "vumacros.h"
.global My_dma_start
.global My_matrix
.global My_light_matrix
.global My_rot_trans_matrix

My_dma_start:
.align 0
DMAcnt *     ; マイクロコード自体をVU1に送る
MPG 0, *
.include "basic.vsm" 
.EndMPG
.EndDmaData

DMAcnt *
unpack 4, 4, V4_32, 0, *  ; スクリーンマトリクス
My_matrix:
fwzyx 0.000000, 0.000000, 0.000000, 35.752483
fwzyx 0.000000, 0.000000, -14.765776, 0.000000
fwzyx 0.050000, 4995000.000000, 102.400002, 102.400002
fwzyx 1.000000, 100000000.000000, 2048.000000, 2048.000000
.EndUnpack

unpack 4, 4, V4_32, 4, * ; ワールド座標系での回転、移動マトリクス
My_rot_trans_matrix:
fwzyx 0.0, -0.000000, -0.000000, 1.000000
fwzyx 0.0, -0.000000, 1.000000, 0.000000
fwzyx 0.0, 1.000000, 0.000000, 0.000000
fwzyx 1.0, 0.000000, 0.000000, 0.000000
.EndUnpack

unpack 4, 4, V4_32, 8, * ; 光源色（平行光源x3, 環境光x1）
fxyzw 1.0, 0.0, 0.0, 0.0 ; 平行光源１ (R,G,B)=(1.0, 0.0, 0.0)
fxyzw 0.0, 0.0, 1.0, 0.0 ; 平行光源２ (R,G,B)=(1.0, 0.0, 0.0)
fxyzw 0.0, 0.0, 0.0, 0.0 ; 平行光源３ (R,G,B)=(1.0, 0.0, 0.0)
fxyzw 0.2, 0.2, 0.2, 1.0 ; 環境光 (R,G,B) = (0.2, 0.2, 0.2)
.EndUnpack

unpack 4, 4, V4_32, 12, * ; 平行光源ベクトル(x3)
My_light_matrix:
fxyzw 0.0, -1.0, 0.0, 0.000000 ; 各列がベクトルを示す。この例では Light1=(0.0, 1.0, 0.0), 
fxyzw 1.0, 0.0, 0.0, 0.000000  ; Light2 = (-1.0, 0.0, 0.0), Light3=(0.0, 0.0, 0.0)
fxyzw 0.0, 0.0, 0.0, 0.000000  ; 第４列は環境光の設定
fxyzw 0.0, 0.0, 0.0, 1.000000
.EndUnpack

MSCAL 0 ; マイクロの実行
BASE 0 ; vif1 base のベースレジスタの設定 
OFFSET 512 ; vif1 offset レジスタの設定
.EndDmaData


; ------------------------------------------------------------------------
; 描画しようとするモデルデータ。
; ここでは１個の立方体（トライアングルストリップ）。

DMAcnt *
unpack[r] 4, 4, V4_32, 0, *
iwzyx 0x00000000, 0x00000412, 0x300a4000, 0x0000800e ; giftag(textureあり）
.EndUnpack
unpack[r] 4, 4, V4_32, 1, *
; ストリップ頂点のワールド座標系での位置
fxyzw -5.0, -5.0, 5.0, 1.0
fxyzw -5.0, -5.0, -5.0, 1.0
fxyzw -5.0, 5.0, 5.0, 1.0
fxyzw -5.0, 5.0, -5.0, 1.0
fxyzw 5.0, 5.0, -5.0, 1.0
fxyzw -5.0, -5.0, -5.0, 1.0
fxyzw 5.0, -5.0, -5.0, 1.0
fxyzw -5.0, -5.0, 5.0, 1.0
fxyzw 5.0, -5.0, 5.0, 1.0
fxyzw -5.0, 5.0, 5.0, 1.0
fxyzw 5.0, 5.0, 5.0, 1.0
fxyzw 5.0, 5.0, -5.0, 1.0
fxyzw 5.0, -5.0, 5.0, 1.0
fxyzw 5.0, -5.0, -5.0, 1.0

; 法線ベクトル
fxyzw -1.0, 0.0, 0.0, 1.0
fxyzw -1.0, 0.0, 0.0, 1.0
fxyzw -1.0, 0.0, 0.0, 1.0
fxyzw -1.0, 0.0, 0.0, 1.0
fxyzw 0.0, 1.0, 0.0, 1.0
fxyzw 0.0, 0.0, -1.0, 1.0
fxyzw 0.0, 0.0, -1.0, 1.0
fxyzw 0.0, -1.0, 0.0, 1.0
fxyzw 0.0, -1.0, 0.0, 1.0
fxyzw 0.0, 0.0, 1.0, 1.0
fxyzw 0.0, 0.0, 1.0, 1.0
fxyzw 0.0, 1.0, 0.0, 1.0
fxyzw 1.0, 0.0, 0.0, 1.0
fxyzw 1.0, 0.0, 0.0, 1.0

; 色
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0

; テクスチャ座標
fxyzw 0.0, 0.0, 1.0, 0.0
fxyzw 0.0, 1.0, 1.0, 0.0
fxyzw 1.0, 0.0, 1.0, 0.0
fxyzw 1.0, 1.0, 1.0, 0.0
fxyzw 0.0, 1.0, 1.0, 0.0
fxyzw 1.0, 0.0, 1.0, 0.0
fxyzw 0.0, 0.0, 1.0, 0.0
fxyzw 1.0, 1.0, 1.0, 0.0
fxyzw 0.0, 1.0, 1.0, 0.0
fxyzw 1.0, 0.0, 1.0, 0.0
fxyzw 0.0, 0.0, 1.0, 0.0
fxyzw 0.0, 1.0, 1.0, 0.0
fxyzw 1.0, 0.0, 1.0, 0.0
fxyzw 1.0, 1.0, 1.0, 0.0

.EndUnpack
MSCNT  ; マイクロコードの実行再開（描画処理）
.EndDmaData

DMAend

