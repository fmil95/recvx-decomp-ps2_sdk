/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - tsquare -
 *
 *                         Version 0.10 
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            tsquare.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *	0.1		Jul,13,1998	suzu	    1st version
 *	0.2		Dec,23,1999	suzu	    changed for libgraph
 *	0.3		May,13,1999	suzu	    add comment
 */
// 
//  これは、VPU0 の座標変換速度を評価するためのサンプルです。
//  同時座標系の基本的な動作に関しては、tballs.c を参照下さい。
//  
//  サンプルには以下のローカルヘッダファイル object.h が必要です。
//  SCREEN, VIEW, LIGHT に関する動作の詳細は tballs.c を参照下さい。
//  頂点の指定方法に関しては tfog.c を参照下さい。
//  実際のオブジェクトの操作は、objfastA.c の objDrvFast() 関数が行ないます。
//  そちらも参照下さい。
//  
//  サンプルには以下のローカルヘッダファイル object.h が必要です。
// 	
#include "object.h"

// 
// Screen information
// 
SCREEN screen = {
	1024,		// distance to screen
	1.0,	0.5,	// aspect ratio
};

// 
// Lighting information
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
// Camera position
// 
VIEW view = {
	{    0,   0,  -1, 0},	// foot direction
	{    0,   0,   1, 0},	// eye direction 
	{ -250,-850, 300, 1},	// standing position
};

// your speed
static VECTOR	 v = {0.5, 2.5, 0, 0};

#define C	180	// cube color
#define S	5	// cube size

#define NX	14	// total NX*NY*NZ cubes
#define NY	14	
#define NZ	14

static void init_cube(OBJECT *obj)
{
	// cube vertex (color + vertex + normat + texture
	static VECTOR	cvnt[] = {

		 {0, 0, 0, 8},	// 8 length triangle mesh
		 // color
		 {C, 0, 0, 0},	 {C, 0, 0, 0},	 {C, 0, 0, 0},	 {C, 0, 0, 0},
		 {0, C, 0, 0},	 {0, C, 0, 0},	 {0, 0, C, 0},	 {0, 0, C, 0},
		 // vertex
		{-S, S, S, 0},	 {S, S, S, 0},	{-S,-S, S, 0},	 {S,-S, S, 0}, 
		{-S,-S,-S, 0},	 {S,-S,-S, 0},	{-S, S,-S, 0},	 {S, S,-S, 0},
		 // normal
		 {0, 0, 1, 0},	 {0, 0, 1, 0},	 {0, 0, 1, 0},	 {0, 0, 1, 0},
		 {0,-1, 0, 0},	 {0,-1, 0, 0},	 {0, 0,-1, 0},	 {0, 0,-1, 0},
		 // texture (not used here)
		 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},
		 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},

		 {0, 0, 0, 8},	// another 8 length triangle mesh
		 {C, C, 0, 0},	 {C, C, 0, 0},	 {C, C, 0, 0},	 {C, C, 0, 0},
		 {0, C, C, 0},	 {0, C, C, 0},	 {C, 0, C, 0},	 {C, 0, C, 0},
		{-S,-S, S, 0},	{-S,-S,-S, 0},	{-S, S, S, 0},	{-S, S,-S, 0},
		 {S, S, S, 0},	 {S, S,-S, 0},	 {S,-S, S, 0},	 {S,-S,-S, 0},
		{-1, 0, 0, 0},	{-1, 0, 0, 0},	{-1, 0, 0, 0},	{-1, 0, 0, 0},
		 {0, 1, 0, 0},	 {0, 1, 0, 0},	 {1, 0, 0, 0},	 {1, 0, 0, 0},
		 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},
		 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},	 {0, 0, 0, 0},

 		 {0, 0, 0, 0},
	};

	// setup member
	memset(obj, 0, sizeof(OBJECT));
	objMakeTag(obj, 0, 0, 0, 0);		// make GIFtag
	obj->view    = &view;			// camera
	obj->light   = &light;			// light
	obj->shadow  = 0;			// shadown to z-plane
	obj->clip    = 0;			// polygon clip
	obj->spec    = 0;			// specular
	obj->rate    = 0.0;			// morphing rate
	obj->cvnt    = cvnt;			// vertex
	obj->box     = 0;			// no rounding box detection
	obj->c       = 0;			// no default color
// 	obj->driver3 = objDrvFast;		// fast driver (C version)
	obj->driver3 = objDrvFastA;		// fast driver (asm version)
}


int draw(int key_code)
{
	static float	rotx = 0.0, roty =  0.0;	// rotation angle	
	static qword	p3_buf[2][0x20000];		// packet buffer
	static int	bufid = 0;

	int	ix, iy, iz;
	float	x, y, z;
	OBJECT	object;
	MATRIX	mw;

	switch (key_code) {
	    case 'n': AddVector(view.p, view.p, v);		break;
	    case 'p': SubVector(view.p, view.p, v);		break;
	    case 'l':
		    light.model = 1-light.model; key_code = 0;	
		    break;
	}
	
	// update
	rotx += 0.10;
	roty += 0.05;
	if (rotx > PI) rotx -= 2*PI;
	if (roty > PI) roty -= 2*PI;

	// update matrix
	ScaleVector(view.zd, view.p, -1);	// keep your eye on (0,0,0)
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
	RotMatrixX(mw, mw, rotx);
	RotMatrixY(mw, mw, roty);

	// draw
	for (iz = 0, z = -NZ*10; iz < NZ; iz++, z += 20)
		for (iy = 0, y = -NY*10; iy < NY; iy++, y += 20)
			for (ix = 0, x = -NX*10; ix < NX; ix++, x += 20) {
				setVector(mw[3], x, y, z, 1);
				objDraw(&object);
			}

	// kick DMA
	objFlush(0);

	// return
	return(key_code);
}

#ifdef OPENGL
void usage(void) {}
#else
void main1(void)
{
	int i;

	while (1) {
		for (i = 0; i < 800; i++) draw('n');
		for (i = 0; i < 800; i++) draw('p');
	}
}
#endif
