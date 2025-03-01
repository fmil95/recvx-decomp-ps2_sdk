/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *                    I/O Proseccor sample Library
 *                          Version 0.50
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                       rs2drv.irx - rsd_cb.c
 *                         call back thread
 *
 *   Version   Date            Design    Log
 *  --------------------------------------------------------------------
 *   0.30      Jun.17.1999     morita    first checked in
 *   0.50      Aug.18.1999     morita    SpuStSetCore etc. added.
 *                                       rewrite for new siflib.
 */

#include <kernel.h>
#include <sys/types.h>
#include <stdio.h>
#include "sif.h"
#include "sifcmd.h"
#include "sifrpc.h"
#include "sdr_i.h"

#define _SCE_IOP_STUB_

//#define PRINTF(x) printf x
#define PRINTF(x)

#if 1
#define SDR_CB_DMA0 1
#define SDR_CB_DMA1 2
#define SDR_CB_IRQ  3
#define SDR_CB_DMA0INT 11
#define SDR_CB_DMA1INT 12
#define SDR_CB_IRQINT  13
#endif

unsigned int  sdrcbFunc(void *data, int size);

volatile static SdrEECBData eeCBData;
volatile int gStThid;
int bid;
sceSifClientData cd;


void sceSifCmdLoop2()
{
  /*コマンドを待つ処理*/
  while(1) {
    if( eeCBData.mode != 0 )
    {
	sceSifCallRpc( &cd, 0, 0, (void*)&eeCBData, sizeof(SdrEECBData), NULL, 0, NULL,0);

	PRINTF(("sceSifCallRpc  IOP -> EE :command %d\n",eeCBData.mode));
	eeCBData.mode = 0;
    }
    /* 次のコマンドが来るまで眠る */
	PRINTF(("******* Sleep *********\n"));
	SleepThread();
	PRINTF(("******* Wake UP *********\n"));
  }
  return;
}

int _sce_sdrDMA0CallBackProc( void* common )
{
	eeCBData.mode = SDR_CB_DMA0;
	iWakeupThread( gStThid );
	return 1;
}

int _sce_sdrDMA1CallBackProc( void* common )
{
	eeCBData.mode = SDR_CB_DMA1;
	iWakeupThread( gStThid );
	return 1;
}

int _sce_sdrIRQCallBackProc( void* common )
{
	eeCBData.mode = SDR_CB_IRQ;
	iWakeupThread( gStThid );
	return 1;
}

int
_sce_sdrDMA0IntrHandler (int core, void *common)
{
    eeCBData.mode = SDR_CB_DMA0INT;
    iWakeupThread (gStThid);
    return NEXT_ENABLE;
}

int
_sce_sdrDMA1IntrHandler (int core, void *common)
{
    eeCBData.mode = SDR_CB_DMA1INT;
    iWakeupThread (gStThid);
    return NEXT_ENABLE;
}

int
_sce_sdrSpu2IntrHandler (int core_bit, void *common)
{
    eeCBData.mode = SDR_CB_IRQINT;
    eeCBData.voice_bit = core_bit;
    iWakeupThread (gStThid);
    return NEXT_ENABLE;
}

int sce_sdrcb_loop()
{
	int i;
	eeCBData.mode = 0;


	//callback用RPCエントリのバインド
	PRINTF(("sceSifBindCmd(sce_SDRST_CB) start \n"));
	while(1) {
		if (sceSifBindRpc( &cd, sce_SDRST_CB, 0) < 0) {
			printf("error \n");
			while(1);
		}
		i = 10000;
		while( i-- ){
		}
		if(cd.serve != 0) break;
	}
	PRINTF(("sceSifBindRpc completed \n"));
	PRINTF(("goto spu stream cmd loop\n"));
	sceSifCmdLoop2();

	return 0;
}


/* ----------------------------------------------------------------
 *	End on File
 * ---------------------------------------------------------------- */
/* DON'T ADD STUFF AFTER THIS */

