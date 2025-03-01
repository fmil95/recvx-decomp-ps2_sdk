[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
		     Copyright (C) 1999 by Sony Computer Entertainment Inc.                                                       All Rights Reserved

ベーシックサンプル basic3d VU0版
================================

<サンプルの解説>
basic3d Core版において行列演算、透視変換等を行う数学関数、3Dグラフィック	スに必要な数学の基礎について理解を深めた後に、高速化の第一ステップとして	VU0マクロ命令への書き換え例を示します。
VU0マクロ命令は、コプロ命令ですのでCPUの命令としてアセンブラ形式での利用	が可能です。また、同時に最大４つのデータを処理することができるため、行列	演算、ベクトル演算との親和性が高くなっています。
このbasic3d VU0版は、Core版の行列演算等をVU0マクロ命令で置き換える例を示	すことで、VU0マクロ命令の利用法についての理解を深めることを目的としてい		ます。そして、さらなる高速化のためにVU0マイクロ命令あるいはVU1マイクロ命	令を利用する際の橋渡しとなることを期待しています。

<ファイル>
	main.c		メインプログラム
	vu0.c		座標変換、透視変換等の行列演算関数
	cube.s		オブジェクトデータ(立方体)
	torus1.s	オブジェクトデータ(トーラス データ分割)
	torus.s		オブジェクトデータ(トーラス データ分割)
	flower.dsm	テクスチャデータ

<起動方法>
	% make		：コンパイル
	% make run	：実行

	コンパイル後は、以下の方法でも実行可能
	% dsedb
	> run main.elf

<コントローラの操作方法>
	↑↓(方向キー) : x軸回転(オブジェクト)
	←→	　　　 : y軸回転(オブジェクト)
	L1L2ボタン　　 : z軸回転(オブジェクト)

	△×ボタン　　 : x軸回転(視点)
	□○ボタン　　 : y軸回転(視点)
	R1R2ボタン　　 : z軸移動(視点)

	SELECTボタン　 : オブジェクトの切替え(CUBE/TORUS)

<仕様>
	テクスチャ付きTriangle Stripのキューブを表示する。
	テクスチャ付きTriangleのトーラスを表示する。

	Zバッファは24bit
	光源は3つ
	テクスチャは32bit

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
                     (VU0ジオメトリ)        (STQ, RGBA, XYZF)配列)
                                                    |
                                                    |
GS内蔵RAM(4M) <-------------------------------------+
  フレーム＆Zバッファ書き込み         DMA転送(DMAtag,GIFtag付加)
  (レンダリング)


プログラムの説明(詳細)
--------------------------------------------------------------------------

<変数・構造体・関数>　(mathfunc.cの関数を除く)

	SampleCubeDataHead[]     :オブジェクトデータ(立方体)へのポインタ
	SampleTorus1DataHead[]   :オブジェクトデータ(トーラス)へのポインタ
	My_texture1[]            :テクスチャデータへのポインタ

	TexEnv	  :sceGsTexEnv構造体を転送するためのパケット作成の構造体
	QWdata	  :128bitデータを扱い易くするための共用体
	GifPacket :Gif(GS)に転送するパケットを生成するための構造体
	ObjData	  :メモリに展開したオブジェクトデータを管理するための構造体

	camera_p  	:カメラ位置
	camera_zd 	:カメラの視線方向ベクトル
	camera_yd 	:カメラの下方向ベクトル
	camera_rot	:カメラの回転用ベクトル

	light0		:ライト0
	light1		:ライト1
	light2		:ライト2
	color0		:ライト0の色
	color1		:ライト1の色
	color2		:ライト2の色

	ambient		:環境光

	obj_trans	:オブジェクトの移動
	obj_rot		:オブジェクトの回転

	local_world	:オブジェクトローカル座標をworld座標系に変換する
			 行列
	world_view	:world座標系を視点座標系に変換する行列
	view_screen	:視点座標系をスクリーン座標系に変換する行列
	local_screen	:ローカル座標をスクリーン座標に変換する行列
							(上３行列の積)

	normal_light	:ワールド座標系における頂点の法線ベクトルとライトの 			    方向ベクトルの内積を取るための行列
			 (ライトの方向ベクトルがセットされる)
	local_light	:normal_light行列にlocal_world行列を掛けて、
			 ローカル座標系における頂点の法線ベクトルと
			 ライトの方向ベクトルの内積を取れるようにする行列
	light_color	:local_light行列と頂点の法線ベクトルの積により
			 生成された内積ベクトルに、各ライトの色(3色＋環境
			 光)を掛けて和をとる行列
	local_color	:local_lightとlight_colorの積を取った行列
			 (内積ベクトルを生成した後に、各内積値を0.0～1.0の
			 範囲でクリッピングする必要があるため、実際は使えな			    い)
	work		:一時的な作業のための行列

	scePadData      pad	:パッドデータを読み込むための構造体
	sceGsDBuff      db	:ダブルバッファを実装するための構造体
	sceGsLoadImage  pTex	:テクスチャをGSローカルメモリに転送するのに    				    用いる構造体
	sceDmaChan      *dmaVif	:DMAによるデータ転送をするための構造体
	TexEnv          texenv	:テクスチャデータを転送するためのパケットを				    作成する構造体

	LoadObj(ObjData *o, u_int *Head)
			:メインメモリに展開したオブジェクトデータをObjData
			 構造体に登録し、操作するための関数。

	ReleaseObj(ObjData *o)
			:登録したオブジェクトデータを開放するための関数

	MakePacket(ObjData *obj, int num)
			:オブジェクトデータに対して透視変換・光源計算などを
			 行い、GSに転送するパケットを生成する関数

<main関数内の変数>
	i, j		:forループなどで用いるカウンタ
	fd		:パッドをopenするとき返り値を受け取る file 
			 descriptor
	fr		:ダブルバッファでフリップを行うためのトグルスイッチ
	delta		:回転角を設定するための変数
	obj		:オブジェクトを登録するための変数


プログラム解説（初期化部分）
--------------------------------------------------------------------------


・オブジェクトデータの読み込み
    LoadObj(&obj[0], SampleCubeDataHead);
    LoadObj(&obj[1], SampleTorus1DataHead);

	オブジェクトデータをSampleCubeDataHead、SampleTorus1DataHeadから読
	み込みます。

・デバイスの初期化
    sceGsResetPath();	//データパスデバイスの初期化(VIF1,VU1,GIF)
    sceDmaReset(1);	//DMAを初期化する(DMA)
    sceVpu0Reset();	//VU0を初期化する(VIF0,VU0)

	プログラムの先頭で、使用するすべてのデバイスの初期化を行います。初
	期化を行っていないデバイスを利用した場合、期待した動作をしないこと
	がありますので、必ず実行してください。

・描画環境の設定
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
		//GSを初期化する
    sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT, 
			SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);
				//ダブルバッファの設定をおこなう
    *(u_long *)&db.clear0.rgbaq = SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10,
					0x80, 0x3f800000);
    *(u_long *)&db.clear1.rgbaq = SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10,
							0x80, 0x3f800000);

	各デバイスの初期化が終わった後に、GSの描画環境の設定を行います。こ
	の設定を行わないと画面上への表示ができません。また、VSyncで同期をと	   っているコントローラの設定も行えませんので、デバイスの初期化直後に
	この設定を一度は行うようにしてください。

・テクスチャ用のイメージ転送
    sceGsSetDefLoadImage(&pTex,13340,IMAGE_SIZE / 64,SCE_GS_PSMCT32,
			0, 0,IMAGE_SIZE, IMAGE_SIZE);//LoadImage情報の設定
    FlushCache(0);
    sceGsExecLoadImage(&pTex, (u_long128 *)My_texture1);//LoadImageの実行
    sceGsSyncPath(0, 0);	//データ転送の終了待ち

	テクスチャ用のイメージをGSローカルメモりに転送するために、LoadImage        の情報を設定します。その後、sceGsExecLoadImage()を実行することで、
	指定されたイメージがGSのローカルメモりに転送されます。この関数は内
	部でDMAを利用しますので、転送前にFlushCache(0)でDキャッシュとメイン        メモり間の整合性を取っています。最後に、sceGsSyncPath()でデータ転送	   の終了待ちをおこないます。これで、GSのローカルメモリへのイメージ転
	送が完了します。

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

	sceGsSetDefTexEnv()を用いて、テクスチャ関連のGS汎用レジスタTEX0_1, 	   TEX0_1, CLAMP_1の設定を行うパケットを作成します。

・テクスチャの環境設定用パケットの転送
    FlushCache(0);
    dmaVif = sceDmaGetChan(SCE_DMA_GIF);
    dmaVif->chcr.TTE = 1;
    sceDmaSendN(dmaVif, &texenv.giftag, texenv.size+1);
				//テクスチャ設定データの転送
    sceGsSyncPath(0, 0);	//データ転送の終了待ち

	生成されたテクスチャ環境設定用のパケットを、PATH3経由のNormal DMA
	によって転送します。

・コントローラの初期化
    fd = open("pad:0", 0);

	1P側のコントローラを使うための初期化を行います。以後は、read()で指
	定したバッファにデータを読み込めます。コントローラのデータはVSync
	毎に更新されます。

プログラム解説（メインループ部分）
--------------------------------------------------------------------------

 以下、メインループの中で行われる、コントローラの情報取得、マトリックスの
生成、パケット生成(座標変換、透視変換、UV->STQ変換、光源計算)、パケットの
生成について説明していきます。

・View-Screenマトリックスの生成
    sceVu0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
	    2048.0f, 2048.0f,1.0f, 16777215.0f,1.0f,65536.0f);

	sceVu0ViewScreenMatrix()を用いて、View-Screenマトリックスを求めま
	す。ここでは、視点から投影スクーリンまでの距離を512、画面のアスペ
	クト比を1:0.47、画面の中心をGdPrimitive座標系で(2048,2048)、Zバッ
	ファの値を1.0-16777215.0、Zの値を1.0-65536.0としたView-Screenマト
	リクスを求めています。

・コントローラの情報取得
    if(pad.button & SCE_PADLdown){
        obj_rot[0] += delta; if(obj_rot[0]>PI) obj_rot[0] -= 2.0f*PI; }
    if(pad.button & SCE_PADLup){
        obj_rot[0] -= delta; if(obj_rot[0]<-PI) obj_rot[0] += 2.0f*PI; }
    if(pad.button & SCE_PADLright){
        obj_rot[1] += delta; if(obj_rot[1]>PI) obj_rot[1] -= 2.0f*PI; }
    if(pad.button & SCE_PADLleft){
        obj_rot[1] -= delta; if(obj_rot[1]<-PI) obj_rot[1] += 2.0f*PI; }
    if(pad.button & SCE_PADL1){
        obj_rot[2] += delta; if(obj_rot[2]>PI) obj_rot[2] -= 2.0f*PI; }
    if(pad.button & SCE_PADL2){
        obj_rot[2] -= delta; if(obj_rot[2]<-PI) obj_rot[2] += 2.0f*PI; }
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
	if(camera_rot[1]<-PI) camera_rot[1] += 2.0f*PI; }
    if(pad.button & SCE_PADR1){
        camera_p[2] -= delta*5; if(camera_p[2]<-100) camera_p[2] = -100; }
    if(pad.button & SCE_PADR2){
        camera_p[2] += delta*5; if(camera_p[2]>-10) camera_p[2] = -10; }
    if(!toggle && (pad.button & SCE_PADselect)){
        obj_switch = (++obj_switch)&0x01; toggle = 1;
    }else if(!(pad.button & SCE_PADselect)){toggle = 0;}

	コントローラのボタン情報は、read()を用いて取得できます。
	ここでは、得られたデータをもとに、オブジェクトとカメラの回転角、移
	動量を更新しています。コントローラの左手側がオブジェクト、右手側の
	ボタンがカメラの操作となっています。

・Local-Worldマトリクス(回転のみ)の生成
    sceVu0UnitMatrix(work);                       //単位行列
    sceVu0RotMatrix(local_world, work, obj_rot);  //回転行列

	Local-Worldマトリスは、Local座標で保持されているオブジェクトごとに
	求める必要があります。回転、平行移動といったアフィン変換が用いられ
	ます。
	このサンプルでは、回転マトリクスおよび平行移動マトリクスを計算して
	それらを乗算したものをLocal-Worldマトリクスとして用いています。
	ただしこのサンプルでは、３行目の平行移動(obj_trans)を設定する前に
        local_lightマトリクスを設定する必要があります。平行移動を加味した
	行列を用いてlocal_lightマトリクスを生成すると、環境光の計算が正し
	く行えません。これは平行移動、環境光いずれも行列の第４列を用いて計
	算するためです。したがって回転行列を設定した直後に、local_lightマ
	トリクスを作ってください。
	もうひとつ注意点として、obj_transの第４要素には必ずゼロを設定して
	ください。マトリクスの[4,4]要素には非常にクリティカルな値がセット
	されていますが、ゼロ以外の値ではこれを変えてしまうことになります。

・ローカルライトマトリクスの生成
    sceVu0NormalLightMatrix( normal_light, light0,light1,light2);
    sceVu0MulMatrix(local_light, normal_light,local_world);

	sceVu0NormalLightMatrix()を用いて、３つの光源からNormal-Lightマト
	リクスを求めます。さらにLocal-Worldマトリクスを乗算して、Local-
	Lightマトリクスを生成します。前述したように、このとき掛ける
	local_worldは、回転のみが設定されたマトリクスである必要があります。

・ライトカラーマトリクスの生成
    sceVu0LightColorMatrix( light_color, color0, color1, color2, ambient);

	光源色および環境色よりsceVu0LightColorMatrix()を用いてライトカラー
        マトリクスを求めます。光源計算時に、各光源の影響色が求まった後に、
	ライトカラーマトリクスを乗算して、最終的なスクリーン上での頂点色が
	決定します。

・Local-Worldマトリクス(回転＆平行移動)の生成
    sceVu0TransMatrix(local_world, local_world, obj_trans);
					//回転＆平行移動行列

	local_lightのマトリクス生成が終了したので、local_screenマトリクス
	を作成する前に平行移動の要素をlocal_worldに加えます。

・World-Viewマトリクスの生成
    sceVu0RotCameraMatrix(world_view, camera_p, camera_zd,camera_yd,
							camera_rot);

	この関数 sceVu0RotCameraMatrix()は、内部でsceVu0CameraMatrix()を呼
	び出します。このsceVu0CameraMatrix()用いて、World-Viewマトリクスを
	求めます。View座標系は視点(eye)を原点(0,0,0)とし、視線方向がZ+、右
	手方向がX+、足方向がY+となっています(右手座標系）。ここでは、world
	に対して視点camera_pを(0,0,-25)、視線camera_zdを(0,0,1)と垂直方向
	camera_ydを(0,1,0)とし、これを基準にWorld-Viewマトリクスを生成しま
	す。sceVu0RotCameraMatrix()関数内部ではこれらのベクトルをcamera_rot	   によって回転し、その結果をsceVu0CameraMatrix()に渡しています。

・Local-Screenマトリクスの生成
    sceVu0MulMatrix(work, world_view, local_world);	     //Local-View
    sceVu0MulMatrix(local_screen, view_screen, work);        //Local-Screen

	ここまでに求められたWorld-Viewマトリクス、Local-Worldマトリクスを
	用いて、まずLocal-Viewマトリクスを求めます。さらに、View-Screenマ
	トリクスを乗算することでLocal-Screenマトリクスが求められます。

・パケットの生成
    MakePacket(&obj[obj_switch], i);

	パケットの生成は、MakePacke()の内部で行われます。詳しくは後で解説
	します。

・GSへのパケット転送
    FlushCache(0);
    sceDmaSend(dmaVif,(u_long128*)((u_int)pBase&0x3ff0|0x80000000));
    sceGsSyncPath(0, 0);

	MakePacket()で生成されたプリミティブのパケットを、PATH3経由のSource        Chain DMAを実行して、転送します。

・描画環境の切り替え、V-Blank待ち
    if(fr&0x01)
        sceGsSetHalfOffset(&db.draw1,2048,2048,sceGsSyncV(0)^0x01);
    else
        sceGsSetHalfOffset(&db.draw0,2048,2048,sceGsSyncV(0)^0x01);

	フィールドごとに動作環境を切り替えた後、V-Blank期間の開始を待ちま
	す。


プログラム解説（パケット生成）
--------------------------------------------------------------------------

　以下にMakePacket()で行うパケット生成処理の内容を説明します。

・パケット初期設定
    pack->size = 0;
    pack->buf = (QWdata *)0x70000000;//SPR

	パケットの書き込み位置を先頭に持ってきます。
	パケットを生成する領域を、ここではスクラッチパッドの先頭に設定して
	います。スクラッチパッドを利用する際は、パケットの大きさがスクラッ
	チパッドの大きさである16KBを越えないように注意してください。メイン
	メモリを利用する場合でも、DMAの転送時にQWCに設定できる値に制限があ
	りますので、パケットのサイズが1MB以上にならないように注意してくだ
	さい。

・DMAtag、GIFtagの生成
    //DMAtag
    pack->buf[pack->size].ul128 = (u_long128)0;
    pack->buf[pack->size++].ui32[0]
        = 0x70000000 | (obj->vertexNum[num]*3 + 1);

    //GIFtag
    pack->buf[pack->size].ul64[0]
	= SCE_GIF_SET_TAG(obj->vertexNum[num], 1, 1,obj->prim,
          SCE_GIF_PACKED,3);
    pack->buf[pack->size++].ul64[1] = 0x0412L;

	パケット先頭にはDMAtagおよびGIFtagをつける必要があります。DMAは、
	Source Chain mode を使用します。送信するパケットのサイズは頂点数×
	3(STQ,RGBA,XYZF2)＋1(GIFtag)、DMAtagのIDは（DMAパケットがひとつだ
	けなので）endを用います。また、GIFはPACKED modeを利用しています。
	プリミティブの設定もGIFtagの中で行うようにしています。REGSの値は、
	0x412(STQ,RGBA,XYZF2)となっています。

・STQ、RGBA、XYZの生成
    sceVu0RotTransPersNClipCol(&pack->buf[pack->size].ul128, local_screen,
         vertex,normal, texUV,color, local_light,light_color, 
         obj->vertexNum[num]);

	sceVu0RotTransPersNClipCol()を用いてパケットの中身(STQ、RGBA、XYZ)
	を求めています。


プログラム解説（VU0マクロ命令）
--------------------------------------------------------------------------

以下、sceVu0RotTransPersNClipCol()の処理について説明します。VU0のマクロ
命令を用いて座標変換・透視変換・光源計算などを行う関数で、本サンプルの核と
なる部分です。vu0.cを参照してください。

・マトリックスの設定
    # Local-Worldマトリックスの設定
    lqc2	vf4,0x0(%1)	#set local_world matrix[0]
    lqc2	vf5,0x10(%1)	#set local_world matrix[1]
    lqc2	vf6,0x20(%1)	#set local_world matrix[2]
    lqc2	vf7,0x30(%1)	#set local_world matrix[3]
    # Local-Lightマトリックスの設定
    lqc2	$vf17,0x0(%6)	#set local_light matirix[0]
    lqc2	$vf18,0x10(%6)	#set local_light matirix[1]
    lqc2	$vf19,0x20(%6)	#set local_light matirix[2]
    # Light-Colorマトリックスの設定
    lqc2	$vf21,0x0(%7)	#set light_color matrix[0]
    lqc2	$vf22,0x10(%7)	#set light_color matrix[1]	
    lqc2	$vf23,0x20(%7)	#set light_color matrix[2]
    lqc2	$vf20,0x30(%7)	#set light_color matrix[3]		

	最初に、VU0のVFレジスタに、Local-Worldマトリクス、Normal-Lightマト
	リクス、Light-Colorマトリクスを設定します。

・頂点／法線／頂点色／STの読み込み
    lqc2	vf8,0x0(%2)	#load XYZ
    lqc2	$vf24,0x0(%4)	#load NORMAL
    lqc2	$vf25,0x0(%5)	#load COLOR
    lqc2	$vf27,0x0(%8)	#load ST

	パケット生成に必要な頂点座標、法線、頂点色、テクスチャ座標をレジス
	タに読み込みます。

・座標変換
    # (X0,Y0,Z0,W0)=[SCREEN/LOCAL]*(X,Y,Z,1)
    vmulax.xyzw     ACC, vf4,vf8
    vmadday.xyzw    ACC, vf5,vf8
    vmaddaz.xyzw    ACC, vf6,vf8
    vmaddw.xyzw     vf12,vf7,vf8

	Local-Screenマトリクスと頂点座標を乗算して、スクリーン座標を求めま
	す。このときのWの値は、View座標系でのZの値と等しくなります。

・透視変換
    # (X1,Y1,Z1,1)=(X0/W0,Y0/W0,Z0/W0,W0/W0)
    vdiv    Q,vf0w,vf12w
    vwaitq
    vmulq.xyzw	vf12,vf12,Q
    vftoi4.xyzw	vf13,vf12

	スクリーン座標を1/Wと乗算します。さらに、GIFパケット用に固定小数値
	に変換します。ここで計算した1/Wは、後にテクスチャ座標と乗算するた
	め保持しておきます。

・光源の影響を求める
    # (L1,L2,L3)=[LLM](Nx,Ny,Nz)
    # LLM:ローカルライトマトリックス
    # L1,L2,L3:光源の影響
    # Nx,Ny,Nz:法線ベクトル
    vmulax.xyzw    ACC, $vf17,$vf24
    vmadday.xyzw   ACC, $vf18,$vf24
    vmaddz.xyzw    $vf24,$vf19,$vf24
    vmaxx.xyz      $vf24,$vf24,$vf0 #負の値は0にする

	ローカルライトマトリクスと法線ベクトルを乗算して、光源の影響を求め
	ます。光源の方向と法線ベクトルの向きが同じ場合、結果が負の値をとる
	のでこれらは0サチュレートします。

・光源の影響色を求める
    # (LTr,LTg,LTb,LTw)=[LCM](L1,L2,L3,1)
    # LCM:ライトカラーマトリックス
    # LTr,LTg,LTb:光源の影響色
    vmulax.xyzw    ACC, $vf21,$vf24
    vmadday.xyzw   ACC, $vf22,$vf24
    vmaddaz.xyzw   ACC, $vf23,$vf24
    vmaddw.xyzw    $vf24,$vf20,$vf0	

	ライトカラーマトリクスと光源の影響を乗算して、光源の影響色を求めま
	す。

・スクリーン上の頂点色を求める
    # (RR,GG,BB) = (R,G,B)*(LTr,LTg,LTb)
    # R,G,B:頂点色
    # RR,GG,BB:スクリーン頂点色
    vmul.xyzw	$vf26,$vf24,$vf25
    # [0..255] サチュレーション
    vmaxx.xyz	$vf26,$vf26,$vf0
    lui		$2,0x437f
    ctc2	$2,$vi21
    vnop
    vnop
    vminii.xyz	$vf26,$vf26,I
    vftoi0.xyzw	$vf26,$vf26

	光源の影響色と頂点色を乗算することで、スクリーン上の頂点色が求まり
	ます。結果は、0-255の範囲でなければなりませんので、その範囲でのサ
	チュレーションを行います。さらに、GIFパケット用に固定小数値に変換
	します。

・STを求める
    # (S,T,Q) = (s,t,1)/w
    vmulq.xyz	$vf28,$vf27,Q

	パースペクティブコレクションのために、透視変換時に保持しておいた
	1/Wの値をかけます。

・STQ、RGBA、XYZの値を保存する
    sqc2	$vf28,0x0(%0)		#store STQ 
    addi	%0,0x10
    sqc2	$vf26,0x0(%0)		#store RGBA 
    addi	%0,0x10
    sqc2	vf13,0x0(%0)		#store XYZ 
    addi	%0,0x10
    #
    addi	%3,-1
    addi	%2,0x10
    addi	%4,0x10
    addi	%5,0x10
    addi	%8,0x10
    bne		$0,%3,_rotTPNCC_loop

	GIFtagの中で設定されている順序にしたがって、STQ、RGBA、XYZの値をパ
	ケットに保存していきます。GIFで送る場合、RGBAQ命令におけるQの値は
	直前のST命令で内部レジスタに保存された値が設定されますので、順序に
	は注意してください。
	以上で、sceVu0RotTransPersNClipCol()のメインループは終了します。指
	定された頂点数になるまで、各アドレスをインクリメントしながら、頂点
	データ読み込み以降の処理を繰り返します。
