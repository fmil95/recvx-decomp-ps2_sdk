/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *	Controller Library Sample
 *
 *                          Version 0.11
 *	                    Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *	                 All Rights Reserved.
 *
 *                              main.c
 *
 *        Version       Date            Design     Log
 *  --------------------------------------------------------------------
 *      0.10            Oct, 8,1999     makoto     Initial
 *	0.11            Oct,14,1999     xiwano     remake
 */


#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <libpkt.h>
#include <sifdev.h>
#include <libdev.h>
#include <sifrpc.h>
#include <libpad.h>

#define SUNIT		0x01
#define PACKETSIZE	(0x100*SUNIT)
#define WORKSIZE	(0x80)
#define WORKBASE	0x70000000
#define SUPPER		(WORKBASE+WORKSIZE)
#define SLOWER		(WORKBASE+WORKSIZE+PACKETSIZE)

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	224
#define OFFX		(((4096-SCREEN_WIDTH)/2)<<4)
#define OFFY		(((4096-SCREEN_HEIGHT)/2)<<4)

/* IOPインタフェース用DMAバッファ */
static u_long128 pad_dma_buf[scePadDmaBufferMax] __attribute__((aligned (64)));

static unsigned char act_direct[6], act_align[6];
static unsigned char mes_state[80];
static unsigned char mes_term[80];
static unsigned char mes_data[80];
static unsigned char* StateMessage[] =
{"DISCONNECT", "FINDPAD", "FINDCTP1", "", "", "EXECCMD", "STABLE", "ERROR" };
/* フォント表示用 */
static int console;
extern void sceDevConsLocate(int cd, u_int lx, u_int ly);
/* 描画環境 */
static sceGsDBuff db;
/* GS初期化 */
static int Gs_Initialize(void)
{
	int sindex;
	sceVif1Packet packet[2];
	sceDmaEnv env;
	sceDmaChan *p1;
	u_long giftagAD[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1),
			 0x000000000000000eL };
	sceDevVif0Reset();
	sceDevVu0Reset();
	sceGsResetPath();
	sceDmaReset(1);

	sceVif1PkInit(&packet[0], (u_long128 *)SUPPER);
	sceVif1PkInit(&packet[1], (u_long128 *)SLOWER);

	sceDmaGetEnv(&env);
	env.notify = 1<<SCE_DMA_VIF1; /* notify channel */
	sceDmaPutEnv(&env);

	p1 = sceDmaGetChan(SCE_DMA_VIF1);
	p1->chcr.TTE = 1;

	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
	sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT,
		SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);

	db.clear0.rgbaq.R = 0x40;
	db.clear0.rgbaq.G = 0x40;
	db.clear0.rgbaq.B = 0x80;

	db.clear1.rgbaq.R = 0x40;
	db.clear1.rgbaq.G = 0x40;
	db.clear1.rgbaq.B = 0x80;

	FlushCache(0);

	sindex = 0;
	sceVif1PkReset(&packet[sindex]);
	sceVif1PkCnt(&packet[sindex], 0);
	sceVif1PkOpenDirectCode(&packet[sindex], 0);
	sceVif1PkOpenGifTag(&packet[sindex], *(u_long128*)&giftagAD[0]);

	sceVif1PkReserve(&packet[sindex],
		sceGsSetDefAlphaEnv((sceGsAlphaEnv *)packet[sindex].pCurrent,0)
		* 4);
	sceVif1PkCloseGifTag(&packet[sindex]);
	sceVif1PkCloseDirectCode(&packet[sindex]);
	sceVif1PkEnd(&packet[sindex], 0);
	sceVif1PkTerminate(&packet[sindex]);

	/* kick Gs initialize packet */
	sceDmaSend(p1,(u_int *)
		(((u_int)packet[sindex].pBase&0x0fffffff)|0x80000000));

	/* display next in odd field when interlace */
	while(!sceGsSyncV(0));

}

/* ボタン情報取得 */
static int pad_button_read(int port, int slot)
{
	int i;
	int term_id;
	unsigned char rdata[32];
	unsigned short paddata, tpad;
	static unsigned short rpad = 0;

	strcpy(mes_data, "");
	term_id = 0;

	if (scePadRead( port, slot, rdata )==0)
	{	
		/* 取得失敗 */
		return 0;
	}

	if( rdata[0] == 0 )
	{
		paddata = 0xffff ^ ((rdata[2]<<8)|rdata[3]);
		for( i = 0; i < 15; i++ )
		{
			sprintf( mes_data+strlen(mes_data), "%02x ", rdata[ i ] );
		}

		tpad = paddata & ~rpad;
		rpad = paddata;

		term_id = (rdata[1]>>4);

		switch(term_id)
		{
			case 7:
			if( tpad & SCE_PADLup ) {
				act_direct[ 0 ] = 1;
				scePadSetActDirect( port, slot, act_direct );
			}
			if( tpad & SCE_PADLdown ) {
				act_direct[ 0 ] = 0;
				scePadSetActDirect( port, slot, act_direct );
			}
			if( tpad & SCE_PADLleft ) {
				act_direct[ 1 ] = 200;
				scePadSetActDirect( port, slot, act_direct );
			}
			if( tpad & SCE_PADLright ) {
				act_direct[ 1 ] = 0;
				scePadSetActDirect( port, slot, act_direct );
			}
			break;
		}
	}
	return (term_id);
}


int main(void)
{
	int i;
	int id;
	int frame;
	int phase;
	int state;
	int port;
	int slot;
	int ret;
	int exid;
	int term_id;
	int rterm_id;

	sceSifInitRpc(0);
	/* load sio2man.irx */
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
			    0, NULL) < 0)
	{
	    printf("Can't load module sio2man\n");
	}
	/* load padman.irx */
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
			    0, NULL) < 0)
	{
	    printf("Can't load module padman\n");
	}

	/* GS初期化 */
	Gs_Initialize();

	/* フォントシステム初期化 */
	sceDevConsInit();
	console = sceDevConsOpen(OFFX + (16<<4), OFFY + (24<<4), 75, 30);
	sceDevConsClear(console);
	strcpy(mes_data, "");
	strcpy(mes_state, "");
	strcpy(mes_term, "");

	/* コントローラライブラリ初期化 */
	scePadInit(0);
	port = 0;
	slot = 0;
	term_id = 0;
	rterm_id = 0;

	/* アクチュエータ情報初期化 */
	for (i=0; i<6; i++)
		act_direct[i] = 0x00;
	/* ポートオープン */
	ret = scePadPortOpen( port, slot, pad_dma_buf );
	if (ret==0)
	{
		printf( "ERROR: scePadPortOpen\n" );
		return 0;
	}

	phase = 0;
	frame = 0;
	sceGsSyncPath(0,0);
	while(1)
	{
		sceGsSwapDBuff(&db, frame);
		++frame;
		/* コントローラの状態チェック */
		state = scePadGetState(port,slot);
		if (state>=0 && state<=7)
			sprintf(mes_state,"%s", StateMessage[state] );
		else
			sprintf(mes_state,"%d", state);
		if (state==scePadStateDiscon)
		{
			phase = 0;
		}

		/* コントローラ情報取得、アクチュエータ設定 */
		switch(phase)
		{
			case 0:
			strcpy(mes_term, "");
			if (state != scePadStateStable &&
			  state != scePadStateFindCTP1 ) break;

			/* コントローラID取得 */
			id = scePadInfoMode(port, slot, InfoModeCurID, 0 );
			if (id==0) break;
			/* 拡張ID 取得 */
			exid = scePadInfoMode(port, slot, InfoModeCurExID,0);
			if (exid>0) id = exid;

			switch(id)
			{
				/* NEGI-COM */
				case 2:
				sprintf(mes_term, "%04x [%s]", id, "NeGi-CON");
				phase = 99;
				break;

				/* KONAMI-GUN */
				case 3:
				sprintf(mes_term, "%03x [%s]", id, "GunCON(Konami)");
				phase = 99;
				break;

				/* STANDARD */
				case 4:
				sprintf(mes_term, "%03x [%s]", id, "STANDARD");
				phase = 40;
				break;

				/* JOY STICK */
				case 5:
				sprintf(mes_term, "%03x [%s]", id, "JOYSTICK");
				phase = 99;
				break;

				/* NAMCO-GUN */
				case 6:
				sprintf(mes_term, "%03x [%s]", id, "GunCON(NAMCO)");
				phase = 99;
				break;

				/* ANALOG */
				case 7:
				sprintf(mes_term, "%03x [%s]", id, "ANALOG");
				phase = 70;
				break;
				/* 釣りコン */
				case 0x100:
				sprintf(mes_term, "%03x [%s]", id, "TSURI-CON");
				phase = 99;
				break;
				/* ジョグコン */
				case 0x300:
				sprintf(mes_term, "%03x [%s]", id, "JOG-CON");
				phase = 99;
				break;

				default:
				sprintf(mes_term, "%03x [%s]", id, "UNKNOWN");
				phase = 99;
				break;
			}
			break;

			case 40:
			if (scePadInfoMode(port, slot, InfoModeCurExID,0)==0)
			{
				phase = 99;
				break;
			}
			phase++;
			case 41:
			/* switch to Extend mode */
			if (scePadSetMainMode(port, slot, 1, 3)==1)
			{
				phase++;
			}
			break;
			case 42:
			if (scePadGetState(port,slot)==scePadStateExecCmd) break;
			phase = 0;
			break;

			case 70:
			if (scePadInfoAct(port, slot, -1, 0)==0)
			{
				/* SCPH-1150 */
				phase = 99;
			}
			/* ACTUATER SETTING (DUALSHOK) */
			act_align[ 0 ] = 0;
			act_align[ 1 ] = 1;
			for( i = 2; i < 6; i++ )
				act_align[ i ] = 0xff;
			if (scePadSetActAlign( port, slot, act_align ))
			{
				phase++;
			}
			break;
			case 71:
			if (scePadGetState(port,slot)==scePadStateExecCmd) break;
			phase = 99;
			break;

			default:
			if (state==scePadStateStable || state==scePadStateFindCTP1)
			{
				if ((term_id = pad_button_read( port, slot ))==0)
				{
					break;
				}
				if (rterm_id!=0 && term_id!=rterm_id)
				{
					rterm_id = 0;
					phase = 0;
				}
				rterm_id = term_id;
			}
			break;
		}
		/* フォント表示 */
		sceDevConsClear(console);
		sceDevConsLocate(console, 0, 0);
		sceDevConsPrintf(console, "LIBPAD SAMPLE\n" );
		sceDevConsPrintf(console, "------------------------------------------------------------------------\n" );
		sceDevConsPrintf(console, "\n" );
		sceDevConsPrintf(console, "  STATUS : %s\n", mes_state );
		sceDevConsPrintf(console, "      ID : %s\n", mes_term );
		sceDevConsPrintf(console, "PAD DATA : %s\n", mes_data );

		sceDevConsDraw(console);

		if(frame&0x01)
		{	/* interrace half pixcel adjust */
			sceGsSetHalfOffset(&db.draw1,2048,2048, sceGsSyncV(0)^0x01);
		}
		else
		{
			sceGsSetHalfOffset(&db.draw0,2048,2048, sceGsSyncV(0)^0x01);
		}
		FlushCache(0);
		sceGsSyncPath(0,0);
	}

	return 0;
}
