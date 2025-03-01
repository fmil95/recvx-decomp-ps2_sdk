/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Controller Liblary Sample
 *
 *                         Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            main.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.10            Jan,21,2000     iwano       New
 */

#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <libpkt.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <libdev.h>
#include <libpad.h>
#include <libmtap.h>

#define PADMAN	"host0:/usr/local/sce/iop/modules/padman.irx"
#define SIO2MAN	"host0:/usr/local/sce/iop/modules/sio2man.irx"
#define MTAPMAN	"host0:/usr/local/sce/iop/modules/mtapman.irx"

extern int scePadGetFrameCount( int port, int slot );

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	448
#define OFFX	(((4096-SCREEN_WIDTH)/2)<<4)
#define OFFY	(((4096-SCREEN_HEIGHT)/2)<<4)


/* フォント表示用 */
extern void sceDevConsLocate(int cd, u_int lx, u_int ly);

/* 描画環境 */
static int frame = 0;
static int sindex = 0;
static sceGsDBuff db;
static sceDmaChan *p1;
static sceVif1Packet packet[2];

/* 描画用パケットバッファ */
#define SUNIT		800
#define PACKETSIZE	(8*SUNIT)
static u_long128 packet_buff[2][PACKETSIZE];

/* メッセージ表示用 */
static unsigned char mes_state[40];
static unsigned char mes_rpcstate[40];
static unsigned char mes_data1[80];
static unsigned char mes_data2[80];


/* コントローラ情報用構造体 */
typedef struct
{
	int port;
	int slot;
	int console;
	int term_id;
	int rterm_id;
	int phase;
	unsigned short pad;
	unsigned short rpad;
	unsigned char act_direct[6];
	unsigned char act_align[6];
	u_long128 pad_dma_buf[scePadDmaBufferMax] __attribute__((aligned (64)));
} TSLOTDATA;

static TSLOTDATA tSlot[4];
static int isExit = 0;


/* GS初期化 */
static int Gs_Initialize(void)
{
	sceDmaEnv env;
	u_long giftagAD[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1),
			 0x000000000000000eL };

	sceDmaReset(1);
	sceGsResetPath();

	sceVif1PkInit(&packet[0], (u_long128 *)packet_buff[0]);
	sceVif1PkInit(&packet[1], (u_long128 *)packet_buff[1]);

	sceDmaGetEnv(&env);
	env.notify = 1<<SCE_DMA_VIF1; /* notify channel */
	sceDmaPutEnv(&env);

	p1 = sceDmaGetChan(SCE_DMA_VIF1);
	p1->chcr.TTE = 1;

	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FIELD);
	sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT,
		SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);

	db.clear0.rgbaq.R = db.clear1.rgbaq.R = 64;
	db.clear0.rgbaq.G = db.clear1.rgbaq.G = 64;
	db.clear0.rgbaq.B = db.clear1.rgbaq.B = 128;

	/* Gs initialize packet */
	for (sindex = 0; sindex<2; sindex++)
	{
		sceVif1PkReset(&packet[sindex]);
		sceVif1PkCnt(&packet[sindex], 0);
		sceVif1PkOpenDirectCode(&packet[sindex], 0);
		sceVif1PkOpenGifTag(&packet[sindex], *(u_long128*)&giftagAD[0]);

		sceVif1PkReserve(&packet[sindex],
			sceGsSetDefAlphaEnv((sceGsAlphaEnv *)packet[sindex].pCurrent,0) * 4);

		sceVif1PkCloseGifTag(&packet[sindex]);
		sceVif1PkCloseDirectCode(&packet[sindex]);
		sceVif1PkEnd(&packet[sindex], 0);
		sceVif1PkTerminate(&packet[sindex]);

		sceGsSyncV(0);
		FlushCache(0);
		sceDmaSend(p1,(u_int *)(((u_int)packet[sindex].pBase)));
		sceGsSyncPath(0,0);
	}
	sindex = 0;
	while(!sceGsSyncV(0));
}

/* ボタン情報取得 */
static int pad_button_read(TSLOTDATA* pSlot)
{
	int i;
	int len;
	int term_id = 0;
	int port, slot;
	unsigned short tpad = 0;
	unsigned char rdata[32];
	unsigned short paddata = 0;

	port  = pSlot->port;
	slot  = pSlot->slot;

	len = scePadRead( port, slot, rdata );

	if (len==0)
	{
		/* 取得失敗 */
		return 0;
	}

	if( rdata[0] == 0 )
	{
		paddata = 0xffff^((rdata[2]<<8)|rdata[3]);
		for( i = 0; i <8; i++ )
		{
			sprintf( mes_data1+strlen(mes_data1), "%02x ", rdata[ i ] );
		}
		for( i = 8; i <16; i++ )
		{
			sprintf( mes_data2+strlen(mes_data2), "%02x ", rdata[ i ] );
		}

		tpad = pSlot->pad = paddata & ~(pSlot->rpad);
		pSlot->rpad = paddata;

		term_id = pSlot->term_id = (rdata[1]);

		switch(term_id>>4)
		{
			case 0x7:
			if( tpad & SCE_PADLup ) {
				pSlot->act_direct[ 0 ] = 1;
			}
			if( tpad & SCE_PADLdown ) {
				pSlot->act_direct[ 0 ] = 0;
			}
			if( tpad & SCE_PADLleft ) {
				pSlot->act_direct[ 1 ] = 200;
			}
			if( tpad & SCE_PADLright ) {
				pSlot->act_direct[ 1 ] = 0;
			}
			if (tpad) scePadSetActDirect( port, slot, pSlot->act_direct );
			break;

		}
		if (tpad==SCE_PADselect) isExit = 1;
	}
	return (term_id);
}

static int attach_controller(TSLOTDATA* pSlot)
{
	int i;
	int id;
	int exid;
	int state;
	int phase;
	int term_id = 0;
	int port, slot;

	port  = pSlot->port;
	slot  = pSlot->slot;
	phase = pSlot->phase;

	/* コントローラの状態チェック */
	state = scePadGetState(port, slot);
	scePadStateIntToStr(state, mes_state);

	if (state==scePadStateDiscon)
	{
		phase = 0;
	}

	strcpy(mes_data1, "");
	strcpy(mes_data2, "");

	/* コントローラ情報取得、アクチュエータ設定 */
	switch(phase)
	{
		case 0:
		if (state != scePadStateStable && 
		  state != scePadStateFindCTP1) break;

		/* コントローラID取得 */
		id = scePadInfoMode(port, slot, InfoModeCurID, 0 );
		if (id==0) break;

		/* 拡張ID 取得 */
		exid = scePadInfoMode(port, slot, InfoModeCurExID,0);
		if (exid>0)
		{
			if (exid<100 && id != exid)
				printf ("ID:%d ExID:%d [%d][%d]\n", id, exid, port, slot);
			id = exid;
		}

		switch(id)
		{
			/* STANDARD */
			case 4:
			phase = 40;
			break;

			/* ANALOG */
			case 7:
			phase = 70;
			break;

			default:
			phase = 99;
			break;
		}
		break;

		case 40:
		/* コントローラタイプ判定 */
		if(scePadInfoMode(port, slot, InfoModeCurExID,0)==0)
		{
			/* Digicon */
			phase = 99;
			break;
		}
		phase++;

		case 41:
		/* PSCTP2.0の拡張モードに移行 */
		if (scePadSetMainMode(port, slot, 1, 0)==0)
		{
			printf ("MainMode faild\n");
			break;
		}
		phase++;

		case 42:
		if (scePadGetReqState(port, slot)==scePadReqStateFaild) phase--;
		if (scePadGetReqState(port, slot)==scePadReqStateComplete) phase = 0;
		break;

		case 70:
		/* アクチュエータ情報取得 */
		if (scePadInfoAct(port, slot, -1, 0)==0)
		{
			/* SCPH-1150 */
			phase = 99;
		}

		/* アクチュエータ情報設定(DUALSOCKのみ) */
		for( i = 0; i < 6; i++ )
			pSlot->act_align[ i ] = 0xff;
		pSlot->act_align[ 0 ] = 0;
		pSlot->act_align[ 1 ] = 1;
		if (scePadSetActAlign( port, slot, pSlot->act_align )==0)
		{
			printf ("ActAlign faild\n");
			break;
		}
		phase++;
		if (scePadGetReqState(port, slot)!=scePadReqStateBusy)
		{
			printf ("ACT ALIGN WARRNING\n");
		}
		break;

		case 71:
		if (scePadGetReqState(port, slot)==scePadReqStateFaild) phase--;
		if (scePadGetReqState(port, slot)==scePadReqStateComplete) phase++;
		break;

		case 72:
		if (scePadInfoPressMode( port, slot ) == 0)
		{
			pSlot->rterm_id = 0;
			phase = 99;
			break;
		}
		phase = 76;

		/* 感圧モードに移行 */
		case 76:
		if (scePadEnterPressMode( port, slot )==0)
		{
			printf ("EnterPressMode Faild\n");
			phase = 72;
			break;
		}
		phase++;
		if (scePadGetReqState(port, slot)!=scePadReqStateBusy)
		{
			printf ("EnterPressMode WARRNING\n");
		}
		break;

		case 77:
		if (scePadGetReqState(port, slot)==scePadReqStateFaild) phase--;
		if (scePadGetReqState(port, slot)==scePadReqStateComplete) phase++;
		break;

		case 78:
		phase = 99;
		break;

		default:
		if (state!=scePadStateStable && state!=scePadStateFindCTP1) break;

		term_id = pad_button_read( pSlot );

		if (term_id > 0 && pSlot->rterm_id!=0 && term_id != pSlot->rterm_id)
		{
			pSlot->rterm_id = 0;
			phase = 0;
		}
		if (term_id>0) pSlot->rterm_id = term_id;
		break;
	}

	scePadReqIntToStr(scePadGetReqState(port, slot), mes_rpcstate);

	/* フォント表示 */
	sceDevConsClear(pSlot->console);
	sceDevConsLocate(pSlot->console, 0, 0);
	sceDevConsPrintf(pSlot->console, "[PORT %d][SLOT %d]\n", port, slot);
	sceDevConsPrintf(pSlot->console, "\n");
	sceDevConsPrintf(pSlot->console, "STATUS : %s\n", mes_state );
	sceDevConsPrintf(pSlot->console, "REQEST : %s\n", mes_rpcstate);
	sceDevConsPrintf(pSlot->console, "DATA-1 : %s\n", mes_data1 );
	sceDevConsPrintf(pSlot->console, "DATA-2 : %s\n", mes_data2 );
	sceDevConsPrintf(pSlot->console, "\n");
	sceDevConsPrintf(pSlot->console, "FRAME= %d/%d", frame%10000, scePadGetFrameCount(port, slot)%10000);
	pSlot->phase = phase;
	return (phase);
}

static int pad_port_open(int port, int slot, TSLOTDATA* pSlot)
{
	int i;
	int ret;

	/* ポートオープン */
	ret = scePadPortOpen( port, slot, pSlot->pad_dma_buf );
	if (ret==0)
	{
		printf( "ERROR: scePadPortOpen\n" );
		return 0;
	}

	/* アクチュエータ情報初期化 */
	for (i=0; i<6; i++)
		pSlot->act_direct[i] = 0x00;

	pSlot->port = port;
	pSlot->slot = slot;
	pSlot->rterm_id = 0;
	pSlot->phase = 0;

	return 1;
}

int main(void)
{
	/* IOP モジュールロード */
	sceSifInitRpc(0);
	while (sceSifLoadModule(SIO2MAN, 0, NULL) < 0)
	{
	    printf("Can't load module sio2man\n");
	}
	while (sceSifLoadModule(MTAPMAN, 0, NULL) < 0)
	{
	    printf("Can't load module mtap\n");
	}
	while (sceSifLoadModule(PADMAN, 0, NULL) < 0)
	{
	    printf("Can't load module padman\n");
	}

	/* GS初期化 */
	Gs_Initialize();

	/* フォントシステム初期化 */
	sceDevConsInit();
	tSlot[0].console = sceDevConsOpen(OFFX + (16<<4), OFFY + (24<<4), 36, 10);
	tSlot[1].console = sceDevConsOpen(OFFX + (16<<4), OFFY + (128<<4), 36, 10);
	tSlot[2].console = sceDevConsOpen(OFFX + (16<<4), OFFY + (232<<4), 36, 10);
	tSlot[3].console = sceDevConsOpen(OFFX + (16<<4), OFFY + (336<<4), 36, 10);

	/* マルチタップライブラリ初期化 */
	sceMtapInit();
	sceMtapPortOpen(0);

	/* コントローラライブラリ初期化 */
	scePadInit(0);
	pad_port_open(0, 0, &tSlot[0]);
	pad_port_open(0, 1, &tSlot[1]);
	pad_port_open(0, 2, &tSlot[2]);
	pad_port_open(0, 3, &tSlot[3]);


	/* メインループ */
	frame = 0;
	while(1)
	{
		attach_controller(&tSlot[0]);
		attach_controller(&tSlot[1]);
		attach_controller(&tSlot[2]);
		attach_controller(&tSlot[3]);

		sceGsSyncV(0);
		sceGsSwapDBuff(&db, frame);
		++frame;

		sceDevConsDrawS(tSlot[0].console);
		sceDevConsDrawS(tSlot[1].console);
		sceDevConsDrawS(tSlot[2].console);
		sceDevConsDrawS(tSlot[3].console);

		sindex ^= 0x01;

		if (isExit==1) break;
	}

	scePadPortClose(0,0);
	scePadPortClose(0,1);
	scePadPortClose(0,2);
	scePadPortClose(0,3);
	scePadEnd();
	return 1;
}



