/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */

#include <eetypes.h> 
#include <eeregs.h>
#include <libdma.h>
#include <libgraph.h>

int DevInit()
{
	*D_CTRL = 0x01;	/* DMA enable */
	*D_PCR  = 0x83ff0002; /* DMA all ch enable *//* Ch.1 CPCOND */
	*D_STAT = 0x0; /* DMA intr disable */


	sceGsResetPath();

	return(0);
}

