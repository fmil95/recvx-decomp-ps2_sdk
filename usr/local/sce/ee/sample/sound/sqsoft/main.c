/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *                     Emotion Engine Library
 *                          Version 1.2.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                        sqsoft - main.c
 *                   EE Software Synthesizer Sample
 *
 *     Version   Date          Design   Log
 *  --------------------------------------------------------------------
 *     1.2.0     Nov.29.1999   morita   use LoadModule service
 *     0.6.0     Oct.12.1999   morita   first checked in.
 */

#include <eekernel.h>
#include <eeregs.h>
#include <sifdev.h>
#include <stdio.h>
#include <sif.h>
#include <csl.h>
#include <cslpcm.h>
#include <liblout.h>
#include <libssyn.h>
#include <sifrpc.h>
#include <libsdr.h>
#include <sdrcmd.h>

#define ERR_STOP while(1)
#define SSB_FILE "host0:../../../../data/sound/sce.ssb"
#define PAD_SIZE(x) ((64-((x)&(64-1)))&(64-1))
#define N_STREAM 2

//SSynthが１回に処理する最大処理単位。フレームレートに依存。
#define N_LO_UNIT 4
#define LO_BUFF_SIZE (N_LO_UNIT*sceLoutInputUnit) 

static struct {
	sceCslPcmStream	hdr;
	char			data[LO_BUFF_SIZE];
	char			pad[PAD_SIZE(sizeof(sceCslPcmStream)+LO_BUFF_SIZE)];
} pcmBf[N_STREAM] __attribute__ ((aligned(64)));

static sceCslCtx loCtx;
static sceCslBuffGrp loBuffGrp[1];
static sceCslBuffCtx loBuffCtx[N_STREAM];
static struct {
#define N_DMA_BUF	4
	sceLoutConf		conf;
	sceSifDmaData	dma[N_DMA_BUF];
} loConf = {
	{
		sceLoutDmaPreWait,
		{0, 1,  0,  1},
		(LO_BUFF_SIZE*2)*2,
		{NULL,NULL},
		N_DMA_BUF
	},
};


#define N_VOICES 32
#define RPC_BUF_SIZE (16*50)

static sceCslCtx ssCtx;
static sceCslBuffGrp ssBuffGrp[2];
static sceCslBuffCtx ssBuffCtx0;
static char voiceBuf[N_VOICES*sceSSynVoiceBufferSize] __attribute__ ((aligned(4)));
static char inBuf[sceSSynInputBufferSize] __attribute__ ((aligned(4)));

static sceSSynConf ssConf = {
	sceLoutInputUnit/sizeof(short),
	48000,
	N_VOICES,
	voiceBuf,	sizeof (voiceBuf),
	1,
	inBuf,		sizeof(inBuf)
};

static sceSSynEnv ssEnv __attribute__ ((aligned(64)));

struct {
	char	d[RPC_BUF_SIZE];
	char	pad[PAD_SIZE(RPC_BUF_SIZE)];
} ssRpcBuf __attribute__ ((aligned(64)));


/*------------------------------------------
  使用するモジュールをIOP側に読み込んで起動
  ------------------------------------------*/
void load_modules( void )
{
	sceSifInitRpc(0);


	//-- libsdはサウンド処理には必須
	while (sceSifLoadModule("host0:../../../../iop/modules/libsd.irx",0, NULL) < 0){
		printf("loading libsd.irx failed\n");
	}
	
	while (sceSifLoadModule("host0:../../../../iop/modules/sdrdrv.irx",0, NULL) < 0){
		printf("loading sdrdrv.irx failed\n");
	}
	
	while (sceSifLoadModule("host0:../../../../iop/modules/modmidi.irx",0, NULL) < 0){
		printf("loading modmidi.irx failed\n");
	}
	while (sceSifLoadModule("host0:../../../../iop/modules/modssyn.irx",0, NULL) < 0){
		printf("loading modssyn.irx failed\n");
	}

	return;
}

/*-------------------
  音素データを読む
  -------------------*/
static void *loadSSB(char *file)
{
	int sfd, flen;
	void *p;
	while ((sfd = sceOpen(file, SCE_RDONLY)) < 0) {
		perror(file);
	}
	while ((flen = sceLseek(sfd, 0, SCE_SEEK_END)) < 0) {
		perror(file);sceClose(sfd);
	}
	while (sceLseek(sfd, 0, SCE_SEEK_SET) < 0) {
		perror(file);sceClose(sfd);
	}
	printf("file length = %d\n", flen);
	while ((p = (void*)malloc(flen)) == NULL) {
		perror("malloc");sceClose(sfd);
	}
	while (sceRead(sfd, p, flen) != flen) {
		perror(file);sceClose(sfd);free(p);
	}
	sceClose(sfd);
	if (sceSSyn_PrepareParameter(p, flen) != sceSSynNoError) {
		printf("sceSSyn_PrepareParameter Error\n");
		ERR_STOP;
	}
	printf("Data = %x\n", p);
	return p;
}

/*-------------------------------
  LineOutモジュールのセットアップ


  -------------------------------*/
/* 
    ※LineOutモジュールの仕様※
    現在のところ、入力バッファの0番が右、1番が左として、SPU2入力ブロックの
    チャンネル１に出力します。
    環境設定用バッファはありません。
    これは暫定仕様で、次バージョンでは変更されます。
*/
int setLineOut(void)
{
	int i;

	// allocate IOP Buffer
	loConf.conf.iopBuf[0] = NULL;
	loConf.conf.iopBuf[1] = (void*)sceSifAllocIopHeap(loConf.conf.iopBufSize);
	if (loConf.conf.iopBuf[1] == NULL) {
		printf("sceSifAllocIopHeap Error\n"); return 1;
	}

	//-- initialize buffer context
	loCtx.extmod = NULL;
	loCtx.callBack = NULL;
	loCtx.conf = &loConf;
	loCtx.buffGrpNum = 1;
	loCtx.buffGrp = loBuffGrp;
	loBuffGrp[0].buffNum = N_STREAM;
	/* 通常、モジュール間のデータ共有はbuffer単位で行なうが、
	   SoftSynthとLineOutはBuffCtxの構造も全く同じなので、BuffCtx
	   まるごと共有している。*/
	loBuffGrp[0].buffCtx = loBuffCtx;
	for (i = 0; i < N_STREAM; i++) {
		loBuffCtx[i].sema = 0;
		loBuffCtx[i].buff = &(pcmBf[i].hdr);
		if (PAD_SIZE((unsigned int)loBuffCtx[i].buff)) {
			printf("port %d need %d bytes pad.\n", i,
				PAD_SIZE((unsigned int)loBuffCtx[i].buff));
			return 1;
		}
		pcmBf[i].hdr.pcmbuf_size = LO_BUFF_SIZE;
		pcmBf[i].hdr.validsize = 0;
		pcmBf[i].hdr.pcmbuf = pcmBf[i].data;
	}

	//-- EEメモリからSPU2へのPCM転送開始
	if (sceLout_Init(&loCtx, (1000*1000)/60) != sceLoutNoError) {
		printf("sceLout_Init Error\n"); return 1;
	}

	return 0;
}

/*---------------------------------
  SoftSynthモジュールのセットアップ
  ---------------------------------*/
int setSoftSynth(void)
{
	int i, k;
	void *param;

	//-- initialize buffer context
	ssCtx.extmod = NULL;
	ssCtx.callBack = NULL;
	ssCtx.conf = &ssConf; //test
	ssCtx.buffGrpNum = 2;
	ssCtx.buffGrp = ssBuffGrp;
	ssBuffGrp[0].buffNum = 1;
	ssBuffGrp[0].buffCtx =  &ssBuffCtx0;
	ssBuffCtx0.sema = 0;
	ssBuffCtx0.buff = &ssEnv;
	ssEnv.input_buff_len = RPC_BUF_SIZE;
	ssEnv.input_buff = ssRpcBuf.d;
	ssEnv.tone_param = NULL;

	ssBuffGrp[1].buffNum = N_STREAM;
	ssBuffGrp[1].buffCtx = loBuffCtx;  //-- LineOutと共有
	if (sceSSyn_Init(&ssCtx, 0 ) != sceSSynNoError) {
		printf("sceSSyn_Init Error\n"); return 1;
	}

	//-- 音素データファイル読みこみ
	param = loadSSB(SSB_FILE); 

	//-- 音素データセット
	if (sceSSyn_Load(&ssCtx, 0, param) != sceSSynNoError) {
		printf("sceSSyn_Load Error %d\n", i); return 1;
	}

	for (i = 0; i < 16; i++) {
		sceSSynChOutAttrib oatt;
		int k;
		for (k = 0; k < sceSynthesizerChOutNum; k++) {
			oatt.ch = i;
			oatt.ch_output = k;
			oatt.mode = (k & 1) ? sceSSynROut : sceSSynLOut;
			oatt.output_line = k;
			oatt.att = SSYN_VOLUME_0DB;
			sceSSyn_SetOutputAssign(&ssCtx, 0, &oatt);
		}
	}
	return 0;
}

/*-------------------
  main
  -------------------*/
int main(int argc, char **argv)
{
	unsigned short tm1;
	char argstring[64] = "host0:sample.sq";


	load_modules();


	//-- initialize hadrware
	sceSdRemoteInit();
	sceSdRemote( 1, rSdInit , SD_INIT_COLD );

	sceSdRemote(1, rSdSetParam, 1|SD_P_BVOLL , 0x3fff);
	sceSdRemote(1, rSdSetParam, 1|SD_P_BVOLR , 0x3fff);

	sceSifInitIopHeap();

	//-- initialize modules
	if( setLineOut() != 0 ) return 1;
	if( setSoftSynth() != 0) return 1;

	//-- communicate IOP modules
	ChangeThreadPriority(GetThreadId(), 20);
	if (sceSSyn_RegisterRpc(&ssCtx, 10) != sceSSynNoError) {
		printf("sceSSyn_RegisterRpc Error\n"); return 1;
	}

	printf("Ready to run IOP module.......\n");
	//-- ここで、EE側はIOP側からのデータ待ちになる

	//-- IOP側モジュールをロード
	while (sceSifLoadModule("host0:../../../../iop/sample/sound/sqsoft/main.irx",
		sizeof(argstring), argstring ) < 0){
		printf("loading ezbgm.irx failed\n"); 
	}
	printf("main.irx loaded\n");


	//-- main loop
	for (;;) {
		unsigned short tm2, tmx;
		unsigned short tm1 = DGET_T3_COUNT();  //Timer
		if (sceSSyn_ATick(&ssCtx) != sceSSynNoError) {
			printf("sceSSyn_ATick Error\n"); return 1;
		}
		if (sceLout_ATick(&loCtx) != sceLoutNoError) {
			printf("sceLout_ATick Error\n"); return 1;
		}
		VSync();
	}

	return 0;
}
/* $Id: main.c,v 1.12 2000/05/23 08:21:35 xokano Exp $ */

