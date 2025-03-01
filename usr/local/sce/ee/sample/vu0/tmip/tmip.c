/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - tmip -
 *
 *                         Version 0.10 
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            tmip.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *	0.1		Jul,13,1998	suzu	    1st version
 *	0.2		Dec,23,1999	suzu	    changed for libgraph
 *	0.3		May,13,1999	suzu	    add comment
 */

//
// これは、オブジェクトの各頂点を２組持ち、２つを線形内挿して新しい頂
// 点を生成する例です。これは MIPMODEL においてモデル間の内挿を行なう
// 場合に使用します。 
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
	1,						 // light mode
	// 	light-0		light-1		light-2
	{  0,  0,150,1}, { 90,  0, 90,1},{  0, 90, 90,1},// light position
	{  0,  0,  0,0}, {  0,  0,  0,0},{  0,  0,  0,0},// light direction
	{0.8,0.0,0.0,0}, {0.0,0.8,0.0,0},{0.0,0.0,0.8,0},// influence
	{0.2,0.2,0.2,0},				 // ambient
};

// 
// 	Camera position information
// 
// カメラ位置を指定します。カメラは鉛直方向に立てられ、常に原点を向く
// ように修正されます。すなわち view.zd = -vies.p となるようにカメラ
// の向きが設定されます。
//
VIEW view = {
	{   0,  0,  -1, 0},		// foot direction
	{   1,  0,  -1, 0},		// eye direction 
	{-200,  0, 600, 1},		// standing position
};

// Your moving speed
// カメラの移動速度を指定します。この値がフレーム毎に view.p より加算
// されます。 
static VECTOR	 v = {0, 1, -2.5, 0};

// 
// initialize object vertex
// 
// オブジェクトの頂点を設定します。オブジェクトは３種類ありそれぞれ以
// 下の形状を持つデータが割り当てられます。
//
//			cvnt		cvnt1
//			---------------------	
//			球		円筒
//			円筒		円錐
//			円錐		球
//
// 内挿係数 rate の値に応じて、cvnt, cvnt1 で定義された二つの形状
// が重み付け平均され新しい頂点が生成されます。
//
// カウンタオブジェクト (cvnt1) の生成には一旦ダミーのオブジェクト 
// wbase を生成しその頂点データのポインタを obj にコピーする方法がとられ
// ます。
//
static void init_morph_obj(OBJECT *obj, 
			int width, int step, void (*func0)(), void (*func1)())
{
	OBJECT	wbase;

	// set basic attributes
	memset(obj, 0, sizeof(OBJECT));
	objMakeTag(obj, 0, 0, 0, 1);
	obj->view    = &view;
	obj->light   = &light;
	obj->shadow  = 1;
	obj->clip    = 0;
	obj->spec    = 0;
	obj->rate    = 0.0;
	obj->c       = malloc(sizeof(VECTOR));
	obj->c1      = malloc(sizeof(VECTOR));
	obj->box     = malloc(8*sizeof(VECTOR));
	obj->driver3 = objDrvNormal;

	setVector(obj->c[0],  100, 200, 255, 0);
	setVector(obj->c1[0], 200, 100,   0, 0);

	// map
	(*func0)(obj,    width, step);
	(*func1)(&wbase, width, step);

	// set counter vertex
	obj->cvnt1 = wbase.cvnt;

}

// 
// draw objects
// 
static void draw_obj(float rotx, float roty, float rate)
{
	static OBJECT	obj0, obj1, obj2;
	static int	first = 1;
	static float	r[] = {1, 2.5, 4, -1};
	MATRIX		mw;
	int		i;
	float		dr = 0.3;

	// if first, generate vertex
	if (first) {
		first = 0;
		init_morph_obj(&obj0, 30, 4, map_corn,     map_sphere);
		init_morph_obj(&obj1, 30, 4, map_sphere,   map_cylinder);
		init_morph_obj(&obj2, 30, 4, map_cylinder, map_corn);
	}

	// make rotation
	UnitMatrix(mw);
	RotMatrixX(mw,  mw, rotx);
	RotMatrixY(mw,  mw, roty);

	// setup
	obj0.mw   = obj1.mw   = obj2.mw   = &mw;

	for (i = 0; r[i] > 0; i++) {

		// update morphing rate
		if (rate > 1.0)	{ rate = 2.0 - rate; dr = -0.3;}
		if (rate < 0.0)	{ rate = 0.0 - rate; dr = +0.3;}
		
		obj0.rate = obj1.rate = obj2.rate = rate;
		rate += dr;

		// draw object 0
		setVector(mw[3],   0*1.72*r[i],  60*r[i], 42, 1); 
		objDraw(&obj0);
		
		// draw object 1
		setVector(mw[3],  30*1.72*r[i], -30*r[i], 42, 1); 
		objDraw(&obj1);

		// draw object 2
		setVector(mw[3], -30*1.72*r[i], -30*r[i], 42, 1); 
		objDraw(&obj2);
	}
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
	static float	rotx = 0.0, roty =  0.0, rate = 1.0;
	static qword	p3_buf[2][0x10000];		// packet buffer
	static int	bufid = 0;
	
	switch (key_code) {
		case 'n': AddVector(view.p, view.p, v);	break;
		case 'p': SubVector(view.p, view.p, v);	break;
		case 'f': light.lp0[2] -= 3;		break;
		case 'b': light.lp0[2] += 3;		break;
		case 'l':
			light.model = (light.model+1)%4;
			key_code = 0;
			break;	
	}
	// update
	rotx += 0.01;
	roty += 0.02;

	if (rotx > PI) rotx -= 2*PI;	
	if (roty > PI) roty -= 2*PI;	

	// for demonstration
	rate += 0.02;
	if (rate > 2.0) rate -= 2.0;

	// break system 
	if (view.p[2] < 32 || view.p[2] > 1024)
		key_code = 0;
	light.lp0[2] = limit(light.lp0[2], 40, 800);

	// update matrix
	// you always look at (0,0,0)
	ScaleVector(view.zd, view.p, -1);
	updateView(&screen, &view);
	updateLight(&light);

	// clear GIF and GS
	objInit(0, p3_buf[bufid]);
	bufid = 1-bufid;

	// draw reference plane
	light.model = 3;
	draw_zplane(280, 40);

	// draw object
	light.model = 2;
	draw_obj(rotx, roty, rate);

	// kick
	objFlush();

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
	printf("linear interporation test (for MIPMODEL):\n");
	printf("\tj/k:        morphing\n");
	printf("\tn/p:        move camera\n");
	printf("\tf/b:        move light source \n");
	printf("\tl:          toggle light mode (parallel/spot)\n");
	printf("\t[SPACE]:    pause\n");
	printf("\t[ESC]:      quit\n");
}
#else
void main1(void)
{
	int i, key_code = 'n';
	while (1) {
		while ((key_code = draw('n')));
		for (i = 0; i < 300; i++) draw(0);
		while ((key_code = draw('p')));
		for (i = 0; i < 600; i++) draw(0);
	}
}
#endif
