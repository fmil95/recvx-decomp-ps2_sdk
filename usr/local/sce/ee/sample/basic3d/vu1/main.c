/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *                    Basic Sample Program
 *                        Version 0.30
 *                            EUC
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *       Version   Date           Design              Log
 *  --------------------------------------------------------------------
 *       0.30      May,17,1999    munekis & thatake   VU1 version
 */

#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <libdev.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
// #include <libpkt.h>
// #include <libvu0.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <libpad.h>

#include "mathfunc.h"

extern u_int My_dma_start __attribute__((section(".vudata")));

extern float My_matrix[] __attribute__((section(".vudata")));
extern float My_rot_trans_matrix[] __attribute__((section(".vudata")));
extern float My_light_matrix[] __attribute__((section(".vudata")));
extern float My_lcolor_matrix[] __attribute__((section(".vudata")));

extern u_int My_cube_start __attribute__((section(".vudata")));
extern u_int My_torus_start __attribute__((section(".vudata")));
extern u_int My_dma_next  __attribute__((section(".vudata")));
extern u_int My_texture1  __attribute__((section(".vudata")));

void SetVu1PacketMatrix(void);

#define PI 3.14159265358979323846f
#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 224.0f
#define SCREEN_OFFSET_X 1728.0f
#define SCREEN_OFFSET_Y 1936.0f

#define IMAGE_SIZE 256

typedef struct {
	sceGifTag giftag;
	sceGsTest gs_test;
        long gs_test1addr;
        sceGsAlphaEnv gs_alpha;
	sceGsTexEnv gs_tex;
}TexEnv;

sceSamp0FVECTOR camera_p = { 0, 0, -25, 0 };
sceSamp0FVECTOR camera_zd = { 0, 0, 1, 1 };
sceSamp0FVECTOR camera_yd = { 0, 1, 0, 1 };
sceSamp0FVECTOR camera_rot = { 0, 0, 0, 0 };

sceSamp0FVECTOR light0 = { 0.0, 1.5, 0.5, 0 };
sceSamp0FVECTOR light1 = { 1.5, -0.5, 0.5, 0 };
sceSamp0FVECTOR light2 = { -1.5, -0.5, 0.5, 0 };

sceSamp0FVECTOR color0  = { 0.3, 0.3, 0.8, 0 };
sceSamp0FVECTOR color1  = { 0.8, 0.3, 0.3, 0 };
sceSamp0FVECTOR color2  = { 0.3, 0.8, 0.3, 0 };

sceSamp0FVECTOR ambient = { 0.2, 0.2, 0.2, 0 };

sceSamp0FVECTOR obj_trans = { 0, 0, 0, 0 };
sceSamp0FVECTOR obj_rot = { 0, 0, 0, 0 };

sceSamp0FMATRIX local_world;
sceSamp0FMATRIX world_view;
sceSamp0FMATRIX view_screen;
sceSamp0FMATRIX local_screen;

sceSamp0FMATRIX normal_light;
sceSamp0FMATRIX light_color;
sceSamp0FMATRIX local_light;
sceSamp0FMATRIX local_color;

sceSamp0FMATRIX work;

u_int paddata;
u_char rdata[32];
u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

sceGsDBuff      db;
sceGsLoadImage  gs_limage;
TexEnv          texenv;


int main()
{
    	int    odev;
    	u_int  frame;
        float  delta = 1.0f / 180.0f * PI;
	int    obj_switch = 0;
	int    toggle = 0;

	sceSifInitRpc(0);
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
			    0, NULL) < 0){
	    printf("Can't load module sio2man\n");
	}
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
			    0, NULL) < 0){
	    printf("Can't load module padman\n");
	}

	sceGsResetPath();
	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

	// --- set double buffer ---
	sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224,
		SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
	*(u_long *)&db.clear0.rgbaq =
		SCE_GS_SET_RGBAQ(0x10, 0x10, 0x20, 0x80, 0x3f800000);
	*(u_long *)&db.clear1.rgbaq =
		SCE_GS_SET_RGBAQ(0x10, 0x10, 0x20, 0x80, 0x3f800000);


	// --- set load image 1---
	sceGsSetDefLoadImage(&gs_limage,
			     6720,
			     IMAGE_SIZE / 64,
			     0,
			     0, 0,
			     IMAGE_SIZE, IMAGE_SIZE);

	FlushCache(0);
	sceGsExecLoadImage(&gs_limage, (u_long128*) &My_texture1);
	sceGsSyncPath(0, 0);

	// --- set texture env 1 ---
	sceGsSetDefTexEnv(&texenv.gs_tex, 0, 6720, IMAGE_SIZE / 64, SCE_GS_PSMCT32,
			  8, 8, 0, 0, 0, 0, 1);
	SCE_GIF_CLEAR_TAG(&texenv.giftag);
	texenv.giftag.NLOOP = 9;
	texenv.giftag.EOP = 1;
	texenv.giftag.NREG = 1;
	texenv.giftag.REGS0 = 0xe;
	sceGsSetDefAlphaEnv(&texenv.gs_alpha, 0);
	*(u_long *) &texenv.gs_alpha.alpha1 = SCE_GS_SET_ALPHA(0, 1, 0, 1, 0);
	*(u_long *) &texenv.gs_test =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 3, 0x3);
	texenv.gs_test1addr = (long)SCE_GS_TEST_1;
	*(u_long*)&texenv.gs_tex.clamp1 = SCE_GS_SET_CLAMP(0, 0, 0, 0, 0, 0);
	// *(u_long*)&texenv.gs_tex.tex11 = SCE_GS_SET_TEX1(1, 0, 1, 1, 0, 0, 0);
	FlushCache(0);
	sceGsPutDrawEnv(&texenv.giftag);
	sceGsSyncPath(0, 0);

	// --- open pad ---
	scePadInit(0);
	scePadPortOpen(0, 0, pad_dma_buf);

        frame = 0;
	odev = !sceGsSyncV(0);


	while(1){

		// --- read pad ---
	    if(scePadRead(0, 0, rdata) > 0){
		paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
	    }
	    else{
		paddata = 0;
	    }

		// --- object rotate & change view point ---
                if(paddata & SCE_PADLdown){
		  obj_rot[0] += delta; if(obj_rot[0]>PI) obj_rot[0] -= 2.0f*PI; }
		if(paddata & SCE_PADLup){
		  obj_rot[0] -= delta; if(obj_rot[0]<-PI) obj_rot[0] += 2.0f*PI; }
		if(paddata & SCE_PADLright){
		  obj_rot[1] += delta; if(obj_rot[1]>PI) obj_rot[1] -= 2.0f*PI; }
		if(paddata & SCE_PADLleft){
		  obj_rot[1] -= delta; if(obj_rot[1]<-PI) obj_rot[1] += 2.0f*PI; }
		if(paddata & SCE_PADL1){
		  obj_rot[2] += delta; if(obj_rot[2]>PI) obj_rot[2] -= 2.0f*PI; }
		if(paddata & SCE_PADL2){
		  obj_rot[2] -= delta; if(obj_rot[2]<-PI) obj_rot[2] += 2.0f*PI; }
		if(paddata & SCE_PADRdown){
		  camera_rot[0] += delta; if(camera_rot[0]>PI*0.4) camera_rot[0] = 0.4f*PI; }
		if(paddata & SCE_PADRup){
		  camera_rot[0] -= delta; if(camera_rot[0]<-PI*0.4) camera_rot[0] = -0.4f*PI; }
		if(paddata & SCE_PADRright){
		  camera_rot[1] += delta; if(camera_rot[1]>PI) camera_rot[1] -= 2.0f*PI; }
		if(paddata & SCE_PADRleft){
		  camera_rot[1] -= delta; if(camera_rot[1]<-PI) camera_rot[1] += 2.0f*PI; }
		if(paddata & SCE_PADR1){
		  camera_p[2] -= delta*5; if(camera_p[2]<-100) camera_p[2] = -100; }
		if(paddata & SCE_PADR2){
		  camera_p[2] += delta*5; if(camera_p[2]>-10) camera_p[2] = -10; }
		if(!toggle && (paddata & SCE_PADselect)){
		  obj_switch = (++obj_switch)&0x01; toggle = 1;
		}else if(!(paddata & SCE_PADselect)){toggle = 0;}

		FlushCache(0);
		sceGsSwapDBuff(&db, frame);
		sceGsSyncPath(0, 0);

		// local -> world (rotate)matrix
		sceSamp0UnitMatrix(work);
		sceSamp0RotMatrix(local_world, work, obj_rot);

		// color&normal matrix setting
		sceSamp0NormalLightMatrix(normal_light, light0, light1, light2);
		sceSamp0LightColorMatrix(light_color, color0, color1, color2, ambient);

		// light(normal) -> local_light matrix
		sceSamp0MulMatrix(local_light, normal_light, local_world);

		// local -> world (rotate&translate)matrix
		sceSamp0TransMatrix(local_world, local_world, obj_trans);

		// world -> view matrix
		sceSamp0RotCameraMatrix(world_view,
				   camera_p, camera_zd, camera_yd, camera_rot);

		// view -> screen matrix
		sceSamp0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
				 2048.0f, 2048.0f, 1.0f, 16777215.0f,
				 1.0f,65536.0f);

		// local -> screen matrix
		sceSamp0MulMatrix(local_screen, view_screen, world_view);

		SetVu1PacketMatrix();

		if(obj_switch==0) *((&My_dma_next)+1) = (u_int)(&My_cube_start);
		else if(obj_switch==1)  *((&My_dma_next)+1) = (u_int)(&My_torus_start);
		// --- kick dma to draw object ---
		*D1_QWC  = 0x00;
		*D1_TADR = (u_int)&My_dma_start & 0x0fffffff;
		*D_STAT = 2;
		FlushCache(0);
		*D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
		sceGsSyncPath(0, 0);

		sceGsSetHalfOffset((frame&1) ? &db.draw1 : &db.draw0, 2048,2048,odev);
// 		printf("running\n");
		frame++;
		odev = !sceGsSyncV(0);
	}

	printf("finish!\n");
	return 0;

}

void SetVu1PacketMatrix(void){

// My_light_matrix <- local_light
    My_light_matrix[0] = local_light[0][0];
    My_light_matrix[1] = local_light[1][0];
    My_light_matrix[2] = local_light[2][0];
    My_light_matrix[3] = local_light[3][0];

    My_light_matrix[4] = local_light[0][1];
    My_light_matrix[5] = local_light[1][1];
    My_light_matrix[6] = local_light[2][1];
    My_light_matrix[7] = local_light[3][1];

    My_light_matrix[8] = local_light[0][2];
    My_light_matrix[9] = local_light[1][2];
    My_light_matrix[10] = local_light[2][2];
    My_light_matrix[11] = local_light[3][2];

    My_light_matrix[12] = local_light[0][3];
    My_light_matrix[13] = local_light[1][3];
    My_light_matrix[14] = local_light[2][3];
    My_light_matrix[15] = local_light[3][3];

// My_lcolor_matrix <- light_color
    My_lcolor_matrix[0] = light_color[0][0];
    My_lcolor_matrix[1] = light_color[1][0];
    My_lcolor_matrix[2] = light_color[2][0];
    My_lcolor_matrix[3] = light_color[3][0];

    My_lcolor_matrix[4] = light_color[0][1];
    My_lcolor_matrix[5] = light_color[1][1];
    My_lcolor_matrix[6] = light_color[2][1];
    My_lcolor_matrix[7] = light_color[3][1];

    My_lcolor_matrix[8] = light_color[0][2];
    My_lcolor_matrix[9] = light_color[1][2];
    My_lcolor_matrix[10] = light_color[2][2];
    My_lcolor_matrix[11] = light_color[3][2];

    My_lcolor_matrix[12] = light_color[0][3];
    My_lcolor_matrix[13] = light_color[1][3];
    My_lcolor_matrix[14] = light_color[2][3];
    My_lcolor_matrix[15] = light_color[3][3];

// My_rot_trans_matrix <- world_view
    My_rot_trans_matrix[0] = local_world[0][0];
    My_rot_trans_matrix[1] = local_world[1][0];
    My_rot_trans_matrix[2] = local_world[2][0];
    My_rot_trans_matrix[3] = local_world[3][0];

    My_rot_trans_matrix[4] = local_world[0][1];
    My_rot_trans_matrix[5] = local_world[1][1];
    My_rot_trans_matrix[6] = local_world[2][1];
    My_rot_trans_matrix[7] = local_world[3][1];

    My_rot_trans_matrix[8] = local_world[0][2];
    My_rot_trans_matrix[9] = local_world[1][2];
    My_rot_trans_matrix[10] = local_world[2][2];
    My_rot_trans_matrix[11] = local_world[3][2];

    My_rot_trans_matrix[12] = local_world[0][3];
    My_rot_trans_matrix[13] = local_world[1][3];
    My_rot_trans_matrix[14] = local_world[2][3];
    My_rot_trans_matrix[15] = local_world[3][3];

// My_matrix <- local_screen
    My_matrix[0] = local_screen[0][0];
    My_matrix[1] = local_screen[1][0];
    My_matrix[2] = local_screen[2][0];
    My_matrix[3] = local_screen[3][0];

    My_matrix[4] = local_screen[0][1];
    My_matrix[5] = local_screen[1][1];
    My_matrix[6] = local_screen[2][1];
    My_matrix[7] = local_screen[3][1];

    My_matrix[8] = local_screen[0][2];
    My_matrix[9] = local_screen[1][2];
    My_matrix[10] = local_screen[2][2];
    My_matrix[11] = local_screen[3][2];

    My_matrix[12] = local_screen[0][3];
    My_matrix[13] = local_screen[1][3];
    My_matrix[14] = local_screen[2][3];
    My_matrix[15] = local_screen[3][3];
}


