/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *                     I/O Processor Library
 *                          Version 1.20
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                         sqhard - main.c
 *                      MIDI Sequencer Sample
 *
 *     Version   Date          Design     Log
 *  --------------------------------------------------------------------
 *     1.20      Nov.28.1999   morita     changed for LoadModule from EE
 *     0.60      Oct.14.1999   morita     first checked in.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <kernel.h>
#include <stdio.h>
#include <sif.h>
#include <csl.h>
#include <cslmidi.h>
#include <modmidi.h>
#include <modssyn.h>
#include <libsd.h>
#include <timerman.h>
#include <sys/file.h>


#define MIDI_IN_PORT    0
#define True 1

#define SEQ1_ADDR 0x100000

#define INTERVAL_MSEC 4167

#define SD_MMIX_SINER  (1 <<  0)
#define SD_MMIX_SINEL  (1 <<  1)

#define STREAMBUF_SIZE 1024
static sceCslCtx midiCtx;
static sceCslBuffGrp midiGrp[2];
static sceCslBuffCtx mInBfCtx[2], mOutBfCtx[1];

static char streamBf[STREAMBUF_SIZE+sizeof(sceCslMidiStream)];
static sceMidiEnv midiEnv;

static sceCslCtx synthCtx;
static sceCslBuffGrp synthGrp;
static sceCslBuffCtx sInBfCtx[2];
static sceSSynEnv synthEnv;

volatile static int gIntrEnableFlag = 0;

char gFilename[64];

typedef struct TimerCtx
{
	int threadID;
	int timerID;
} TimerCtx;

int gComPort[16];

/*--------------------------
  タイマー用割り込みハンドラ
  --------------------------*/
int IntFunc( TimerCtx* timer )
{
	int m ;
	m = iGetTimerStatus(timer->timerID);


	if ( m & tEQUF_1 ) {
		iWakeupThread( timer->threadID );
	}
	return 1;
}


/*-------------------
  タイマー設定
  -------------------*/
void setTimer( TimerCtx* timer )
{
	// --- use H-Sync timer
	timer->timerID = AllocHardTimer(TC_HLINE , 16, 1);
	RegisterIntrHandler(GetHardTimerIntrCode(timer->timerID),
			HTYPE_C, (void*)IntFunc, timer );
	SetTimerCompare(timer->timerID, 66);  /* 1/240 sec interval */
	SetTimerMode( timer->timerID , tEXTC_1 | tGATF_0 | tZRET_1 | tREPT_1 | tCMP_1);
}

void startTimer( TimerCtx* timer )
{
	EnableIntr(GetHardTimerIntrCode(timer->timerID));
//	gIntrEnableFlag = 1;
}

/*-------------------
  タイマー削除
  -------------------*/
void clrTimer(TimerCtx* timer)
{
	ReleaseIntrHandler(GetHardTimerIntrCode(timer->timerID));
        FreeHardTimer(timer->timerID);
}

void stopTimer(TimerCtx* timer)
{
	int oldstat, stat;
	stat = DisableIntr(GetHardTimerIntrCode(timer->timerID), &oldstat);
}


/*-------------------
  1Tick毎の処理
  -------------------*/
static void ATick(void)
{
	sceMidi_ATick(&midiCtx);
	sceSSyn_ATick(&synthCtx);
}


/*-------------------
  CallBackの例
  -------------------*/
int metaMsgCB(unsigned char metaNo,unsigned char *bf, unsigned int len, unsigned int private)
{
	sceCslMidiStream *pHdr = (sceCslMidiStream*)private;
	pHdr = pHdr;
	printf("META %02x\n", (int)metaNo);
	return True;
}

int repeatCB(sceMidiLoopInfo *pInf, unsigned int private)
{
	sceCslMidiStream *pHdr = (sceCslMidiStream*)private;
	pHdr = pHdr;
	printf("Repeat %s %d %d %x\n",
		(pInf->type == sceMidiLoopInfoType_Midi) ? "Midi" : "Song",
		(int)pInf->loopTimes, (int)pInf->loopCount, pInf->loopId);
	return True;
}

/*-------------------
  Module Context設定
  -------------------*/
void setModuleContext( void )
{
	midiCtx.extmod = NULL;
	midiCtx.callBack = NULL;
	midiCtx.buffGrpNum = 2;
	midiCtx.buffGrp = midiGrp;
	  midiGrp[0].buffNum = 2;
	  midiGrp[0].buffCtx = mInBfCtx;
	  midiGrp[1].buffNum = 1;
	  midiGrp[1].buffCtx = mOutBfCtx;
	    mInBfCtx[0].sema = 0;
	    mInBfCtx[0].buff = NULL;
	    mInBfCtx[1].sema = 0;
	    mInBfCtx[1].buff = &midiEnv;
	    mOutBfCtx[0].sema = 0;
	    mOutBfCtx[0].buff = streamBf;
	    ((sceCslMidiStream*)streamBf)->buffsize 
		= STREAMBUF_SIZE+sizeof(sceCslMidiStream);
	    ((sceCslMidiStream*)streamBf)->validsize = 0;

	synthCtx.extmod = NULL;
	synthCtx.callBack = NULL;
	synthCtx.buffGrpNum = 1;
	synthCtx.buffGrp = &synthGrp;
	  synthGrp.buffNum = 2;
	  synthGrp.buffCtx = sInBfCtx;
	    sInBfCtx[0].sema = 0;
	    sInBfCtx[0].buff = streamBf;
	    sInBfCtx[1].sema = 0;
	    sInBfCtx[1].buff = &synthEnv;

	return;
}

/*---------------------
  HardSynthセットアップ
  ---------------------*/
int setSoftSynth( void )
{
	if (sceSSyn_Init(&synthCtx,INTERVAL_MSEC) != sceSSynNoError) { //-- 1/240sec
		printf("sceSSyn_Init Error\n"); return 1;
	}

	return 0;
}


#define ALLOC_SIZE (128*1024)

/*--------------------------
  Midi Sequencerセットアップ
  --------------------------*/
int setMidiSeqencer( void )
{
	void* sq;
	int fd, i;

	sq  = AllocSysMemory( 0, ALLOC_SIZE, NULL );
	if (( fd = open ( gFilename, O_RDONLY)) < 0) {
		printf("file open failed. %s\n",gFilename );
		return -1;
	}
	if( read( fd, sq, ALLOC_SIZE ) < 0 ){
		printf("file read failed. %s\n",gFilename );
		return -1;
	}

	midiCtx.buffGrp[0].buffCtx[0].buff = sq;
	if (sceMidi_Load(&midiCtx, MIDI_IN_PORT) != sceMidiNoError) {
		printf("sceMidi_Load(%d) error\n", i); return 1;
	}
	if (sceMidi_Init(&midiCtx, INTERVAL_MSEC ) != sceMidiNoError) { //-- 1/240sec
		printf("sceMidi_Init Error\n"); return 1;
	}

	midiEnv.chMsgCallBackPrivateData = (unsigned int)streamBf;
	midiEnv.metaMsgCallBack = metaMsgCB;
	midiEnv.metaMsgCallBackPrivateData = (unsigned int)streamBf;
	midiEnv.repeatCallBack = repeatCB;
	midiEnv.repeatCallBackPrivateData = (unsigned int)streamBf;
	midiEnv.excOutPort = 1;
	for (i = 0; i < sceMidiNumMidiCh; i++) {
		midiEnv.outPort[i] =  1 << 0;
	}

	if (sceMidi_SelectMidi(&midiCtx, MIDI_IN_PORT, 0) != sceMidiNoError) {
		printf("sceMidi_SelectMidi Error\n"); return 1;
	}
	if (sceMidi_MidiSetLocation(&midiCtx, MIDI_IN_PORT, 0) != sceMidiNoError)
	{
		printf("sceMidi_MidiSetLocation Error\n"); return 1;
	}
	printf("LOCATION = %d\n", sceMidi_GetEnv(&midiCtx, 1)->position);
	
	if (sceMidi_MidiPlaySwitch(&midiCtx, MIDI_IN_PORT,sceMidi_MidiPlayStart) != sceMidiNoError) {
		printf("sceMidi_MidiPlaySwitch Error\n"); return 1;
	}

	return 0;
}

/*-------------------
  エフェクト設定
  -------------------*/
void setEffect( void )
{
	int i;
	sceSdEffectAttr r_attr;

	//--- エフェクト設定
	for( i = 0; i < 2; i++ )
	{
		//--- reverb end addressを core0とcore1とで別の領域に設定する
		sceSdSetAddr( i|SD_A_EEA, 0x1fffff - 0x20000*i );
		// --- set reverb attribute
		r_attr.depth_L  = 0x3800;
		r_attr.depth_R  = 0x3800;
		r_attr.mode = SD_REV_MODE_STUDIO_A | SD_REV_MODE_CLEAR_WA;
		sceSdSetEffectAttr( i, &r_attr );
		// --- reverb on
		sceSdSetCoreAttr( i|SD_C_EFFECT_ENABLE, 1 );
	}
	//-- core0 out は core1 のWET側には入れない
	sceSdSetParam( 1|SD_P_MMIX , ~( SD_MMIX_SINEL | SD_MMIX_SINER ) );

	return;
}


/*-------------------
  メイン
  -------------------*/
int myMain(void)
{
	int i;
	TimerCtx timer;
	int count = 0;

	//-- initialize hardware
#if 0 //-- SPUの初期化はEE側のsceSdRemoteInitで行なわれている
	sceSdInit(0);
	EnableIntr(INUM_DMA_4);
	EnableIntr(INUM_DMA_7);
	EnableIntr(INUM_SPU);
#endif

	setEffect();
	for( i = 0; i < 2; i++ ) //- set master volume
	{
		sceSdSetParam( i|SD_P_MVOLL , 0x3fff ) ;
		sceSdSetParam( i|SD_P_MVOLR , 0x3fff ) ;
	}

	timer.threadID = GetThreadId();
	setTimer(&timer);

	//-- initialize modules
	setModuleContext();

	setSoftSynth();

	setMidiSeqencer();

	//-- main loop
	startTimer(&timer);
	while (sceMidi_isInPlay(&midiCtx, MIDI_IN_PORT)) {
		ATick();
		SleepThread(); //-- 仕事の無い時は寝る
	}
	
	printf("/////// count %d ///////", ++count );
	stopTimer(&timer);

	clrTimer(&timer);
	printf("EXIT............\n");
	return 0;
}


/*-------------------------
  mainをスレッドとして作る。
  -------------------------*/
//-- sdrdrv.irxと同時に動作するので、sdrdrv.irxのプライオリティ(現在のとこ
//   ろ34・変更の可能性あり)よりも低くしておく必要がある。

int start( int argc, char *argv[] )
{
 	struct ThreadParam param;
	int thid;

	strcpy( gFilename, argv[1] ); //-- EEからargを受け取っている
	printf("filename = %s\n", gFilename );

	param.attr         = TH_C;
	param.entry        = myMain;
	param.initPriority = 36;     //-- sdrdrv.irxよりも低くする
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


