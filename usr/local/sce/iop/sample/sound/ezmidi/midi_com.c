/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *                  I/O Proseccor sample program
 *                          Version 1.20
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                      ezmidi.irx - midi_com.c
 *                    command dispatch routine
 *
 *   Version   Date            Design    Log
 *  --------------------------------------------------------------------
 *   1.20      Nov.23.1999     morita    first checked in.
 */


#include <kernel.h>
#include <sys/types.h>
#include <stdio.h>
#include "sif.h"
#include "sifcmd.h"
#include "sifrpc.h"
#include <libsd.h>
#include "ezmidi_i.h"

int gRpcArg[16];	//--- EEから転送されるRPCの引数の受け口

static void* midiFunc(unsigned int fno, void *data, int size);

extern int MidiStart(  int foo );
extern int MidiInit(  int foo );
extern int MidiSetHd( int port, EZMIDI_BANK* bank );
extern int MidiSetSq( int addr );
extern int MidiStop( int addr );
extern int MidiSeek(  int addr );
extern int MidiTransBd( EZMIDI_BANK* bank );
extern int MidiTransBdPacket(  EZMIDI_BANK* bank );
extern int MidiGetIopFileLength( char* filename  );
extern int MidiGetStatus(void);


/* ------------------------------------------------------------------------
   ezmidiモジュールのメインスレッド。
   実行後、割り込み環境の初期化とコマンドの登録を行い、以後はEEからリクエス
   トがあるまでウエイトする。
   ------------------------------------------------------------------------*/
int sce_midi_loop()
{
	sceSifQueueData qd;
	sceSifServeData sd;

	//-- 割り込み環境の初期化を行っておく。
	CpuEnableIntr();
	EnableIntr( INUM_DMA_4 );
	EnableIntr( INUM_DMA_7 );
	
	//--- リクエストによってコールされる関数を登録

	sceSifInitRpc(0);

	sceSifSetRpcQueue( &qd, GetThreadId() );
	sceSifRegisterRpc( &sd, EZMIDI_DEV, midiFunc, (void*)gRpcArg, NULL, NULL, &qd );
	PRINTF(("goto midi cmd loop\n"));

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

static void* midiFunc(unsigned int command, void *data, int size)
{ 
	int ch;

//	asm volatile( "break 1");

	PRINTF(( " midifunc %x, %x, %x, %x\n", *((int*)data + 0), 
		*((int*)data + 1), *((int*)data + 2),*((int*)data + 3) ));

	ch = command&0x000F;

	switch( command&0xFFF0 )
	{
	case EzMIDI_START:
		MidiStart(*((int*)data) );
		break;
	case EzMIDI_INIT:
		ret = MidiInit(*((int*)data) );
		break;
	case EzMIDI_SETSQ:
		MidiSetSq(*((int*)data) );
		break;
	case EzMIDI_SETHD_P0:
	case EzMIDI_SETHD_P1:
		MidiSetHd( ch, (EZMIDI_BANK*)((int*)data) );
		break;
	case EzMIDI_STOP:
		MidiStop(*((int*)data) );
		break;
	case EzMIDI_SEEK:
		MidiSeek( *((int*)data) );
		break;
	case EzMIDI_TRANSBD:
		ret = MidiTransBd((EZMIDI_BANK*)((int*)data) );
		break;
	case EzMIDI_TRANSBDPACKET:
		ret = MidiTransBdPacket( (EZMIDI_BANK*)((int*)data) );
		break;
	case EzMIDI_GETFILELENGTH:
		ret = MidiGetIopFileLength((char*)((int*)data) );
		break;
	case EzMIDI_GETSTATUS:
		ret = MidiGetStatus();
		break;
	case EzMIDI_SETATTR_P0:
	case EzMIDI_SETATTR_P1:
		MidiSetPortAttr( ch, *((int*)data));
		break;
	case EzMIDI_SETVOL_P0:
	case EzMIDI_SETVOL_P1:
		MidiSetPortVolume( ch, *((int*)data));
		break;

	default:
		ERROR(("EzMIDI driver error: unknown command %d \n", *((int*)data) ));
		break;
	}
	PRINTF(( "return value = %x \n", ret )); 
	return (void*)(&ret);
}


/* ----------------------------------------------------------------
 *	End on File
 * ---------------------------------------------------------------- */
/* DON'T ADD STUFF AFTER THIS */

