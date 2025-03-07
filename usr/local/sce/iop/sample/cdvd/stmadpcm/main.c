/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
*/
/*
 *              I/O Processor Library Sample Program
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

#define MEDIA_CD

#define _SB_TOP   0x10000
#define _SEC_SIZE 2
#define _BUF_SIZE (2048 * _SEC_SIZE)
#define _SEC_HALF 1
#define _BUF_HALF (2048 * _SEC_HALF)
#define _VOICE_NUM 22
#define _VOICE (1 << _VOICE_NUM)
#define _VOLUME 0x3fff

// for GETSTATUS
#define ADPCM_STATUS_PRELOADED        0x00003000
#define ADPCM_STATUS_RUNNING          0x00005000
#define ADPCM_STATUS_BUFCHG           0x00006000

volatile int gAdpcmStatus= ADPCM_STATUS_PRELOADED;
int buf_side= 0;
int gSem = 0;

// SPU2 ローカルメモリ内の配置
unsigned char  ring_buf[2048 * 80] __attribute__((aligned (16)));
unsigned char  sbuf[_BUF_SIZE] __attribute__((aligned (16)));

#define _ADPCM_MARK_START 0x04
#define _ADPCM_MARK_LOOP  0x02
#define _ADPCM_MARK_END   0x01

#define _AdpcmSetMarkSTART(a,s) { \
  *((unsigned char *)((a)+1)) = \
        (_ADPCM_MARK_LOOP | _ADPCM_MARK_START); \
  *((unsigned char *)((a)+0x10+1)) = \
        _ADPCM_MARK_LOOP; \
  *((unsigned char *)((a)+(s)-0x0f)) = \
        _ADPCM_MARK_LOOP; \
        FlushDcache (); }
#define _AdpcmSetMarkEND(a,s) { \
  *((unsigned char *)((a)+1)) = \
         _ADPCM_MARK_LOOP; \
  *((unsigned char *)((a)+0x10+1)) = \
        _ADPCM_MARK_LOOP; \
  *((unsigned char *)((a)+(s)-0x0f)) = \
        (_ADPCM_MARK_LOOP | _ADPCM_MARK_END); \
        FlushDcache (); }

#define _AdpcmSetMarkSTARTpre(a,s) { \
  *((unsigned char *)((a)+1)) = \
        (_ADPCM_MARK_LOOP | _ADPCM_MARK_START); \
  *((unsigned char *)((a)+0x10+1)) = \
        _ADPCM_MARK_LOOP; \
        FlushDcache (); }
#define _AdpcmSetMarkENDpre(a,s) { \
  *((unsigned char *)((a)+(s)-0x0f)) = \
        (_ADPCM_MARK_LOOP | _ADPCM_MARK_END); \
        FlushDcache (); }

int my_main(int arg);

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

/* internal */
static int
_AdpcmDmaInt (void* common)     // DMA Interrupt
{
    iSignalSema (gSem);
    return 1;  // 割り込みを再度許可
}

/* internal */
static SD_IRQ_CBProc
_AdpcmSpu2Int (void)            // SPU2 Interrupt
{
    iSignalSema (gSem);
    return (SD_IRQ_CBProc) 1;   // 割り込みを再度許可
}

// ADPCM_SETVOICE
void
AdpcmSetVoice (int ch, unsigned int vnum)
{
    sceSdSetParam (SD_CORE_0 | (vnum << 1) | SD_VP_VOLL, 0);
    sceSdSetParam (SD_CORE_0 | (vnum << 1) | SD_VP_VOLR, 0);
    sceSdSetParam (SD_CORE_0 | (vnum << 1) | SD_VP_PITCH, 0x1000);
//  pitchの設定例 サンプリングレート 48K    PITCH 0x1000
//                                   44.1K  PITCH 0x0eb3
//                                   32K    PITCH 0x0aab
//                                   22.05K PITCH 0x075a
//                                   16K    PITCH 0x0555
//                                   11.25K PITCH 0x03ad
    sceSdSetParam (SD_CORE_0 | (vnum << 1) | SD_VP_ADSR1, 0x000f);
    sceSdSetParam (SD_CORE_0 | (vnum << 1) | SD_VP_ADSR2, 0x1fc0);
    sceSdSetAddr  (SD_CORE_0 | (vnum << 1) | SD_VA_SSA, _SB_TOP);
    return;
}

// ADPCM_SETVOLDIRECT
void
AdpcmSetVolumeDirect (unsigned int vol,int vnum)
{
    sceSdSetParam (SD_CORE_0 | (vnum << 1) | SD_VP_VOLL, vol);
    sceSdSetParam (SD_CORE_0 | (vnum << 1) | SD_VP_VOLR, vol);
    return;
}

int my_main(int arg)
{
	int	cnt0, ret, start_st,file_sec;
	u_int read_sec;
 	u_int   err;
        struct SemaParam sem;
        sceCdlFILE fp;
        sceCdRMode mode;
        char filename[16]= "\\PS_MONO.VB;1";

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

	// 割り込みコールバック関数
    	sceSdSetTransCallback (0, _AdpcmDmaInt); // 転送終了割り込み
    	sceSdSetIRQCallback(_AdpcmSpu2Int); // SPU 割り込み

        /* ライブラリ関数でファイルを読む */
	printf("Search Filename: %s\n",filename);
	ret= sceCdSearchFile(&fp, filename); /* ファイルの格納位置を検索する */
	if(!ret){
		printf("sceCdSearchFile fail :%d\n",ret);
                return(-1);
	}
	sceCdDiskReady(0);
	
        /* ストリーム関数でファイルを読む */
        mode.trycount= 0;
			 /* エラー発生時は２５５回リトライする。 */
        mode.spindlctrl= SCECdSpinNom;
			 /*エラー発生時は回転速度を落してリード */
        mode.datapattern= SCECdSecS2048;
			 /* ストリーマはデータサイズ２０４８byteのみサポート */
	file_sec= fp.size / 2048; if(fp.size % 2048) file_sec++;
	start_st= fp.lsn;
	sceCdStStart(start_st , &mode);

	//メインボリューム設定
        for( cnt0= 0; cnt0 < 2; cnt0++ ){
            sceSdSetParam( cnt0 | SD_P_MVOLL , 0x3fff ) ;
            sceSdSetParam( cnt0 | SD_P_MVOLR , 0x3fff ) ;
        }

	AdpcmSetVoice (0, _VOICE_NUM);

	read_sec= 0;
        // 最初の転送: バッファ全域にデータを送る
        read_sec+= sceCdStRead(_SEC_SIZE,(u_int *)sbuf, STMBLK, &err);
        if(err){
            printf("Disk ERROR Found Code= %d\n",err);
	    sceCdStStop();
            return(-1);
        }
        _AdpcmSetMarkSTARTpre(sbuf, _BUF_SIZE);
        _AdpcmSetMarkENDpre(sbuf, _BUF_SIZE);
        sceSdVoiceTrans(0, (SD_TRANS_MODE_WRITE | SD_TRANS_BY_DMA),
                          sbuf, (u_char *)_SB_TOP, _BUF_SIZE);
	while(read_sec < file_sec){

            //-- バッファの発音終了待ち
            WaitSema(gSem);
	    switch (gAdpcmStatus) {
	        case ADPCM_STATUS_PRELOADED:
		    // SPU2 割り込み待ち 全域転送終了、演奏開始
            	    sceSdSetAddr (SD_CORE_0 | SD_A_IRQA,
						 (u_int)_SB_TOP + _BUF_HALF);
        	    AdpcmSetVolumeDirect ( _VOLUME, _VOICE_NUM );
        	    sceSdSetCoreAttr (SD_CORE_0 | SD_C_IRQ_ENABLE, 1);
        						// SPU 割り込み有効
        	    sceSdSetSwitch (SD_CORE_0 | SD_S_KON, _VOICE); // キーオン
                    gAdpcmStatus= ADPCM_STATUS_RUNNING;
            	    buf_side= 0;    // preload 後は前半に転送
            	    break;
	        case ADPCM_STATUS_RUNNING:
		    //   バッファの半分の領域の演奏終了を待ち
		    // 非演奏領域にデータを転送
        	    read_sec+= sceCdStRead(_SEC_HALF,(u_int *)sbuf,
							    STMBLK, &err);
		    if(err){
		        printf("Disk ERROR Found Code= %d\n",err);
		        read_sec= file_sec;
		        continue;
		    }
               	    sceSdSetCoreAttr (SD_CORE_0 | SD_C_IRQ_ENABLE, 0);
               	    if(!buf_side){	 // マークの修正
		        _AdpcmSetMarkSTART(sbuf, _BUF_HALF);
		    }else{	 
		        _AdpcmSetMarkEND(sbuf, _BUF_HALF);
		    }
                    // _BUF_HALF 分転送
               	    sceSdVoiceTrans (0, (SD_TRANS_MODE_WRITE | SD_TRANS_BY_DMA),
			       sbuf, (u_char *)(_SB_TOP + _BUF_HALF * buf_side),
			       _BUF_HALF);
                    gAdpcmStatus= ADPCM_STATUS_BUFCHG; // 状態遷移
               	    break;
	        case ADPCM_STATUS_BUFCHG:
		    // 転送終了後バッファ切替え
                    // SPU2 割り込みアドレス変更
                    sceSdSetAddr (SD_CORE_0 | SD_A_IRQA,
					 _SB_TOP + _BUF_HALF * buf_side);
                    buf_side ^= 1; // バッファ状態の切替え
                    // SPU2 割り込み有効
                    sceSdSetCoreAttr (SD_CORE_0 | SD_C_IRQ_ENABLE, 1);
                    gAdpcmStatus= ADPCM_STATUS_RUNNING; // 状態遷移
           	    break;
        	default:
           	    break;
	    }
        }
	sceSdBlockTrans( 0, SD_TRANS_MODE_STOP, NULL, 0 ); // SPU2 割り込み停止
        sceSdSetCoreAttr (SD_CORE_0 | SD_C_IRQ_ENABLE, 0); // ボイス停止
	AdpcmSetVolumeDirect (0,_VOICE_NUM);
        sceSdSetSwitch (SD_CORE_0 | SD_S_KOFF, _VOICE);
	sceCdStStop();

        printf("sample end.\n");
        return(0);
}
/* end of file.*/
