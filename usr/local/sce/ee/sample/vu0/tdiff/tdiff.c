/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - tdiff -
 *
 *                         Version 0.10 
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            tdiff.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *	0.1		Jul,13,1998	suzu	    1st version
 *	0.2		Dec,23,1999	suzu	    changed for libgraph
 *	0.3		May,13,1999	suzu	    add comment
 */
// これは、特殊なオブジェクトのディスプレイリストを作成するサンプルです。
// 通常のオブジェクトはオブジェクトを構成するポリゴンには同じマトリク
// スが適用されます。このマトリクスをポリゴンごとにある範囲内でディス
// トーションを与えることで、霧や多数の破片を疑似的に表現することがで
// きます。
//
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
	1024,			// distance to screen
	1.0,	0.5,		// aspect ratio
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
	{ 0,   0,-40,1},{  0,  0, 40,1},{-40, 40, 40,1},// light source position
	{ 1,   1,  1,0},{  0,  0, -1,0},{  1, -1, -1,0},// light direction 
	{0.8,0.2,0.2,0},{0.2,0.8,0.0,2},{0.0,0.0,0.0,0},// light influence
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
	{    0,   0,  -1, 0},		// foot direction
	{    1,  -1,  -1, 0},		// eye direction 
	{ -600,  50,  50, 1},		// standing position
};

// Your moving speed
// カメラの移動速度を指定します。この値がフレーム毎に view.p より加算
// されます。 
static VECTOR	v = {1.0, 0, 0, 0};	// your speed

//
// パーティクルのパラメータを指定します。
//
#define C	240	// Color
#define S	  5	// Size
#define O	  6	// Origin

// special object drivers
extern void objDrvParticleAttributes(VECTOR r, int n, int s);
extern qword *objDrvParticle(qword *pbase, OBJECT *obj);

static void init_cube(OBJECT *obj)
{
	// Color, Vertex, Normal, (Texture)
	static VECTOR	cvnt[] = {
	{   0,   0,   0,4},
	{   C,   C,   C,0},{  C,   C,   C,0},{   C,   C,   C,0},{  C,   C,   C,0},
	{-S+O, S+O, S+O,0},{S+O, S+O, S+O,0},{-S+O,-S+O, S+O,0},{S+O,-S+O, S+O,0}, 
	{   0,   0,   1,0},{  0,   0,   1,0},{   0,   0,   1,0},{  0,   0,   1,0},
	{   0,   0,   0,0},{  0,   0,   0,0},{   0,   0,   0,0},{  0,   0,   0,0},
	};

	// setup member
	memset(obj, 0, sizeof(OBJECT));
	objMakeTag(obj, 0, 0, 0, 0);	// GIFtag
	obj->spec    = 0;		// specular	
	obj->shadow  = 0;		// shadow to z-plane
	obj->rate    = 0.0;		// morphing rate
	obj->clip    = 1;		// polygon clip
	obj->box     = 0;		// rounding box
	obj->cvnt    = cvnt;		// vertex
	obj->c       = 0;		// no fixed color
	obj->view    = &view;		// camera
	obj->light   = &light;		// light
	obj->driver3 = objDrvParticle;	// driver
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
	static qword	p3_buf[2][0x10000];	// packet buffer
	static int	bufid = 0;		// buffer ID
	static VECTOR	orot, prot;		// object/particle rotation 
	OBJECT	object;
	MATRIX	mw;

	// update camera postion
	switch (key_code) {
	    case 'n': AddVector(view.p, view.p, v); break;
	    case 'p': SubVector(view.p, view.p, v); break;
	    case 'l':
		light.model = (light.model+1)%4;
		break;
	}

	// stop moving to avoid black out
	if (view.p[0] < -300 || view.p[0] > 300)
		key_code = 0;

	// rotate each particle and it's parents
	orot[0] += 0.5*PI/180;	// parent rotation
	orot[1] += 0.2*PI/180;
	prot[0] += 1.0*PI/180;	// particle rotation
	prot[1] += 2.0*PI/180;

	if (prot[0] > PI) prot[0] -= 2*PI;
	if (prot[1] > PI) prot[1] -= 2*PI;
	if (orot[0] > PI) orot[0] -= 2*PI;
	if (orot[1] > PI) orot[1] -= 2*PI;

	// update matrix
	// you always look at (0,0,0)
	ScaleVector(view.zd, view.p, -1);
	updateView(&screen, &view);
	updateLight(&light);

	// clear GIF and GS
	objInit(0, p3_buf[bufid]);
	bufid = 1-bufid;

	// init
	init_cube(&object);
	
	// make matrix
	object.mw = &mw;
	UnitMatrix(mw);
	RotMatrixX(mw, mw, orot[0]);
	RotMatrixY(mw, mw, orot[1]);

	// set particle attributes: see objpart.c
	objDrvParticleAttributes(prot, 5500, 128);
	objDraw(&object);

	// kick DMA
	objFlush(0);

	// return
	return(key_code);
}

// entry for OpenGL simulator
//
// 以下の行は OpenGL を用いたシミュレータ環境上で動作するためのもので
// す。通常は OPENGL を定義する必要ありません。
//
#ifdef OPENGL
void usage(void) {}
#else
void main1(void)
{
	int i;
	while (1) {
		for (i = 0; i < 1200; i++) draw('n');
		for (i = 0; i < 1200; i++) draw('p');
	}
}
#endif
