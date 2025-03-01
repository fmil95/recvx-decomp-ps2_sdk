/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *               Graphics Framework Sample Program
 *                  
 *              ---- Scene Viewer Version 0.40 ----
 * 
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                          Name : framebuf.c
 *
 *            Version   Date           Design      Log
 *  --------------------------------------------------------------------
 *            0.40      Jun,16,2000    aoki
 */


#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgraph.h>
#include "defines.h"
#include "framebuf.h"


/* this arg is NOT cpsm but psm */
u_int getClutGsSize(u_int psm)
{
    switch (psm){
    case SCE_GS_PSMCT32: 
    case SCE_GS_PSMCT24:
    case SCE_GS_PSMCT16:
	return 0;
    case SCE_GS_PSMT8H:
    case SCE_GS_PSMT8:
	return 2;
    case SCE_GS_PSMT4:
    case SCE_GS_PSMT4HH:
    case SCE_GS_PSMT4HL:
	return 1;
    default:
	ErrPrintf("getGsSize: illegal psm format\n");
	return 0;
    }
}

u_int getTexGsSize(u_int bw, u_int psm, u_int height)
{
    int pixel_per_word;
    u_int size;

    switch (psm){
    case SCE_GS_PSMCT32: 
    case SCE_GS_PSMCT24:
    case SCE_GS_PSMT8H:
    case SCE_GS_PSMT4HH:
    case SCE_GS_PSMT4HL:
	pixel_per_word=1; 
	break;
    case SCE_GS_PSMCT16:
	pixel_per_word=2; 
	break;
    case SCE_GS_PSMT8:
	pixel_per_word=4; 
	break;
    case SCE_GS_PSMT4:
	pixel_per_word=8; 
	break;
    default:
	pixel_per_word=0;
	ErrPrintf("getTextSize: illegal psm format\n");
	break;
    }
    size=32*((((bw*height)/pixel_per_word)+31)/32);

    return size;
}

u_int getCbp(u_int gssize, int align, int resident)
{
    static u_int cbp=INIT_CBP;
    u_int v;
    
    if (resident){
	switch (align){
	case FW_GS_ALIGN_ANY:
	    cbp-=gssize;
	    v=cbp;
	    break;
	case FW_GS_ALIGN_PAGE:
	    cbp-=gssize;
	    cbp=32*(cbp/32);			/* page break align (2048/64=32) */
	    v=cbp;
	    break;
	default:
	    ErrPrintf("getCbp: illegal align value\n");
	    return 0;
	}
	if (cbp<INIT_CBP-CLUT_RES_SIZE){
	    ErrPrintf("getCbp: too many resident CLUTs\n");
	    v=INIT_CBP-CLUT_RES_SIZE;
	}
    } else{
	v=INIT_CBP-CLUT_RES_SIZE-gssize;
	v=32*(v/32);			/* page break align (2048/64=32) */

    }

    return v;
}

u_int getTbp(u_int gssize, int resident)
{
    static u_int tbp=INIT_TBP;
    u_int v;
    
    if (resident){
	v=tbp;
	tbp+=gssize;
	tbp=32*((tbp+31)/32);			/* page break align (2048/64=32) */

	if (tbp>INIT_TBP+TEXEL_RES_SIZE){
	    ErrPrintf("getTbp: too many resident texels\n");
	    v=INIT_TBP+TEXEL_RES_SIZE-gssize;
	}
    } else{
	v=INIT_TBP+TEXEL_RES_SIZE;
    }

    return v;
}
