/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                        - multi movie -
 *
 *                         Version 1.20
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            ldimage.c
 *                  send image data to GS using PATH3
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.10           Mar.29.1999     umemura     the first version
 *       0.20           Aug.13.1999     ywashizu    new IPU movie format
 *       1.20           Oct,15,1999     ywashizu    new IPU movie format
 */


#include <eeregs.h>
#include <eekernel.h>
#include <eestruct.h>
#include <libgraph.h>

#include "ezmovie.h"
#include "ezcube.h"

static void setGIFtag(
    u_int *p,
    u_long regs,
    u_int nreg,
    u_int flg,
    u_int prim,
    u_int pre,
    u_int eop,
    u_int nloop
);
static void setGIFad(u_int *p, u_int reg, u_long value);
static void setBITBLTBUF(u_int *p, u_int dbp, u_int dbw, u_int dpsm);
static void setTRXPOS(u_int *p, u_int dir, u_int dsax, u_int dsay);
static void setTRXREG(u_int *p, u_int rrw, u_int rrh);
static void setTRXDIR(u_int *p, u_int xdir);
static void setDMAscTag(
    u_int *p,
    u_int spr,
    void *addr,
    u_int irq,
    u_int id,
    u_int pce,
    u_int qwc
);
static void setLoadImageTagsGeneral(u_int *tags, void *image, int skip,
				    IDEC_MOVIE movie);

void setLoadImageTags(u_int *tags, void *image, IDEC_MOVIE movie)
{
    setLoadImageTagsGeneral(tags, image, 16*16*4, movie);
}

void setLoadImageTagsTile(u_int *tags, void *image, IDEC_MOVIE movie)
{
    setLoadImageTagsGeneral(tags, image, 0, movie);
}

// ////////////////////////////////////////////////////////////////
// 
// void setLoadImageTagsGeneral(u_int *tags, void *image, int skip,
//                              IDEC_MOVIE movie)
// 
// 	[ packet creation ]
// 
// 	DMA tag, cnt		(3)
// 	    GIF tag a+d		(2)
// 		BITBLTBUF
// 		TRXREG
// -----------------------------------------------------------
// 	DMA tag, cnt		(4)
// 	    GIF tag a+d		(2)
// 		TRXPOS
// 		TRXDIR
// 	    GIF tag image	(w*h*4/16)
// 	DMA tag, ref		(w*h*4/16)
// -----------------------------------------------------------
// 		...
// 		...
// -----------------------------------------------------------
// 	DMA tag, cnt		(4)
// 	    GIF tag a+d		(2)
// 		TRXPOS
// 		TRXDIR
// 	    GIF tag image EOP	(w*h*4/16)
// 	DMA tag, refe		(w*h*4/16)
// -----------------------------------------------------------
static void setLoadImageTagsGeneral(u_int *tags, void *image, int skip,
				    IDEC_MOVIE movie)
{
    u_int *p;
    u_int dbp, dbw, dpsm;
    u_int dir, dsax, dsay;
    u_int rrw, rrh;
    u_int xdir;
    int mbx = movie.width >> 4;
    int mby = movie.height >> 4;
    int i, j;

    // //////////////////////////////////////
    // 
    //  set params
    // 

    dbp = movie.texbp;
    dbw = movie.texbw;
    dpsm = SCE_GS_PSMCT32;

    dir = 0;

    dsax = 0;
    dsay = 0;

    rrw = 16;
    rrh = 16;

    xdir = 0;

    // //////////////////////////////////////
    // 
    //  packet creation
    // 
    p = (u_int*)UncachedAddr(tags);
    setDMAscTag(
	p, // u_int *p,
	0, // u_int spr,
	0, // u_int addr,
	0, // u_int irq,
	1, // u_int id,
	0, // u_int pce,
	3
    ); p += 4;
    setGIFtag(p, 0x0e, 1, 0, 0, 0, 0, 2); p += 4;
    setBITBLTBUF(p, dbp, dbw, dpsm); p += 4;
    setTRXREG(p, rrw, rrh); p += 4;

    // --------------------------------
    for (j = 0; j < mby; j++) {
	for (i = 0; i < mbx; i++) {
	    int eop = (j == mby - 1 && i == mbx - 1)? 1: 0;

	    setDMAscTag(
		p, // u_int *p,
		0, // u_int spr,
		0, // u_int addr,
		0, // u_int irq,
		1, // u_int id,
		0, // u_int pce,
		4
	    ); p += 4;
	    setGIFtag(p, 0x0e, 1, 0, 0, 0, 0, 2); p += 4;
	    setTRXPOS(p, dir, dsax + i*16, dsay + j*16); p += 4;
	    setTRXDIR(p, xdir); p += 4;
	    setGIFtag(p, 0, 0, 2, 0, 0, eop, 16*16*4/16); p += 4;
	    setDMAscTag(
		p, // u_int *p,
		0, // u_int spr,
		DmaAddr(image), // void *addr,
		0, // u_int irq,
		3, // u_int id, (ref)
		0, // u_int pce,
		16*16*4/16
	    ); p += 4;
	    image = (u_char*)image + skip;
	}
    }
    // DMA tag end
    setDMAscTag(
	p, // u_int *p,
	0, // u_int spr,
	NULL, // void *addr,
	0, // u_int irq,
	7, // u_int id,
	0, // u_int pce,
	0
    ); p += 4;
    // --------------------------------
}

static void setDMAscTag(
    u_int *p,
    u_int spr,
    void *addr,
    u_int irq,
    u_int id,
    u_int pce,
    u_int qwc
)
{
    u_long *val = (u_long*)p;
    *val = \
	  (u_long)spr << 63
	| (u_long)((u_int)addr & 0xfffffff0) << 32
	| (u_long)irq << 31
	| (u_long)id << 28
	| (u_long)pce << 26
	| (u_long)qwc << 0;
}

static void setGIFtag(
    u_int *p,
    u_long regs,
    u_int nreg,
    u_int flg,
    u_int prim,
    u_int pre,
    u_int eop,
    u_int nloop
)
{
    p[0] = eop<<15 | nloop;
    p[1] = pre<<(46-32) | prim<<(47-32) | flg<<(58-32) | nreg<<(60-32);
    p[2] = regs&0xffffffff;
    p[3] = regs>>32;
}

static void setGIFad(u_int *p, u_int reg, u_long value)
{
    p[0] = value&0xffffffff;
    p[1] = value>>32;
    p[2] = reg;
    p[3] = 0x00;
}

static void setBITBLTBUF(u_int *p, u_int dbp, u_int dbw, u_int dpsm)
{
    u_long val =
	  ((u_long)dpsm << 56)
	| ((u_long)dbw << 48)
	| ((u_long)dbp << 32);

    setGIFad(p, SCE_GS_BITBLTBUF, val);
}

static void setTRXPOS(u_int *p, u_int dir, u_int dsax, u_int dsay)
{
    u_long val =
	  ((u_long)dir << 59)
	| ((u_long)dsay << 48)
	| ((u_long)dsax << 32);
    setGIFad(p, SCE_GS_TRXPOS, val);
}

static void setTRXREG(u_int *p, u_int rrw, u_int rrh)
{
    u_long val =
	  ((u_long)rrh << 32)
	| ((u_long)rrw << 0);
    setGIFad(p, SCE_GS_TRXREG, val);
}

static void setTRXDIR(u_int *p, u_int xdir)
{
    setGIFad(p, SCE_GS_TRXDIR, (u_long)xdir);
}

void loadImage(u_int *tags)
{
    DPUT_D2_TADR((u_int)DmaAddr(tags));
    DPUT_D2_QWC(0);
    DPUT_D2_CHCR(0x105);
}

