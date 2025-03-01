/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - iga -
 *
 *                         Version 0.1.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : devinit.c
 *                     Description : main procedures
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.1.0           Mar,26,1999     okadaa    1st version
 */

#include <eekernel.h>
#include <eeregs.h>
#include <libdma.h>
#include <libgraph.h>

void DevInit()
{
	sceDmaEnv denv;

	sceDmaReset(1);
	sceDmaGetEnv(&denv);
	denv.notify = 0x0100; /* enable Ch.8 CPCOND */
	sceDmaPutEnv(&denv);

	sceGsResetPath();
}

