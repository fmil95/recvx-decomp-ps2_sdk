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
 *                          Name : main.c
 *
 *            Version   Date           Design      Log
 *  --------------------------------------------------------------------
 *            0.30      Apr,4,2000     thatake     Beta0
 *            0.40      Jun,16,2000     	   +animation, clut
 *            0.50      Jul,17,2000
 */

#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <libpkt.h>
#include <libvu0.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <libdev.h>
#include <libpad.h>
#include <math.h>
#include "defines.h"
#include "gmain.h"

extern u_int *model_data[];

u_char rdata[32];
u_long128 pad_dma_buf[scePadDmaBufferMax] __attribute__((aligned(64)));

void ErrPrintf(char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");
}

// load iop modules
void load_modules ()
{
    sceSifInitRpc (0);

    while (sceSifLoadModule ("host0:/usr/local/sce/iop/modules/sio2man.irx", 0, NULL) < 0) {
	ErrPrintf("  loading %s failed", "sio2man");
    }
    while (sceSifLoadModule ("host0:/usr/local/sce/iop/modules/padman.irx", 0, NULL) < 0) {
	ErrPrintf("  loading %s failed", "padman");
    }

    printf ("loading modules ... done.\n");

    return;
}





int main(int argc, char *argv[])
{
    u_int paddata=0;
    u_int frame;
    int    odev;

    load_modules ();

    g_init();
    

    /* --- open pad --- */
    scePadInit(0);
    scePadPortOpen(0, 0, pad_dma_buf);
    
    frame = 0;
    odev = !sceGsSyncV(0);

    g_read_model((u_int *)model_data);

    while (1){
      /* --- read pad --- */
	if(scePadRead(0, 0, rdata) > 0){
	    paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);   
	}
	else{ paddata = 0; }

	g_main(frame, paddata);
	
	frame++;

	odev = !sceGsSyncV(0);
	g_sync(frame, odev);
    }

    g_finish();

    return 0;

}

