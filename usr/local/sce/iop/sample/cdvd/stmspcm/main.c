/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
*/
/*
 *             I/O Processor Library Sample Program
 *
 *                         - CD/DVD -
 *
 *      Copyright (C) 2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *
 */

#include <stdio.h>
#include <kernel.h>
#include <sys/file.h>
#include <string.h>
#include <libsd.h>
#include <libcdvd.h>
#include <string.h>

/* ================================================================
 *
 *      Program
 *
 * ================================================================ */

#define BASE_priority   82
#define READ_SECTOR	1
#define BUFF_SIZE	(READ_SECTOR * 2048)
#define TOTAL_BUFF_SIZE	(READ_SECTOR * 2048 * 2)

#define MEDIA_CD

//	ストリーミングバッファ
unsigned char  ring_buf[2048 * 80] __attribute__((aligned (16)));
//	ストレートPCM用バッファ
unsigned char  sbuf[TOTAL_BUFF_SIZE] __attribute__((aligned (16)));

int my_main(int arg);
int gSem = 0;

int start( int argc, char *argv[] )
{
        struct ThreadParam param;
        int     thid;

        /* --- initialize thread --- */
        param.attr         = TH_C;
        param.entry        = my_main;
        param.initPriority = BASE_priority;
        param.stackSize    = 0x800;
        param.option       = 0;
        thid = CreateThread(&param);
        if( thid > 0 ) {
                StartThread(thid,0);
                return 0;       /* 0: program is resident */
        } else {
                return 1;       /* 1: program is deleted */
        }
}

static int IntFunc0( void* common )
{
        iSignalSema( gSem );
        return 1;  //--割り込みを再度許可するのに必要
}

int my_main(int arg)
{
	int	cnt0, ret, start_st, file_sec, read_sector;
 	u_int   err, which, *buf_addr;
        struct SemaParam sem;

	sceCdlFILE fp;
        sceCdRMode mode;
        char filename[80]= "\\M_STEREO.INT;1";

	printf("sample start.\n");

	printf(" sceSdInit\n");
        sceSdInit(0);
	printf(" sceCdInit\n");
	sceCdInit(SCECdINIT);
#ifdef MEDIA_CD
	sceCdMmode(SCECdCD);
#else
	sceCdMmode(SCECdDVD);
#endif
	printf(" sceCdStInit\n");
        sceCdStInit(80,5,(u_int)ring_buf);
	printf(" sceCdDiskReady\n");
	sceCdDiskReady(0);

        //-- 割り込み環境の初期化を行っておく。
        CpuEnableIntr();
        EnableIntr( INUM_DMA_4 );
        EnableIntr( INUM_DMA_7 );
        EnableIntr( INUM_SPU );

        /* セマフォ作成 */
	sem.initCount = 0;
        sem.maxCount = 1;
        sem.attr = AT_THFIFO;
        gSem= CreateSema(&sem);

        sceSdSetTransCallback( 0, IntFunc0 );

        /* ライブラリ関数でファイルを読む */
	printf("Search Filename: %s\n",filename);
	ret= sceCdSearchFile(&fp, filename);
			/* ファイルの格納位置を検索する */
	if(!ret){
		printf("sceCdSearchFile fail :%d\n",ret);
                return(-1);
	}

	sceCdDiskReady(0);
	
        /* ストリーム関数でファイルを読む */
        mode.trycount= 0;
                        /* エラー発生時は２５５回リトライする。 */
        mode.spindlctrl= SCECdSpinNom;
                        /* エラー発生時は回転速度を落してリード             */
        mode.datapattern= SCECdSecS2048;
			/* ストリーマはデータサイズ２０４８byteのみサポート */

	file_sec= fp.size / 2048; if(fp.size % 2048) file_sec++;
	start_st= fp.lsn;
	sceCdStStart(start_st , &mode);

        read_sector= sceCdStRead(READ_SECTOR * 2,(u_int *)sbuf, STMBLK, &err);
        if(err){
                printf("Disk error code= 0x%08x\n", err);
        }

	/* ボリュームを設定する。*/
        for( cnt0= 0; cnt0 < 2; cnt0++ ){
            sceSdSetParam( cnt0 | SD_P_MVOLL , 0x3fff ) ;
            sceSdSetParam( cnt0 | SD_P_MVOLR , 0x3fff ) ;
        }
        sceSdSetParam( 0 | SD_P_BVOLL , 0x3fff );
        sceSdSetParam( 0 | SD_P_BVOLR , 0x3fff );

	/* データの転送の設定を行う */
        sceSdBlockTrans( 0, SD_TRANS_MODE_WRITE|SD_BLOCK_MEM_DRY|SD_BLOCK_LOOP,
						 sbuf, TOTAL_BUFF_SIZE );

	for(cnt0= READ_SECTOR * 2; cnt0 < file_sec; cnt0+= READ_SECTOR){

                WaitSema(gSem); //-- バッファの発音終了待ち
                which = 1 - (sceSdBlockTransStatus( 0, 0 ) >> 24);
		buf_addr= (u_int *)(sbuf + (which * BUFF_SIZE));
        	read_sector= sceCdStRead(READ_SECTOR, buf_addr, STMBLK, &err);
        	if(err){
                     printf("Disk error code= 0x%08x\n", err);
        	}
        }
        sceSdSetParam( 0 | SD_P_BVOLL , 0 );
        sceSdSetParam( 0 | SD_P_BVOLR , 0 );
	sceSdBlockTrans( 0, SD_TRANS_MODE_STOP, NULL, 0 );
	sceCdStStop();

        printf("sample end.\n");
        return(0);
}
/* end of file.*/
