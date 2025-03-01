/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *                          Version 1.30
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         EzMIDI - midi_rpc.c
 *                        rpc routine for EzMIDI
 *
 *     Version    Date          Design     Log
 *  --------------------------------------------------------------------
 *     1.30       Dec.12.1999   morita     first checked in.
 */

#include <eekernel.h>
#include <sif.h>
#include <sifrpc.h>
#include "ezmidi_i.h"

#define DATA_SIZE_STRING (64)
#define DATA_SIZE_NORMAL (16)

//-- キャッシュライン不整合を避けるために64byte確保する
static u_int sbuff[16] __attribute__((aligned (64)));
static sceSifClientData gCd;


/* ------------
   rpcの初期化
   ------------*/
int ezMidiInit( void )
{
	int fd, i,cid;
	char *cp;

	sceSifInitRpc(0);

	while(1) {
		if (sceSifBindRpc( &gCd, EZMIDI_DEV, 0) < 0) {
			printf("error: sceSifBindRpc \n");
			while(1);
		}
		i = 10000;
		while( i-- ){}
		if(gCd.serve != 0) break;
	}

	return 1;
}


/* ----------------------------------------------
   IOP側のAPIをrpcで起こし、commandとdataを渡す
   ----------------------------------------------*/
int ezMidi( int command,  int data )
{
	int i, rsize = 0;

	if( (command&0x8000) !=0 )
		rsize = 64;

	if(  (command&0x1000) != 0  ){
		sceSifCallRpc( &gCd,  command, 0, (void *)data
		, DATA_SIZE_STRING, (void *)(&sbuff[0]),rsize,NULL,NULL);
	}
	else{
		sbuff[0] = data;
		sceSifCallRpc( &gCd,  command, 0, (void *)(&sbuff[0])
		, DATA_SIZE_NORMAL, (void *)(&sbuff[0]),rsize,NULL,NULL );
	}

	return sbuff[0];
}

static sceSifDmaData transData;

/* ------------------------------------------------------------------------
   ------------------------------------------------------------------------*/
int ezTransToIOP( void *iopAddr, void *eeAddr, int size )
{
	unsigned int did;

	transData.data = (u_int)eeAddr;
	transData.addr = (u_int)iopAddr;
	transData.size = size;
	transData.mode = 0;

	//-- FlushCasheは遅いので、非キャッシュアクセスの方が良いことも。
	FlushCache(WRITEBACK_DCACHE); 

	did = sceSifSetDma( &transData, 1 );
	while( sceSifDmaStat( did ) >= 0 ){}
	PRINTF(("send data completed \n"));
	if( did == 0 ) return -1;
	return 0;
}


