/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 1.6
 */
/* 
 *              Emotion Engine Library Sample Program
 * 
 *                         - rseqplay -
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
 *   0.50      Aug.19.1999     morita    error correction in seq_table
 */

#include <eekernel.h>
#include <librspu2.h>
#include <rspu2cmd.h>
#include <sifrpc.h>

#define  SEQ_FILENAME	"host0:fuga.seq"
#define  VH_FILENAME	"host0:simple.vh"
#define  VB_FILENAME	"host0:simple.vb"

#define  VH_ADDR_OFST	0x10000
#define  VB_ADDR_OFST	0x20000
#define  ALLOC_IOP_HEAP_SIZE   0x100000
#define  MVOL		127		/* main volume */
#define  SVOL		64		/* seq data volume */
#define  SEQ_MAX	4
#define  SEP_MAX	1

/* tickmode: currentry only NOTICK mode available... */
#define NOTICK		1

#define PRINTF(x) printf x

static int gDataTopAddr;
int gMainThreadID;


void dataset(void)
{
	sceSifInitIopHeap();
	PRINTF(("allocate IOP heap memory - " ));

	while ((gDataTopAddr = sceSifAllocIopHeap (ALLOC_IOP_HEAP_SIZE)) < 0) {
	    printf( "\nCan't alloc heap \n");
	}
	PRINTF(("alloced 0x%x  ", gDataTopAddr));

	while (sceSifLoadIopHeap( SEQ_FILENAME, (void*)gDataTopAddr) < 0) {
	    printf( "\nCan't load seq file to iop heap\n" ); 
	}
	while (sceSifLoadIopHeap( VH_FILENAME, (void*)(VH_ADDR_OFST+gDataTopAddr) ) < 0) {
	     printf( "\nCan't load vh file to iop heap\n" ); 
	}
	while (sceSifLoadIopHeap( VB_FILENAME, (void*)(VB_ADDR_OFST+gDataTopAddr) ) < 0) {
	     printf( "\nCan't load vb file to iop heap\n" ); 
	}
	PRINTF(("- data loaded 0x%x \n", (int)gDataTopAddr ));

	return;
}


int rseqplay(void) 
{
	int	i;
	int time = 0;
	short env;
	int vab, seq;
	SpuCommonAttr c_attr;
	int seq_table;

	PRINTF(( "rseqplay start...\n" ));

	sceSpu2Remote( 1, rSsInit );
	sceSpu2Remote( 1, rSpuSetCore, 1 );

	//--- seq_table のための領域を、IOPのヒープから確保する。
	seq_table = sceSifAllocIopHeap( SS_SEQ_TABSIZ * SEQ_MAX * SEP_MAX );
	if( seq_table < 0 ) { printf( "\nCan't alloc heap \n"); return -1; }
	sceSpu2Remote( 1, rSsSetTableSize, seq_table, SEQ_MAX, SEP_MAX);

#ifdef NOTICK
	/* set tick mode = NOTICK */
	/* currentry only NOTICK mode available... */
	sceSpu2Remote( 1, rSsSetTickMode, SS_NOTICK);
#else  /* NOTICK */
	/* set tick mode = TICK240 */
	sceSpu2Remote( 1, rSsSetTickMode, SS_TICK240);	
#endif /* NOTICK */

	vab = sceSpu2Remote( 1, rSsVabOpenHead,  (u_char *)(gDataTopAddr+VH_ADDR_OFST), -1);
	if (vab == -1) {
		PRINTF(("SsVabOpenHead : failed !!!\n"));
		return 1;
	}

	PRINTF(("SsVabTransBody\n"));
	if (sceSpu2Remote( 1, rSsVabTransBody, gDataTopAddr+VB_ADDR_OFST, vab) != vab) {
		PRINTF(("SsVabTransBody : failed !!!\n"));
		return 1;
	}

	sceSpu2Remote( 1, rSsVabTransCompleted, SS_WAIT_COMPLETED);

	sceSpu2Remote( 1, rSpuSetCore, 1);
	sceSpu2Remote( 1, rSsSetMVol, MVOL, MVOL);	/* set main volume */

	seq = sceSpu2Remote( 1, rSsSeqOpen, gDataTopAddr, vab);
	sceSpu2Remote( 1, rSsStart );			/* start sound */
	sceSpu2Remote( 1, rSsSeqSetVol, seq, SVOL, SVOL); 
	sceSpu2Remote( 1, rSsSeqPlay, seq, SSPLAY_PLAY, 1);

	while ( sceSpu2Remote(1, rSsIsEos, seq, 0) ) {
#ifdef NOTICK
		sceSpu2Remote( 0, rSsSeqCalledTbyT );
		VSync();
#endif /* NOTICK */
	}

 	sceSpu2Remote( 1, rSsSeqClose, seq);		/* close seq data */
 	sceSpu2Remote( 1, rSsVabClose, vab);		/* close vab data */
	sceSpu2Remote( 1, rSsEnd );			/* sound system end */
	sceSpu2Remote( 1, rSsQuit );

	return 0;
}


int main(void)
{
    sceSifInitRpc(0);
    while (sceSifLoadModule("host0:../../../..//iop/modules/rspu2drv.irx",
			0, NULL) < 0){
	printf("Can't load module rspu2drv\n");
    }

	sceSpu2RemoteInit();

	dataset();
	
	rseqplay();

	return 0;
}
