/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 1.6
 */
/* 
 *              Emotion Engine Library Sample Program
 * 
 *                         - rvoice -
 * 
 *                           Shift-JIS
 * 
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 * 
 *                            main.c
 * 
 *   Version   Date            Design    Log
 *  --------------------------------------------------------------------
 *   0.30      Jun,22,1999     morita    provisional version
 *   0.50      Aug.19.1999     morita    add sif callback test etc.
 */

#include <eekernel.h>
#include <librspu2.h>
#include <rspu2cmd.h>
#include <sifrpc.h>

#define DMA_CB_TEST 0
#define IRQ_CB_TEST 1
#define SIF_CB_TEST 0
#define IRQ_ADDR_OFST  0x1000  //offset from top of vag

//--- wave file
#define VAG_FILENAME  "host0:piano.vag"
#define VAG_SIZE 20944
#define VAG_ADDR 0x5010
#define ALLOC_IOP_HEAP_SIZE   0x100000

#define	REVERB_DEPTH 	0x3fff
#define	REVERB_MODE 	SPU_REV_MODE_HALL
#define S_PITCH 0x1000

#define PRINTF(x) printf x

int	gMainThreadID;
int	gCBThreadID;
char	gDoremi[8] = { 36, 38, 40, 41, 43, 45, 47, 48}; 

//--- この内容をIOPにDMA転送するので、64byte境界にアラインされている必要がある。
int	gR_buffer[16] __attribute__((aligned (64)));


#if SIF_CB_TEST
void CBrpcComplete(void *data)
{
//--- これは子スレッドではなく割り込みハンドラであることに注意。
	scePrintf("///// SIF complete interrupt. /////\n");
	iWakeupThread( gMainThreadID );
	return;
}
#endif

#if DMA_CB_TEST
int CBspuDMA( void* common )
{
	PRINTF(("///// DMA interrupt detected. /////\n"));
	WakeupThread( gMainThreadID );
	return 1;
}
#endif

#if IRQ_CB_TEST
int CBspuIRQ( void* common )
{
	struct ThreadParam tp;
	printf("///// IRQ interrupt detected. /////\n");	
	FlushCache(0);
	return 1;
}
#endif


int dataset(void)
{
	void* vagAddr;
	sceSifInitIopHeap();
	PRINTF(("allocate IOP heap memory - " ));
	while ((vagAddr = (void*)sceSifAllocIopHeap(ALLOC_IOP_HEAP_SIZE)) < 0) {
	    printf( "\nCan't alloc heap \n");
	}
	PRINTF(("alloced 0x%x  ", vagAddr));
	while (sceSifLoadIopHeap(VAG_FILENAME, vagAddr) < 0) {
	    printf( "\nCan't load file to iop heap \n");
	}
	PRINTF(("- data loaded 0x%x \n", (int)vagAddr ));

	return (int)vagAddr;
}


int rvoice(void)
{
	int i, j;
	SpuVoiceAttr s_attr;
	SpuReverbAttr r_attr;
	SpuCommonAttr c_attr;
	int vagAddr;
	int keyStatus;

	PRINTF(( "rvoice start...\n" ));

	vagAddr = dataset();

	//--- set common attribute
	for( i = 0; i < 2; i++ )
	{
		sceSpu2Remote( 1, rSpuSetCore, i );
		// --- set commonn attribute
		c_attr.mask = (SPU_COMMON_MVOLL |  SPU_COMMON_MVOLR);
		c_attr.mvol.left  = 0x3fff;	/* Master volume (left) */
		c_attr.mvol.right = 0x3fff;	/* Master volume (right) */
		sceSpu2Remote( 1, rSpuSetCommonAttr, (int)&c_attr );


		sceSpu2Remote( 1, rSpuSetReverbEndAddr, 0x1fffff - 0x20000*i );
		sceSpu2Remote( 1, rSpuSetReverbModeDepth, 0, 0 );
	}

#if SIF_CB_TEST
	sceSpu2RemoteCallBack( (sceSifEndFunc)CBrpcComplete );
	sceSpu2Remote( 0, rSpuGetCommonAttr, &gR_buffer );
	SleepThread();
	PRINTF(("- commonAttr.mvol.left = %x \n", ((SpuCommonAttr*)&gR_buffer)->mvol.left ));
#endif

	//--- data transfer
	sceSpu2Remote( 1, rSpuSetTransferMode, SPU_TRANSFER_BY_DMA );
#if DMA_CB_TEST
	sceSpu2Remote( 1, rSpuSetTransferCallback, CBspuDMA );
#endif
	sceSpu2Remote( 1, rSpuSetTransferStartAddr, VAG_ADDR );
	sceSpu2Remote( 1, rSpuWrite, vagAddr+64, VAG_SIZE);
#if DMA_CB_TEST
	SleepThread();
#else
	sceSpu2Remote( 1, rSpuIsTransferCompleted, SPU_TRANSFER_WAIT );
#endif

#if IRQ_CB_TEST
	//--- spu IRQ test
	sceSpu2Remote( 1, rSpuSetCore, 0 );
	sceSpu2Remote( 1, rSpuSetIRQAddr, VAG_ADDR + IRQ_ADDR_OFST );
	sceSpu2Remote( 1, rSpuSetIRQCallback, CBspuIRQ );
	sceSpu2Remote( 1, rSpuSetIRQ, SPU_ON );
#endif

	//--- set voice attribute
	for( i = 0; i < 2; i++ )
	{
		sceSpu2Remote( 1, rSpuSetCore, i );
		// --- set voice attribute
		s_attr.mask = (SPU_VOICE_VOLL |
		   SPU_VOICE_VOLR |
		   SPU_VOICE_SAMPLE_NOTE |
		   SPU_VOICE_NOTE |
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
		s_attr.voice = SPU_ALLCH;
		s_attr.volume.left  = 0x1fff;		/* Left volume */
		s_attr.volume.right = 0x1fff;		/* Right volume */
		s_attr.note         = (38<<8);		/* Pitch */
		s_attr.sample_note  = (60<<8);		/* Sample Note */
		s_attr.pitch        = S_PITCH;		/* Pitch */
		s_attr.addr         = (unsigned int)VAG_ADDR; /* Waveform data  */
		s_attr.a_mode       = SPU_VOICE_EXPIncN; /* Attack curve */
		s_attr.s_mode       = SPU_VOICE_EXPDecN; /* Sustain curve */
		s_attr.r_mode       = SPU_VOICE_EXPDecN; /* Release curve */
		s_attr.ar           = 30;	/* Attack rate value */
		s_attr.dr           = 14;	/* Decay rate value */
		s_attr.sl           = 14;	/* Sustain level value */
		s_attr.sr           = 52;	/* Sustain rate value */
		s_attr.rr           = 13;	/* Release rate value */
		sceSpu2Remote( 1, rSpuSetVoiceAttr, (int)&s_attr  );

		// --- set reverb attribute
		r_attr.mask = ( SPU_REV_DEPTHL | SPU_REV_DEPTHR | SPU_REV_MODE );
		r_attr.depth.left = 0;
		r_attr.depth.right = 0;
		r_attr.mode = REVERB_MODE | SPU_REV_MODE_CLEAR_WA;
		sceSpu2Remote( 1, rSpuSetReverbModeParam, (int)&r_attr  );
	}

	sceSpu2Remote( 1, rSpuSetCore, 0 );
	// --- reverb on
	sceSpu2Remote( 1, rSpuSetReverb, SPU_ON );
	sceSpu2Remote( 1, rSpuSetReverbModeDepth, REVERB_DEPTH, REVERB_DEPTH );
	sceSpu2Remote( 1, rSpuSetReverbVoice, SPU_ON, SPU_ALLCH );


	// --- Ring!
	for( j = 0; j < 8; j++ )
	{
#if IRQ_CB_TEST
		sceSpu2Remote( 1, rSpuSetIRQ, SPU_RESET );
#endif
		s_attr.note         = (gDoremi[j]<<8);
		s_attr.voice = SPU_VOICECH(j);
 		s_attr.mask = (SPU_VOICE_NOTE );
		sceSpu2Remote( 1, rSpuSetVoiceAttr, (int)&s_attr );
		sceSpu2Remote( 1, rSpuSetKey, SpuOn, SPU_VOICECH(j) );
		PRINTF(( " ++ key_on  note : %d  ", s_attr.note>>8 ));

		//--- get voice status test
		((SpuVoiceAttr*)&gR_buffer)->voice = SPU_VOICECH( j );
		sceSpu2Remote( 1, rSpuGetVoiceAttr, &gR_buffer );
		PRINTF(("envx : %x  ", ((SpuVoiceAttr*)&gR_buffer)->envx ));

		sceSpu2Remote( 1, rSpuGetAllKeysStatus, &gR_buffer );
		keyStatus = sceSpu2Remote( 1, rSpuGetKeyStatus, SPU_VOICECH( j ) );
		PRINTF(("stat A : %x  B : %x \n", 
			((char*)(&gR_buffer))[j], keyStatus ));

		for( i = 0; i < 90; i++ )
		{
			VSync();
		}
	}

	sceSpu2Remote( 1, rSpuSetKey, SpuOff, SPU_ALLCH );
	sceSpu2Remote( 1, rSpuSetReverbModeDepth, 0, 0 );
	PRINTF(( "rvoice completed...\n" ));

	return 0;
}


int main(void)
{
	struct ThreadParam tp;

	sceSifInitRpc(0);
	while (sceSifLoadModule("host0:../../../../iop/modules/rspu2drv.irx",
			    0, NULL) < 0){
	    printf("Can't load module rspu2drv\n");
	}
	
	gMainThreadID = GetThreadId();
	ChangeThreadPriority( gMainThreadID, 10 );
	
	sceSpu2RemoteInit();
#if( IRQ_CB_TEST || DMA_CB_TEST )
	sceSpu2CallbackInit(5);
#endif

	rvoice();

	return;
}
