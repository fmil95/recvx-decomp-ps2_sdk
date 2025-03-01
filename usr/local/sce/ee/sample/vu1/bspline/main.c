/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - bspline -
 *
 *                         Version 0.3.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *          Name : main.c
 *          Description : main procedure 
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.3.0          Jun,1,1999     munekis     first version
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

#include "noise.h"

extern unsigned int My_dma_setting __attribute__((section(".vudata")));
extern unsigned int My_dma_start1 __attribute__((section(".vudata")));
extern unsigned int My_dma_start2 __attribute__((section(".vudata")));
extern unsigned int My_reftag __attribute__((section(".vudata")));
extern unsigned int My_light __attribute__((section(".vudata")));

extern float B_matrix[] __attribute__((section(".vudata")));
extern unsigned int  B_blending[] __attribute__((section(".vudata")));
extern unsigned int My_bspline_start __attribute__((section(".vudata")));
extern unsigned int My_bspline_end __attribute__((section(".vudata")));
extern unsigned int My_texture1 __attribute__((section(".vudata")));



void SetBlendingCoeff(void);
void CreateViewingMatrix(sceVu0FVECTOR, sceVu0FVECTOR, sceVu0FMATRIX);
void SetViewPosition(sceVu0FVECTOR, float, float);
void CreateControlPointArea(u_int, u_int, u_int, u_int, u_int);
void SetControlPoint(u_int, u_int, u_int, u_int, u_int, u_int *addr);
void MoveLight(u_int);



// ---- params for display ----
#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 224.0f
#define SCREEN_OFFSET_X 1728.0f
#define SCREEN_OFFSET_Y 1936.0f

#define FOV 42.0f
#define ASPECT 1.42f
#define PI 3.14159265358979323846f

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


// --- size of parametric uv ---
#include "grid.h"
#include "torus.h"


// --- double buffer ---
u_int *cppkt[2];
float *cp, *cp_mod;


int main()
{
    	int odev;
    	u_int i, j, frame, draw_frame;
	u_int index;
    	float f, theta, dtheta, phi, f1, fmax, fmin, *offset;
	u_int n_vloop, *reftag;
        u_int n_u, n_v, u_closed, v_closed;
        u_int modelID, modelFlag, shading, toggle;

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

	sceGsResetPath();
	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

	// --- set double buffer ---
	sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224, 
                SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
        *(u_long *)&db.clear0.rgbaq = 
                SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10, 0x80, 0x3f800000);
        *(u_long *)&db.clear1.rgbaq =
                SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10, 0x80, 0x3f800000);

	// --- set load image 1---
#define IMAGE_SIZE 256
        sceGsSetDefLoadImage(&gs_limage,
                             6715,
                             IMAGE_SIZE / 64,
                             0,
                             0, 0,
                             IMAGE_SIZE, IMAGE_SIZE);
        FlushCache(0);
        sceGsExecLoadImage(&gs_limage, (u_long128*) &My_texture1);
        sceGsSyncPath(0, 0);


	// --- set texture env ---
        sceGsSetDefTexEnv(&texenv1.gs_tex, 0, 6715,
                          IMAGE_SIZE / 64, SCE_GS_PSMCT32,
                          8, 8, 0, 0, 0, 0, 1);
        SCE_GIF_CLEAR_TAG(&texenv1.giftag);
        texenv1.giftag.NLOOP = 9;
        texenv1.giftag.EOP = 1;
        texenv1.giftag.NREG = 1;
        texenv1.giftag.REGS0 = 0xe;
        sceGsSetDefAlphaEnv(&texenv1.gs_alpha, 0);
        *(u_long *) &texenv1.gs_alpha.alpha1 = SCE_GS_SET_ALPHA(0, 2, 0, 1, 0);
        *(u_long *) &texenv1.gs_test =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 3, 0x3);
        texenv1.gs_test1addr = (long)SCE_GS_TEST_1;
        FlushCache(0);
        sceGsPutDrawEnv(&texenv1.giftag);
        sceGsSyncPath(0, 0);


	interest[0] = 0.0f;
	interest[1] = 0.0f;
	interest[2] = 0.0f;
	interest[3] = 0.0f;

	FlushCache(0);

	// defaults theta
	theta = 0.0f;
	phi = 0.0f;
	dtheta = 2.0f * PI / 180.0f;
	
        frame = 0;
        draw_frame = 0;

	odev = !sceGsSyncV(0);

	scePadInit(0);
	scePadPortOpen(0, 0, pad_dma_buf);


        *D1_QWC  = 0x00;
        *D1_TADR = (u_int)&My_dma_setting & 0x0fffffff;
        *D_STAT = 2;
        FlushCache(0);
        *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

	sceGsSyncPath(0, 0);


	// --- Set Blending coefficient ----
	SetBlendingCoeff();
        modelID = 0;
        modelFlag = 1;
	shading = 1;
	cp_mod = NULL;
        offset = NULL;
        n_u = N_U1;
        n_v = N_V1;
        u_closed = U_CLOSED1;
        v_closed = V_CLOSED1;
	toggle = 0;

        // --- initialize noise ---
        init_noise();

	f = 0.0f;

        while(1){
		
		// --- change model / change wire/shading ---
		if(modelFlag) {
                	if(modelID == 0) {

	                	// --- create dma data area (single V line set) ---
				modelFlag = 0;
                        	cp = cp1;
                        	n_u = N_U1;
                        	n_v = N_V1;
                        	u_closed = U_CLOSED1;
                        	v_closed = V_CLOSED1;
	                	CreateControlPointArea(n_u, n_v, u_closed, v_closed, shading);
				
				if(cp_mod != NULL) free(cp_mod);
                        	cp_mod = (float*) malloc(n_u * n_v * 4 * sizeof(float));
                		memcpy(cp_mod, cp, n_u * n_v * 4 * sizeof(float));
				if(offset != NULL) free(offset);
				offset = (float*) malloc(n_u * n_v * sizeof(float));
                	
			} else if(modelID == 1) {

	                	// --- create dma data area (single V line set) ---
				modelFlag = 0;
                        	cp = cp2;
                        	n_u = N_U2;
                        	n_v = N_V2;
                        	u_closed = U_CLOSED2;
                        	v_closed = V_CLOSED2;
	                	CreateControlPointArea(n_u, n_v, u_closed, v_closed, shading);

				if(cp_mod != NULL) free(cp_mod);
                        	cp_mod = (float*) malloc(n_u * n_v * 4 * sizeof(float));
                		memcpy(cp_mod, cp, n_u * n_v * 4 * sizeof(float));

				if(offset != NULL) free(offset);
				offset = (float*) malloc(n_u * n_v * sizeof(float));


			}
		}

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

		// --- model change ---
                if(!toggle ) {
			if(paddata&SCE_PADselect) {
                        	modelID = (++modelID) & 0x01;
                        	modelFlag = 1;
				toggle = 1;
			} else if(paddata&SCE_PADR1) {
				shading = (++shading) & 0x01;
                        	modelFlag = 1;
				toggle = 1;
			}
		} else if(!(paddata&SCE_PADselect) && !(paddata&SCE_PADR1)) {
			toggle = 0;
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
		if(theta > 2.0f *  PI) {
			theta = 2.0f * PI;
		} else if(theta < 0.0f) {
			theta = 0.0f;
		}
		if(phi > PI * 0.49f) {
			phi = PI * 0.49f;
		} else if(phi < -PI * 0.49f) {
			phi = -PI * 0.49f;
		}

		SetViewPosition(view, theta, phi);
		MoveLight(frame);


		sceGsSyncPath(0, 0);

		// --- Viewing Matrix ---
		CreateViewingMatrix(view, interest, viewmat);

               	*D1_QWC  = 0x00;
               	*D1_TADR = (u_int)&My_dma_start1 & 0x0fffffff;
               	*D_STAT = 2;
               	FlushCache(0);
               	*D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

		sceGsSyncPath(0, 0);


		if(v_closed)	
			n_vloop = n_v;
		else
			n_vloop = n_v - 3;

		reftag = (u_int*) &My_reftag;
		SetControlPoint(0, n_u, n_v, u_closed, v_closed, cppkt[0]);
		reftag[1] = 0x80000000 | (u_int) cppkt[0];

		for(i = 1; i < n_vloop; i++) {

                	*D1_QWC  = 0x00;
                	*D1_TADR = (u_int)&My_dma_start2 & 0x0fffffff;
                	*D_STAT = 2;
                	FlushCache(0);
                	*D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

			SetControlPoint(i, n_u, n_v, u_closed, v_closed, cppkt[i%2]);
			reftag[1] = 0x80000000 | (u_int) cppkt[i%2];
			sceGsSyncPath(0, 0);

		}

                *D1_QWC  = 0x00;
                *D1_TADR = (u_int)&My_dma_start2 & 0x0fffffff;
                *D_STAT = 2;
                FlushCache(0);
                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

		// --- deformation test ---
		f += 0.20f;
                fmax = 0.0f;
                fmin = 1000000.0f;
		for(j = 0; j < n_v; j++) {
			for(i = 0; i < n_u; i++) {
				index = ((j * n_u) + i);
                                offset[index] = turbulence((float)i, (float)j, f);
                                if(fmax < offset[index]) fmax = offset[index];
                                if(fmin > offset[index]) fmin = offset[index];

			}
		}

                f1 = fmax - fmin;
                for(j = 0; j < n_v; j++) {
		        for(i = 0; i < n_u; i++) {
				index = ((j * n_u) + i) * 4 + 1;
				cp_mod[index] = cp[index] + 5.0f * ((offset[(j * n_u) + i] - fmin) / f1 - 0.5f);
		        }
		}

		 
		frame++;

                odev = !sceGsSyncV(0);

        }

}







#define N_DIV 4
void SetBlendingCoeff(void) {

	float B[16], BN[16], t1, t2, t3, fwidth, *fBlending;
	float ST[4];
	u_int i, k, index, index2;
		
	fwidth = 1.0f / (N_DIV - 1);
	index = 0;
	index2 = 0;

	for(i = 0; i < N_DIV; i++) {
		t1 = fwidth * i;
        	t2 = t1 * t1;
        	t3 = t2 * t1;
		
		B[index++] = (   - t3 + 3 * t2 - 3 * t1 + 1) / 6.0f;
		B[index++] = ( 3 * t3 - 6 * t2          + 4) / 6.0f;
        	B[index++] = (-3 * t3 + 3 * t2 + 3 * t1 + 1) / 6.0f;
        	B[index++] = (     t3                      ) / 6.0f;

		BN[index2++] = (- 3 * t2 +  6 * t1 - 3) / 6.0f;
		BN[index2++] = (  9 * t2 - 12 * t1    ) / 6.0f;
		BN[index2++] = (- 9 * t2 +  6 * t1 + 3) / 6.0f;
		BN[index2++] = (  3 * t2              ) / 6.0f;

	}

	ST[0] = 0.0f;
	ST[1] = 0.3333333f;
	ST[2] = 0.6666666f;
	ST[3] = 1.0f;

	fBlending = (float*) &B_blending;
	k = 0;
	// ---------------------------------------------------------------
	// --- (0, 0) ---
	for(i = 0; i < 4; i++) 
		fBlending[k++] = B [i + 0 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 0 * 4];
	
	for(i = 0; i < 4; i++) 
		fBlending[k++] = B [i + 0 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 0 * 4];

	fBlending[k    ] = ST[0];
	fBlending[k + 1] = ST[0];
	fBlending[k + 2] = 1.0f;
	k += 4;

	// --- (1, 0) ---
	for(i = 0; i < 4; i++) 
		fBlending[k++] = B [i + 1 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 1 * 4];

	for(i = 0; i < 4; i++) 
		fBlending[k++] = B [i + 0 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 0 * 4];
	
	fBlending[k    ] = ST[0];
	fBlending[k + 1] = ST[1];
	fBlending[k + 2] = 1.0f;
	k += 4;


	// --- (0, 1) ---
	for(i = 0; i < 4; i++) 
		fBlending[k++] = B [i + 0 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 0 * 4];
	
	for(i = 0; i < 4; i++) 
		fBlending[k++] = B [i + 1 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 1 * 4];
	
	fBlending[k    ] = ST[1];
	fBlending[k + 1] = ST[0];
	fBlending[k + 2] = 1.0f;
	k += 4;


	// --- (1, 1) ---
	for(i = 0; i < 4; i++) 
		fBlending[k++] = B [i + 1 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 1 * 4];
	
	for(i = 0; i < 4; i++) 
		fBlending[k++] = B [i + 1 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 1 * 4];
	
	fBlending[k    ] = ST[1];
	fBlending[k + 1] = ST[1];
	fBlending[k + 2] = 1.0f;
	k += 4;


	// --- (0, 2) ---
	for(i = 0; i < 4; i++) 
		fBlending[k++] = B [i + 0 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 0 * 4];
	
	for(i = 0; i < 4; i++) 
		fBlending[k++] = B [i + 2 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 2 * 4];
	
	fBlending[k    ] = ST[2];
	fBlending[k + 1] = ST[0];
	fBlending[k + 2] = 1.0f;
	k += 4;


	// --- (1, 2) ---
	for(i = 0; i < 4; i++) 
		fBlending[k++] = B[i + 1 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 1 * 4];

	for(i = 0; i < 4; i++)
		fBlending[k++] = B[i + 2 * 4];
	for(i = 0; i < 4; i++)
		fBlending[k++] = BN[i + 2 * 4];
	
	fBlending[k    ] = ST[2];
	fBlending[k + 1] = ST[1];
	fBlending[k + 2] = 1.0f;
	k += 4;



	// --- (0, 3) ---
	for(i = 0; i < 4; i++) 
		fBlending[k++] = B [i + 0 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 0 * 4];
	
	for(i = 0; i < 4; i++) 
		fBlending[k++] = B [i + 3 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 3 * 4];
	
	fBlending[k    ] = ST[3];
	fBlending[k + 1] = ST[0];
	fBlending[k + 2] = 1.0f;
	k += 4;


	// --- (1, 3) ---
	for(i = 0; i < 4; i++) 
		fBlending[k++] = B [i + 1 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 1 * 4];
	
	for(i = 0; i < 4; i++) 
		fBlending[k++] = B [i + 3 * 4];
	for(i = 0; i < 4; i++) 
		fBlending[k++] = BN[i + 3 * 4];
	
	fBlending[k    ] = ST[3];
	fBlending[k + 1] = ST[1];
	fBlending[k + 2] = 1.0f;
	k += 4;


	// ---------------------------------------------------------------
	 // --- (1, 0) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 1 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 1 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 0 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 0 * 4];
	
	fBlending[k    ] = ST[0];
	fBlending[k + 1] = ST[1];
	fBlending[k + 2] = 1.0f;
	k += 4;


        // --- (2, 0) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 2 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 2 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 0 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 0 * 4];
	
	fBlending[k    ] = ST[0];
	fBlending[k + 1] = ST[2];
	fBlending[k + 2] = 1.0f;
	k += 4;


        // --- (1, 1) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 1 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 1 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 1 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 1 * 4];
	
	fBlending[k    ] = ST[1];
	fBlending[k + 1] = ST[1];
	fBlending[k + 2] = 1.0f;
	k += 4;


        // --- (2, 1) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 2 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 2 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 1 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 1 * 4];
	
	fBlending[k    ] = ST[1];
	fBlending[k + 1] = ST[2];
	fBlending[k + 2] = 1.0f;
	k += 4;


        // --- (1, 2) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 1 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 1 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 2 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 2 * 4];
	
	fBlending[k    ] = ST[2];
	fBlending[k + 1] = ST[1];
	fBlending[k + 2] = 1.0f;
	k += 4;


        // --- (2, 2) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 2 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 2 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 2 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 2 * 4];
	
	fBlending[k    ] = ST[2];
	fBlending[k + 1] = ST[2];
	fBlending[k + 2] = 1.0f;
	k += 4;


        // --- (1, 3) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 1 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 1 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 3 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 3 * 4];
	
	fBlending[k    ] = ST[3];
	fBlending[k + 1] = ST[1];
	fBlending[k + 2] = 1.0f;
	k += 4;


        // --- (2, 3) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 2 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 2 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 3 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 3 * 4];
	
	fBlending[k    ] = ST[3];
	fBlending[k + 1] = ST[2];
	fBlending[k + 2] = 1.0f;
	k += 4;




	// ---------------------------------------------------------------
	// --- (2, 0) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 2 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 2 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 0 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 0 * 4];
	
	fBlending[k    ] = ST[0];
	fBlending[k + 1] = ST[2];
	fBlending[k + 2] = 1.0f;
	k += 4;


        // --- (3, 0) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 3 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 3 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 0 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 0 * 4];
	
	fBlending[k    ] = ST[0];
	fBlending[k + 1] = ST[3];
	fBlending[k + 2] = 1.0f;
	k += 4;


        // --- (2, 1) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 2 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 2 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 1 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 1 * 4];
	
	fBlending[k    ] = ST[1];
	fBlending[k + 1] = ST[2];
	fBlending[k + 2] = 1.0f;
	k += 4;


        // --- (3, 1) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 3 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 3 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 1 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 1 * 4];
	
	fBlending[k    ] = ST[1];
	fBlending[k + 1] = ST[3];
	fBlending[k + 2] = 1.0f;
	k += 4;


        // --- (2, 2) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 2 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 2 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 2 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 2 * 4];
	
	fBlending[k    ] = ST[2];
	fBlending[k + 1] = ST[2];
	fBlending[k + 2] = 1.0f;
	k += 4;


        // --- (3, 2) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 3 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 3 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 2 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 2 * 4];
	
	fBlending[k    ] = ST[2];
	fBlending[k + 1] = ST[3];
	fBlending[k + 2] = 1.0f;
	k += 4;


        // --- (2, 3) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 2 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 2 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 3 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 3 * 4];
	
	fBlending[k    ] = ST[3];
	fBlending[k + 1] = ST[2];
	fBlending[k + 2] = 1.0f;
	k += 4;


        // --- (3, 3) ---
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 3 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 3 * 4];
	
        for(i = 0; i < 4; i++) 
                fBlending[k++] = B [i + 3 * 4];
        for(i = 0; i < 4; i++) 
                fBlending[k++] = BN[i + 3 * 4];
	
	fBlending[k    ] = ST[3];
	fBlending[k + 1] = ST[3];
	fBlending[k + 2] = 1.0f;
	k += 4;

}



void CreateViewingMatrix(sceVu0FVECTOR view, 
			 sceVu0FVECTOR interest,
			 sceVu0FMATRIX persMat) {


	float viewLength, upLength, rightLength, fFOV; 
	sceVu0FMATRIX viewMat, transMat, projMat, screenMat;
	sceVu0FVECTOR viewVector, upVector, rightVector, tmpVector1;

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
        B_matrix[0] = persMat[0][0];
        B_matrix[1] = persMat[0][1];
        B_matrix[2] = persMat[0][2];
        B_matrix[3] = persMat[0][3];

        B_matrix[4] = persMat[1][0];
        B_matrix[5] = persMat[1][1];
        B_matrix[6] = persMat[1][2];
        B_matrix[7] = persMat[1][3];

        B_matrix[8] = persMat[2][0];
        B_matrix[9] = persMat[2][1];
        B_matrix[10] = persMat[2][2];
        B_matrix[11] = persMat[2][3];

        B_matrix[12] = persMat[3][0];
        B_matrix[13] = persMat[3][1];
        B_matrix[14] = persMat[3][2];
        B_matrix[15] = persMat[3][3];



}



void SetViewPosition(sceVu0FVECTOR view, float theta, float phi) {

#define RADIUS 30.0f

	view[0] = RADIUS * cosf(phi) * cosf(theta);
	view[1] = RADIUS * sinf(phi);
	view[2] = RADIUS * cosf(phi) * sinf(theta);
	view[3] = 1.0f;
}


void MoveLight(u_int frame) {

	float f1, *fLight;
	static float theta = 0.0f, dtheta = 0.03f;
	
	theta += dtheta;
	if(theta > 0.8 * PI) dtheta = -0.03f;
	if(theta < 0.2 * PI) dtheta = 0.03f;

	fLight = (float*) &My_light;
	f1 = cosf(theta);
        fLight[0] = f1;
	f1 = sinf(theta);
        fLight[4] = f1;


}
