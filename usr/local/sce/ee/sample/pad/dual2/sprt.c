/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */

#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdev.h>
#include <libdma.h>
#include <libpkt.h>
#include "pstypes.h"

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	224
#define OFFX	(((4096-SCREEN_WIDTH)/2)<<4)
#define OFFY	(((4096-SCREEN_HEIGHT)/2)<<4)

static float fog = 1.0f;

void sceMakeSprt(SPRTX* sprt, int x, int y, int w, int h)
{
	sprt->tex0 = 0;
	sprt->tbase = 0;
	sprt->fog = fog;
	setXY0(sprt, x, y);
	setWH(sprt, w, h);
	setUV(sprt, 0, 0);
	setRGB0(sprt, 0x80, 0x80, 0x80);
	sprt->a0 = 0x80;
}

void sceVif1AddSprite(void* pp, SPRTX* sprt, int n)
{
	int i;
	int alpha = 0;
	sceVif1Packet* p;

	p = (sceVif1Packet*)pp;

	for (i=0; i<n; i++)
	{
		if (i%3 == 0)
		{
			sceVif1PkAddGsAD(p, SCE_GS_PRIM,SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE,0,0,0,alpha,0,1,0,0));
			sceVif1PkAddGsAD(p, SCE_GS_RGBAQ,SCE_GS_SET_RGBAQ(sprt->r0,sprt->g0,sprt->b0, 0x80,*(u_int *)&sprt->fog));
		}
		sceVif1PkAddGsAD(p, SCE_GS_ALPHA_1, SCE_GS_SET_ALPHA( 1, 2, 0, 0, 0x80));
		sceVif1PkAddGsAD(p, SCE_GS_TEXA, SCE_GS_SET_TEXA(0,0x80,0));
		sceVif1PkAddGsAD(p, SCE_GS_UV, SCE_GS_SET_UV(0, 0));
		sceVif1PkAddGsAD(p, SCE_GS_XYZ2,SCE_GS_SET_XYZ(OFFX+(sprt->x0<<4), OFFY+((sprt->y0/2)<<4),1));
		sceVif1PkAddGsAD(p, SCE_GS_UV, SCE_GS_SET_UV(sprt->w<<4, sprt->h<<4));
		sceVif1PkAddGsAD(p, SCE_GS_XYZ2,SCE_GS_SET_XYZ(OFFX+((sprt->x0+sprt->w)<<4), OFFY+((sprt->y0/2+sprt->h/2)<<4),1));
		sprt++;
	}
}
