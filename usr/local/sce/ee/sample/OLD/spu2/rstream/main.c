/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 1.6
 */
/* 
 *              Emotion Engine Library Sample Program
 * 
 *                         - rstream -
 * 
 *                           Shift-JIS
 * 
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 * 
 *                            main.c
 * 
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.30            Jun,22,1999     morita    provisional version
 *      0.50            Aug,15,1999     morita    add rSpuStSetCore
 */

#include <eekernel.h>
#include <librspu2.h>
#include <rspu2cmd.h>
#include <sifrpc.h>

#define ALLOC_IOP_HEAP_SIZE	0x100000
#define UNCACHE			0x20000000
#define SPU_BUFSIZE		0x4000
#define SPU_BUFSIZEHALF		0x2000
#define DATA_SIZE		(int)0x7a000
#define VOICE_LIMIT 		2
#define ST_STOP_INIT_VAL	0
#define SPU_MALLOC_MAX		VOICE_LIMIT
#define SPU_MEMORY_TOP		0x5010
#define SPU_MEMORY_MAX		(2*1024*1024)
#define CH_L0			0
#define CH_R0			1

#define PRINTF(x) printf x

static   int gDataTopAddr;
int      gCallbackThreadID;
int      gMainThreadID;

//--- IOPにDMA転送されるので、64byte境界でアラインされるようにする。
static   SpuStEnv gSt __attribute__((aligned (64)));
static   SpuStEnv* gStPtr;
unsigned int data_start_addr [2];
unsigned int played_size;
volatile int gEnd = 1;


void dataset(void)
{
	sceSifInitIopHeap();
	PRINTF(("allocate IOP heap memory - " ));
	while ((gDataTopAddr = sceSifAllocIopHeap(ALLOC_IOP_HEAP_SIZE)) < 0) {
	    printf( "\nCan't alloc heap \n");
	}
	PRINTF(("alloced 0x%x  ", gDataTopAddr));

	data_start_addr[0] = gDataTopAddr;
	data_start_addr[1] = gDataTopAddr+DATA_SIZE;

	while (sceSifLoadIopHeap("host0:tr1r_pad.vb", (void*)data_start_addr[0]) < 0) {
	    printf( "\nCan't load R-ch file to iop heap \n"); 
	}
	while (sceSifLoadIopHeap("host0:tr1l_pad.vb", (void*)data_start_addr[1] ) < 0) {
	    printf( "\nCan't load L-ch file to iop heap \n"); 
	}
	PRINTF(("- data loaded 0x%x \n", (int)gDataTopAddr ));

	return;
}


SpuStCallbackProc
spustCB_transfer_finished (unsigned int voice_bit)
{
	int i;
	static loop = 0;

	if (played_size <= (DATA_SIZE - SPU_BUFSIZEHALF)) {
		played_size += SPU_BUFSIZEHALF;
		for (i = 0; i < VOICE_LIMIT; i ++) {
		    gStPtr->voice [i].data_addr += SPU_BUFSIZEHALF;
		}
	} else {
		/* return to TOP */
		for (i = 0; i < VOICE_LIMIT; i ++) {
		    gStPtr->voice [i].data_addr = data_start_addr [i];
		}
		played_size = SPU_BUFSIZEHALF;
		loop++;
	}

	if( loop == 2 ){
		for (i = 0; i < VOICE_LIMIT; i ++) {
		    gStPtr->voice [i].status    = SPU_ST_STOP;
		    gStPtr->voice [i].last_size = SPU_BUFSIZEHALF;
		}

	}

	gStPtr->size = SPU_BUFSIZE;
	sceSpu2StreamEnvSet( gStPtr );

	return 0;
}


SpuStCallbackProc
spustCB_preparation_finished (unsigned int voice_bit, int p_status)
{
	if (p_status == SPU_ST_PREPARE) {
		spustCB_transfer_finished (voice_bit);
	}

	sceSpu2StreamEnvSet( gStPtr );
	sceSpu2Remote( 1, rSpuStTransfer,  SPU_ST_START, voice_bit);

	return 0;
}


SpuStCallbackProc
spustCB_stream_finished (unsigned int voice_bit, int s_status)
{
	sceSpu2Remote( 1, rSpuSetKey,  SPU_OFF, voice_bit);
	gEnd = 0;
	return 0;
}


int rstream(void)
{
	int i, c;
	SpuVoiceAttr s_attr;
	SpuCommonAttr c_attr;
	unsigned int buffer_addr [SPU_MALLOC_MAX];

	sceSpu2Remote( 1, rSpuStInit, 0);

	//--- 転送時にキャッシュをフラッシュせずにすむよう、非キャッシュ領域
	//--- でアクセスする。
	gStPtr = (SpuStEnv*)((unsigned int)(&gSt) + (unsigned int)UNCACHE ) ;
	PRINTF(("gStPtr = %x, %x \n", (int)&gSt, (int)gStPtr ));
	sceSpu2StreamEnvInit( gStPtr );

	sceSpu2Remote( 1, rSpuSetTransferMode, SPU_TRANSFER_BY_DMA );
	sceSpu2Remote( 1, rSpuSetTransferStartAddr, SPU_MEMORY_TOP);
	sceSpu2Remote( 1, rSpuWrite0, SPU_MEMORY_MAX - SPU_MEMORY_TOP );

	PRINTF(( "rstream start...\n" ));

	sceSpu2Remote( 1, rSpuInitMalloc, SPU_MALLOC_MAX, NULL);
	
	c_attr.mask = (SPU_COMMON_MVOLL |
		       SPU_COMMON_MVOLR);


	for( i = 0; i < 2; i++ ){
	sceSpu2Remote(1, rSpuSetCore, i);
	sceSpu2Remote(1, rSpuSetKey, SPU_OFF, SPU_ALLCH);

	c_attr.mvol.left  = 0x3fff;	/* Master volume (left) */
	c_attr.mvol.right = 0x3fff;	/* Master volume (right) */
	sceSpu2Remote(1, rSpuSetCommonAttr, &c_attr);
	}

	sceSpu2Remote( 1, rSpuSetTransferMode, SPU_TRANSFER_BY_DMA);


	/* For finishing SPU streaming preparation */
	sceSpu2Remote( 1, rSpuStSetPreparationFinishedCallback, 
			(SpuStCallbackProc)spustCB_preparation_finished);
	/* For next transferring */
	sceSpu2Remote( 1, rSpuStSetTransferFinishedCallback, 
			(SpuStCallbackProc)spustCB_transfer_finished);
	/* For finising SPU streaming with some voices */
	sceSpu2Remote( 1, rSpuStSetStreamFinishedCallback, 
			(SpuStCallbackProc)spustCB_stream_finished);

	for (i = 0; i < VOICE_LIMIT; i ++) {
	    if ( (buffer_addr [i] = 
		sceSpu2Remote(1, rSpuMalloc, SPU_BUFSIZE) ) == -1 )
	    {
		return -1;	/* ERROR */
	    }
	    PRINTF(("SpuMalloc buffer addr = %x\n", buffer_addr [i]));
	}


	for( i = 0; i < 2; i++ ){
		sceSpu2Remote(1, rSpuSetCore, i);
		/* attribute masks */
		s_attr.mask = (SPU_VOICE_PITCH 
				| SPU_VOICE_WDSA
			       );
		
		/* attribute values: L-ch */
		s_attr.pitch        = 0x1800;
		s_attr.addr  = 0x2800*2;
		s_attr.voice = SPU_ALLCH;
		sceSpu2Remote( 1, rSpuSetVoiceAttr, &s_attr);
	}
	sceSpu2Remote(1, rSpuSetCore, 1);
	sceSpu2Remote(1, rSpuStSetCore, 1);


	/* attribute masks */
	s_attr.mask = (SPU_VOICE_VOLL |
		       SPU_VOICE_VOLR |
		       SPU_VOICE_PITCH |
		       SPU_VOICE_WDSA |
		       SPU_VOICE_ADSR_AMODE |
		       SPU_VOICE_ADSR_SMODE |
		       SPU_VOICE_ADSR_RMODE |
		       SPU_VOICE_ADSR_AR |
		       SPU_VOICE_ADSR_DR |
		       SPU_VOICE_ADSR_SR |
		       SPU_VOICE_ADSR_RR |
		       SPU_VOICE_ADSR_SL
		       );
	
	s_attr.volume.left  = 0x3fff;
	s_attr.volume.right = 0x0;
	s_attr.pitch        = 0x1200;
	s_attr.a_mode       = SPU_VOICE_LINEARIncN;
	s_attr.s_mode       = SPU_VOICE_LINEARIncN;
	s_attr.r_mode       = SPU_VOICE_LINEARDecN;
	s_attr.ar           = 0x0;
	s_attr.dr           = 0x0;
	s_attr.sr           = 0x0;
	s_attr.rr           = 0x3;
	s_attr.sl           = 0xf;
	
	
	for (i = CH_L0; i < VOICE_LIMIT; i += 2) {
	    s_attr.voice = SPU_VOICECH (i);
	    s_attr.addr  = buffer_addr [i];
	    sceSpu2Remote(1,  rSpuSetVoiceAttr, &s_attr);
	}
	
	/* attribute values: R-ch */
	s_attr.volume.left  = 0x0;
	s_attr.volume.right = 0x3fff;
	for (i = CH_R0; i < VOICE_LIMIT; i += 2) {
	    s_attr.voice = SPU_VOICECH (i);
	    s_attr.addr  = buffer_addr [i];
	    sceSpu2Remote(1,  rSpuSetVoiceAttr, &s_attr);
	}
	
	
	for (i = 0; i < VOICE_LIMIT; i ++) {
		gStPtr->voice [i].buf_addr = buffer_addr [i];
		gStPtr->voice [i].data_addr = data_start_addr [i];
		gStPtr->voice [i].status    = SPU_ST_PLAY;
	}
	gStPtr->size = SPU_BUFSIZE;
	played_size = SPU_BUFSIZEHALF;
	sceSpu2StreamEnvSet( gStPtr );
	sceSpu2Remote(1,  rSpuStTransfer, SPU_ST_PREPARE
		, SPU_VOICECH (CH_L0) | SPU_VOICECH (CH_R0));


	while( gEnd ){}


	sceSpu2Remote( 1, rSpuStQuit );
	DeleteThread( gCallbackThreadID );
	PRINTF(( "rstream completed...\n" ));

	return 0;
}


int main(void)
{
    sceSifInitRpc(0);
    while (sceSifLoadModule("host0:../../../../iop/modules/rspu2drv.irx",
			0, NULL) < 0){
	printf("Can't load module rspu2drv\n");
    }

	gMainThreadID = GetThreadId();
	ChangeThreadPriority(gMainThreadID, 10);

	sceSpu2RemoteInit();
	gCallbackThreadID = sceSpu2CallbackInit(5);

	dataset();
	rstream();

	return 0;
}
