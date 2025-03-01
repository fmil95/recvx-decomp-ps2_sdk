[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

ベーシックサンプル basic3d Core版
=================================

<サンプルの解説>
EE Coreのみを用いて行列演算、透視変換等を行う数学関数を実装し、3Dグラフ		ィックスに必要な数学の基礎について理解を深めます。また、このような数学関	数をvu0,vu1に実装する際の手助けとなるよう、basic3d vu0版およびvu1版で用		いている関数と同等の関数を実現します。さらに、演算結果を直接GSに渡すパケ	ットを作成することにより、DMAの動作とともにGSの動作の基本仕様について確		認します。

<ファイル>
	core.c		メインプログラム
	mathfunc.c	透視変換等の行列演算関数(Core版)
	mathfunc.h	透視変換等の行列演算関数のヘッダファイル
	cube.s		オブジェクトデータ(立方体)
	torus1.s	オブジェクトデータ(トーラス データ分割)
	flower.dsm	テクスチャデータ

<コンパイル方法>
	% make

<起動方法>
	% make run

	コンパイル後、以下の方法でも実行可能
	% dsedb
	> run core.elf

<仕様>
        テクスチャ付きTriangle Stripのキューブを表示する。
        テクスチャ付きTriangleのトーラスを表示する。
        (selectボタンで切り替え)

        Zバッファは24bit
        光源は3つ
        テクスチャは32bit

<コントローラの操作方法>
	↑↓(方向キー) : オブジェクトのx軸回転
        ←→	       : オブジェクトのy軸回転
        L1L2ボタン     : オブジェクトのz軸回転
        □○ボタン     : ワールド座標系に対するカメラのy軸回転
        △×ボタン     : ワールド座標系に対するカメラのx軸回転
        R1R2ボタン     : カメラの前進後退回転
        SELECTボタン   : オブジェクト切り替え

<処理概略>
	1. テクスチャをロード
	2. パッドオープン
	3. 透視変換行列・光源計算行列などを設定
	4. 透視変換・光源計算を行い、GSに渡すパケットを生成
	6. GSへのDMA kick
	7. 2に戻る

データフロー
--------------------------------------------------------------------------

<初期化過程>
メインRAM                          GS内蔵RAM(4M)
  テクスチャデータ ----------------> テクスチャデータ展開
  オブジェクトデータ展開
  透視変換行列等の展開

<実行処理過程>
メインRAM                                スクラッチパッド(16K)
  オブジェクトデータ --------------------> GS転送用パケットデータ展開
                     透視変換、光源計算    (プリミティブデータ
                     (CPUcoreジオメトリ)    (STQ, RGBA, XYZF)配列)
                                                    |
                                                    |
GS内蔵RAM(4M) <-------------------------------------+
 フレーム＆Zバッファ書き込み         DMA転送(DMAtag,GIFtag付加)
 (レンダリング)


プログラムの説明(詳細)
--------------------------------------------------------------------------

・変数／構造体／関数(mathfunc.cの関数を除く)

        SampleCubeDataHead[]     :オブジェクトデータ(立方体)へのポインタ
        SampleTorus1DataHead[]   :オブジェクトデータ(トーラス)へのポインタ
        My_texture1[]            :テクスチャデータへのポインタ

        TexEnv          :sceGsTexEnv構造体を転送するパケット作成の構造体
        QWdata          :128bitデータを扱い易くするための共用体
        GifPacket       :Gif(GS)に転送するパケットを生成するための構造体
        ObjData         :メモリに展開したオブジェクトデータを管理する構造体

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

        scePadData      pad     :パッドデータを読み込む構造体
        sceGsDBuff      db      :ダブルバッファを実装するための構造体
        sceGsLoadImage  pTex    :テクスチャをGSローカルメモリに転送するた
                                 めの構造体
        sceDmaChan      *dmaVif :DMAによるデータ転送をするための構造体
        TexEnv          texenv  :テクスチャデータを転送するためのパケット
                                 を作成する構造体

        LoadObj(ObjData *o, u_int *Head)
                        :メインRAMに展開したオブジェクトデータをObjData構
                         造体に登録し、操作するための関数

        ReleaseObj(ObjData *o)
                        :登録したオブジェクトデータを開放するための関数

        MakePacket(ObjData *obj, int num)
                        :オブジェクトデータに対して透視変換・光源計算など
                         を行い、GSに転送するパケットを生成する関数

・main 関数内の変数

        i, j            :forループなどで用いるカウンタ
        fd              :パッドをopenした際に返り値を受け取るファイル記述子
        fr              :ダブルバッファでフリップを行うためのスイッチ
        delta           :回転角を設定するための変数
        obj[2]          :オブジェクトを登録するための変数(cube&torus)
        obj_switch      :オブジェクトを切り替えるためのトグルスイッチ
        toggle          :selectボタンの読み込みをトグルで制御する変数

・mathfunc.c でサポートする変数および関数

        sceSamp0IVECTOR[4]      :128bit境界で配置するint型の配列(要素4)
        sceSamp0FVECTOR[4]      :128bit境界で配置するfloat型の配列(要素4)
        sceSamp0FMATRIX[4][4]   :128bit境界で配置するfloat型の配列(要素4x4)

　関数についての詳しい説明はmathfunc.cにありますのでそちらを参照してくださ
い。mathfunc.cで実装される関数のほとんどはVu0マクロで実装され、libvu0により
提供しています。Core版で実装する関数名はsceSamp0******ですが、VU0版では
sceVu0******となっています。

　なお、Core版とVU0版/VU1版では行列の配列形式が異なりますので注意してくださ
い。Core版では一般的な配列ですが、VU0版/VU1版ではVUの演算機能を有効に利用す
るため、転地形式になっています。


プログラム解説（初期化部分）
--------------------------------------------------------------------------

・オブジェクトデータの読み込み
    LoadObj(&obj[0], SampleCubeDataHead);
    LoadObj(&obj[1], SampleTorus1DataHead);

	サンプルで利用するオブジェクトデータを、SampleCubeDataHeadおよび
	SampleCubeDataHeadからObjData構造体に読み込みます。

・デバイスの初期化
    sceGsResetPath();	//データパスデバイスの初期化(VIF1,VU1,GIF)
    sceDmaReset(1);	//DMAを初期化する(DMA)

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
	この設定を行わないと画面上への表示ができません。また、VSyncで同期を
	とっているコントローラの設定も行えませんので、デバイスの初期化直後
	にこの設定を一度は行うようにしてください。

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
    texenv.size = sceGsSetDefTexEnv(&texenv.gs_tex, 0, 13340, 
        IMAGE_SIZE / 64,SCE_GS_PSMCT32, 8, 8, 0, 0, 0, 0, 1);
        //テクスチャ設定データの作成
    SCE_GIF_CLEAR_TAG(&texenv.giftag);
    texenv.giftag.NLOOP = texenv.size;
    texenv.giftag.EOP = 1;
    texenv.giftag.NREG = 1;
    texenv.giftag.REGS0 = 0xe;
    *(u_long*)&texenv.gs_tex.clamp1
        = SCE_GS_SET_CLAMP(0, 0, 0, 0, 0, 0);

	sceGsSetDefTexEnv()を用いて、テクスチャ関連のGS汎用レジスタTEX0_1,
	TEX0_1, CLAMP_1の設定を行うパケットを作成します。

・テクスチャの環境設定用パケットの転送
    FlushCache(0);
    dmaVif = sceDmaGetChan(SCE_DMA_GIF);
    dmaVif->chcr.TTE = 1;
    sceDmaSendN(dmaVif, &texenv.giftag, texenv.size+1);
                        //テクスチャ設定データの転送
    sceGsSyncPath(0, 0);//データ転送の終了待ち

	生成されたテクスチャ環境設定用のパケットを、PATH3経由のNormal DMA
	を実行して転送します。

・コントローラの初期化
    fd = open("pad:0", 0);

	1P側のコントローラを使うための初期化を行います。以後はread()を用い
	て、指定したバッファにデータを読み込めます。コントローラのデータは
	VSync毎に更新されます。


プログラムの解説（メインループ部分）
--------------------------------------------------------------------------

・View-Screenマトリクスの生成
    sceSamp0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
                2048.0f, 2048.0f,1.0f, 16777215.0f,1.0f,65536.0f);

	sceSamp0ViewScreenMatrix()を用いてView-Screenマトリクスを求めます。
	視点から投影スクリーンまでの距離を512、画面のアスペクト比を1:0.47、
	画面の中心をGdPrimitive座標系で(2048,2048)、Zバッファの値を1.0-
	16777215.0、Zの値を1.0-65536.0としたView-Screenマトリクスを求めて
	います。
        Core版ではメインループの外で初期化しています。実行中にこのマトリク
	スを変更することは希なためです。

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
	ここでは得られたデータをもとに、オブジェクト、カメラ双方の回転角、
	移動量を更新しています。コントローラの左手側がオブジェクト、右手側
	のボタンがカメラの操作となっています。また、selectボタンがオブジェ
	クトの切り替えです。

・Local-Worldマトリックス(回転のみ)の生成
    sceSamp0UnitMatrix(work);                   //単位行列
    sceSamp0RotMatrix(local_world, work, rot);  //回転行列

	Local-Worldマトリクスは、Local座標で保持されているオブジェクトごと
	に求める必要があります。回転、平行移動といったアフィン変換が用いら
	れます。
	このサンプルでは、回転マトリックスおよび平行移動マトリクスを計算し
	て、それらを乗算したものをLocal-Worldマトリックスとして用いていま
	す。
	ただしこのサンプルでは、3行目の平行移動(obj_trans)を設定する前に
	Local_Lightマトリクスを設定する必要があります。平行移動を加味した
	行列を用いてLocal_Lightマトリクスを生成すると、環境光の計算が正し
	く行えません。これは平行移動、環境光いずれも行列の第4列を用いて計
	算するためです。したがって回転行列を設定した直後に、Local_Lightマ
	トリクスを作ってください。

	もうひとつ注意点として、obj_transの第4要素には必ずゼロを設定してく
	ださい。マトリクスの[4,4]要素には非常にクリティカルな値がセットさ
	れていて、ゼロ以外の値ではこれを変えてしまうことになるためです。

・ローカルライトマトリックスの生成
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

・Local-Worldマトリックス(回転＆平行移動)の生成
    sceSamp0TransMatrix(local_world, local_world, obj_trans);
        //回転＆平行移動

	Local_Lightのマトリクスの生成が終了したので、Local_Worldマトリクス
	に平行移動要素(obj_trans)を加えて、真のLocal_Worldマトリクスを生成
	します。

・Local-Screenマトリックスの生成
    sceSamp0MulMatrix(world_screen, view_screen, world_view);
    sceSamp0MulMatrix(local_screen, world_screen, local_world);

	ここまでに求められたView-Screenマトリクス、World-Viewマトリクスを
	用いて、まずWorld-Screenマトリクスを求めます。さらにLocal-Worldマ
	トリクスを掛け合わせることで、Local-Screenマトリックスが求められま
	す。
	この時点で掛け合わせるLocal_Worldマトリクスは、回転と平行移動の要
	素が正しくセットされたものである必要があります。

・パケット生成
    MakePacket(&obj[obj_switch], i);

	パケットの生成は、MakePacket()の内部で行われます。引数objは、表示
	するオブジェクトのObjData構造体へのポインタです。これを切り替える
	ことで簡単に表示オブジェクトを変更することができます。

・パケット初期設定　（MakePacket()）
    pack->size = 0;
    pack->buf = (QWdata *)0x70000000;//SPR

	パケットの書き込みアドレスを0x70000000に指定しています。これはスク
	ラッチパッド領域の先頭ですので、パケットの大きさがスクラッチパッド
	サイズの16KBを越えないように注意してください。メインメモリにパケッ
	トを生成する場合でも、DMAの転送時にQWCに設定できる値に制限がありま
	すので、パケットのサイズは1MB以上にならないように注意してください。
	このサンプルにおけるトーラスデータは、VU1版でVUMem1のダブルバッフ
	ァを使用するため、その容量を超えないように4KB程度のブロックサイズ
	に分割されています。

・DMAtag、GIFtagの生成　（MakePacket()）
    //DMAtag
    pack->buf[pack->size].ul128 = (u_long128)0;
    pack->buf[pack->size++].ui32[0] = 0x70000000|(obj->vertexNum[num]*3+1);
    //GIFtag
    pack->buf[pack->size].ul64[0]
        = SCE_GIF_SET_TAG(obj->vertexNum[num], 1, 1,obj->prim,
                                                SCE_GIF_PACKED,3);
    pack->buf[pack->size++].ul64[1] = 0x0412L;

	パケット先頭には、DMAtagおよびGIFtagをつける必要があります。
	DMAは、Source Chain modeを利用しています。送信するパケットのサイズ
	は頂点数x3(STQ,RGBA,XYZF2)+1(GIFtag)、DMAtagのIDはend(0x70000000)
	です（DMAパケットがひとつだけなので）。
	GIFはPACKED mode(SCE_GIF_PACKED)を使用し、プリミティブの設定も
	GIFtagの中で行うようにしています(obj->prim)。REGSの値は0x412(STQ,
	RGBA,XYZF2)となっています。

・透視変換　（MakePacket()）
    Q = sceSamp0RotTransPers(v01, local_screen, vertex[j], 1);
    sceSamp0ScaleVector(tex, texUV[j], Q); //for Perspective correction

	sceSamp0RotTransPers()関数は透視変換を行う関数です。詳しい説明は
	mathfunc.c内に書いてあります。出力結果(v01)はint型配列で、GSに直接
	渡せる形式(GSの座標系)になっています。また、返り値Qはv01の第4要素
	の逆数で、float型です。これはパースペクティブ補正に用いるもので、
	テクスチャ座標(S, T, Q)に用います。
        texUVはテクスチャのUV座標(U, V, 1.0 ,0)がセットされています。これ
	らの値にQを掛けることでSTQテクスチャ座標が求まります。

・光源計算　（MakePacket()）
    sceSamp0NormalColorVector(c01, local_light, light_color,
                         normal[j], color[j]);

	頂点の法線ベクトル・色情報と、ライトのベクトル・色情報から光源計算
	後の頂点の色を決定します。これもGSに直接渡せる形式で値が帰ってきま
	す。
	関数内部では、ライトのベクトルと頂点の法線ベクトルとの内積を求めた
	時点で、一度内積値を0.0～1.0の間でクリッピングします。さらに３つの
	光源の影響を足しあわせた後、さらに0.0～255.0の間でクリッピングを行
	っています。

・パケットへの格納　（MakePacket()）
    pack->buf[pack->size++].ul128 = *((u_long128*)tex);
    pack->buf[pack->size++].ul128 = *((u_long128*)c01);
    pack->buf[pack->size++].ul128 = *((u_long128*)v01);

	求められた各値を(S,T,Q) (R,G,B,A) (X,Y,Z,F) の順でパケットに詰め込
	みます。

・GSへの転送
    MakePacket(&obj, i);
    FlushCache(0);
    sceDmaSend(dmaVif, (u_long128*)((u_int)obj.pack[i].buf|0x80000000));
    sceGsSyncPath(0, 0);

	MakePacket()で生成されたプリミティブのパケットを、PATH3経由の
	Source Chain DMAを実行して転送します。
	一度に作れるパケットサイズに制限があるため、これをオブジェクトの分
	割ブロックの個数分だけ繰り返します。

・描画環境の切り替え、V-Blank待ち
    if(fr&0x01)
        sceGsSetHalfOffset(&db.draw1,2048,2048,sceGsSyncV(0)^0x01);
    else
        sceGsSetHalfOffset(&db.draw0,2048,2048,sceGsSyncV(0)^0x01);

	フィールド毎に動作環境を切り替えた後、V-Blank期間の開始を待ちます。

