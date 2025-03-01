/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - iga -
 *
 *                         Version 0.1.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : sample.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.1.0           Mar,26,1999     okadaa    1st version
 */

#include <stdlib.h>
#include <stdio.h>
#include <eekernel.h>
#include <eeregs.h>
#include <math.h>
#include <libdma.h>
#include <libgraph.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <libvu0.h>
#include <libpad.h>

extern unsigned int My_dma_start0 __attribute__((section(".vudata")));
extern unsigned int My_obj __attribute__((section(".vudata")));
extern unsigned int My_obj_end __attribute__((section(".vudata")));

#include "tex.h" /* texture data with GIFtag for path3 */

#define NBALLS	168

sceVu0FMATRIX VLmtx; /* ViewLocal Matrix */
sceVu0FMATRIX SVmtx; /* ScreenView Matrix */
sceVu0FMATRIX SLmtx; /* ScreenLocal Matrix */
sceVu0FVECTOR theta[NBALLS]; /* angle of each object */
sceVu0FVECTOR trans[NBALLS]; /* position of each object */
sceVu0FVECTOR dtheta[NBALLS]; /* rotation quantity per one frame */
sceVu0FVECTOR dtrans[NBALLS]; /* transfer quantity per one frame */
sceVu0FVECTOR color[NBALLS]; /* color of each object */
sceVu0FVECTOR glight; /* light vector */

extern void DevInit(void); /* Device Initialize routine */
void WaitDMA(void); /* Waiting the end of Ch.8 DMA transfer */
void WaitVU1(void); /* Waiting the end of VU1 calculation */
void SetSVmatrix(sceVu0FMATRIX svm); /* Setting ScreenView Matrix */
void RotMatrix(sceVu0FVECTOR rot, sceVu0FVECTOR trs, sceVu0FMATRIX slm);
void MulMatrix(sceVu0FMATRIX svm, sceVu0FMATRIX vlm, sceVu0FMATRIX slm);
void textureLoad(void); /* Load texture data to GS local memory */
void SetMatrix(int id); /* Make path1 packet data */
void MakePosition(void); /* Make parameters for positon of each object */
float dist(sceVu0FVECTOR *src, sceVu0FVECTOR *dst); /* Distance 2 objects */

static sceGsDBuff db;
static sceVu0FVECTOR zerov; /* all zero vector */
static u_int *dpmem[2], *dstaddr; /* pointer for work area */
static sceDmaChan *d1, *d2, *d8;

/* Get ScrachPad Address for making DMApacket data */
#define GetDp(id)	((id&1)?(0x70002000):(0x70000000))

#define V4_32	0x6c /* the mode of unpack insn. in VIFcode */

float speed; /* the speed of flying objects */

u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

int main()
{
	int i; /* loop counter */
	u_int frame; /* frame number counter */
	int oddeven; /* field id flag(odd or even) */

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

	/*
	 * initialize for all devices and parameters.
	 */
	DevInit();

	d1 = sceDmaGetChan(1);
	d2 = sceDmaGetChan(2);
	d8 = sceDmaGetChan(8);

	/* DMApacket area */
	dpmem[0] = (u_int *)malloc(sizeof(u_int)*20*4*NBALLS);
	dpmem[1] = (u_int *)malloc(sizeof(u_int)*20*4*NBALLS);

	/* all zero vector */
	zerov[0] = 0.0f;
	zerov[1] = 0.0f;
	zerov[2] = 0.0f;
	zerov[3] = 0.0f;

	/* light vector */
	glight[0] = 0.578f;
	glight[1] = -0.578f;
	glight[2] = 0.578f;

	/* the speed of flying objects */
	speed = 0.0f;

	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

	sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224,
		SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);
	db.clear0.rgbaq.R = 0xc8;
	db.clear0.rgbaq.G = 0xc8;
	db.clear0.rgbaq.B = 0xc8;
	db.clear1.rgbaq.R = 0xc8;
	db.clear1.rgbaq.G = 0xc8;
	db.clear1.rgbaq.B = 0xc8;

	/* Set ScreenView matrix */
	SetSVmatrix(SVmtx);

	/* Load texture data to GS local memory */
	textureLoad();

	/* open controller */
	scePadInit(0);
	scePadPortOpen(0, 0, pad_dma_buf);

	/* Make initial values for each object */
	for(i = 0; i < NBALLS; i ++){
		theta[i][0] =
			(float)((rand() % 360)-0) * 2.0f *
				3.1415926536f / 360.0f;
		theta[i][1] =
			(float)((rand() % 360)-0) * 2.0f *
				3.1415926536f / 360.0f;
		theta[i][2] =
			(float)((rand() % 360)-0) * 2.0f *
				3.1415926536f / 360.0f;
		trans[i][0] = (float)((rand() % 10000) - 5000);
		trans[i][1] = (float)((rand() % 10000) - 5000);
		trans[i][2] = (float)((rand() % 6000) + 28000);
		dtheta[i][0] =
			(float)((rand() % 6)-3) * 2.0f * 3.1415926536f / 360.0f;
		dtheta[i][1] =
			(float)((rand() % 6)-3) * 2.0f * 3.1415926536f / 360.0f;
		dtheta[i][2] =
			(float)((rand() % 6)-3) * 2.0f * 3.1415926536f / 360.0f;
		dtrans[i][0] = (float)((rand() % 50)-25);
		dtrans[i][1] = (float)((rand() % 50)-25);
		dtrans[i][2] = 0.0f - (float)((rand() % 220));
		color[i][0] = (float)(rand() % 200);
		color[i][1] = (float)(rand() % 200);
		color[i][2] = (float)(rand() % 200);
		color[i][3] = 128.0f;
	}

	/* send GS initialize parmeter */
	d1->chcr.TTE = 1;
	sceDmaSend(d1, &My_dma_start0);

	oddeven = !sceGsSyncV(0);
	frame = 0;
	while(1){

		/* read data of cotroller */
	    if(scePadRead(0, 0, rdata) > 0){
		paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
	    }
	    else{
		paddata = 0;
	    }

		/* get the address of DMApacket area */
		dstaddr = dpmem[frame&1];

		/* change the speed status by controller */
		if(paddata&SCE_PADLup) speed -= 5.0f;
		if(paddata&SCE_PADLdown) speed += 5.0f;
		if(speed > 1000.0f) speed = 1000.0f;
		if(speed < -1000.0f) speed = -1000.0f;

		if(!(paddata&SCE_PADk))
			MakePosition();

		/* setting object behavior */
		for(i = 0; i < NBALLS; i++){
			if(theta[i][0] > 6.283185306f)
				theta[i][0] -= 6.283185306f;
			if(theta[i][1] > 6.283185306f)
				theta[i][1] -= 6.283185306f;
			if(theta[i][2] > 6.283185306f)
				theta[i][2] -= 6.283185306f;
			if(theta[i][0] < 6.283185306f)
				theta[i][0] += 6.283185306f;
			if(theta[i][1] < 6.283185306f)
				theta[i][1] += 6.283185306f;
			if(theta[i][2] < 6.283185306f)
				theta[i][2] += 6.283185306f;

			if(trans[i][0] > 9000.0f){
				if(dtrans[i][0] > 0.0f)
					dtrans[i][0] = 0.0f - dtrans[i][0];
			}
			if(trans[i][0] <-9000.0f){
				if(dtrans[i][0] < 0.0f)
					dtrans[i][0] = 0.0f - dtrans[i][0];
			}
			if(trans[i][1] > 7000.0f){
				if(dtrans[i][1] > 0.0f)
					dtrans[i][1] = 0.0f - dtrans[i][1];
			}
			if(trans[i][1] <-7000.0f){
				if(dtrans[i][1] < 0.0f)
					dtrans[i][1] = 0.0f - dtrans[i][1];
			}
			if(trans[i][2] > 50000.0f){
				if(dtrans[i][2] > 0.0f)
					dtrans[i][2] = 0.0f - dtrans[i][2];
				trans[i][2] = trans[i][2] - 39000.0f;
			}
			if(trans[i][2] < 10000.0f){
				trans[i][2] = 50000.0f;
			}
			RotMatrix(theta[i], trans[i], VLmtx);
			MulMatrix(SVmtx, VLmtx, SLmtx);

			/* Making DMApacket */
			SetMatrix(i);
		}

		/* set DMAend tag at the last of DMApacket*/
		*(u_int *)((u_int)(dstaddr) | 0x20000000) = 0x70000000;
		dstaddr+=1;
		*(u_int *)((u_int)(dstaddr) | 0x20000000) = 0x00000000;
		dstaddr+=1;
		*(u_int *)((u_int)(dstaddr) | 0x20000000) = 0x00000000;
		dstaddr+=1;
		*(u_int *)((u_int)(dstaddr) | 0x20000000) = 0x00000000;
		dstaddr+=1;

		/* Waiting the end of making DMApacket by SetMatrix() */
		WaitDMA();

		/* Waiting the end of drawing previous frame */
		sceGsSyncPath(0, 0);

		/* Waiting the head of V-Blank */
		oddeven = !sceGsSyncV(0);

		/* Add half pixel to offset address for interlace */
		sceGsSetHalfOffset((frame&1)?(&db.draw1):(&db.draw0),
			2048, 2048, oddeven);

		/* Writeback data from D-Cache to Mamory */
		FlushCache(WRITEBACK_DCACHE);

		sceGsSwapDBuff(&db, frame); /* Change Draw/Disp buffers */

		/* waitng double buffer parameter setting to GS*/
		while(DGET_D2_CHCR() & 0x0100);

		/* Sending DMApacket by path1 */
		sceDmaSend(d1, dpmem[frame&1]);

		frame++;
	}
	return 0;
}

void WaitDMA(void)
{
	asm(" WDMA:");
	asm("	bc0f WDMA");
	asm("	nop");
}

void WaitVU1(void)
{
	asm(" WVU1:");
	asm("	bc2t WVU1");
	asm("	nop");
}

void SetSVmatrix(sceVu0FMATRIX svm)
{
	/* Set View -> Screen matrix */
	svm[3][0] = 0.0f;
	svm[2][0] = 0.0f;
	svm[1][0] = 0.0f;
	svm[0][0] = 768.0f;
	svm[3][1] = 0.0f;
	svm[2][1] = 0.0f;
	svm[1][1] = 384.0f;
	svm[0][1] = 0.0f;
	svm[3][2] = 1.0f;
	svm[2][2] = -15640.0f;
	svm[1][2] = 2048.0f;
	svm[0][2] = 2048.0f;
	svm[3][3] = 0.0f;
	svm[2][3] = 1025001000.0f;
	svm[1][3] = 0.0f;
	svm[0][3] = 0.0f;
}

void RotMatrix(sceVu0FVECTOR rot, sceVu0FVECTOR trs, sceVu0FMATRIX vlm)
{

	float c0, c1, c2;
	float s0, s1, s2;

	c0 = cosf(rot[0]);
	c1 = cosf(rot[1]);
	c2 = cosf(rot[2]);
	s0 = sinf(rot[0]);
	s1 = sinf(rot[1]);
	s2 = sinf(rot[2]);

	vlm[0][0] = c1*c2;
	vlm[0][1] = s0*s1*c2 - s2*c0;
	vlm[0][2] = s1*c0*c2 + s0*s2;
	vlm[0][3] = trs[0];
	vlm[1][0] = s2*c1;
	vlm[1][1] = s0*s1*s2 + c0*c2;
	vlm[1][2] = s1*c0*s2 - s0*c2;
	vlm[1][3] = trs[1];
	vlm[2][0] = -s1;
	vlm[2][1] = s0*c1;
	vlm[2][2] = c0*c1;
	vlm[2][3] = trs[2];
	vlm[3][0] = 0.0f;
	vlm[3][1] = 0.0f;
	vlm[3][2] = 0.0f;
	vlm[3][3] = 1.0f;

}

void MulMatrix(sceVu0FMATRIX svm, sceVu0FMATRIX vlm, sceVu0FMATRIX slm)
{
	sceVu0FMATRIX svm0, vlm0, slm0;

	sceVu0TransposeMatrix(svm0, svm);
	sceVu0TransposeMatrix(vlm0, vlm);
	sceVu0MulMatrix(slm0, svm0, vlm0);
	sceVu0TransposeMatrix(slm, slm0);
}

void textureLoad(void)
{
	sceDmaSendN(d2, tex_data, 0x20e);

	sceGsSyncPath(0, 0);
}

float dist(sceVu0FVECTOR *src, sceVu0FVECTOR *dst)
{
	float ret;

	__asm__ __volatile__("
		lqc2	vf4, 0x0(%1)
		lqc2	vf5, 0x0(%2)
		vsub.xyz vf5, vf5, vf4
		vmul.xyz vf4, vf5, vf5
		vmr32.xy vf5, vf4
		vmr32.x  vf6, vf5
		vadd.x   vf7, vf4, vf5
		vadd.x   vf5, vf6, vf7
		vsqrt   Q, vf5x
		vwaitq
		cfc2     $2, $vi22
		mtc1     $2,%0
		" :"=f"(ret) : "r" (src), "r" (dst) :"$2");

	/* return the distance of two vectors */
	return ret;
}

void MakePosition(void)
{
	int i, j;
	float len[2];

	for(i = 0; i < NBALLS; i++){
		theta[i][0] += dtheta[i][0];
		theta[i][1] += dtheta[i][1];
		theta[i][2] += dtheta[i][2];
		trans[i][0] += dtrans[i][0];
		trans[i][1] += dtrans[i][1];
		trans[i][2] += (dtrans[i][2]+speed);
	}

	/* Collision Check and bouncing */
	for(i = 0; i < (NBALLS-1); i++){
		for(j = i+1; j < NBALLS; j++){
			if((len[0] = dist(&trans[i],&trans[j])) < 2000.0f){
				len[1] = dist(&dtrans[j], &zerov);
				dtrans[i][0] = (trans[i][0] - trans[j][0])*len[1]/len[0];
				dtrans[i][1] = (trans[i][1] - trans[j][1])*len[1]/len[0];
				dtrans[i][2] = (trans[i][2] - trans[j][2])*len[1]/len[0];

				len[1] = dist(&dtrans[i], &zerov);
				dtrans[j][0] = (trans[j][0] - trans[i][0])*len[1]/len[0];
				dtrans[j][1] = (trans[j][1] - trans[i][1])*len[1]/len[0];
				dtrans[j][2] = (trans[j][2] - trans[i][2])*len[1]/len[0];

			}
		}
	}
}

/*
 *  1. Makeing the parameter for the calculation of VU1 on ScratchPad RAM
 *  2. transfer data from ScratchPad RAM to the area of DMApacket(MainMemory)
 */
void SetMatrix(int id)
{
	u_int *dp;

	dp = (u_int *)GetDp(id);

	/* DMAcnt (qwc = 18) */
	*dp++ = 0x10000000 | 18;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;

	*dp++ = 0;
	*dp++ = SCE_VIF1_SET_BASE(120, 0);
	*dp++ = SCE_VIF1_SET_OFFSET(240, 0);
	*dp++ = SCE_VIF1_SET_UNPACK(0, 17, V4_32, 0);

	/* local -> Screen matrix */
	*dp++ = *(u_int *)&(SLmtx[0][0]);
	*dp++ = *(u_int *)&(SLmtx[1][0]);
	*dp++ = *(u_int *)&(SLmtx[2][0]);
	*dp++ = *(u_int *)&(SLmtx[3][0]);
	*dp++ = *(u_int *)&(SLmtx[0][1]);
	*dp++ = *(u_int *)&(SLmtx[1][1]);
	*dp++ = *(u_int *)&(SLmtx[2][1]);
	*dp++ = *(u_int *)&(SLmtx[3][1]);
	*dp++ = *(u_int *)&(SLmtx[0][2]);
	*dp++ = *(u_int *)&(SLmtx[1][2]);
	*dp++ = *(u_int *)&(SLmtx[2][2]);
	*dp++ = *(u_int *)&(SLmtx[3][2]);
	*dp++ = *(u_int *)&(SLmtx[0][3]);
	*dp++ = *(u_int *)&(SLmtx[1][3]);
	*dp++ = *(u_int *)&(SLmtx[2][3]);
	*dp++ = *(u_int *)&(SLmtx[3][3]);

	/* local -> View matrix */
	*dp++ = *(u_int *)&(VLmtx[0][0]);
	*dp++ = *(u_int *)&(VLmtx[1][0]);
	*dp++ = *(u_int *)&(VLmtx[2][0]);
	*dp++ = 0;
	*dp++ = *(u_int *)&(VLmtx[0][1]);
	*dp++ = *(u_int *)&(VLmtx[1][1]);
	*dp++ = *(u_int *)&(VLmtx[2][1]);
	*dp++ = 0;
	*dp++ = *(u_int *)&(VLmtx[0][2]);
	*dp++ = *(u_int *)&(VLmtx[1][2]);
	*dp++ = *(u_int *)&(VLmtx[2][2]);
	*dp++ = 0;

	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0x3f800000;

	/* light matrix (world) */
	*dp++ = *(u_int *)&glight[0];
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = *(u_int *)&glight[1];
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = *(u_int *)&glight[2];
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0x3f800000;

	/* light color matrix */
	*dp++ = 0x3f800000;
	*dp++ = 0x3f800000;
	*dp++ = 0x3f800000;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0x3e800000;
	*dp++ = 0x3e800000;
	*dp++ = 0x3e800000;
	*dp++ = 0x3f800000;

	/* vertex color */
	*dp++ = *(u_int *)&color[id][0];
	*dp++ = *(u_int *)&color[id][1];
	*dp++ = *(u_int *)&color[id][2];
	*dp++ = *(u_int *)&color[id][3];

	/* DMAref (qwc = 2912) */
	*dp++ = 0x30000000 | 2912; /* size of object data (qword) */
	*dp++ = (u_int)&My_obj; /* object data */
	*dp++ = 0;
	*dp++ = 0;

	if(id)
		WaitDMA(); /* Waiting the end of previous transer */

	DPUT_D_STAT(0x100); /* Clear the D_STAT.CIS8 for "bc0f" insn. */
	d8->sadr = (u_long128 *)(GetDp(id) & 0x0fffffff);
	sceDmaSendN(d8, dstaddr, ((u_int)dp-GetDp(id))>>4);

	/* change the address of DMApacket area */
	dstaddr += (((u_int)dp - GetDp(id))>>2);
}

