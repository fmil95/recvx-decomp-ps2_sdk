/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */ 
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - tfog -
 *
 *                         Version 0.10 
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            tfog.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *	0.1		Jul,13,1998	suzu	    1st version
 *	0.2		Dec,23,1999	suzu	    changed for libgraph
 *	0.3		May,13,1999	suzu	    add comment
 */
// これは、PS2 で推奨されるデータ列の構造とクリップ動作の例を示します。
// 同時座標系の基本的な動作に関しては、tballs.c を参照下さい。
//
// サンプルには以下のローカルヘッダファイル object.h が必要です。
//	
#include "object.h"

// 
// 	Screen information
// 
// スクリーンまでの距離、およびアスペクト比を設定します。
// 640x240 のインターレースダブルバッファで表示する場合、ピクセルの形
// は 1:0.5 の長方形になります。
//
SCREEN screen = {
	1024,		// distance to screen
	1.0,	0.5,	// aspect ratio
};

// 
// 	Lighting information
// 
// 光源（３光源）の位置、方向、光源の影響、および環境色を設定します。
// ここでは光源の方向は光源の位置から自動計算されるので値を設定する必
// 要はありません。ここでは light-0 のみを使用します。
//
LIGHT light = {
	1,						// light mode
// 	light-0		light-1		light-2
	{ 0, 0,50,1},	{-40,40,40,1},	{-40,40,40,1},	// light source position
	{ 0, 0, 0,0},	{  0, 0, 0,0},	{  0, 0, 0,0},	// light direction 
	{0.8,0.8,0.8,0},{0,0,0,0},	{0,0,0,0},	// light influence
	{0.2,0.2,0.2,0},				// ambient
};

// 
// 	Camera position information
// 
// カメラ位置を指定します。カメラは鉛直方向に立てられ、常に原点を向く
// ように修正されます。すなわち view.zd = -vies.p となるようにカメラ
// の向きが設定されます。
//
VIEW view = {
	{    0,   0,  -1, 0},	// foot direction
	{    1,   1,  -1, 0},	// eye direction 
	{ -100,-100, 150, 1},	// standing position
};

// Your moving speed
// カメラの移動速度を指定します。この値がフレーム毎に view.p より加算
// されます。 
static VECTOR	 v = {0.5, 2.5, 0, 0};

//
// 立方体の頂点データを生成し、描画します。
// 立方体は 2 組のストリップメッシュの組合せで作ることができます。
// データはストリップメッシュを単位として
//	
//		color, vertex, normal, texture
//	
// の順序で置かれます。(cvnt)
//
// メッシュの先頭の w フィールドにはストリップ長さが記されます。
// 頂点の w フィールドは適用されるマトリクス ID が記されます
// 通常オブジェクトあたりの変換行列は 1 種類であるのでここは 0 が埋め
// られます。 
//
// スクリーンに入らない立方体はオブジェクト単位でクリップされます。
// オブジェクトクリップはオブジェクトのアトリビュートとしてそのオブジェ
// クトを包洛する直方体 (box) を定義し box に対してクリップチェックを
// 行ないます。box の一部が画面に含まれる場合はオブジェクトの全頂点が
// 変換されます。
//
// これに対して立方体の下に描かれる地面は頂点単位のクリップが行なわれ
// ます。
// オブジェクトクリップはオブジェクトの全頂点に対してクリップチェック
// を行なわないで済むため高速ですが、その反面、厳密なクリップをすることが
// できません。GS プリミティブ座標系は (-2048,-2048)-(2048,2048) でラッ
// プラウンドするため、オブジェクトが大きい場合は、各頂点が GS プリミ
// ティブ座標系の有効範囲に収まるかどうかを再度厳密にチェックする必要があ
// ります。
//
// クリッピング操作の実際は objsimpl.c のコードを参照下さい。
//
//

#define C	255	// cube color
#define S	5	// cube size
#define N	680	// cube number

static void draw_cube(MATRIX *mw)
{
	// cube : color + vertex + normal + (texture)
	static VECTOR	cvnt[] = {

		 // 8 length triangle strip
		 {0, 0, 0, 8},	
		 // color	 
		 {C, 0, 0, 0},	 {C, 0, 0, 0},	 {C, 0, 0, 0},	 {C, 0, 0, 0},
		 {0, C, 0, 0},	 {0, C, 0, 0},	 {0, 0, C, 0},	 {0, 0, C, 0},
		 // vertex
		{-S, S, S, 0},	 {S, S, S, 0},	{-S,-S, S, 0},	 {S,-S, S, 0}, 
		{-S,-S,-S, 0},	 {S,-S,-S, 0},	{-S, S,-S, 0},	 {S, S,-S, 0},
		 // normal
		 {0, 0, 1, 0},	 {0, 0, 1, 0},	 {0, 0, 1, 0},	 {0, 0, 1, 0},
		 {0,-1, 0, 0},	 {0,-1, 0, 0},	 {0, 0,-1, 0},	 {0, 0,-1, 0},
		 // texture (not used)
		 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},
		 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},

		 // 2nd vertex (8 length triangle strip)
		 {0, 0, 0, 8},
		 {C, C, 0, 0},	 {C, C, 0, 0},	 {C, C, 0, 0},	 {C, C, 0, 0},
		 {0, C, C, 0},	 {0, C, C, 0},	 {C, 0, C, 0},	 {C, 0, C, 0},
		{-S,-S, S, 0},	{-S,-S,-S, 0},	{-S, S, S, 0},	{-S, S,-S, 0},
		{ S, S, S, 0},	 {S, S,-S, 0},	 {S,-S, S, 0},	 {S,-S,-S, 0},
		{-1, 0, 0, 0},	{-1, 0, 0, 0},	{-1, 0, 0, 0},	{-1, 0, 0, 0},
		 {0, 1, 0, 0},	 {0, 1, 0, 0},	 {1, 0, 0, 0},	 {1, 0, 0, 0},
		 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},
		 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},

		 // terminator
 		 {0, 0, 0, 0},
	};

	// rounding box for object clip
	static VECTOR b[] = {
		{-S,-S,-S, 1},	  {S,-S,-S, 1},	 {-S, S,-S, 1},	 {S, S,-S, 1}, 
		{-S,-S, S, 1},	  {S,-S, S, 1},	 {-S, S, S, 1},	 {S, S, S, 1}, 
	};
	OBJECT	object;
	memset(&object, 0, sizeof(OBJECT));
	objMakeTag(&object, 0, 0, 0, 0);	// GIFtag
	object.view   = &view;			// camera
	object.light  = &light;			// light
	object.mw     = mw;			// model-world matrix
	object.shadow = 1;			// shadow
	object.clip   = 0;			// clip detection
	object.spec   = 0;			// specular
	object.rate   = 0.0;			// morphing rate
	object.cvnt   = cvnt;			// cvnt
	object.c      = 0;			// no fixed color
	object.box    = b;			// rounding box
		object.driver3 = objDrvSimple;		// object driver
		
	objDraw(&object);			// draw
}

// 
// 	main entry
// 
// 関数 draw() が毎フレーム呼び出されます。
// 各設定の初期やフレームダブルバッファのハンドリングは sce_pp3.c で
// まとめて記述されます。関数 draw() はすべての雑用が処理された後呼び
// 出されます。従ってここでは GS の設定に関する情報とコードは含まれま
// せん。
// 
int draw(int key_code)
{
	static float	rotx = 0.0, roty =  0.0;	// rotation angle	
	static int	lmodel = 2;			// lighting mode
	static qword	p3_buf[2][0x20000];		// packet buffer
	static int	bufid = 0;			// buffer ID
	MATRIX		mw;				// model world
	int		ix, iy;

	// update your position
	switch (key_code) {
	    case 'n': AddVector(view.p, view.p, v);		break;
	    case 'p': SubVector(view.p, view.p, v);		break;
	    case 'f': light.lp0[2] -= 3;			break;
	    case 'b': light.lp0[2] += 3;			break;
	    case 'l': lmodel = (lmodel+1)%4; key_code = 0;	break;
	}

	// stop your postion to avoid blackout
	if (view.p[1] < -500 || view.p[1] > 500)
		key_code = 0;

	// stop light position
	light.lp0[2] = limit(light.lp0[2], 50, 400);

	// set eye point
	// keep your eye on (0,0,0)
	ScaleVector(view.zd, view.p, -1);

	// update matrix
	updateView(&screen, &view);
	updateLight(&light);

	// setup
	objInit(0, p3_buf[bufid]);
	bufid = 1-bufid;

	// make display list
	light.model = 3;
	draw_zplane(N, 40);	

	// update cube rotation
	rotx += 0.02;
	roty += 0.04;
	if (rotx > PI)	rotx -= 2*PI;
	if (roty > PI)	roty -= 2*PI;

	UnitMatrix(mw);
	RotMatrixX(mw, mw, rotx);
	RotMatrixY(mw, mw, roty);

	// draw cubes
	// Note that mw[3] indicates translation.
	light.model = lmodel;
	for (iy = -N; iy < N; iy += 40)
		for (ix = -N; ix < N; ix += 40) {
			setVector(mw[3], ix, iy, 15, 1);
			draw_cube(&mw);
		}

	// kick DMA
	objFlush();

	// return
	return(key_code);
}


// entry for OpenGL simulator
//
// 以下の行は OpenGL を用いたシミュレータ環境上で動作するためのもので
// す。通常は OPENGL を定義する必要ありません。
//
#ifdef OPENGL
void usage(void)
{
	printf("spot light operation test: \n");
	printf("\tn/p:        move camera\n");
	printf("\tf/b:        move light source \n");
	printf("\tl:          toggle light mode (parallel/spot)\n");
	printf("\t[SPACE]:    pause\n");
	printf("\t[ESC]:      quit\n");
}
#else
void main1(void)
{
	int i;
	while (1) {
		for (i = 0; i < 180; i++)
			draw(' ');
		while (draw('n'));

		for (i = 0; i < 180; i++)
			draw(' ');
		while (draw('p'));
	}
}
#endif
