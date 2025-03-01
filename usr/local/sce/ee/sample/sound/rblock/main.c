/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                           - rblock -
 *
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                             main.c
 *
 *     Version   Date          Design     Log
 *  --------------------------------------------------------------------
 *     1.50      Mar.30.2000   kaol       For new DMA/SPU2 interrupt handler
 *     0.60      Oct.14.1999   morita     first checked in.
 */

#include <eekernel.h>
#include <sifrpc.h>
#include <libsdr.h>
#include <sdrcmd.h>

#define NEW_INTR_CB	1	   //新しい割り込みハンドラ環境

#define IS_LOOP		0
#define PCM_FILENAME	"host0:knot.int"
#define PCM_SIZE	1604888
#define ALLOC_IOP_HEAP_SIZE   1604888
#define PRINTF(x) printf x
#define TRANS_CH  0

int gCallbackThreadID;
int gMainThreadID;
static int gDataTopAddr;

volatile int trans_flag = 0;
volatile int intr_flag = 0;

void dataset(void)
{
    sceSifInitIopHeap();
    PRINTF(("allocate IOP heap memory - " ));
    while ((gDataTopAddr = sceSifAllocIopHeap(ALLOC_IOP_HEAP_SIZE)) < 0) {
        printf( "\nCan't alloc heap \n");
    }
    PRINTF(("alloced 0x%x  ", gDataTopAddr));

    while (sceSifLoadIopHeap( PCM_FILENAME, (void*)gDataTopAddr) < 0) {
        printf( "\nCan't load PCM file to iop heap \n"); 
    }
    PRINTF(("- data loaded 0x%x \n", (int)gDataTopAddr ));
    
    return;
}


#if NEW_INTR_CB
int
IntTrans (int core, void *common)
{
    (* ((int *) common)) ++;
    PRINTF (("///// AutoDMA interrupt detected (%d). /////\n",
	     (* ((int *) common))));
    return 0;
}
#else
int
IntTrans( void* common )
{
    trans_flag ++;
    PRINTF (("///// AutoDMA interrupt detected (%d). /////\n",
	     trans_flag));
    return 0;
}
#endif

#if NEW_INTR_CB
int
IntSPU2 (int core_bit, void *common)
{
    sceSdRemote (1, rSdSetCoreAttr, SD_CORE_0 | SD_C_IRQ_ENABLE, 0);
    (* ((int *) common)) ++;
    PRINTF (("##### SPU2 interrupt detected (%d). ######\n",
	     (* ((int *) common))));
    return 0;
}
#else
int
IntSPU2( void* common )
{
    sceSdRemote (1, rSdSetCoreAttr, SD_CORE_0 | SD_C_IRQ_ENABLE, 0);
    intr_flag ++;
    PRINTF (("##### SPU2 interrupt detected (%d). #####\n",
	     intr_flag));
    return 0;
}
#endif

int rAutoDma( void )
{
    unsigned short flg1;
    static int thid;
    volatile int i, j, k;
    int do_intr;

    sceSdRemote( 1, rSdInit , SD_INIT_COLD );

    for ( i = 0; i < 2; i++ ) {
	sceSdRemote( 1, rSdSetParam, i|SD_P_MVOLL , 0x3fff );
	sceSdRemote( 1, rSdSetParam, i|SD_P_MVOLR , 0x3fff );
	// エフェクト側には入れない
	sceSdRemote( 1, rSdSetParam, i|SD_P_MMIX , ~( SD_MMIX_MINEL | SD_MMIX_MINER ) );
    }

    // --- 準備・再生
    trans_flag = intr_flag = 0;
    sceSdRemote (1, rSdSetAddr, SD_CORE_0 | SD_A_IRQA, 0x4100);
#if NEW_INTR_CB
    sceSdRemote (1, rSdSetTransIntrHandler, TRANS_CH, IntTrans, &trans_flag); 
    sceSdRemote (1, rSdSetSpu2IntrHandler,            IntSPU2,  &intr_flag);
#else
    sceSdRemote (1, rSdSetTransCallback, TRANS_CH, IntTrans); 
    sceSdRemote (1, rSdSetIRQCallback,             IntSPU2);
#endif

#if IS_LOOP
#define TRANS_MODE (SD_TRANS_MODE_WRITE | SD_BLOCK_MEM_DRY|SD_BLOCK_LOOP)
#define TRANS_SIZE (PCM_SIZE/512)*512
#else
#define TRANS_MODE (SD_TRANS_MODE_WRITE | SD_BLOCK_MEM_DRY|SD_BLOCK_ONESHOT)
#define TRANS_SIZE PCM_SIZE
#endif
    sceSdRemote (1, rSdBlockTrans, TRANS_CH, TRANS_MODE, gDataTopAddr, TRANS_SIZE);
    sceSdRemote (1, rSdSetParam, TRANS_CH|SD_P_BVOLL , 0x3fff);
    sceSdRemote (1, rSdSetParam, TRANS_CH|SD_P_BVOLR , 0x3fff);

    do_intr = 0;
    while (1) {
#if IS_LOOP
	if (sceSdRemote (1, rSdBlockTransStatus, TRANS_CH) == 0) {
	    break;
	}
#endif
	if ((trans_flag % 2) == 1) {
	    if (do_intr == 0) {
		sceSdRemote (1, rSdSetCoreAttr, SD_CORE_0 | SD_C_IRQ_ENABLE, 1);
		do_intr = 1;
	    }
	} else {
	    if (do_intr == 1) do_intr = 0;
	}
	VSync ();
    }
#if NEW_INTR_CB && !IS_LOOP
    while (trans_flag == 0) {
	printf (".");
    }
    printf ("\n");
#endif
    // --- 終了処理
    sceSdRemote(1, rSdBlockTrans, TRANS_CH, SD_TRANS_MODE_STOP, NULL, 0 );
    DeleteThread( gCallbackThreadID );
    PRINTF(("Quit...\n"));

    return 0;
}


int
main(void)
{
    sceSifInitRpc(0);
    while (sceSifLoadModule("host0:../../../../iop/modules/libsd.irx",
                        0, NULL) < 0){
        printf("Can't load module libsd\n");
    }
    while (sceSifLoadModule("host0:../../../../iop/modules/sdrdrv.irx",
                        0, NULL) < 0){
        printf("Can't load module sdrdrv\n");
    }

    //--- Callback Threadを動かすために、mainを最高プライオリティから下げる
    gMainThreadID = GetThreadId();
    ChangeThreadPriority(gMainThreadID , 10 );

    sceSdRemoteInit();

    //--- Callback Threadをmainよりも高いプライオリティで作る
    gCallbackThreadID = sceSdRemoteCallbackInit(5);
    dataset();
    
    rAutoDma();

    return 0;
}
