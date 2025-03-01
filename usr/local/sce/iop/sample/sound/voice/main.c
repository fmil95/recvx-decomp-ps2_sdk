/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              I/O Processor Library Sample Program
 * 
 *                          - voice -
 * 
 *                           Shift-JIS
 * 
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 * 
 *                            main.c
 * 
 *     Version   Date          Design     Log
 *  --------------------------------------------------------------------
 *     1.50      Mar.28.2000   kaol       For new interrupt callbacks
 *     1.20      Nov.29.1999   morita     load on-the-fly
 *     0.60      Oct.14.1999   morita     first checked in.
 */

#include <stdio.h>
#include <kernel.h>
#include <libsd.h>
#include <sys/file.h>

#define BATCH_MODE      0          //バッチ処理を行なう
#define DMA_CB_TEST	1          //DMA割り込みのテスト
#define IRQ_CB_TEST	1          //IRQ割り込みのテスト
#define IRQ_ADDR_OFST	0x1000     //IRQの位置（波形先頭からのoffset
#define NEW_INTR_CB	1	   //新しい割り込みコールバック環境

#define TMP_OUT		(u_int*)0x110000  //バッチの結果出力

#define DMA_CH 0

#define PRINTF(x) printf x
#define BASE_priority  60


#define WAV_DATA_SIZE	20944
#define VAG_FILENAME	"host1:piano.vag"
#define VAG_ADDR	0x15010
#define	REVERB_DEPTH 	0x3fff

#include "init_bat.h"  //libsdのバッチコマンド

char gDoremi[8] = { 36, 38, 40, 41, 43, 45, 47, 48}; 
int gEndFlag = 0;

int soundTest ( void );


void wait_loop( void )
{
	volatile long i,j = 13;

	for ( i = 0; i < 1000 ; i++ ){
		j = j*3;
	}
}

int
#if NEW_INTR_CB
IntTrans (int ch, void* common)
#else
IntTrans ( void* common )
#endif
{
#if NEW_INTR_CB
    int *c = (int *) common;
    (*c) ++;
    printf("##### interrupt detected. count: %d CORE ch: %d #####\n", *c, ch);
#else
    gEndFlag = 1;
    printf("///// interrupt detected. /////\n");
#endif
    return 1;
}

#if NEW_INTR_CB
int IntFunc (int core, void* common)
#else
int IntFunc( void* common )
#endif
{
#if NEW_INTR_CB
    int *c = (int *) common;
    (*c) ++;
    printf("///// interrupt detected (%d). CORE-bit: %d /////\n", *c, core);
#else
	gEndFlag = 1;
	printf("///// interrupt detected. /////\n");
#endif
	return 1;
}

char* dataset(int *size)
{
	int fd;
	char *buffer;

	fd = open(VAG_FILENAME, O_RDONLY);
 	*size = lseek(fd, 0, 2);
	if( *size <= 0 ) { printf( "\nCan't load VAG file to iop heap \n" ); return NULL; }
	lseek( fd, 0, 0);

	PRINTF(("allocate IOP heap memory - " ));
	buffer = AllocSysMemory(0, *size, NULL);
	if( buffer == NULL ) { printf( "\nCan't alloc heap \n"); return NULL; }
	PRINTF(("alloced 0x%x  \n", (int)buffer));

	read( fd, buffer, *size);
	close(fd);
	return buffer;
}



int soundTest(void)
{
	int i, j, size;
	char *wavBuffer;
	sceSdEffectAttr r_attr;

	PRINTF(( "voice start...\n" ));

	if( (wavBuffer = dataset(&size)) == NULL ){ return -1; }

	sceSdInit(0);
	CpuEnableIntr();
	EnableIntr( INUM_DMA_4 );
	EnableIntr( INUM_DMA_7 );
	EnableIntr( INUM_SPU );

#if BATCH_MODE
	i = sceSdProcBatch( gBatchCommonInit, NULL, 4 );  //--- バッチ処理
	PRINTF(("sceSdProcBatch count = %d \n", i ));

	sceSdProcBatch( gBatchCommonInit2, TMP_OUT, 7 );  //--- バッチ処理
	printf(" check wite_iop: %x\n", *((short*)0x90000) );
#else
	for( i = 0; i < 2; i++ )
	{
		sceSdSetParam( i|SD_P_MVOLL , 0x3fff ) ;
		sceSdSetParam( i|SD_P_MVOLR , 0x3fff ) ;
	}
#endif

	// --- data transfer
#if DMA_CB_TEST
#if NEW_INTR_CB
        sceSdSetTransIntrHandler (DMA_CH, (sceSdSpu2IntrHandler) IntTrans, (void *) &gEndFlag);
#else
	sceSdSetTransCallback( DMA_CH, (void*)IntFunc );
#endif
#endif

	PRINTF(("Data transfer ...\n" ));
	sceSdVoiceTrans( DMA_CH, SD_TRANS_MODE_WRITE|SD_TRANS_BY_DMA
		, (u_char*)(wavBuffer+64)
		, (u_char*)VAG_ADDR, (u_int)WAV_DATA_SIZE );
	PRINTF(("TSA = %x \n", sceSdGetAddr( DMA_CH|SD_A_TSA ) ) );


#if DMA_CB_TEST
	while( gEndFlag == 0 ){}
#else
	sceSdVoiceTransStatus(DMA_CH, SD_TRANS_STATUS_WAIT );
#endif


#if IRQ_CB_TEST
	sceSdSetAddr( SD_CORE_0|SD_A_IRQA , VAG_ADDR + IRQ_ADDR_OFST );
#if NEW_INTR_CB
	gEndFlag = 0;
        sceSdSetSpu2IntrHandler ((sceSdSpu2IntrHandler) IntFunc, (void *) &gEndFlag);
#else
	sceSdSetIRQCallback( (void*)IntFunc );
#endif
	sceSdSetCoreAttr( SD_CORE_0|SD_C_IRQ_ENABLE, 1 );
#endif


	for( i = 0; i < 2; i++ )
	{

#if BATCH_MODE
		//--- バッチ処理
		j = sceSdProcBatchEx( gBatchVoiceInit, NULL, 6, 0xFFFFFF );
		j = sceSdProcBatchEx( gBatchVoiceInit2, TMP_OUT, 6, 0xFFFFFF );
		PRINTF(("sceSdProcBatchEx count = %d \n", j ));
#else
		for( j = 0; j < 24; j++ ){
			sceSdSetParam( i|(j<<1)|SD_VP_VOLL , 0x1eff );
			sceSdSetParam( i|(j<<1)|SD_VP_VOLR , 0x1eff );
			sceSdSetParam( i|(j<<1)|SD_VP_PITCH , 0x400 );
			sceSdSetAddr ( i|(j<<1)|SD_VA_SSA , VAG_ADDR );
			sceSdSetParam( i|(j<<1)|SD_VP_ADSR1 , 
			  SD_ADSR1(SD_ADSR_A_EXP, 30, 14, 14) );
			sceSdSetParam( i|(j<<1)|SD_VP_ADSR2 , 
			  SD_ADSR2(SD_ADSR_S_EXP_DEC, 52, SD_ADSR_R_EXP, 13));
		}
#endif
		// --- set reverb attribute
		r_attr.depth_L  = 0;
		r_attr.depth_R  = 0;
//		r_attr.delay    = 30;
//		r_attr.feedback = 200;
		r_attr.mode = SD_REV_MODE_HALL | SD_REV_MODE_CLEAR_WA;
		sceSdSetEffectAttr( SD_CORE_0, &r_attr );
	}

	// --- reverb on
	sceSdSetCoreAttr( SD_CORE_0|SD_C_EFFECT_ENABLE, 1 );
	sceSdSetParam( SD_CORE_0|SD_P_EVOLL , REVERB_DEPTH);
	sceSdSetParam( SD_CORE_0|SD_P_EVOLR , REVERB_DEPTH);
	sceSdSetSwitch( SD_CORE_0|SD_S_VMIXEL , 0xFFFFFF );
	sceSdSetSwitch( SD_CORE_0|SD_S_VMIXER , 0xFFFFFF );


	// --- Ring!
	for( j = 15; j < 23; j++ )
	{
#if IRQ_CB_TEST
		sceSdSetCoreAttr( SD_CORE_0|SD_C_IRQ_ENABLE, 0 );
		sceSdSetCoreAttr( SD_CORE_0|SD_C_IRQ_ENABLE, 1 );
#endif
		sceSdSetParam( SD_CORE_0|(j<<1)|SD_VP_PITCH , 
			sceSdNote2Pitch( 60, 0, gDoremi[j-15], 0 ) );
		sceSdSetSwitch( SD_CORE_0|SD_S_KON , 0x1<<j );

		PRINTF(( " ++ key_on  pitchA %x, pitchB %x \n", 
			sceSdNote2Pitch( 60, 0, gDoremi[j-15], 0 ), 
			sceSdGetParam( SD_CORE_0|(j<<1)|SD_VP_PITCH  )   ));

		for( i = 0; i < 3000; i++ )
		{
			wait_loop(); 
		}
		sceSdSetSwitch( SD_CORE_0|SD_S_KOFF , 0x1<<j );

	}

	sceSdSetSwitch( SD_CORE_0|SD_S_KOFF , 0xFFFFFF );

	PRINTF(( "voice completed...\n" ));
	return 0;
}


int start(int argc, char *argv[])
{
 	struct ThreadParam param;
	int	thid;


	/* --- initialize thread --- */
	param.attr         = TH_C;
	param.entry        = soundTest;
	param.initPriority = BASE_priority;
	param.stackSize    = 0x800;
	param.option       = 0;
	thid = CreateThread(&param);
	if( thid > 0 ) {
		StartThread(thid,0);
		return 0;	/* 0: program is resident */
	} else {
		return 1;	/* 1: program is deleted */
	}
}


