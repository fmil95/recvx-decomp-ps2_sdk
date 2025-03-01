/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
/*
 *
 *	texture load from raw file
 *
 */
#include <eekernel.h>
#include <eeregs.h>
#include <math.h>
#include <stdio.h>
#include <libgraph.h>
#include <libdma.h>
#include <libvu0.h>
#include <libdev.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <libpad.h>


// set giftag and GS registers to load texture
u_long *setTextureHeader(u_long *buffer, u_int dbp, u_int dbw, u_int dpsm, u_int rrw, u_int rrh, int col)
{
    *buffer++ = SCE_GIF_SET_TAG(4, 0, 0, 0, 0, 1); //nloop, eop, pre, prim, flg, nreg
    *buffer++ = 0xe; // A+D

    *buffer++ = SCE_GS_SET_BITBLTBUF(0,0,0, dbp, dbw, dpsm); //sbp, sbw, spsm, dbp, dbw, dpsm
    *buffer++ = SCE_GS_BITBLTBUF;

    *buffer++ = SCE_GS_SET_TRXPOS(0,0,0,0,0); //ssax, ssay, dsax, dsay, dir
    *buffer++ = SCE_GS_TRXPOS;

    *buffer++ = SCE_GS_SET_TRXREG(rrw, rrh); //rrw, rrh
    *buffer++ = SCE_GS_TRXREG;

    *buffer++ = SCE_GS_SET_TRXDIR(0); //dir (0: host-local)
    *buffer++ = SCE_GS_TRXDIR;

    *buffer++ = SCE_GIF_SET_TAG((rrw*rrh*col+15)/16, 1, 0, 0, 2, 0); //nloop, eop, pre, prim, flg, nreg
    *buffer++ = 0x0;

    return buffer;
}

// read texture from file
void readtex(char *fn, void *buffer, int size)
{
    int fd;
    int v;

    fd = sceOpen(fn, SCE_RDONLY);
    if (fd<0){ printf("texture file open error! (%s)\n", fn); return; }

    v=sceRead(fd, buffer, size);
    if (v!=size){ printf("texture read error! (%s)\n", fn); return; }

    sceClose(fd);
    return;
}

