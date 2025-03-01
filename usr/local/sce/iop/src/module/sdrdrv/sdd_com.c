/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *                    I/O Proseccor sample Library
 *                          Version 0.50
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                      rs2drv.irx - rsd_com.c
 *                initialize & command dispatch routine
 *
 *   Version   Date            Design    Log
 *  --------------------------------------------------------------------
 *   0.30      Jun.17.1999     morita    first checked in
 *   0.50      Oug.10.1999     morita    SpuStSetCore etc. added.
 *             May.14.2000     kaol      Thread priority changed.
 */


#include <kernel.h>
#include <sys/types.h>
#include <stdio.h>
#include "sif.h"
#include "sifcmd.h"
#include "sifrpc.h"
#include <libsd.h>
#include "sdr_i.h"
#include <sdrcmd.h>

#define _SCE_IOP_STUB_
//#define PRINTF(x) printf x
#define PRINTF(x)

int gRpcArg[16];	//--- EEから転送されるRPCの引数の受け口
extern int bid;
extern volatile int gStThid;
extern int sce_sdrcb_loop();
extern int sceSdrChangeThreadPriority (int, int);

static void* sdrFunc(unsigned int fno, void *data, int size);

/* ------------------------------------------------------------------------
   sdrモジュールのメインスレッド。
   実行後、割り込み環境の初期化とコマンドの登録を行い、以後はEEからリクエス
   トがあるまでウエイトする。
   ------------------------------------------------------------------------*/
int sce_sdr_loop()
{
	sceSifQueueData qd;
	sceSifServeData sd;

	//-- 割り込み環境の初期化を行っておく。
	CpuEnableIntr();
	EnableIntr( INUM_DMA_4 );
	EnableIntr( INUM_DMA_7 );
	EnableIntr( 9 );

	if( ! sceSifCheckInit() )
		sceSifInit();
	sceSifInitRpc(0);

	
	//--- リクエストによってコールされる関数を登録

	sceSifSetRpcQueue( &qd, GetThreadId() );
	sceSifRegisterRpc( &sd, sce_SDR_DEV, sdrFunc, (void*)gRpcArg, NULL, NULL, &qd );
	PRINTF(("goto sdr cmd loop\n"));

	//--- コマンド待ちループ
	sceSifRpcLoop(&qd);

	return 0;
}


/* ------------------------------------------------------------------------
   EEからのリクエストによって起こされる関数。
   引数は*dataに格納されている。先頭4バイトは予備用で使われていない。
   この関数の返値が、EE側のRPCの返値になる。
   引数が構造体の場合は、gRpcDataに送られているのでそれを使用する。
   構造体をEEに返す場合は、第１引数のアドレス（EE側）に値を送る。
   ------------------------------------------------------------------------*/
int ret = 0;
sceSdEffectAttr e_attr;

static void* sdrFunc(unsigned int command, void *data, int size)
{ 
	struct ThreadParam param;

//	asm volatile( "break 1");

	PRINTF(( " sdrfunc %x, %x, %x, %x\n", *((int*)data + 0), 
		*((int*)data + 1), *((int*)data + 2),*((int*)data + 3) ));

	switch( command&0xFFF0 )
	{
	case rSdInit:
		ret = sceSdInit( *((int*)data + 1) );
		break;
	case rSdSetParam:
		sceSdSetParam( *((int*)data + 1), *((int*)data + 2) );
		 break;
	case rSdSetSwitch:
		sceSdSetSwitch( *((int*)data + 1), *((int*)data + 2) );
		 break;
	case rSdSetAddr:
		sceSdSetAddr( *((int*)data + 1), *((int*)data + 2) );
		 break;
	case rSdSetCoreAttr:
		sceSdSetCoreAttr( *((int*)data + 1), *((int*)data + 2) );
		 break;
	case rSdGetParam:
		ret = sceSdGetParam( *((int*)data + 1) );
		 break;
	case rSdGetSwitch:
		ret = sceSdGetSwitch( *((int*)data + 1) );
		 break;
	case rSdGetAddr:
		ret = sceSdGetAddr( *((int*)data + 1) );
		 break;
	case rSdGetCoreAttr:
		ret = sceSdGetCoreAttr( *((int*)data + 1) );
		 break;

	case rSdNote2Pitch:
		ret = sceSdNote2Pitch( *((int*)data + 1), *((int*)data + 2) , *((int*)data + 3) , *((int*)data + 4) );
		break;
	case rSdPitch2Note:
		ret = sceSdPitch2Note( *((int*)data + 1), *((int*)data + 2) , *((int*)data + 3) );
		break;

	case rSdProcBatch:
		ret = sceSdProcBatch( (sceSdBatch*)*((int*)data + 1), (u_int*)*((int*)data + 2) , *((int*)data + 3) );
		 break;
	case rSdProcBatchEx:
		ret = sceSdProcBatchEx( (sceSdBatch*)*((int*)data + 1), (u_int*)*((int*)data + 2) , *((int*)data + 3), *((int*)data + 4) );
		 break;
	case rSdVoiceTrans:
		ret = sceSdVoiceTrans( *((int*)data + 1), *((int*)data + 2), 
		(u_char*)*((int*)data + 3) , (u_char*)*((int*)data + 4) , 
		*((int*)data + 5) );
		 break;
	case rSdBlockTrans:
		ret = sceSdBlockTrans (*((int*)data + 1), *((int*)data + 2), (u_char*)*((int*)data + 3), *((int*)data + 4), (u_char*)*((int*)data + 5));
		 break;
	case rSdVoiceTransStatus:
		ret = sceSdVoiceTransStatus( *((int*)data + 1), *((int*)data + 2) );
		 break;
	case rSdBlockTransStatus:
//		printf("--- sceSdBlockTransStatus :in\n");
		ret = sceSdBlockTransStatus(*((int*)data + 1), *((int*)data + 2));
//		printf("--- sceSdBlockTransStatus :out %d \n", 0xFFF&ret);
		 break;

	case rSdSetTransCallback:		
		if( *((int*)data + 1) == 0 ){
		ret = (int)sceSdSetTransCallback( 0, (SD_TRANS_CBProc)_sce_sdrDMA0CallBackProc );
		}
		else{
		ret = (int)sceSdSetTransCallback( 1, (SD_TRANS_CBProc)_sce_sdrDMA1CallBackProc );
		}
		 break;
	case rSdSetIRQCallback:
		ret = (int)sceSdSetIRQCallback( (SD_IRQ_CBProc)_sce_sdrIRQCallBackProc );
		break;

	case rSdSetTransIntrHandler:		
		if (*((int*)data + 1) == 0) {
		    ret = (int) sceSdSetTransIntrHandler (0, _sce_sdrDMA0IntrHandler, (void *) NULL);
		}
		else{
		    ret = (int) sceSdSetTransIntrHandler (1, _sce_sdrDMA1IntrHandler, (void *) NULL);
		}
		 break;
	case rSdSetSpu2IntrHandler:
		ret = (int) sceSdSetSpu2IntrHandler (_sce_sdrSpu2IntrHandler, (void *) NULL);
		break;

	case rSdSetEffectAttr:
		ret = sceSdSetEffectAttr ( command&0xF, (sceSdEffectAttr*)((int*)data));
		break;
	case rSdGetEffectAttr:
		sceSdGetEffectAttr( command&0xF, (sceSdEffectAttr*)&ret );
		 break;
	case rSdClearEffectWorkArea:
		ret = sceSdClearEffectWorkArea( *((int*)data + 1), *((int*)data + 2),  *((int*)data + 3) );
		break;

	case rSdChangeThreadPriority:
		ret = sceSdrChangeThreadPriority (*((int*)data + 1), *((int*)data + 2));
		break;
	case 0xe620:
	  	//--- sceSdRemoteCallbackInit()から呼ばれて、callback用のスレッド
		//--- （sce_sdrcb_loop) をcreate/startする。
		//--- sce_sdrcb_loopでは、EE側コールバック関数のbindを行なって
		//--- からコマンド待ちループに入る。
 		param.attr         = TH_C;
  		param.entry        = sce_sdrcb_loop;
		param.initPriority = initial_priority_cb;
  		param.stackSize    = 0x800;
  		param.option       = 0;
  		gStThid = CreateThread(&param);
  		StartThread(gStThid,0);
  		printf("SDR callback thread created\n");
		break;
	default:
		printf("SDR driver error: unknown command %d \n", *((int*)data) );
		break;
	}
	PRINTF(( "return value = %x \n", ret )); 
 return (void*)(&ret);
}

/* ----------------------------------------------------------------
 *	End on File
 * ---------------------------------------------------------------- */
/* DON'T ADD STUFF AFTER THIS */

