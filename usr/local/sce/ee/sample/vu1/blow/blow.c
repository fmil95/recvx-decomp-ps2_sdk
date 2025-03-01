/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */

/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - blow -
 *
 *                         Version 0.3.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : blow.c
 *                     Description : main procedures
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.1.0           Mar,26,1999     munekis    1st version
 *      0.3.0           Jun, 2,1999     munekis    add gas example
 */


#include <eekernel.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <libdev.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <libvu0.h>
#include <libpad.h>

extern unsigned int My_dma_start __attribute__((section(".vudata")));
extern unsigned int My_dma_start_part __attribute__((section(".vudata")));
extern unsigned int My_dma_start_shadow_part __attribute__((section(".vudata")));

extern unsigned int My_matrix __attribute__((section(".vudata")));
extern unsigned int My_shadow_matrix __attribute__((section(".vudata")));
extern unsigned int My_ground_matrix __attribute__((section(".vudata")));
extern unsigned int My_offset __attribute__((section(".vudata")));
extern unsigned int My_shadow_offset __attribute__((section(".vudata")));

extern unsigned int My_texture1 __attribute__((section(".vudata")));
extern unsigned int My_texture2 __attribute__((section(".vudata")));
extern unsigned int My_texture3 __attribute__((section(".vudata")));

extern unsigned int My_part __attribute__((section(".vudata")));
extern unsigned int My_part1 __attribute__((section(".vudata")));
extern unsigned int My_shadow_part __attribute__((section(".vudata")));
extern unsigned int My_shadow_part1 __attribute__((section(".vudata")));
extern unsigned int My_part_src __attribute__((section(".vudata")));

extern unsigned int My_part_second_pole  __attribute__((section(".vudata")));
extern unsigned int My_part1_second_pole  __attribute__((section(".vudata")));
extern unsigned int My_part_third_pole  __attribute__((section(".vudata")));
extern unsigned int My_part1_third_pole __attribute__((section(".vudata")));

extern unsigned int My_shadow_part_pole2 __attribute__((section(".vudata")));
extern unsigned int My_shadow_part1_pole2 __attribute__((section(".vudata")));
extern unsigned int My_shadow_part_pole3 __attribute__((section(".vudata")));
extern unsigned int My_shadow_part1_pole3 __attribute__((section(".vudata")));



float weight, dweight, screenOffset[16];


void CreateViewingMatrix(sceVu0FVECTOR, sceVu0FVECTOR, sceVu0FMATRIX);
void SetViewPosition(sceVu0FVECTOR, float, float);
void SetParticlePosition(u_int);


#ifdef _GAS_PARTICLE
#include "gas.h"
#else
#include "blow.h"
#endif

u_int bound[NUM_BLOCK * NUM_PART * NUM_EXPLODE];
u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));



// --- param for double buffer ---
sceGsDBuff db;
sceGsLoadImage gs_limage, gs_limage2;

struct {
        sceGifTag giftag;
	sceGsTest gs_test;
        long gs_test1addr;
        sceGsAlphaEnv gs_alpha;
        sceGsTexEnv gs_tex;
} texenv1, texenv2;

int main()
{
    	int odev;
    	u_int i, frame;
    	float theta, dtheta, phi;
	sceVu0FVECTOR view;
	sceVu0FVECTOR interest;
	sceVu0FMATRIX viewmat;

	u_int paddata;
	u_char rdata[32];

	sceSifInitRpc(0);
	while(sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
			 0, NULL) < 0){
	    printf("Can't load module sio2man\n");
	}
	while(sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
			 0, NULL) < 0){
	    printf("Can't load module padman\n");
	}




		sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

	// --- set double buffer ---
	sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224, 
                SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
        *(u_long *)&db.clear0.rgbaq = 
                SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10, 0x80, 0x3f800000);
        *(u_long *)&db.clear1.rgbaq =
                SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10, 0x80, 0x3f800000);

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

	// --- set load image 2 ---
        sceGsSetDefLoadImage(&gs_limage2,
                             14364,
                             1,
                             0,
                             0, 0,
                             32, 32);
        FlushCache(0);
        sceGsExecLoadImage(&gs_limage2, (u_long128*) &My_texture2);
        sceGsSyncPath(0, 0);

	// --- set load image 3 ---
        sceGsSetDefLoadImage(&gs_limage2,
                             15388,
                             1,
                             0,
                             0, 0,
                             32, 32);
        FlushCache(0);
        sceGsExecLoadImage(&gs_limage2, (u_long128*) &My_texture3);
        sceGsSyncPath(0, 0);


	// --- init bound flag ---
	for(i = 0; i < NUM_BLOCK * NUM_PART * NUM_EXPLODE; i++)
		bound[i] = 0;


	interest[0] = 0.0f;
	interest[1] = 0.7f;
	interest[2] = 0.0f;
	interest[3] = 0.0f;



	FlushCache(0);

	// defaults theta
	theta = 0.0f;
	phi = 0.0f;
	dtheta = 2.0f * PI / 180.0f;
	
        frame = 0;

	SetParticlePosition(frame);
	frame++;
	odev = !sceGsSyncV(0);


	// --- open pad ---
	scePadInit(0);
	scePadPortOpen(0, 0, pad_dma_buf);


        while(1){
		

          	sceGsSetHalfOffset((frame&1) ? &db.draw1 : &db.draw0, 2048,2048,odev);
                FlushCache(0);
		sceGsSwapDBuff(&db, frame);



		// --- read pad ---
		if(scePadRead(0, 0, rdata) > 0){
		    paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
		}
		else{
		    paddata = 0;
		}


		// --- theta direction ---
		if(paddata&SCE_PADLleft) {
                	theta += dtheta;
		} else if (paddata&SCE_PADLright) {
                	theta -= dtheta;
		}

		// --- phi direction ---
		if(paddata&SCE_PADLup) {
                	phi += dtheta;
		} else if (paddata&SCE_PADLdown) {
                	phi -= dtheta;
		}

		// --- adjust invalid angle ---
		if(theta > 2.0f * PI) {
			theta -= 2.0f * PI;
		} else if(theta < 0.0f) {
			theta += 2.0f * PI;
		}

		if(phi >= 0.49f * PI) {
			phi = 0.49f * PI;
		} else if(phi <= -0.49f * PI) {
			phi = -0.49f * PI;
		}



		// --- Viewing Matrix ---
		SetViewPosition(view, theta, phi);
		CreateViewingMatrix(view, interest, viewmat);

		// ---- draw ground ----
		// ==== set texture env for DESK ===
        	sceGsSetDefTexEnv(&texenv2.gs_tex, 0, 6720, 
			  IMAGE_SIZE / 64, SCE_GS_PSMCT32,
                          6, 6, 0, 0, 0, 0, 1);
        	SCE_GIF_CLEAR_TAG(&texenv2.giftag);
        	texenv2.giftag.NLOOP = 9;
        	texenv2.giftag.EOP = 1;
        	texenv2.giftag.NREG = 1;
        	texenv2.giftag.REGS0 = 0xe;
		sceGsSetDefAlphaEnv(&texenv2.gs_alpha, 0);
		*(u_long *) &texenv2.gs_alpha.alpha1 = SCE_GS_SET_ALPHA(0, 2, 0, 1, 0);
		*(u_long *) &texenv2.gs_test =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 3, 0x3);
        	texenv2.gs_test1addr = (long)SCE_GS_TEST_1;
        	FlushCache(0);
        	sceGsPutDrawEnv(&texenv2.giftag);
        	sceGsSyncPath(0, 0);

                *D1_QWC  = 0x00;
                *D1_TADR = (u_int)&My_dma_start & 0x0fffffff;
                *D_STAT = 2;
                FlushCache(0);
                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

		sceGsSyncPath(0, 0);


		// --- particle setup ---
		if(!(paddata&SCE_PADRright))
			SetParticlePosition(frame);

		// ---- DRAW PARTICLE SHADOW ----
		// ==== set texture env ===
        	sceGsSetDefTexEnv(&texenv1.gs_tex, 0, 15388, 
			  1, SCE_GS_PSMCT32,
                          5, 5, 0, 0, 0, 0, 1);
        	SCE_GIF_CLEAR_TAG(&texenv1.giftag);
        	texenv1.giftag.NLOOP = 9;
        	texenv1.giftag.EOP = 1;
        	texenv1.giftag.NREG = 1;
        	texenv1.giftag.REGS0 = 0xe;
		sceGsSetDefAlphaEnv(&texenv1.gs_alpha, 0);

#ifdef _GAS_PARTICLE
		*(u_long *) &texenv1.gs_alpha.alpha1 = SCE_GS_SET_ALPHA(2, 0, 0, 1, 0);
#else
		*(u_long *) &texenv1.gs_alpha.alpha1 = SCE_GS_SET_ALPHA(0, 2, 0, 1, 0);
#endif



		*(u_long *) &texenv1.gs_test =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 0x1);
        	texenv1.gs_test1addr = (long)SCE_GS_TEST_1;
        	FlushCache(0);
        	sceGsPutDrawEnv(&texenv1.giftag);
        	sceGsSyncPath(0, 0);

                *D1_QWC  = 0x00;
                *D1_TADR = (u_int)&My_dma_start_shadow_part & 0x0fffffff;
                *D_STAT = 2;
                FlushCache(0);
                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
		sceGsSyncPath(0, 0);
		
		
		// ---- DRAW PARTICLE ----
		// ==== set texture env ===
        	sceGsSetDefTexEnv(&texenv1.gs_tex, 0, 14364, 
			  1, SCE_GS_PSMCT32,
                          5, 5, 0, 0, 0, 0, 1);
        	SCE_GIF_CLEAR_TAG(&texenv1.giftag);
        	texenv1.giftag.NLOOP = 9;
        	texenv1.giftag.EOP = 1;
        	texenv1.giftag.NREG = 1;
        	texenv1.giftag.REGS0 = 0xe;
		sceGsSetDefAlphaEnv(&texenv1.gs_alpha, 0);
		*(u_long *) &texenv1.gs_alpha.alpha1 = SCE_GS_SET_ALPHA(0, 2, 0, 1, 0);
		*(u_long *) &texenv1.gs_test =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 0x1);
        	texenv1.gs_test1addr = (long)SCE_GS_TEST_1;
        	FlushCache(0);
        	sceGsPutDrawEnv(&texenv1.giftag);
        	sceGsSyncPath(0, 0);


                *D1_QWC  = 0x00;
                *D1_TADR = (u_int)&My_dma_start_part & 0x0fffffff;
                *D_STAT = 2;
                FlushCache(0);
                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

		sceGsSyncPath(0, 0);

		frame++;
                odev = !sceGsSyncV(0);

        }

}






void CreateViewingMatrix(sceVu0FVECTOR view, 
			 sceVu0FVECTOR interest,
			 sceVu0FMATRIX persMat) {


	float viewLength, upLength, rightLength; 
	float fFOV; 
	sceVu0FMATRIX viewMat, transMat, projMat, screenMat;
	sceVu0FVECTOR viewVector, upVector, rightVector, tmpVector1;
	sceVu0FVECTOR tmpVector2;

	sceVu0SubVector(viewVector, interest, view);
	viewLength = sqrtf(viewVector[0] * viewVector[0] + 
			   viewVector[1] * viewVector[1] +
		           viewVector[2] * viewVector[2]);	
	sceVu0Normalize(viewVector, viewVector);

	upVector[0] = 0.0f; 
	upVector[1] = 1.0f; 
	upVector[2] = 0.0f; 
	upVector[3] = 1.0f; 

	sceVu0OuterProduct(tmpVector1, viewVector, upVector);
	sceVu0Normalize(tmpVector1, tmpVector1);
	sceVu0OuterProduct(upVector, tmpVector1, viewVector);
	sceVu0OuterProduct(rightVector, upVector, viewVector);

	
	// ------ set offset for screen ------
	sceVu0ScaleVector(tmpVector2, rightVector, -1.0f);
	sceVu0SubVector(tmpVector1, tmpVector2, upVector);
	sceVu0ScaleVector(tmpVector1, tmpVector1, PART_SIZE);
	screenOffset[0] = tmpVector1[0];
	screenOffset[1] = tmpVector1[1];
	screenOffset[2] = tmpVector1[2];
	
	sceVu0SubVector(tmpVector1, upVector, rightVector);
	sceVu0ScaleVector(tmpVector1, tmpVector1, PART_SIZE);
	screenOffset[4] = tmpVector1[0];
	screenOffset[5] = tmpVector1[1];
	screenOffset[6] = tmpVector1[2];
	
	sceVu0SubVector(tmpVector1, rightVector, upVector);
	sceVu0ScaleVector(tmpVector1, tmpVector1, PART_SIZE);
	screenOffset[8] = tmpVector1[0];
	screenOffset[9] = tmpVector1[1];
	screenOffset[10] = tmpVector1[2];

	sceVu0AddVector(tmpVector1, rightVector, upVector);
	sceVu0ScaleVector(tmpVector1, tmpVector1, PART_SIZE);
	screenOffset[12] = tmpVector1[0];
	screenOffset[13] = tmpVector1[1];
	screenOffset[14] = tmpVector1[2];

        ((u_int *)&My_offset)[0] = *(u_int *)&(screenOffset[0]);
        ((u_int *)&My_offset)[1] = *(u_int *)&(screenOffset[1]);
        ((u_int *)&My_offset)[2] = *(u_int *)&(screenOffset[2]);

        ((u_int *)&My_offset)[4] = *(u_int *)&(screenOffset[4]);
        ((u_int *)&My_offset)[5] = *(u_int *)&(screenOffset[5]);
        ((u_int *)&My_offset)[6] = *(u_int *)&(screenOffset[6]);

        ((u_int *)&My_offset)[8] = *(u_int *)&(screenOffset[8]);
        ((u_int *)&My_offset)[9] = *(u_int *)&(screenOffset[9]);
        ((u_int *)&My_offset)[10] = *(u_int *)&(screenOffset[10]);

        ((u_int *)&My_offset)[12] = *(u_int *)&(screenOffset[12]);
        ((u_int *)&My_offset)[13] = *(u_int *)&(screenOffset[13]);
        ((u_int *)&My_offset)[14] = *(u_int *)&(screenOffset[14]);

	// ========== set offset for ground =========
        tmpVector1[0] = 1.0;
        tmpVector1[1] = 0.0;
        tmpVector1[2] = 1.0;
        tmpVector1[3] = 0.0;
        sceVu0ScaleVector(tmpVector1, tmpVector1, PART_SIZE_SHADOW);
        ((u_int *)&My_shadow_offset)[0] = *(u_int *)&(tmpVector1[0]);
        ((u_int *)&My_shadow_offset)[1] = *(u_int *)&(tmpVector1[1]);
        ((u_int *)&My_shadow_offset)[2] = *(u_int *)&(tmpVector1[2]);


        tmpVector1[0] = -1.0;
        tmpVector1[2] = 1.0;
        sceVu0ScaleVector(tmpVector1, tmpVector1, PART_SIZE_SHADOW);
        ((u_int *)&My_shadow_offset)[4] = *(u_int *)&(tmpVector1[0]);
        ((u_int *)&My_shadow_offset)[5] = *(u_int *)&(tmpVector1[1]);
        ((u_int *)&My_shadow_offset)[6] = *(u_int *)&(tmpVector1[2]);

        tmpVector1[0] = 1.0;
        tmpVector1[2] = -1.0;
        sceVu0ScaleVector(tmpVector1, tmpVector1, PART_SIZE_SHADOW);
        ((u_int *)&My_shadow_offset)[8] = *(u_int *)&(tmpVector1[0]);
        ((u_int *)&My_shadow_offset)[9] = *(u_int *)&(tmpVector1[1]);
        ((u_int *)&My_shadow_offset)[10] = *(u_int *)&(tmpVector1[2]);

        tmpVector1[0] = -1.0;
        tmpVector1[2] = -1.0;
        sceVu0ScaleVector(tmpVector1, tmpVector1, PART_SIZE_SHADOW);
        ((u_int *)&My_shadow_offset)[12] = *(u_int *)&(tmpVector1[0]);
        ((u_int *)&My_shadow_offset)[13] = *(u_int *)&(tmpVector1[1]);
        ((u_int *)&My_shadow_offset)[14] = *(u_int *)&(tmpVector1[2]);



	fFOV = FOV * PI / 180.0f;
	upLength = viewLength * sin(fFOV * 0.5) / cos(fFOV * 0.5);
	rightLength = ASPECT * upLength;

	sceVu0ScaleVector(viewVector, viewVector, 1.0f / viewLength);
	sceVu0ScaleVector(upVector, upVector, 1.0f / upLength);
	sceVu0ScaleVector(rightVector, rightVector, 1.0f / rightLength);

	memset(viewMat, 0, sizeof(sceVu0FMATRIX));

	viewMat[0][0] = rightVector[0];
	viewMat[1][0] = rightVector[1];
	viewMat[2][0] = rightVector[2];

	viewMat[0][1] = upVector[0];
	viewMat[1][1] = upVector[1];
	viewMat[2][1] = upVector[2];
	
	viewMat[0][2] = viewVector[0];
	viewMat[1][2] = viewVector[1];
	viewMat[2][2] = viewVector[2];

	viewMat[3][3] = 1.0f;

	memset(transMat, 0, sizeof(sceVu0FMATRIX));
	transMat[0][0] = 1.0f;
	transMat[1][1] = 1.0f;
	transMat[2][2] = 1.0f;
	transMat[3][3] = 1.0f;

	transMat[3][0] = - interest[0];
	transMat[3][1] = - interest[1];
	transMat[3][2] = - interest[2];

	sceVu0MulMatrix(projMat, transMat, viewMat);


	projMat[0][3] = projMat[0][2]; 
	projMat[1][3] = projMat[1][2]; 
	projMat[2][3] = projMat[2][2]; 

	screenMat[0][0] = SCREEN_WIDTH / 2.0f;
    	screenMat[1][0] = 0.0f;
    	screenMat[2][0] = 0.0f;
    	screenMat[3][0] = SCREEN_WIDTH / 2.0f + SCREEN_OFFSET_X;

    	screenMat[0][1] = 0.0f;
    	screenMat[1][1] = - SCREEN_HEIGHT / 2.0f;
    	screenMat[2][1] = 0.0f;
    	screenMat[3][1] = SCREEN_HEIGHT / 2.0f + SCREEN_OFFSET_Y;

    	screenMat[0][2] = 0.0f;
    	screenMat[1][2] = 0.0f;
    	screenMat[2][2] = -100000.0f;
    	screenMat[3][2] =  100000000.0f;

    	screenMat[0][3] = 0.0f;
    	screenMat[1][3] = 0.0f;
    	screenMat[2][3] = 0.0f;
    	screenMat[3][3] = 1.0f;

	sceVu0MulMatrix(persMat, screenMat, projMat);

	// --- MATRIX for PARTICLE ---
        ((u_int *)&My_matrix)[0] = *(u_int *)&(persMat[0][0]);
        ((u_int *)&My_matrix)[1] = *(u_int *)&(persMat[0][1]);
        ((u_int *)&My_matrix)[2] = *(u_int *)&(persMat[0][2]);
        ((u_int *)&My_matrix)[3] = *(u_int *)&(persMat[0][3]);

        ((u_int *)&My_matrix)[4] = *(u_int *)&(persMat[1][0]);
        ((u_int *)&My_matrix)[5] = *(u_int *)&(persMat[1][1]);
        ((u_int *)&My_matrix)[6] = *(u_int *)&(persMat[1][2]);
        ((u_int *)&My_matrix)[7] = *(u_int *)&(persMat[1][3]);

        ((u_int *)&My_matrix)[8] = *(u_int *)&(persMat[2][0]);
        ((u_int *)&My_matrix)[9] = *(u_int *)&(persMat[2][1]);
        ((u_int *)&My_matrix)[10] = *(u_int *)&(persMat[2][2]);
        ((u_int *)&My_matrix)[11] = *(u_int *)&(persMat[2][3]);

        ((u_int *)&My_matrix)[12] = *(u_int *)&(persMat[3][0]);
        ((u_int *)&My_matrix)[13] = *(u_int *)&(persMat[3][1]);
        ((u_int *)&My_matrix)[14] = *(u_int *)&(persMat[3][2]);
        ((u_int *)&My_matrix)[15] = *(u_int *)&(persMat[3][3]);

	// --- MATRIX for SHADOW PARTICLE ---
        ((u_int *)&My_shadow_matrix)[0] = *(u_int *)&(persMat[0][0]);
        ((u_int *)&My_shadow_matrix)[1] = *(u_int *)&(persMat[0][1]);
        ((u_int *)&My_shadow_matrix)[2] = *(u_int *)&(persMat[0][2]);
        ((u_int *)&My_shadow_matrix)[3] = *(u_int *)&(persMat[0][3]);

        ((u_int *)&My_shadow_matrix)[4] = *(u_int *)&(persMat[1][0]);
        ((u_int *)&My_shadow_matrix)[5] = *(u_int *)&(persMat[1][1]);
        ((u_int *)&My_shadow_matrix)[6] = *(u_int *)&(persMat[1][2]);
        ((u_int *)&My_shadow_matrix)[7] = *(u_int *)&(persMat[1][3]);

        ((u_int *)&My_shadow_matrix)[8] = *(u_int *)&(persMat[2][0]);
        ((u_int *)&My_shadow_matrix)[9] = *(u_int *)&(persMat[2][1]);
        ((u_int *)&My_shadow_matrix)[10] = *(u_int *)&(persMat[2][2]);
        ((u_int *)&My_shadow_matrix)[11] = *(u_int *)&(persMat[2][3]);

        ((u_int *)&My_shadow_matrix)[12] = *(u_int *)&(persMat[3][0]);
        ((u_int *)&My_shadow_matrix)[13] = *(u_int *)&(persMat[3][1]);
        ((u_int *)&My_shadow_matrix)[14] = *(u_int *)&(persMat[3][2]);
        ((u_int *)&My_shadow_matrix)[15] = *(u_int *)&(persMat[3][3]);

	// --- MATRIX for GROUND ---
        ((u_int *)&My_ground_matrix)[0] = *(u_int *)&(persMat[0][0]);
        ((u_int *)&My_ground_matrix)[1] = *(u_int *)&(persMat[0][1]);
        ((u_int *)&My_ground_matrix)[2] = *(u_int *)&(persMat[0][2]);
        ((u_int *)&My_ground_matrix)[3] = *(u_int *)&(persMat[0][3]);

        ((u_int *)&My_ground_matrix)[4] = *(u_int *)&(persMat[1][0]);
        ((u_int *)&My_ground_matrix)[5] = *(u_int *)&(persMat[1][1]);
        ((u_int *)&My_ground_matrix)[6] = *(u_int *)&(persMat[1][2]);
        ((u_int *)&My_ground_matrix)[7] = *(u_int *)&(persMat[1][3]);

        ((u_int *)&My_ground_matrix)[8] = *(u_int *)&(persMat[2][0]);
        ((u_int *)&My_ground_matrix)[9] = *(u_int *)&(persMat[2][1]);
        ((u_int *)&My_ground_matrix)[10] = *(u_int *)&(persMat[2][2]);
        ((u_int *)&My_ground_matrix)[11] = *(u_int *)&(persMat[2][3]);

        ((u_int *)&My_ground_matrix)[12] = *(u_int *)&(persMat[3][0]);
        ((u_int *)&My_ground_matrix)[13] = *(u_int *)&(persMat[3][1]);
        ((u_int *)&My_ground_matrix)[14] = *(u_int *)&(persMat[3][2]);
        ((u_int *)&My_ground_matrix)[15] = *(u_int *)&(persMat[3][3]);

}



void SetViewPosition(sceVu0FVECTOR view, float theta, float phi) {


#define RADIUS 65.0f

	view[0] = RADIUS * cosf(phi) * cosf(theta);
	view[1] = RADIUS * sinf(phi);
	view[2] = RADIUS * cosf(phi) * sinf(theta);
	view[3] = 1.0f;
}





