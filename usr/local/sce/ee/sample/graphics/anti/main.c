/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                    -- AntiAlias sample --
 *
 *                        Version 0.5.0
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : main.c
 *
 *       Version   Date           Design       Log
 *  --------------------------------------------------------------------
 *       0.5.0     Oct,12,1999    thatake      first version
 */

#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <libdev.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <libpad.h>

#include "mathfunc.h"

extern u_int My_dma_start __attribute__((section(".vudata")));
extern u_int My_dma_start2 __attribute__((section(".vudata")));
extern u_int My_cup_start[] __attribute__((section(".vudata")));
extern u_int My_cup_blockcenter __attribute__((section(".vudata")));
extern u_int My_cup_ini[] __attribute__((section(".vudata")));
extern u_int My_giftag __attribute__((section(".vudata")));
extern float My_coefficient __attribute__((section(".vudata")));

extern float My_matrix[] __attribute__((section(".vudata")));
extern float My_light_matrix[] __attribute__((section(".vudata")));
extern float My_light_color_matrix[] __attribute__((section(".vudata")));

void SetVu1PacketMatrix(void);
void memswap(int *a, int *b, int size);
void zqsort(void *base, void *qword, int n, int size);

// ---- structures and defines for physics ----

#define PI 3.14159265358979323846f
#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 224.0f
#define SCREEN_OFFSET_X 1728.0f
#define SCREEN_OFFSET_Y 1936.0f

#define IMAGE_SIZE 256

#define VIEW_LENGTH 20.0f
#define INIT_THETA -90.0f
#define INIT_PHI 0.0f
#define ANGLE_DELTA 1.0f

typedef struct {
        sceGifTag giftag;
        sceGsTest gs_test;
        long gs_test1addr;
        sceGsAlphaEnv gs_alpha;
        sceGsTexEnv gs_tex;
}TexEnv;

sceSamp0FVECTOR camera_p =   { 50.0f, -31.0f, 50.0f, 0.0f };
sceSamp0FVECTOR camera_zd =  { -50.0f, 31.0f, -50.0f, 1.0f };
sceSamp0FVECTOR camera_yd =  { 0.0f, 1.0f, 0.0f, 1.0f };
sceSamp0FVECTOR camera_rot = { 0.0f, 0.0f,   0.0f, 0.0f };
sceSamp0FVECTOR camera_int = { 0.0f, 0.0f, 0.0f, 0.0f };

sceSamp0FVECTOR light0 = {  0.2f,  1.5f, 0.5f, 0.0f };
sceSamp0FVECTOR light1 = {  1.0f, -0.5f, -1.5f, 0.0f };
sceSamp0FVECTOR light2 = { -1.0f, -0.5f, -0.5f, 0.0f };

sceSamp0FVECTOR color0  = { 0.8f, 0.8f, 0.8f, 1.0f };
sceSamp0FVECTOR color1  = { 0.0f, 0.0f, 0.0f, 1.0f };
sceSamp0FVECTOR color2  = { 0.0f, 0.0f, 0.0f, 1.0f };

sceSamp0FVECTOR ambient = { 0.3f, 0.3f, 0.3f, 0.0f };

sceSamp0FVECTOR obj_trans = { 0.0f, -2.0f, 0.0f, 0.0f };
sceSamp0FVECTOR obj_rot =   { 0.0f, 0.0f, 0.0f, 0.0f };

sceSamp0FMATRIX local_world;
sceSamp0FMATRIX local_view;
sceSamp0FMATRIX world_view;
sceSamp0FMATRIX view_screen;
sceSamp0FMATRIX local_screen;

sceSamp0FMATRIX normal_light;
sceSamp0FMATRIX light_color;
sceSamp0FMATRIX local_light;
sceSamp0FMATRIX local_color;

sceSamp0FMATRIX unit;
sceSamp0FMATRIX work;

u_int paddata;
u_char rdata[32];
u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

sceGsDBuff      db;
sceGsLoadImage  gs_limage;
TexEnv          texenv;

#define SORTNUM 7

int main()
{
        int i;
        int    odev;
        u_int  frame;
        float  delta, theta, phi;
	float  view_length = VIEW_LENGTH;
	int    obj_switch = 0;
        int    clip_switch = 0;
	sceSamp0FVECTOR *cupcenter = (sceSamp0FVECTOR*)&My_cup_blockcenter;
	sceSamp0FVECTOR tmpcenterf;
	float zsort[SORTNUM];

	sceSifInitRpc(0);
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
			    0, NULL) < 0){
	    printf("Can't load module sio2man\n");
	}
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
			    0, NULL) < 0){
	    printf("Can't load module padman\n");
	}
        delta = DEG_TO_RAD(ANGLE_DELTA);
        theta = DEG_TO_RAD(INIT_THETA);
        phi = DEG_TO_RAD(INIT_PHI);

	sceSamp0UnitMatrix(unit);

        sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

        // --- set double buffer ---
        sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224,
                SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);
        *(u_long *)&db.clear0.rgbaq =
                SCE_GS_SET_RGBAQ(0x00, 0x00, 0x30, 0x80, 0x3f800000);
        *(u_long *)&db.clear1.rgbaq =
                SCE_GS_SET_RGBAQ(0x00, 0x00, 0x30, 0x80, 0x3f800000);

        // --- set texture environment ---
        sceGsSetDefTexEnv(&texenv.gs_tex,0,8768,IMAGE_SIZE/64,SCE_GS_PSMCT32,
                          8, 8, 0, 0, 0, 0, 1);
                SCE_GIF_CLEAR_TAG(&texenv.giftag);
                texenv.giftag.NLOOP = 9;
                texenv.giftag.EOP = 1;
                texenv.giftag.NREG = 1;
                texenv.giftag.REGS0 = 0xe;
                sceGsSetDefAlphaEnv(&texenv.gs_alpha, 0);
        *(u_long *) &texenv.gs_alpha.alpha1 = SCE_GS_SET_ALPHA(0, 1, 0, 1, 0);
        *(u_long *) &texenv.gs_test =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 3, 0x3)
;
        texenv.gs_test1addr = (long)SCE_GS_TEST_1;
        *(u_long*)&texenv.gs_tex.clamp1 = SCE_GS_SET_CLAMP(0, 0, 0, 0, 0, 0);
        *(u_long*)&texenv.gs_tex.tex11 = SCE_GS_SET_TEX1(0, 0, 1, 1, 0, 0, 0);

        FlushCache(0);
        sceGsPutDrawEnv(&texenv.giftag);
        sceGsSyncPath(0, 0);


        frame = 0;
        odev = !sceGsSyncV(0);

        // --- pad ---
	scePadInit(0);
	scePadPortOpen(0, 0, pad_dma_buf);

        while(1){
		// --- read pad ---
	    if(scePadRead(0, 0, rdata) > 0){
		paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
	    }
	    else{
		paddata = 0;
	    }

		// --- camera rotation ---
		if(paddata & SCE_PADLup) {
			phi -= delta;
                        if(phi < -0.4999f * PI) phi = -0.4999f * PI;
		} else if(paddata & SCE_PADLdown) {
			phi += delta;
                        if(phi > 0.0f) phi = 0.0f;
		}

		if(paddata & SCE_PADLright) theta += delta;
		else if(paddata & SCE_PADLleft) theta -= delta;

		if(paddata & SCE_PADL1){
		        view_length += 0.5f;
			if(view_length > 100.0f) view_length = 100.0f;
		}
	        else if(paddata & SCE_PADL2){
		        view_length -= 0.5f;
		        if(view_length < 15.0f) view_length = 15.0f;
	        }
		if(paddata & SCE_PADR1){
		  obj_switch = 1;
		}else{obj_switch = 0;}
		if(paddata & SCE_PADR2){
		  clip_switch = 1;
		}else{clip_switch = 0;}

		theta += 0.3*delta;

		if(theta>2*PI) theta -= 2*PI;
		if(theta<-2*PI) theta += 2*PI;

                sceGsSetHalfOffset((frame&1)? &db.draw1 : &db.draw0,
				   2048, 2048, odev);
		FlushCache(0);
                sceGsSwapDBuff(&db, frame);
                sceGsSyncPath(0, 0);

		camera_p[0] = view_length * cosf(phi) * cosf(theta);
		camera_p[1] = view_length * sinf(phi);
		camera_p[2] = view_length * cosf(phi) * sinf(theta);

		camera_zd[0] = camera_int[0] - camera_p[0]; 
		camera_zd[1] = camera_int[1] - camera_p[1]; 
		camera_zd[2] = camera_int[2] - camera_p[2]; 

                sceSamp0LightColorMatrix(light_color, color0, color1, 
					 color2, ambient);

                // world -> view matrix
                sceSamp0RotCameraMatrix(world_view,
                        camera_p, camera_zd, camera_yd, camera_rot);

                // view -> screen matrix
                sceSamp0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
                                 2048.0f, 2048.0f, 1.0f, 16777215.0f,
                                 1.0f, 65536.0f);

                // local -> world (rotate)matrix
		sceSamp0RotMatrix(local_world, unit, obj_rot);

		// local -> light matrix
		sceSamp0NormalLightMatrix(normal_light,light0,light1,light2);
		sceSamp0MulMatrix(local_light, normal_light, local_world);

                // local -> world (rotate&translate)matrix
	        sceSamp0TransMatrix(local_world, local_world, obj_trans);

                // local -> screen matrix
                sceSamp0MulMatrix(local_view, world_view, local_world);
                sceSamp0MulMatrix(local_screen, view_screen, local_view);

		// initialize the pointer of data block (for sorting)
		for(i=0; i<SORTNUM; i++){
		  My_cup_start[0+i*4] = My_cup_ini[0+i*4];
		  My_cup_start[1+i*4] = My_cup_ini[1+i*4];
		  My_cup_start[2+i*4] = My_cup_ini[2+i*4];
		  My_cup_start[3+i*4] = My_cup_ini[3+i*4];
		}

		// sorting blocks by z-value
		for(i=0; i<SORTNUM; i++){
                  sceSamp0ApplyMatrix(tmpcenterf, local_view, cupcenter[i]);
		  zsort[i] = (tmpcenterf[2]*16);
		}
		zqsort(zsort, &My_cup_start, SORTNUM, 4);

		// set GIFtag&clip_value
		if(!obj_switch) 
		  (&My_giftag)[1] = 0x3045c000;
		else (&My_giftag)[1] = 0x3005c000;
		if(!clip_switch)
		  My_coefficient = 600.0f/zsort[0];
		else My_coefficient = 0.0f;

                SetVu1PacketMatrix();

                // --- kick dma to draw object ---
                *D1_QWC  = 0x00;
                *D1_TADR = (u_int)&My_dma_start & 0x0fffffff;
                *D_STAT = 2;
                FlushCache(0);
                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
                sceGsSyncPath(0, 0);

                frame++;
                odev = !sceGsSyncV(0);
        }

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

        // My_light_color
        My_light_color_matrix[0] = light_color[0][0];
        My_light_color_matrix[1] = light_color[1][0];
        My_light_color_matrix[2] = light_color[2][0];
        My_light_color_matrix[3] = light_color[3][0];

        My_light_color_matrix[4] = light_color[0][1];
        My_light_color_matrix[5] = light_color[1][1];
        My_light_color_matrix[6] = light_color[2][1];
        My_light_color_matrix[7] = light_color[3][1];

        My_light_color_matrix[8] = light_color[0][2];
        My_light_color_matrix[9] = light_color[1][2];
        My_light_color_matrix[10] = light_color[2][2];
        My_light_color_matrix[11] = light_color[3][2];

        My_light_color_matrix[12] = light_color[0][3];
        My_light_color_matrix[13] = light_color[1][3];
        My_light_color_matrix[14] = light_color[2][3];
        My_light_color_matrix[15] = light_color[3][3];

}

void memswap(int *a, int *b, int size)
{
  int tmp;
  int i;
  for(i=0; i<size; i++){
    tmp=a[i]; a[i]=b[i]; b[i]=tmp;
  }
}

void zqsort(void *base, void *qword, int n, int size)
{
  int *base2=base;
  int *p=base2+1;
  int *q;
  int *qword2=(int*)qword;
  int *s=qword2+size;
  int *t;
  int i, x=0;

  if(n<=1) return;
  if(n==2){
    if(*base2 < *p){
      memswap(base2, p, 1);
      memswap(qword2,s, size);
    }
    return;
  }
  memswap(base2, &base2[(n/2)*1], 1);
  memswap(qword2,&qword2[(n/2)*size], size);
  q=base2;
  t=qword2;
  for(i=1; i<n ;i++){
    if(*base2 < *p){
      x++;
      q+=1; 
      t+=size;
      if(p!=q){
	memswap(p,q, 1);
	memswap(s,t, size);
      }
    }
    p+=1;
    s+=size;
  }
  if(q!=base2){
    memswap(base2,q, 1);
    memswap(qword2,t, size);
  }
  zqsort(base2, qword2, x, size);
  zqsort(q+1, t+size, n-x-1, size);

}







