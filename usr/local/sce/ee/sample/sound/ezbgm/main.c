/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *                          Version 1.20
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                        EzBGM - main.c
 *                        main routine
 *
 *     Version    Date          Design     Logf
 *  --------------------------------------------------------------------
 *     1.20       Nov.23.1999   morita     first checked in.
 */

#include <eekernel.h>
#include <libsdr.h>
#include <sdrcmd.h>
#include "bgm_i.h"
#include <libdev.h>
#include <libpad.h>

/*
   シークが間に合わないときはPACKET_SIZEを増やせば改善しますが、レーテンシは
   増加します。
   ネットワーク越しにホストのディスクをアクセスする場合、ネットワークの遅延
   によって正常に演奏できないことがあります。

   2ファイルの同時演奏のために、かなり大きくバッファを取っていますが、1ファイ
   ルのみであれば、12kByte程度でも演奏できます。

   バッファのサイズを2ファイル間で変えているのは、ファイルリードのタイミング
   がなるべくぶつからないようにするためです。
*/

#define PACKET_SIZE1      (512*150)    //-- 512の倍数であること
#define PACKET_SIZE2      (512*170)   //-- 512の倍数であること


#define CNTL_RUN  0
#define CNTL_STOP 1

char gFilename[2][64]; //-- 注意！このままIOPにDMA転送されます

int ezBgmInit( void );
int ezBgm( int command,  int status );


u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));
u_long128 pad_dma_buf2[scePadDmaBufferMax]  __attribute__((aligned (64)));

/*------------------------------------------
  使用するモジュールをIOP側に読み込んで起動
  ------------------------------------------*/
void load_modules( void )
{
	sceSifInitRpc(0);

	//-- Pad用
	while(sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",0, NULL) < 0){
		printf("loading sio2man.irx failed\n");
	}
	while(sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",0, NULL) < 0){
		printf("loading padman.irx failed\n");
	}

	//-- libsdはサウンド処理には必須
	while(sceSifLoadModule("host0:../../../../iop/modules/libsd.irx",0, NULL) < 0){
		printf("loading libsd.irx failed\n");
	}
	
	//-- EzBGM用ドライバモジュール
	while(sceSifLoadModule("host0:../../../../iop/sample/sound/ezbgm/ezbgm.irx",0, NULL) < 0){
		printf("loading ezbgm.irx failed\n");
	}

	return;
}


/*----------------------------------------
  EzBGMのセットアップから演奏スタートまで
  ----------------------------------------*/
int init_bgm( int wav_count )
{
	int i, info;

	ezBgmInit();

	//-- 以下３行の設定はlibsdrを使ってもよい
	ezBgm( EzBGM_SDINIT, SD_INIT_COLD );   //-- sceSdInit()を行なう
	ezBgm( 0|EzBGM_SETMASTERVOL, 0x3fff3fff ); //-- master vol core0
	ezBgm( 1|EzBGM_SETMASTERVOL, 0x3fff3fff ); //-- master vol core1

	for( i = 0; i < wav_count ; i++ ){
		info = ezBgm( i|EzBGM_OPEN, (int)gFilename[i] );
		printf("  wav:%1d %s  %d byte(body)  ", i,gFilename[i],info&(~WAV_STEREO_BIT) );

		if( (info&WAV_STEREO_BIT) == 0 ){  //-- モノラル
			printf("mono \n");
			ezBgm( i|EzBGM_INIT, PACKET_SIZE2*3 );
			ezBgm( i|EzBGM_SETMODE, BGM_MODE_REPEAT_FORCED|BGM_MODE_MONO );
		}
		else{ //-- ステレオ
			printf("stereo \n");
			ezBgm( i|EzBGM_INIT, PACKET_SIZE1*4 );
			ezBgm( i|EzBGM_SETMODE, BGM_MODE_REPEAT_FORCED|BGM_MODE_STEREO );
		}
		ezBgm( i|EzBGM_PRELOAD, 0 );
		ezBgm( i|EzBGM_SETVOL, 0x3fff3fff );
		ezBgm( i|EzBGM_START, 0 );
	}
	//--- 以上でBGM演奏がスタート ---------------------
	printf("EzBGM start!\n");

	return 0;
}


/*----------------------
  メインループ
  ----------------------*/
int main_loop( int wav_count )
{
	int run_status[2], cntl_status[2], i, tPause = 0;
	u_int paddata, oldpad, ch;
	u_char rdata[32];

	scePadInit(0);
	scePadPortOpen(0, 0, pad_dma_buf);

	while( 1 )
	{
	    oldpad = paddata;
	    FlushCache(0);
	    if(scePadRead(0, 0, rdata) > 0){
		paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
		if( wav_count == 1 ) paddata &= ~(SCE_PADRleft|SCE_PADRdown);
	    }
	    else{
		paddata = 0;
	    }

	    if( paddata != oldpad )
	    {
		run_status[0] = ezBgm( 0|EzBGM_GETMODE, 0)&~BGM_MASK_STATUS;
		run_status[1] = ezBgm( 1|EzBGM_GETMODE, 0)&~BGM_MASK_STATUS;
		if ( (paddata & SCE_PADRup) || (paddata & SCE_PADRright)  ) ch = 0;
		if ( (paddata & SCE_PADRleft) || (paddata & SCE_PADRdown) ) ch = 1;

		//-- STOP
		if ( (paddata & SCE_PADRup) || (paddata & SCE_PADRleft) ){  
			if( (run_status[ch] & BGM_MODE_RUNNING) != 0 ){
				ezBgm( ch|EzBGM_STOP, 0 );
				cntl_status[ch] = CNTL_STOP;
			}
		}
		//-- PLAY
		else if( (paddata & SCE_PADRright ) || (paddata & SCE_PADRdown ) ){  
			if( run_status[ch] != 0 ){
				continue;
			}
			ezBgm( ch|EzBGM_SEEK, 0 );
			ezBgm( ch|EzBGM_PRELOAD, 0 );
			ezBgm( ch|EzBGM_START, 0 );
			cntl_status[ch] = CNTL_RUN;
		}
		//-- PAUSE
		else if (paddata & SCE_PADstart ){  
			if( tPause == 0 ){
			    for( i = 0; i < wav_count; i++ ){
				if( (run_status[i] & BGM_MODE_RUNNING) != 0 ){
					ezBgm( i|EzBGM_STOP, 0 );
				}
			    }
			    tPause = 1;
			}
			else if ( tPause == 1 ){
			    for( i = 0; i < wav_count; i++ ){
				if( (run_status[i] == 0) && (cntl_status[i] == CNTL_RUN) ){
					ezBgm( i|EzBGM_PRELOAD, 0 );
					ezBgm( i|EzBGM_START, 0 );
				}
			    }
			    tPause = 0;
			}
		}
		//-- QUIT
		else if (paddata & SCE_PADselect ){  
			for( i = 0; i < wav_count; i++ ){
				ezBgm( i|EzBGM_SETVOL, 0x0 );
				ezBgm( i|EzBGM_STOP, 0 );
				ezBgm( i|EzBGM_CLOSE, 0 );
				ezBgm( i|EzBGM_QUIT, 0 );
			}
			ezBgm( 0|EzBGM_SETMASTERVOL, 0x0 ); //-- master vol core0
			ezBgm( 1|EzBGM_SETMASTERVOL, 0x0 ); //-- master vol core1
			return;
		}
	    }
	    VSync();
	}
	return;
}


/*----------------------
  エントリ
  ----------------------*/
int main( int argc, char *argv[]  )
{
	int i, wav_count=0, status;

	//--- モジュールのロード
	load_modules();

	printf("EzBGM initialize...\n");

	//--- argsのパース
	if( (argc < 2) || argc > 3 ){
		printf("\n\nusage: %s <wav file> [<wav file>]\n\n", argv[0]);
		return 0;
	}

	for( i = 0; i < argc-1 ; i++ ){
		strcpy( gFilename[i], argv[i+1]);
		wav_count++;
	}

	//--- BGMをスタートさせる
	init_bgm(wav_count);


	//--- BGMをコントロールする
	main_loop(wav_count);
	

	printf("EzBGM finished...\n");
	return 0;
}


