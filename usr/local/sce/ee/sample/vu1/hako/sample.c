/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - hako -
 *
 *                         Version 0.1.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : hako.c
 *                     Description : main procedures
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.1.0           Mar,26,1999     okadaa    1st version
 */


#include <stdio.h>
#include <stdlib.h>
#include <eekernel.h>
#include <eeregs.h>
#include <math.h>
#include <libgraph.h>
#include <libdma.h>

extern unsigned int My_dma_start0 __attribute__((section(".vudata")));
extern unsigned int My_dma_start __attribute__((section(".vudata")));
extern unsigned int My_param __attribute__((section(".vudata")));

typedef struct {
	float m[4][4];
} FMATRIX;

typedef struct {
	float v[4];
} FVECTOR;

FMATRIX VLmtx; /* ViewLocal matrix */
FVECTOR theta; /* angle of the object */
FVECTOR trans; /* position of the object */
FVECTOR dtheta; /* rotation quantity per one frame */

sceGsDBuff db;
sceDmaChan *d1, *d2;

/* Make ViewLocal matrix */
void RotMatrix(FVECTOR *rot, FVECTOR *trs, FMATRIX *vlm);
/* Set parameter to DMApacket */
void SetMatrix(void);

int main()
{
	int odev;
	u_int frame;

	/* Initialize devices */
	sceDmaReset(1);
	sceGsResetPath();

	d1 = sceDmaGetChan(1);
	d2 = sceDmaGetChan(2);
	d1->chcr.TTE = 1; /* DMAtag transfer enable */

	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
	sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224,
		SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);

	/* initial parameter */
	theta.v[0] = 0.0f;
	theta.v[1] = 0.0f;
	theta.v[2] = 0.0f;
	trans.v[0] = 0.0f;
	trans.v[1] = 0.0f;
	trans.v[2] = 1000.0f;
	dtheta.v[0] =
		(float)((rand() % 6)-3) * 2.0f * 3.1415926536f / 360.0f;
	dtheta.v[1] =
		(float)((rand() % 6)-3) * 2.0f * 3.1415926536f / 360.0f;
	dtheta.v[2] =
		(float)((rand() % 6)-3) * 2.0f * 3.1415926536f / 360.0f;

	/* send VIF1/VU1 initialize data */
	sceDmaSend(d1, &My_dma_start0);

	frame = 0;
	odev = !sceGsSyncV(0);

	while(1){
		/* Waiting previous drawing */
		sceGsSyncPath(0, 0);

		/* Set clear color */
		((sceGsRgbaq *)&db.clear0.rgbaq)->R = frame & 0xff;
		((sceGsRgbaq *)&db.clear0.rgbaq)->G = 0x80;
		((sceGsRgbaq *)&db.clear0.rgbaq)->B = 0x80;
		((sceGsRgbaq *)&db.clear0.rgbaq)->Q = 1.0f;
		((sceGsRgbaq *)&db.clear1.rgbaq)->R = frame & 0xff;
		((sceGsRgbaq *)&db.clear1.rgbaq)->G = 0x80;
		((sceGsRgbaq *)&db.clear1.rgbaq)->B = 0x80;
		((sceGsRgbaq *)&db.clear1.rgbaq)->Q = 1.0f;

		odev = !sceGsSyncV(0);
		/* add half pixel to offset address for interlace */
		sceGsSetHalfOffset((frame&1)?(&db.draw1):(&db.draw0),
			2048, 2048, odev);
		/* Writeback data from D-Cache to Memory */
		FlushCache(WRITEBACK_DCACHE);

		sceGsSwapDBuff(&db, frame);

		/* change parameters */
		theta.v[0] += dtheta.v[0];
		theta.v[1] += dtheta.v[1];
		theta.v[2] += dtheta.v[2];
		if(theta.v[0] > 6.283185306f) theta.v[0] -= 6.283185306f;
		if(theta.v[1] > 6.283185306f) theta.v[1] -= 6.283185306f;
		if(theta.v[2] > 6.283185306f) theta.v[2] -= 6.283185306f;

		/* Make ViewLocal matrix */
		RotMatrix(&theta, &trans, &VLmtx);
		/* Set parameters to DMApacket */
		SetMatrix();

		/* Wait for sceGsSwapDBuff() */
		sceDmaSync(d2, 0, 0);

		/* Writeback data by SetMatrix() from D-Cache to Memory */
		FlushCache(WRITEBACK_DCACHE);

		/* Send DMApacket */
		sceDmaSend(d1, &My_dma_start);

		frame++;
	}
	return 0;
}

void RotMatrix(FVECTOR *rot, FVECTOR *trs, FMATRIX *vlm)
{
	float c0, c1, c2;
	float s0, s1, s2;

	c0 = cosf(rot->v[0]);
	c1 = cosf(rot->v[1]);
	c2 = cosf(rot->v[2]);
	s0 = sinf(rot->v[0]);
	s1 = sinf(rot->v[1]);
	s2 = sinf(rot->v[2]);

	vlm->m[0][0] = c1*c2;
	vlm->m[0][1] = s0*s1*c2 - s2*c0;
	vlm->m[0][2] = s1*c0*c2 + s0*s2;
	vlm->m[0][3] = trs->v[0];
	vlm->m[1][0] = s2*c1;
	vlm->m[1][1] = s0*s1*s2 + c0*c2;
	vlm->m[1][2] = s1*c0*s2 - s0*c2;
	vlm->m[1][3] = trs->v[1];
	vlm->m[2][0] = -s1;
	vlm->m[2][1] = s0*c1;
	vlm->m[2][2] = c0*c1;
	vlm->m[2][3] = trs->v[2];
	vlm->m[3][0] = 0.0f;
	vlm->m[3][1] = 0.0f;
	vlm->m[3][2] = 0.0f;
	vlm->m[3][3] = 1.0f;

}

void SetMatrix(void)
{
	/* Set the ViewLocal matrix to DMApacket */
	((u_int *)&My_param)[0] = *(u_int *)&(VLmtx.m[0][0]);
	((u_int *)&My_param)[1] = *(u_int *)&(VLmtx.m[1][0]);
	((u_int *)&My_param)[2] = *(u_int *)&(VLmtx.m[2][0]);
	((u_int *)&My_param)[3] = *(u_int *)&(VLmtx.m[3][0]);

	((u_int *)&My_param)[4] = *(u_int *)&(VLmtx.m[0][1]);
	((u_int *)&My_param)[5] = *(u_int *)&(VLmtx.m[1][1]);
	((u_int *)&My_param)[6] = *(u_int *)&(VLmtx.m[2][1]);
	((u_int *)&My_param)[7] = *(u_int *)&(VLmtx.m[3][1]);

	((u_int *)&My_param)[8] = *(u_int *)&(VLmtx.m[0][2]);
	((u_int *)&My_param)[9] = *(u_int *)&(VLmtx.m[1][2]);
	((u_int *)&My_param)[10] = *(u_int *)&(VLmtx.m[2][2]);
	((u_int *)&My_param)[11] = *(u_int *)&(VLmtx.m[3][2]);

	((u_int *)&My_param)[12] = *(u_int *)&(VLmtx.m[0][3]);
	((u_int *)&My_param)[13] = *(u_int *)&(VLmtx.m[1][3]);
	((u_int *)&My_param)[14] = *(u_int *)&(VLmtx.m[2][3]);
	((u_int *)&My_param)[15] = *(u_int *)&(VLmtx.m[3][3]);
}
