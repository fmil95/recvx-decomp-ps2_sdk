/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *                   I/O Proseccor sample Code
 *                          Version 1.20
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                       ezbgm.irx - bgm_main.c
 *                           entry function
 *
 *   Version   Date            Design    Log
 *  --------------------------------------------------------------------
 *   1.20      Nov.23.1999     morita    first checked in
 */

#include <kernel.h>
#include <sys/types.h>
#include <stdio.h>
#include <sif.h>
#include <sifcmd.h>
#include <sifrpc.h>
#include "bgm_i.h"

ModuleInfo Module = {"ezbgm_driver", 0x0102 };

extern int sce_bgm_loop();
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

	printf("EzBGM driver version 1.2.0\n");

	param.attr         = TH_C;
	param.entry        = sce_bgm_loop;
	param.initPriority = BASE_priority-2;
	param.stackSize    = 0x800;
	param.option       = 0;
	th = CreateThread(&param);
	if (th > 0) {
		StartThread(th,0);
		printf(" Exit EzBGM loader thread \n");
		return 0;
	}else{
		return 1;
	}
}


/* ----------------------------------------------------------------
 *	End on File
 * ---------------------------------------------------------------- */
/* DON'T ADD STUFF AFTER THIS */

