/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
// Version 0.00
// ASCII
// 
// Copyright (C) 1998-1999 Sony Computer Entertainment Inc. All Rights Reserved.
// 
// Version        Date            Design      Log
// --------------------------------------------------------------------
// 0.00           10/13/99        koji        1st version

#include <stdio.h>
#include <stdlib.h>
#include <eekernel.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <sifdev.h>
#include <libvu0.h>
#include <math.h>
#include "pdriver.h"

#define PI (3.141592f)
#define R 255.0f
#define G 255.0f
#define B 255.0f
#define A 127.0f

#define NVERTEX	(0x0c * 2)
#define PRIM	0x8b

static sceVu0FVECTOR vertex[NVERTEX] = {
	{ 0.0,  0.0,  0.0,  1.0},
	{ 1.5,  3.0,  0.0,  1.0},
	{ 1.5,  1.5,  0.0,  1.0},
	{ 0.0,  0.0,  0.0,  1.0},
	{ 3.0,  1.5,  0.0,  1.0},
	{ 1.5,  1.5,  0.0,  1.0},
	{ 0.0,  0.0,  0.0,  1.0},
	{ 3.0,  3.0,  0.0,  1.0},
	{ 2.9,  3.1,  0.0,  1.0},
	{ 0.0,  0.0,  0.0,  1.0},
	{ 3.0,  3.0,  0.0,  1.0},
	{ 3.1,  2.9,  0.0,  1.0},

	{ 0.0,  0.0,  0.1,  1.0},
	{ 1.5,  3.0,  0.1,  1.0},
	{ 1.5,  1.5,  0.1,  1.0},
	{ 0.0,  0.0,  0.1,  1.0},
	{ 3.0,  1.5,  0.1,  1.0},
	{ 1.5,  1.5,  0.1,  1.0},
	{ 0.0,  0.0,  0.1,  1.0},
	{ 3.0,  3.0,  0.1,  1.0},
	{ 2.9,  3.1,  0.1,  1.0},
	{ 0.0,  0.0,  0.1,  1.0},
	{ 3.0,  3.0,  0.1,  1.0},
	{ 3.1,  2.9,  0.1,  1.0},
};
static sceVu0FVECTOR nrml[NVERTEX];
static sceVu0FVECTOR texUV[NVERTEX];		// dummy
static sceVu0FVECTOR color[NVERTEX];
static float delta = 5.0f * PI / 180.0f;

typedef union {
	u_long128	q;
	u_long		d[2];
	u_int		w[4];
} DATA16;

extern void sceVu0RotTransPersNClipCol(u_long128 *v0,
			sceVu0FMATRIX m0, sceVu0FVECTOR *v1, sceVu0FVECTOR *nv,
			sceVu0FVECTOR *st, sceVu0FVECTOR *c, sceVu0FMATRIX ll,
			sceVu0FMATRIX lc, int n) ;
extern void sceVu0RotCameraMatrix(sceVu0FMATRIX m, sceVu0FVECTOR p,
			sceVu0FVECTOR zd, sceVu0FVECTOR yd, sceVu0FVECTOR rot);


static INLINE void SetAlphaEnv(sceDmaChan *dmaVif) {
	int sz;
	static struct {
		sceGifTag   	giftag;
		sceGsAlphaEnv	al;
	} alpha;

	sz = sceGsSetDefAlphaEnv(&alpha.al, 0);
	SCE_GIF_CLEAR_TAG(&alpha.giftag);
	alpha.giftag.NLOOP = sz;
	alpha.giftag.EOP = 1;
	alpha.giftag.NREG = 1;
	alpha.giftag.REGS0 = 0xe;
	FlushCache(0);
	dmaVif->chcr.TTE = 1;
	sceDmaSendN(dmaVif, &alpha.giftag, sz+1);
	sceGsSyncPath(0, 0);
}


static INLINE DATA16 *MakePacket(sceVu0FMATRIX ls,
									sceVu0FMATRIX ll, sceVu0FMATRIX lc) {
	int n = 0;
	DATA16 *p = (DATA16 *)0x70000000;					// on Scratchpad

	p[n].q = (u_long128)0;
	p[n++].w[0] = 0x70000000|(NVERTEX*3+1);

	// add vertex info(GIFtag, STQ & RGBA & XYZ)
	p[n].d[0] = SCE_GIF_SET_TAG(NVERTEX, 1, 1, PRIM, SCE_GIF_PACKED, 3);
	p[n++].d[1] = 0x0412L;

	sceVu0RotTransPersNClipCol(&p[n].q, ls, vertex, nrml,
										texUV, color, ll, lc, NVERTEX);
	return p;
}


int main(void) {
	int		i, odev;
	u_int	fr = 0;
	DATA16	*pkt;
	static sceGsDBuff  db;
	static sceDmaChan *dmaVif;

	static sceVu0FVECTOR eyePos  = { 0.0,  0.0,  -55, 0 };
	static sceVu0FVECTOR eyeZd   = { 0.0,  0.0,  1.0, 1 };
	static sceVu0FVECTOR eyeYd   = { 0.0,  1.0,  0.0, 1 };
	static sceVu0FVECTOR eyeRot  = { 0.0,  0.0,  0.0, 0 };
	static sceVu0FVECTOR ambient = { 0.2,  0.2,  0.2, 0 };
	static sceVu0FVECTOR trans   = { 0.0,  0.0,  0.0, 0 };
	static sceVu0FVECTOR rot     = { 0.0,  0.0,  0.0, 0 };
	static sceVu0FVECTOR light0  = { 0.0,  1.5,  0.5, 0 };
	static sceVu0FVECTOR light1  = { 1.5, -1.5,  0.5, 0 };
	static sceVu0FVECTOR light2  = {-1.5, -1.5,  0.5, 0 };
	static sceVu0FVECTOR lcol[3];	// light colors
	static sceVu0FMATRIX localWorld, worldView, viewScreen, localScreen;
	static sceVu0FMATRIX normalLight, localLight, lightColor, tmp;

	if (InitProtocolDriver() < 0) {
		printf("InitProtocolDriver() failed.\n");
		return 1;
	}

	sceGsResetPath(); sceDmaReset(1); sceVpu0Reset();
	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
	sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224, SCE_GS_ZGREATER,
										SCE_GS_PSMZ24, SCE_GS_CLEAR);
	*(u_long *)&db.clear0.rgbaq = *(u_long *)&db.clear1.rgbaq
		= SCE_GS_SET_RGBAQ(0x10, 0x10, 0x18, 0x80, 0x3f800000);
	sceGsSyncPath(0, 0);

	dmaVif = sceDmaGetChan(SCE_DMA_GIF);
	SetAlphaEnv(dmaVif);

	// view -> screen matrix
	sceVu0ViewScreenMatrix(viewScreen, 512.0f, 1.0f, 0.47f,
			2048.0f, 2048.0f, 1.0f, 16777215.0f, 1.0f, 65536.0f);

	for (i=0; i<NVERTEX; i++) {		// init vertex's colors and normals
		color[i][0]=R; color[i][1]=G; color[i][2]=B; color[i][3]=A;
		nrml[i][0]=0.0f; nrml[i][1]=0.0f; nrml[i][3]=1.0f;
		if (i < NVERTEX/2) nrml[i][2]=-1.0f;
		else               nrml[i][2]= 1.0f;
	}

	odev = !sceGsSyncV(0);
	while(1) {
		static int xyb[3] = {0,0,0};
		sceGsSetHalfOffset((fr&1) ? &db.draw1 : &db.draw0, 2048, 2048, odev);
		FlushCache(0);
		sceGsSwapDBuff(&db, fr++);
		sceGsSyncPath(0, 0);

		rot[1] += delta;
		if (rot[1] > PI) rot[1] -= 2.0f*PI;

		GetMouseInfo(xyb);
		trans[0] = xyb[0]/9.4f;
		trans[1] = xyb[1]/9.4f;
		for (i=0; i<3; i++) {
			switch (xyb[2]) {
				case 1:
					rot[1] += delta;
					if (rot[1] > PI) rot[1] -= 2.0f*PI;
					lcol[i][0]=0.8f; lcol[i][1]=0.0f; lcol[i][2]=0.0f; break;
				case 2:
					if (!i) {
						rot[1] -= delta;
						if (rot[1] <-PI) rot[1] += 2.0f*PI;
					}
					lcol[i][0]=0.0f; lcol[i][1]=0.8f; lcol[i][2]=0.0f; break;
				case 3:
					rot[1] += delta;
					if (rot[1] > PI) rot[1] -= 2.0f*PI;
					lcol[i][0]=0.0f; lcol[i][1]=0.0f; lcol[i][2]=0.8f; break;
				default:
					lcol[i][0]=0.8f; lcol[i][1]=0.8f; lcol[i][2]=0.8f; break;
			}
			lcol[i][3]=1.0f;
		}

		sceVu0NormalLightMatrix(normalLight, light0, light1, light2);
		sceVu0LightColorMatrix(lightColor, lcol[0], lcol[1], lcol[2], ambient);
		sceVu0UnitMatrix(localWorld);
		sceVu0RotMatrixZ(localWorld, localWorld, (45.0f*PI/180.0f));
		sceVu0RotMatrix(localWorld, localWorld, rot);
		sceVu0RotMatrixZ(localWorld, localWorld, -(45.0f * PI / 180.0f));
		sceVu0MulMatrix(localLight, normalLight, localWorld);
		sceVu0TransMatrix(localWorld, localWorld, trans);
		sceVu0RotCameraMatrix(worldView, eyePos, eyeZd, eyeYd, eyeRot);
		sceVu0MulMatrix(tmp, worldView, localWorld);
		sceVu0MulMatrix(localScreen, viewScreen, tmp);

		pkt = MakePacket(localScreen, localLight, lightColor);
		sceDmaSend(dmaVif, (u_long128 *)((u_int)pkt|0x80000000));
		sceGsSyncPath(0, 0);

		odev = !sceGsSyncV(0);
    }

    return 0;
}

// Local variables:
// tab-width: 4
// End:
// vi:set tabstop=4:

