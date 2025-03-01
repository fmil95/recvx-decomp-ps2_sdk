/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 1.6
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                       - mpeg streaming -
 *
 *                         Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            disp.c
 *                    functions for Display
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.10           12.17.1999      umemura     the first version
 */
#include <stdio.h>
#include <libgraph.h>
#include <libpkt.h>
#include "defs.h"
#include "disp.h"
#include "videodec.h"
#include "vobuf.h"

volatile int isCountVblank = 0;
volatile int vblankCount = 0;
volatile int isFrameEnd = 0;
volatile int isUp = 0;
volatile int handler_error = 0;

static u_int localTags[bound(20*4, 16)] bss_align(64);
static u_int clearTags[bound(20*4, 16)] bss_align(64);

extern int frd;

typedef struct {
    int x, y, w, h;
} Rect;

u_int *setGIFtag(
    u_int *p,
    u_long regs,
    u_int nreg,
    u_int flg,
    u_int prim,
    u_int pre,
    u_int eop,
    u_int nloop
);
u_int *setGIFad(u_int *p, u_int reg, u_long value);
u_int *setTEXFLUSH(u_int *p);
u_int *setTEX1_1(u_int *p, u_int lcm, u_int mxl, u_int mmag,
    u_int mmin, u_int mtba, u_int l, u_int k);
u_int *setTEX0_1(u_int *p, u_int tbp, u_int tbw, u_int psm,
    u_int tw, u_int th, u_int tcc, u_int tfx, u_int cbp, u_int cpsm,
    u_int csm, u_int csa, u_int cld);
u_int *setPRIM(u_int *p, u_int prim, u_int iip, u_int tme,
    u_int fge, u_int abe, u_int aa1, u_int fst, u_int ctxt, u_int fix);
u_int *setUV(u_int *p, u_int u, u_int v);
u_int *setRGBAQ(u_int *p, u_int r, u_int g, u_int b,
    u_int a, u_int q);
u_int *setXYZ2(u_int *p, u_int x, u_int y, u_int z);
u_int *setFRAME_1(u_int *p,
	u_int fbp, u_int fbw, u_int psm, u_int fbmask);
u_int *setTEST_1(u_int *p, u_int ate, u_int atst, u_int aref,
    u_int afail, u_int date, u_int datm, u_int zte, u_int ztst);
u_int *setSCISSOR_1(u_int *p,
    u_int scax0, u_int scax1, u_int scay0, u_int scay1);
u_int *setXYOFFSET_1(u_int *p, u_int ofx, u_int ofy);
u_int *setPRMODECONT(u_int *p, u_int ac);
u_int *setPRMODE(u_int *p, u_int iip, u_int tme, u_int fge,
    u_int abe, u_int aa1, u_int fst, u_int ctxt, u_int fix);
u_int *setCLAMP_1(u_int *p, u_int wms, u_int wmt, u_int minu,
    u_int maxu, u_int minv, u_int maxv);
u_int *setBITBLTBUF(u_int *p, u_int dbp, u_int dbw, u_int dpsm);
u_int *setTRXPOS(u_int *p, u_int dir, u_int dsax, u_int dsay);
u_int *setTRXREG(u_int *p, u_int rrw, u_int rrh);
u_int *setTRXDIR(u_int *p, u_int xdir);
u_int *setDMAscTag(
    u_int *p,
    u_int spr,
    void *addr,
    u_int irq,
    u_int id,
    u_int pce,
    u_int qwc
);
u_int *setTexRect(u_int *p, Rect *poly, Rect *tex);
u_int *setFlatRect(u_int *p, Rect *rect, u_int val);

// ////////////////////////////////////////////////////////////////
//
// Create display
//
void dispCreate(
    Display *d,
    int fb_w,
    int fb_h
)
{
    u_int *p;

    isCountVblank = 0;
    vblankCount = 0;
    isFrameEnd = 0;
    isUp = 0;
    handler_error = 0;

    d->fbp0 = 0;
    d->fbp1 = (bound(fb_w, 64) * bound(fb_h, 32)) >> 11;
    d->fb_w = fb_w;
    d->fb_h = fb_h;

    // //////////////////////////////////////
    // 
    //  Initialize GS
    // 
    sceGsResetGraph(
    	0,	// all reset
	SCE_GS_INTERLACE,	// interlace
	SCE_GS_NTSC,		// NTSC
	SCE_GS_FRAME		// frame
    );
    sceGsSetDefDispEnv(
    	&d->dispenv,
	SCE_GS_PSMCT32,
	DISP_WIDTH,
	DISP_HEIGHT,
	0,
	0
    );
    if (MAX_WIDTH > DISP_WIDTH) {
	d->dispenv.dispfb.DBX = (MAX_WIDTH - DISP_WIDTH) / 2;
    }
    if (MAX_HEIGHT > DISP_HEIGHT) {
	d->dispenv.dispfb.DBY = (MAX_HEIGHT - DISP_HEIGHT) / 2;
    }
    d->dispenv.dispfb.FBW = bound(MAX_WIDTH, 64) / 64;
    sceGsPutDispEnv(&d->dispenv);

    sceGsResetPath();

    p = (u_int*)UncAddr(localTags);
    p = setGIFtag(p, 0x0e, 1, 0, 0, 0, 1, 6);
    p = setPRMODECONT(p, 1);
    p = setFRAME_1(p,
    	0, // fbp
	bound(d->fb_w, 64)/64, // fbw
	SCE_GS_PSMCT32, // psm
	0 // fbmask
    );
    p = setTEST_1(p,
    	0, // ate
	0, // atst
	0, // aref
	0, // afail
	0, // date
	0, // datm
	0, // zte
	0 // ztst
    );
    p = setSCISSOR_1(p,
	0, // scax0
	d->fb_w - 1, // scax1
	0, // scay0
	bound(d->fb_h, 32) * 2 + d->fb_h * 3 // scay1
    );
    p = setXYOFFSET_1(p, 0, 0);
    p = setCLAMP_1(p,
    	1, // wms,
	1, // wmt,
	0, // minu,
	0, // maxu,
	0, // minv,
	0 // maxv
    );
    DPUT_D2_MADR((u_int)DmaAddr(localTags));
    DPUT_D2_QWC(7);
    DPUT_D2_CHCR(0x101);

    sceGsSyncPath(0, 0);

}

// ////////////////////////////////////////////////////////////////
//
// Delete display
//
void dispDelete(Display *d)
{
}

// ////////////////////////////////////////////////////////////////
//
// Set tags for image transfer using path3
//
void dispSetTags(
    Display *d, u_int *tags, void *image, int index,
    int disp_x, int disp_y,
    int disp_w, int disp_h,
    int image_w, int image_h
)
{
    u_int *p;
    u_int dbp, dbw, dpsm;
    u_int dir, dsax, dsay;
    u_int rrw, rrh;
    u_int xdir;
    int mbx = image_w >> 4;
    int mby = image_h >> 4;
    int i, j;
    Rect poly0;
    Rect tex0;
    Rect poly1;
    Rect tex1;

    p = (u_int*)UncAddr(tags);

    poly0.x = 0 << 4;
    poly0.y = (bound(d->fb_h, 32) * 2 + d->fb_h * 2) << 4;
    poly0.w = (image_w) << 4;
    poly0.h = (image_h / 2) << 4;

    if (index == 0) {

	tex0.x = 8; // 0.5
	tex0.y = 8; // 0.5
	tex0.w = image_w << 4;
	tex0.h = image_h << 4;

	poly1.x = disp_x << 4;
	poly1.y = disp_y << 4;
	poly1.w = disp_w << 4;
	poly1.h = disp_h << 4;

	tex1.x = 8; // 0.5
	tex1.y = ((d->fb_h * 2) << 4) + 8; // 0.5
	tex1.w = image_w << 4;
	tex1.h = (image_h / 2) << 4;

	// //////////////////////////////////////
	// 
	//  set params
	// 
	dbp = (bound(d->fb_w, 64) * bound(d->fb_h, 32) * 2)/64;
	dbw = bound(d->fb_w, 64)/64;
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
	p = setDMAscTag(
	    p, // u_int *p,
	    0, // u_int spr,
	    0, // u_int addr,
	    0, // u_int irq,
	    1, // u_int id,
	    0, // u_int pce,
	    3
	);
	p = setGIFtag(p, 0x0e, 1, 0, 0, 0, 0, 2);
	p = setBITBLTBUF(p, dbp, dbw, dpsm);
	p = setTRXREG(p, rrw, rrh);

	// --------------------------------
	for (i = 0; i < mbx; i++) {
	    for (j = 0; j < mby; j++) {
		p = setDMAscTag(
		    p, // u_int *p,
		    0, // u_int spr,
		    0, // u_int addr,
		    0, // u_int irq,
		    1, // u_int id,
		    0, // u_int pce,
		    4
		);
		p = setGIFtag(p, 0x0e, 1, 0, 0, 0, 0, 2);
		p = setTRXPOS(p, dir, dsax + i*16, dsay + j*16);
		p = setTRXDIR(p, xdir);
		p = setGIFtag(p, 0, 0, 2, 0, 0, 0, 16*16*4/16);
		p = setDMAscTag(
		    p, // u_int *p,
		    0, // u_int spr,
		    DmaAddr(image), // void *addr,
		    0, // u_int irq,
		    3, // u_int id, (ref)
		    0, // u_int pce,
		    16*16*4/16
		);
		image = (u_char*)image + 16*16*4;
	    }
	}
    } else {
	tex0.x = 8; // 0.5
	tex0.y = 16 + 8; // 1.5
	tex0.w = image_w << 4;
	tex0.h = image_h << 4;

	poly1.x = disp_x << 4;
	poly1.y = (disp_y + bound(d->fb_h, 32)) << 4;
	poly1.w = disp_w << 4;
	poly1.h = disp_h << 4;

	tex1.x = 8; // 0.5
	tex1.y = ((d->fb_h * 2) << 4) + 8; // 0.5
	tex1.w = image_w << 4;
	tex1.h = (image_h / 2) << 4;
    }
    // --------------------------------
    p = setDMAscTag(
	p, // u_int *p,
	0, // u_int spr,
	0, // u_int addr,
	0, // u_int irq,
	7, // u_int id,
	0, // u_int pce,
	16
    );
    p = setGIFtag(p, 0x0e, 1, 0, 0, 0, 1, 15);
    p = setTEXFLUSH(p);
    p = setTEX1_1(p,
        0, // lcm
	0, // mxl
	0, // mmag
	0, // mmin
	0, // mtba,
	0, // l
	0  // k
    );
    p = setTEX0_1(p,
	(bound(d->fb_w, 64) * bound(d->fb_h, 32) * 2)/64, // tbp
	bound(d->fb_w, 64)/64, // tbw
	SCE_GS_PSMCT32, // psm
	10, // tw
	10, // th
	0, // tcc
	1, // tfx
	0, // cbp
	0, // cpsm
	0, // csm
	0, // csa
	0  // cld
    );
    p = setTexRect(p, &poly0, &tex0);

    // --------------------------------
    //  texture mapping to convert size
    p = setTEXFLUSH(p);
    p = setTEX1_1(p,
        0, // lcm
	0, // mxl
	1, // mmag
	1, // mmin
	0, // mtba,
	0, // l
	0  // k
    );
    p = setTexRect(p, &poly1, &tex1);
}

// ////////////////////////////////////////////////////////////////
//
// Switch display buffer
//
void dispSwitch(Display *d, int id)
{
    d->dispenv.dispfb.FBP = (id)? d->fbp1: d->fbp0;
    sceGsPutDispEnv(&d->dispenv);
}

// ////////////////////////////////////////////////////////////////
//
// Clear whole display area
//
void dispClear(Display *d, u_int val)
{
    u_int *p;
    Rect rect;

    rect.x = 0 << 4;
    rect.y = 0 << 4;
    rect.w = d->fb_w << 4;
    rect.h = (bound(d->fb_h, 32) * 2 + d->fb_h * 3) << 4;

    p = (u_int*)UncAddr(clearTags);
    p = setGIFtag(p, 0x0e, 1, 0, 0, 0, 1, 4);
    p = setFlatRect(p, &rect, val);

    DPUT_D2_MADR((u_int)DmaAddr(clearTags));
    DPUT_D2_QWC(5);
    DPUT_D2_CHCR(0x101);

    sceGsSyncPath(0, 0);
}

// ////////////////////////////////////////////////////////////////
//
// Set Rectangle (Texture)
//
u_int *setTexRect(u_int *p, Rect *poly, Rect *tex)
{
    p = setPRIM(p,
    	0x6, // prim
	0, // iip
	1, // tme
	0, // fge
	0, // abe
	0, // aa1
	1, // fst
	0, // ctxt
	0  // fix
    );
    p = setUV(p, tex->x, tex->y);
    p = setXYZ2(p, poly->x, poly->y, 0);

    p = setUV(p, tex->x + tex->w, tex->y + tex->h);
    p = setXYZ2(p, poly->x + poly->w, poly->y + poly->h, 0);

    return p;
}

// ////////////////////////////////////////////////////////////////
//
// Set Rectangle (Flat)
//
u_int *setFlatRect(u_int *p, Rect *rect, u_int val)
{
    p = setPRIM(p,
    	0x6, // prim
	0, // iip
	0, // tme
	0, // fge
	0, // abe
	0, // aa1
	1, // fst
	0, // ctxt
	0  // fix
    );
    p = setXYZ2(p, rect->x, rect->y, 0);

    p = setRGBAQ(p, val & 0xff, (val>>8) & 0xff,
    	(val>>16) & 0xff, (val>>24) & 0xff, 0);
    p = setXYZ2(p, rect->x + rect->w, rect->y + rect->h, 0);

    return p;
}

// ////////////////////////////////////////////////////////////////
//
// General functions to set GS registers
//
u_int *setDMAscTag(
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
    *val = 	  (u_long)spr << 63
	| (u_long)((u_int)addr & 0xfffffff0) << 32
	| (u_long)irq << 31
	| (u_long)id << 28
	| (u_long)pce << 26
	| (u_long)qwc << 0;
    return p + 4;
}

u_int *setGIFtag(
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
    return p + 4;
}

u_int *setGIFad(u_int *p, u_int reg, u_long value)
{
    p[0] = value&0xffffffff;
    p[1] = value>>32;
    p[2] = reg;
    p[3] = 0x00;
    return p + 4;
}

u_int *setTEXFLUSH(u_int *p)
{
    return setGIFad(p, SCE_GS_TEXFLUSH, 0);
}

u_int *setTEX1_1(u_int *p, u_int lcm, u_int mxl, u_int mmag,
    u_int mmin, u_int mtba, u_int l, u_int k)
{
    u_long val = SCE_GS_SET_TEX1_1(lcm, mxl,  mmag,  mmin, mtba, l, k);
    return setGIFad(p, SCE_GS_TEX1_1, val);
}

u_int *setTEX0_1(u_int *p, u_int tbp, u_int tbw, u_int psm,
    u_int tw, u_int th, u_int tcc, u_int tfx, u_int cbp, u_int cpsm,
    u_int csm, u_int csa, u_int cld)
{
    u_long val = SCE_GS_SET_TEX0_1(tbp, tbw, psm, tw, th, tcc, tfx,
    	cbp, cpsm, csm, csa, cld);
    return setGIFad(p, SCE_GS_TEX0_1, val);
}

u_int *setPRIM(u_int *p, u_int prim, u_int iip, u_int tme,
    u_int fge, u_int abe, u_int aa1, u_int fst, u_int ctxt, u_int fix)
{
    u_long val = SCE_GS_SET_PRIM(prim, iip, tme, fge, abe,
	aa1, fst, ctxt, fix);
    return setGIFad(p, SCE_GS_PRIM, val);
}

u_int *setUV(u_int *p, u_int u, u_int v)
{
    u_long val = SCE_GS_SET_UV(u, v);
    return setGIFad(p, SCE_GS_UV, val);
}

u_int *setRGBAQ(u_int *p, u_int r, u_int g, u_int b,
    u_int a, u_int q)
{
    u_long val = SCE_GS_SET_RGBAQ(r, g, b, a, q);
    return setGIFad(p, SCE_GS_RGBAQ, val);
}

u_int *setXYZ2(u_int *p, u_int x, u_int y, u_int z)
{
    u_long val = SCE_GS_SET_XYZ2(x, y, z);
    return setGIFad(p, SCE_GS_XYZ2, val);
}

u_int *setFRAME_1(u_int *p,
	u_int fbp, u_int fbw, u_int psm, u_int fbmask)
{
    u_long val = SCE_GS_SET_FRAME_1(fbp, fbw, psm, fbmask);
    return setGIFad(p, SCE_GS_FRAME_1, val);
}

u_int *setTEST_1(u_int *p, u_int ate, u_int atst, u_int aref,
    u_int afail, u_int date, u_int datm, u_int zte, u_int ztst)
{
    u_long val = SCE_GS_SET_TEST_1(ate,
    	atst, aref, afail, date, datm, zte, ztst);
    return setGIFad(p, SCE_GS_TEST_1, val);
}

u_int *setSCISSOR_1(u_int *p,
    u_int scax0, u_int scax1, u_int scay0, u_int scay1)
{
    u_long val = SCE_GS_SET_SCISSOR_1(scax0, scax1, scay0, scay1);
    return setGIFad(p, SCE_GS_SCISSOR_1, val);
}

u_int *setXYOFFSET_1(u_int *p, u_int ofx, u_int ofy)
{
    u_long val = SCE_GS_SET_XYOFFSET_1(ofx, ofy);
    return setGIFad(p, SCE_GS_XYOFFSET_1, val);
}

u_int *setPRMODECONT(u_int *p, u_int ac)
{
    u_long val = SCE_GS_SET_PRMODECONT(ac);
    return setGIFad(p, SCE_GS_PRMODECONT, val);
}

u_int *setPRMODE(u_int *p, u_int iip, u_int tme, u_int fge,
    u_int abe, u_int aa1, u_int fst, u_int ctxt, u_int fix)
{
    u_long val = SCE_GS_SET_PRMODE(iip, tme, fge, abe, aa1, fst,
    	ctxt, fix);
    return setGIFad(p, SCE_GS_PRMODE, val);
}

u_int *setCLAMP_1(u_int *p, u_int wms, u_int wmt, u_int minu,
    u_int maxu, u_int minv, u_int maxv)
{
    u_long val = SCE_GS_SET_CLAMP_1(wms, wmt, minu, maxu, minv, maxv);
    return setGIFad(p, SCE_GS_CLAMP_1, val);
}

u_int *setBITBLTBUF(u_int *p, u_int dbp, u_int dbw, u_int dpsm)
{
    u_long val =
	  ((u_long)dpsm << 56)
	| ((u_long)dbw << 48)
	| ((u_long)dbp << 32);

    return setGIFad(p, SCE_GS_BITBLTBUF, val);
}

u_int *setTRXPOS(u_int *p, u_int dir, u_int dsax, u_int dsay)
{
    u_long val =
	  ((u_long)dir << 59)
	| ((u_long)dsay << 48)
	| ((u_long)dsax << 32);
    return setGIFad(p, SCE_GS_TRXPOS, val);
}

u_int *setTRXREG(u_int *p, u_int rrw, u_int rrh)
{
    u_long val =((u_long)rrh << 32)
	| ((u_long)rrw << 0);
    return setGIFad(p, SCE_GS_TRXREG, val);
}

u_int *setTRXDIR(u_int *p, u_int xdir)
{
    return setGIFad(p, SCE_GS_TRXDIR, (u_long)xdir);
}

// ///////////////////////////////////////////////////////////////////
//
// Load image data to GS using path3
//
void loadImage(u_int *tags)
{
    *D2_TADR = (u_int)DmaAddr(tags);
    *D2_QWC  = 0;
    *D2_CHCR = 0x105;
}

// /////////////////////////////////////////////////////////////////////
//
// vblank handler
//
int vblankHandler(int val)
{
    extern Display display;
    isUp = ((*GS_CSR) >> 13) & 1;

    if (isCountVblank) {
	VoTag *tag;

    	vblankCount++;

	handler_error = sceGsSyncPath(1, 0);
	if (!handler_error) { // no error
	    tag = voBufGetTag(&voBuf);
	    if (!tag) {
		frd++;
		EI();
		return 0;
	    }

	    if (!isUp && tag->status == VOBUF_STATUS_FULL) {
	    	dispSwitch(&display, 0);
		loadImage(tag->v[1]);
		tag->status = VOBUF_STATUS_TOPDONE;
	    } else if (isUp && tag->status == VOBUF_STATUS_TOPDONE) {
	    	dispSwitch(&display, 1);
		loadImage(tag->v[0]);
		tag->status = VOBUF_STATUS_;
		isFrameEnd = 1;
	    }
	}
    }

    ExitHandler();
    return 0;
}

// ///////////////////////////////////////////////////////////////
// 
//  Handler to check the end of image transfer
// 
int handler_endimage(int val)
{

    if (isCountVblank) {
	if (isFrameEnd) {
	    voBufDecCount(&voBuf);
	    isFrameEnd = 0;
	}
    }
    ExitHandler();
    return 0;
}

// ///////////////////////////////////////////////////////////////////
// 
//  Wait until even/odd field
//  Start to count vblank
// 
void startDisplay(int waitEven)
{
    // wait untill even field
    while (sceGsSyncV(0) == waitEven)
    	;

    frd = 0;
    isCountVblank = 1;
    vblankCount = 0;
}

// ///////////////////////////////////////////////////////////////////
// 
//  Stop to count vblank
// 
void endDisplay()
{
    isCountVblank =  0;
    frd = 0;
}

