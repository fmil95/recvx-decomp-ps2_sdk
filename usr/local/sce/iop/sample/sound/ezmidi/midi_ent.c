/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *                   I/O Proseccor sample Code
 *                          Version 1.30
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                        ezmidi.irx - midi_ent.c
 *                           entry function
 *
 *   Version   Date            Design    Log
 *  --------------------------------------------------------------------
 *   1.30      Nov.12.1999     morita    first checked in
 */

#include <kernel.h>
#include <sys/types.h>
#include <stdio.h>
#include <sif.h>
#include <sifcmd.h>
#include <sifrpc.h>
#include "ezmidi_i.h"

ModuleInfo Module = {"ezmidi_driver", 0x0103 };

extern int sce_midi_loop();
extern volatile int gStThid;

int create_th();

int start ()
{
	struct ThreadParam param;
	int th;

	CpuEnableIntr();

	if( ! sceSifCheckInit() )
		sceSifInit();
	sceSifInitRpc(0);

	printf("EzMIDI driver version 1.2.0\n");

	param.attr         = TH_C;
	param.entry        = sce_midi_loop;
	param.initPriority = BASE_priority-2;
	param.stackSize    = 0x800;
	param.option       = 0;
	th = CreateThread(&param);
	if (th > 0) {
		StartThread(th,0);
		printf(" Exit EzMIDI loader thread \n");
		return 0;
	}else{
		return 1;
	}
}


/* ----------------------------------------------------------------
 *	End on File
 * ---------------------------------------------------------------- */
/* DON'T ADD STUFF AFTER THIS */

