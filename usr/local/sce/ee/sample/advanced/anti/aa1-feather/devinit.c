/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                       - feather aa1 sample  -
 *
 *                         Version 0.1.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : devinit.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.1.0           Oct,07,1999     sasaki    modified from feather
 */

#include <eetypes.h> 
#include <eeregs.h>
#include <libdma.h>
#include <libgraph.h>

void DevInit()
{

	*D_CTRL = 0x01;	/* DMA enable */
	*D_PCR  = 0x83ff0002; /* DMA all ch enable *//* Ch.1 CPCOND */
	*D_STAT = 0x0; /* DMA intr disable */

	sceGsResetPath();

}

