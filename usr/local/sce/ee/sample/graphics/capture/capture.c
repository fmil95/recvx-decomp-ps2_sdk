/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - capture -
 *
 *                         Version 0.3.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : capture.c
 *                     Description : main procedures
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.3.0           Jun, 8,1999     munekis    1st version
 */


#include <eekernel.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <libdev.h>
#include <math.h>
#include <stdlib.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <stdio.h>
#include <libpad.h>

extern unsigned int My_dma_start[] __attribute__((section(".vudata")));
extern float My_param[] __attribute__((section(".vudata")));
extern float My_weight[] __attribute__((section(".vudata")));
extern float My_lightMatrix[] __attribute__((section(".vudata")));
extern u_int My_texture1[] __attribute__((section(".vudata")));


typedef struct {
        float m[4][4];
} FMATRIX;

typedef struct {
        float v[4];
} FVECTOR;

FMATRIX SLmtx;
FVECTOR theta;
FVECTOR trans;
FVECTOR dtheta;
FVECTOR dtrans;

float weight, dweight;

void RotMatrix(FVECTOR *rot, FVECTOR *trs, FMATRIX *slm);
void SetMatrix(void);
void BlendObject(float);
void ChangeLight(u_int);
void StoreImage(void);




#define IMAGE_SIZE 256
#define DEFORM_CYCLE 30

sceGsDBuff db;
sceGsLoadImage gs_limage;
sceGsStoreImage gs_simage;


struct {
	sceGifTag giftag;
	sceGsTexEnv gs_tex;
} texenv;


u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

int main()
{
    	int odev;
    	u_int frame;
	u_int paddata;
	u_char rdata[32];

	sceSifInitRpc(0);
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
			    0, NULL) < 0){
	    printf("Can't load module sio2man\n");
	}
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
			    0, NULL) < 0){
	    printf("Can't load module padman\n");
	}

	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
	sceGsResetPath();

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


	sceGsExecLoadImage(&gs_limage, (u_long128*) My_texture1);
	sceGsSyncPath(0, 0);

	FlushCache(0);

        theta.v[0] = 170.0f;
        theta.v[1] = 120.0f;
        theta.v[2] = 0.0f;
        trans.v[0] = 0.0f;
        trans.v[1] = 0.0f;
        trans.v[2] = 10.0f;

        dtheta.v[0] =
               (float)(3.5 * 2.0f * 3.1415926536f / 360.0f);
        dtheta.v[1] =
               (float)(1.5 * 2.0f * 3.1415926536f / 360.0f);
        dtheta.v[2] =
               (float)(2.5 * 2.0f * 3.1415926536f / 360.0f);

	weight = 0.0f;
	dweight = 1.0f / (float) DEFORM_CYCLE;

        frame = 0;
	odev = !sceGsSyncV(0);

	// --- open pad ---
	scePadInit(0);
	scePadPortOpen(0, 0, pad_dma_buf);


        while(1){
		

          	sceGsSetHalfOffset((frame&1) ? &db.draw1 : &db.draw0, 2048,2048,odev);
                FlushCache(0);
		sceGsSwapDBuff(&db, frame);

		// --- set texture env 1 ---
		sceGsSetDefTexEnv(&texenv.gs_tex, 0, 6720, IMAGE_SIZE / 64, SCE_GS_PSMCT32, 
			  8, 8, 2, 0, 0, 0, 1);
		SCE_GIF_CLEAR_TAG(&texenv.giftag); 
		texenv.giftag.NLOOP = 4;
		texenv.giftag.EOP = 1;
		texenv.giftag.NREG = 1;
		texenv.giftag.REGS0 = 0xe;
		*(u_long*)&texenv.gs_tex.clamp1 = SCE_GS_SET_CLAMP(0, 0, 0, 0, 0, 0); 
		FlushCache(0);
		sceGsPutDrawEnv(&texenv.giftag);
		sceGsSyncPath(0, 0);


		// --- read pad ---
		if(scePadRead(0, 0, rdata) > 0){
		    paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
		}
		else{
		    paddata = 0;
		}

		if((frame / DEFORM_CYCLE) % 2) {
			weight -= dweight;
		} else {
			weight += dweight;
		}


		if(!(paddata&SCE_PADRright)) {
                	theta.v[0] += dtheta.v[0];
                	theta.v[1] += dtheta.v[1];
                	theta.v[2] += dtheta.v[2];
		}


                if(theta.v[0] > 6.283185306f) theta.v[0] -= 6.283185306f;
                if(theta.v[1] > 6.283185306f) theta.v[1] -= 6.283185306f;
                if(theta.v[2] > 6.283185306f) theta.v[2] -= 6.283185306f;
		if(weight > 1.0f) weight = 1.0f;
		if(weight < 0.0f) weight = 0.0f;

                RotMatrix(&theta, &trans, &SLmtx);
                SetMatrix();


                BlendObject(weight);
		ChangeLight(frame);



		// --- kick dma to draw object ---
                *D1_QWC  = 0x00;
                *D1_TADR = (u_int)My_dma_start & 0x0fffffff;
                *D_STAT = 2;
                FlushCache(0);
                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

		sceGsSyncPath(0, 0);
		



		if(paddata&SCE_PADRdown) {

		        printf("capturing...\n");
		        frame++;
                        sceGsSetHalfOffset((frame&1) ? &db.draw1 : &db.draw0, 2048,2048,odev);
                        FlushCache(0);
		        sceGsSwapDBuff(&db, frame);

		        // --- kick dma to draw object ---
                        *D1_QWC  = 0x00;
                        *D1_TADR = (u_int)&My_dma_start & 0x0fffffff;
                        *D_STAT = 2;
                        FlushCache(0);
                        *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

		        sceGsSyncPath(0, 0);

		        StoreImage();
                        return 0;
		}


                frame++;
                odev = !sceGsSyncV(0);


        }

}


void RotMatrix(FVECTOR *rot, FVECTOR *trs, FMATRIX *slm)
{
        float c0, c1, c2;
        float s0, s1, s2;

        c0 = cosf(rot->v[0]);
        c1 = cosf(rot->v[1]);
        c2 = cosf(rot->v[2]);
        s0 = sinf(rot->v[0]);
        s1 = sinf(rot->v[1]);
        s2 = sinf(rot->v[2]);

        slm->m[0][0] = c1*c2;
        slm->m[0][1] = s0*s1*c2 - s2*c0;
        slm->m[0][2] = s1*c0*c2 + s0*s2;
        slm->m[0][3] = trs->v[0];
        slm->m[1][0] = s2*c1;
        slm->m[1][1] = s0*s1*s2 + c0*c2;
        slm->m[1][2] = s1*c0*s2 - s0*c2;
        slm->m[1][3] = trs->v[1];
        slm->m[2][0] = -s1;
        slm->m[2][1] = s0*c1;
        slm->m[2][2] = c0*c1;
        slm->m[2][3] = trs->v[2];
        slm->m[3][0] = 0.0f;
        slm->m[3][1] = 0.0f;
        slm->m[3][2] = 0.0f;
        slm->m[3][3] = 1.0f;


}



void SetMatrix(void)
{
        My_param[0] = SLmtx.m[0][0];
        My_param[1] = SLmtx.m[1][0];
        My_param[2] = SLmtx.m[2][0];
        My_param[3] = SLmtx.m[3][0];

        My_param[4] = SLmtx.m[0][1];
        My_param[5] = SLmtx.m[1][1];
        My_param[6] = SLmtx.m[2][1];
        My_param[7] = SLmtx.m[3][1];

        My_param[8] = SLmtx.m[0][2];
        My_param[9] = SLmtx.m[1][2];
        My_param[10] = SLmtx.m[2][2];
        My_param[11] = SLmtx.m[3][2];

        My_param[12] = SLmtx.m[0][3];
        My_param[13] = SLmtx.m[1][3];
        My_param[14] = SLmtx.m[2][3];
        My_param[15] = SLmtx.m[3][3];

}



void BlendObject(float w) {

        float w1;
      
	w1 = 1.0f - w;
	My_weight[0] = w1; 
	My_weight[1] = w; 
}



void ChangeLight(u_int frame) {

        float w, cos_theta, sin_theta;
        u_int div;

#define	LIGHT_CYCLE 120
#define	ANGLE 340.0f

        // --- calc weight value ---
        div = frame / LIGHT_CYCLE;
        w = ((float) frame - (float) (div * LIGHT_CYCLE)) / LIGHT_CYCLE;
        if(div % 2) { 
                w = 1.0f - w;
        }

	cos_theta = cosf(w * ANGLE * 3.1415926536f / 180.0f);
	sin_theta = sinf(w * ANGLE * 3.1415926536f / 180.0f);

	My_lightMatrix[0] = cos_theta; 
	My_lightMatrix[1] = sin_theta; 

	My_lightMatrix[4] = cos_theta; 
	My_lightMatrix[6] = sin_theta; 
	
	My_lightMatrix[9] = cos_theta; 
	My_lightMatrix[10] = sin_theta; 

}


void StoreImage(void) {

#define WIDTH 640
#define HEIGHT 224

        u_long128 *dst1, *dst2;
	int fd, i;

	// to create image file. 
        fd = sceOpen("host:image.rgba", SCE_WRONLY | SCE_TRUNC | SCE_CREAT);
        
        dst1 = (u_long128*) malloc(WIDTH * HEIGHT * 4);
        dst2 = (u_long128*) malloc(WIDTH * HEIGHT * 4);

        sceGsSetDefStoreImage(&gs_simage, 0, WIDTH / 64, SCE_GS_PSMCT32, 0, 0, WIDTH, HEIGHT);
	FlushCache(0);
	sceGsExecStoreImage(&gs_simage, dst1);
	sceGsSyncPath(0, 0);

        sceGsSetDefStoreImage(&gs_simage, 2240, WIDTH / 64, 
                SCE_GS_PSMCT32, 0, 0, WIDTH, HEIGHT);
	FlushCache(0);
	sceGsExecStoreImage(&gs_simage, dst2);
	sceGsSyncPath(0, 0);

	for(i = 0; i < HEIGHT; i++) {
                sceWrite(fd, dst1 + i * WIDTH / 4, WIDTH * 4);
                sceWrite(fd, dst2 + i * WIDTH / 4, WIDTH * 4);
        }

	free(dst1);
	free(dst2);
	sceClose(fd);

	return;
}
