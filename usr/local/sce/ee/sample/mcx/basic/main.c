/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - balls -
 *
 *                         Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            main.c
 *                     main function of cube
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.10            Jan,14,1999     yutaka    
 *	1.00		Jul,31,2000	Honda
 */


#include <eekernel.h>
#include <stdlib.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libvu0.h>
#include <libdma.h>
#include <libpkt.h>
#include <sifdev.h>		/* SCE_SEEK_SET/CUR/END */
#include <libdev.h>
#include <sifrpc.h>
#include <libpad.h>
#include <libmc.h>
#include <sifdev.h>
#include <libmcx.h>


#define SUNIT		0x08		/* プリセットするボールの数*/
#define PACKETSIZE	(0x100*SUNIT)
#define WORKSIZE	(0x80)
#define WORKBASE	0x70000000
#define SUPPER		(WORKBASE+WORKSIZE)
#define SLOWER		(WORKBASE+WORKSIZE+PACKETSIZE)

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	224
#define OFFX		(((4096-SCREEN_WIDTH)/2)<<4)
#define OFFY		(((4096-SCREEN_HEIGHT)/2)<<4)
#define WALL_X		((SCREEN_WIDTH-16)<<4)	/* reflection point */
#define WALL_Y		((SCREEN_HEIGHT-8)<<4)

#define UseMultiTap	0


void loadIopModule(void);


#define LocDispStatus	68
#define SizeConsX	78
#define MaxDirView	16

#define ScrlHight	50

typedef struct {
	unsigned char *Title;
	int (*Func)();
	} Menu;

sceGsDBuff db;
int console;
int frame;
unsigned char sectBuff[128], paramBuff[128];
int mcport = 0, mcslot = 0;
int pdport = 0, pdslot = 0;
u_long128 padwork[4][scePadDmaBufferMax] __attribute__((aligned (64)));

void init_config_system(void);
void cgaDispTitle(int, int);
void BUG_WALKAROUND(sceVif1Packet *);
void remapColorLut(int);
int inpstr(unsigned char *, int, int, int, int);
void dispPageCont(unsigned char *, unsigned char *, int);
unsigned atox(unsigned char *);
int isdigit(unsigned char);
int isxdigit(unsigned char);
unsigned char toupper(unsigned char);


int menuMoveFunc(int, int);
int mnIdleFunc(int, int);
int mnGetDir(int, int);
int mnGetInfo(int, int);
int mnDispTrans(int, int);
int mnReadDev(int, int);
int mnWriteDev(int, int);
int mnEditMem(int, int);
int mnEditUifs(int, int);
int mnSetLed(int, int);


void dispError(int);

unsigned char week2asc[7][4] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};



#define MaxExecTitle	8
#define MaxVertTitle	3

Menu menu[MaxExecTitle] = {
	{ "get dir",		mnGetDir },
	{ "pda info",		mnGetInfo },
	{ "disp trans",		mnDispTrans },
	{ "read dev",		mnReadDev },
	{ "write dev",		mnWriteDev },
	{ "edit mem",		mnEditMem },
	{ "edit UIFS",		mnEditUifs },
	{ "LED",		mnSetLed },
};



/*#main --- main routine */
int main(int argc, char *argv[]) {
	sceVif1Packet packet[2];	/* DMAパケットハンドラ */
	sceDmaEnv env;			/* DMA 共通環境 */
	sceDmaChan *p1;
	int sindex;
	u_long giftag[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 1, 12),
			 0x0000f43434343610L };	/* スプライト用GIFタグ */
	u_long giftagAD[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1),
			 0x000000000000000eL };	/* A＋D用GIFタグ */

	unsigned char padbuff[34];
	int ret, oldbtn, curbtn;
	int phase, func, exec;
	int count;


	loadIopModule();

	sceDevVif0Reset();
	sceDevVu0Reset();
	sceGsResetPath();
	sceDmaReset(1);

	*T0_MODE = T_MODE_CLKS_M | T_MODE_CUE_M; /* Hcount Timer Setting */
	sindex = 0;

	sceVif1PkInit(&packet[0], (u_long128 *)SUPPER);
	sceVif1PkInit(&packet[1], (u_long128 *)SLOWER);

	sceDmaGetEnv(&env);		/* DMA共通環境取得 */
	env.notify = 1<<SCE_DMA_VIF1; /* notify channel */
	sceDmaPutEnv(&env);

	/* DMA VIF１チャンネルデスクリプタ取得 */
	p1 = sceDmaGetChan(SCE_DMA_VIF1);
	p1->chcr.TTE = 1;

	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
	sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT,
		SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);

	db.clear0.rgbaq.R = 0x0;	/* clear color for dbuf0 */
	db.clear0.rgbaq.G = 0x80;
	db.clear0.rgbaq.B = 0x80;

	db.clear1.rgbaq.R = 0x0;	/* clear color for dbuf1 */
	db.clear1.rgbaq.G = 0x80;
	db.clear1.rgbaq.B = 0x80;

	FlushCache(0);

	sceVif1PkReset(&packet[sindex]);
	sceVif1PkCnt(&packet[sindex], 0);
	sceVif1PkOpenDirectCode(&packet[sindex], 0);
	sceVif1PkOpenGifTag(&packet[sindex], *(u_long128*)&giftagAD);

	/* add alpha environment packet */
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

	/* wait for Gs initialize packet end */
	sceGsSyncPath(0,0);

	/* make preset packet (double buffer on scratch pad ram) */
	for(sindex=0;sindex<2;sindex++) {
		sceVif1PkReset(&packet[sindex]);

		sceVif1PkCnt(&packet[sindex], 0);
		sceVif1PkOpenDirectCode(&packet[sindex], 0);
		sceVif1PkOpenGifTag(&packet[sindex], *(u_long128*)&giftag);

		sceVif1PkCloseGifTag(&packet[sindex]);
		sceVif1PkCloseDirectCode(&packet[sindex]);

		/* SCRATCH IN/OUT DMA BUG 回避 */
		BUG_WALKAROUND(&packet[sindex]);
		sceVif1PkEnd(&packet[sindex], 0);
		sceVif1PkTerminate(&packet[sindex]);
		}

	sindex = 0;		/* scrach packet double buffer index */
	while(!sceGsSyncV(0));	/* display next in odd field when interlace*/

	sceMcInit();

#if UseMultiTap
	ret = sceMtapInit();
	printf( "sceMtapInit : %d\n", ret );

	ret = sceMtapPortOpen(0);
	printf( "sceMtapPortOpen( %d ) : %d\n", 0, ret );
	ret = sceMtapPortOpen(2);
	printf( "sceMtapPortOpen( %d ) : %d\n", 2, ret );
	ret = sceMtapGetConnection(2);
	printf( "sceMtapGetConnection( %d ) : %d\n", 2, ret );

	sceGsSyncV( 0 );
	for(count=0; count < 2; ++count) {
		ret = sceMcGetSlotMax(count);
		printf( "max slot %d = %d\n", count, ret );
		}
#endif

	scePadInit(0);
	ret = scePadPortOpen(pdport, pdslot, padwork[pdslot]);
	printf(" port open =%d \n", ret);

	sceDevConsInit();
	console = sceDevConsOpen(OFFX + (8<<4), OFFY + ((4 +4)<<4), SizeConsX,30);
	printf(" cons init = %x \n", console);
	remapColorLut(console);


	ret = sceMcxInit();
	printf(" mcx init = %d \n", ret);

	frame = 0;		/* frame double buffer index */
	phase = 0;
	func = 0;
	exec = 0;
	oldbtn = 0;
	padbuff[0] = 1;

	sceDevConsClearBox(console, 0,0,SizeConsX,30);
	cgaDispTitle(console, 0);

	while(1) {
//		*T0_COUNT = 0;		/* Hcounter reset */
		sceGsSwapDBuff(&db, frame);
		++frame;

		if(frame > 20) {
			scePadRead(pdport, pdslot, padbuff);
			}
		if(!padbuff[0]) {
			curbtn = ~((padbuff[2]<<8)|padbuff[3]);
		}
		else {
			curbtn = oldbtn;
		}

		if(!exec) {
			if((curbtn ^ oldbtn) & curbtn) {
				func = menuMoveFunc(func,
					(curbtn ^ oldbtn) & curbtn);
				cgaDispTitle(console, func);
			}
			if((curbtn ^ oldbtn) & curbtn & SCE_PADstart) {
				/* 関数を スタートモードで呼び出す */
				cgaDispTitle(console, ~func);
				sceDevConsClearBox(console, 0,MaxVertTitle+1,
					SizeConsX,30- MaxVertTitle);
				exec = (*(menu[func].Func))(0, curbtn);
				if(!exec) {
					cgaDispTitle(console, func);
				}
			}			
			else {
				/* アイドリング中の１秒に１回はカードポートの
				   ステータスをチェックしに行く */
				if( !(frame % 60) ) {
					func = ~func;
					exec = mnIdleFunc(0, curbtn);
					if(!exec) {
						func = ~func;
					}
				}
			}
		}
		else {
			if(func >=0) {
				exec = (*(menu[func].Func))(1, curbtn);
				if(!exec) {
					cgaDispTitle(console, func);
				}
			}
			else {
				exec = mnIdleFunc(1, curbtn);
				/* アイドリングチェックが終ったら元に戻す */
				if(!exec) {
					func = ~func;
				}
			}
		}

		sceDevConsDraw(console);

		count = *T0_COUNT;
		if(!padbuff[0]) {
			oldbtn = ~((padbuff[2]<<8)|padbuff[3]);
		}

		if(frame&0x01) {	/* interrace half pixcel adjust */
			sceGsSetHalfOffset(&db.draw1,2048,2048,
			sceGsSyncV(0)^0x01);
		}
		else {
			sceGsSetHalfOffset(&db.draw0,2048,2048,
			sceGsSyncV(0)^0x01);
		}
		FlushCache(0);
		sceGsSyncPath(0,0);
	}

	return 0;
}



/* MEM->SCRATCH,SCRATCH->MEMのDMAを同時にキックする場合は SCRATCH->MEMの
   DMAの１パケットのサイズが必ず４の倍数QWORDになるように調整する */
void BUG_WALKAROUND(sceVif1Packet *pkt) {
	int dma_size,i;

	dma_size = ((u_int)pkt->pCurrent - (u_int)pkt->pBase - 0x10)/0x10;

	for(i=0;i<dma_size%4;i++) {
		sceVif1PkAddCode(pkt,SCE_VIF1_SET_NOP(0));
		sceVif1PkAddCode(pkt,SCE_VIF1_SET_NOP(0));
		sceVif1PkAddCode(pkt,SCE_VIF1_SET_NOP(0));
		sceVif1PkAddCode(pkt,SCE_VIF1_SET_NOP(0));
	}
}


/*#cgaDispTitle --- */
void cgaDispTitle(int console, int select) {
	int count;

	for(count=0; count< MaxExecTitle; ++count) {
		if(count== select) {
			sceDevConsAttribute(console, 4);
		}
		if( count == ~select) {
			sceDevConsAttribute(console, 5);
		}
		sceDevConsLocate(console,
			count/MaxVertTitle*16, count% MaxVertTitle);
		sceDevConsPrintf(console, "%2d.%s",count+1, menu[count].Title);

		sceDevConsAttribute(console, 7);
	}
	sceDevConsLocate(console, 0, MaxVertTitle);
	sceDevConsPrintf(console, "------------------------------------------------------------------------------");
}


/*#remapColorLut --- */
void remapColorLut(int console) {

	sceDevConsSetColor(console, 0, 0x00,0x00,0x00);		/* black */
	sceDevConsSetColor(console, 1, 0x80,0x90,0xFF);		/* blue */
	sceDevConsSetColor(console, 2, 0xFF,0x80,0x80);		/* red */
	sceDevConsSetColor(console, 3, 0xFF,0x60,0xFF);		/* gamenta */
	sceDevConsSetColor(console, 4, 0x60,0xFF,0x60);		/* green */
	sceDevConsSetColor(console, 5, 0x60,0xFF,0xFF);		/* cyan */
	sceDevConsSetColor(console, 6, 0xFF,0xFF,0x60);		/* yellow */
	sceDevConsSetColor(console, 7, 0xFF,0xFF,0xFF);		/* white */
}


/*#menuMoveFunc --- */
int menuMoveFunc(int cur, int button) {

	if(button & SCE_PADLup) {
		if(cur > 0) {
			--cur;
		}
	}
	else if(button & SCE_PADLdown) {
		if(cur < (MaxExecTitle -1)) {
			++cur;
		}
	}
	else if(button & SCE_PADLleft) {
		if(cur >= MaxVertTitle) {
			cur -= MaxVertTitle;
		}
	}
	else if(button & SCE_PADLright) {
		if( (cur+ MaxVertTitle) < MaxExecTitle) {
			cur += MaxVertTitle;
		}
	}

	return cur;
}


/*#mnIdleFunc --- */
int mnIdleFunc(int mode, int btn) {
	static int type, free, format, result, called;
	unsigned char *str;

	if(!mode || !called) {
		called = !sceMcGetInfo(mcport, mcslot, &type, &free, &format);
	}
	else {
		if(!sceMcSync(1, 0, &result)) {
			return 1;
		}

		called = 0;
printf(" get info res = %d  type %d  format %d \n", result, type, format);

		if(format) {
			str = "Formatted";
		}
		else {
			str = "No Format";
		}

		switch(result) {
		case 0:
			break;
		case -1:
		case -2:
			str = "Changed";
			break;
		default:
			str = "No Card";
			break;
		}
		sceDevConsLocate(console, LocDispStatus, 0);
		sceDevConsPrintf(console, "%-10s", str);

		if(result >= -2) {
			sceDevConsLocate(console, LocDispStatus, 1);
			sceDevConsPrintf(console, "type %4s",
				type!=2 ? "128K":"8M");

			sceDevConsLocate(console, LocDispStatus, 2);
			sceDevConsPrintf(console, "free %4d", free);
		}
		else {
			sceDevConsLocate(console, LocDispStatus, 1);
			sceDevConsPrintf(console, "         ");
			sceDevConsLocate(console, LocDispStatus, 2);
			sceDevConsPrintf(console, "         ");
		}
		return 0;
	}
	return 1;
}


#define MaxGetDir	1999

/*#mnGetDir --- */
int mnGetDir(int mode, int btn) {
	static sceMcTblGetDir table[MaxGetDir];
	static unsigned char path[1024];
	static int oldbtn=0;
	static int phase, ent, view, line;
	int ret, count, curbtn;
	static unsigned char result[80];

	/* 初期化モード */
	if(!mode) {
		oldbtn = btn;
		phase = 0;
		ent = 0;
		view = line = 0;
		*result = 0;
	}

	curbtn = btn & (oldbtn ^ btn);		/* up-edge だけを検出 */
	oldbtn = btn;

	ret = 0;
	if(curbtn & SCE_PADselect) {
		phase = 2;
		ret = -1;
		line = -1;
	}

	switch(phase) {
	case 0:
		ret = sceMcGetDir(mcport, mcslot, "./*", 0, MaxGetDir, table);
		sceMcSync(0, 0, &ret);
printf(" getdir = %d \n", ret);
		if(ret >=0) {
			ent = ret;
			++phase;
			view = line = 0;
			sceDevConsLocate(console, 0,MaxVertTitle+MaxDirView+5);
			sceDevConsPrintf(console, "total %d files found", ent);
		}
		break;
	case 1:
		ret = sceMcChdir(mcport, mcslot, ".", path);
		sceMcSync(0, 0, &ret);
		if(ret) {
			break;
		}

		ret = sceMcGetEntSpace(mcport, mcslot, ".");
		sceMcSync(0, 0, &ret);
		if(ret >= 0) {
			++phase;
			sceDevConsClearBox(console, 0, MaxVertTitle+2, 79, 1);
			sceDevConsLocate(console, 0, MaxVertTitle +2);
			sceDevConsPrintf(console, "dir(%d) - %s", ret, path);
		}
		break;
	case 3:
		if(curbtn & SCE_PADL1) {
			if(view > ScrlHight) {
				view -= ScrlHight;
				line += ScrlHight;
				if(line >= MaxDirView) {
					line = MaxDirView -1;
				}
			}
		}
		else if(curbtn & SCE_PADL2) {
			if( (view +MaxDirView) < ent) {
				view += ScrlHight;
				line -= ScrlHight;
				if(line < 0) {
					line = 0;
				}
			}
		}
		else if(curbtn & SCE_PADLup) {
			if(line) {
				--line;
			}
			else if(view > 0) {
				view -= 4;
				line += 3;
			}
		}
		else if(curbtn & SCE_PADLdown) {
			if(line < (MaxDirView -1) && (view + line) < (ent -1)){
				++line;
			}
			else if((view +MaxDirView) < ent) {
				view += 4;
				line -= 3;
			}
		}

		/* ボタンが押されて次のディレクトリに移動したときは case 2
		   を実行して redraw する */
		if( !(curbtn & (SCE_PADL1 | SCE_PADL2 | SCE_PADLup |
		SCE_PADLdown)) ){
			break;
		}
	case 2:
		if(phase==2) {
			++phase;
		}

		sceDevConsClearBox(console, 0, MaxVertTitle+3,
			80, MaxDirView+2);
		sceDevConsLocate(console, 0, MaxVertTitle+3);
		if(!view) {
			sceDevConsPrintf(console, "----------------------------------------------------\n");
		}
		else {
			sceDevConsPrintf(console, "-------------- more previous entries ---------------\n");
		}
		for(count=0; count<MaxDirView; ++count) {
			if((count + view) >= ent) {
				sceDevConsPrintf(console, "\n");
				continue;
			}
			if(count==line) {
				sceDevConsAttribute(console, 4);
			}

			sceDevConsPrintf(console,
				"%2d: %-20s   &7%7d byte  attr: %X \n",
				count +view +1,
				table[count +view].EntryName,
				table[count +view].FileSizeByte,
				table[count +view].AttrFile);
		}
		if((count+ view) >= ent) {
			sceDevConsPrintf(console, "----------------------------------------------------\n");
		}
		else {
			sceDevConsPrintf(console, "-------------- more entries afterwards -------------\n");
		}
		break;
	}

	if(ret <0){
		if(ret != -1) {
			dispError(ret);
		}
		return 0;
	}

	return 1;
}



typedef struct {
	char X, Y;
	char Type;
	int Min, Max;
} StatParam;

#define StatXs		5
#define StatYs		(MaxVertTitle + 2)


StatParam pdaStat[] = {
	{ 24, 0,  1,  0, 14, },
	{ 15, 1,  0,  0,  1, },
	{ 15, 2,  0,  0,  1, },
	{ 15, 3,  0,  0,  1, },
	{ 15, 4,  0,  0,  1, },
	{ 15, 5,  4,  0,  0, },
	{  7, 6,  3,  0,9999, },
	{ 12, 6,  2,  1, 12, },
	{ 15, 6,  2,  1, 31, },
	{ 19, 6,  5,  0,  6, },
	{ 24, 6,  1,  0, 23, },
	{ 27, 6,  2,  0, 59, },
	{ 30, 6,  2,  0, 59, },
};


StatParam uifsStat[] = {
	{ 10, 0,  1,  0, 23, },
	{ 13, 0,  2,  0, 59, },
	{ 16, 0,  7,  0,  1, },
	{ 10, 1,  7,  0,  1, },
	{ 10, 2,  8,  0,  2, },
	{ 10, 3,  9,  0,  2, },
	{ 10, 4,  10,  0,  1, },
	{ 10, 5,  6,  0,  0xFFFC, },
};



/*#dispPdaStat --- */
void dispPdaStat(unsigned value, int target, int select) {
	int type;

	if(target < 20) {
		sceDevConsLocate(console, StatXs + pdaStat[target].X,
			StatYs + pdaStat[target].Y);
	}
	else {
		sceDevConsLocate(console, StatXs + uifsStat[target -20].X,
			StatYs + uifsStat[target -20].Y);
	}
	if(select > 0) {
		sceDevConsPrintf(console, "&4");
	}
	else if(!select){
		sceDevConsPrintf(console, "&5");
	}

	if(target < 20) {
		type = pdaStat[target].Type;
	}
	else {
		type = uifsStat[target -20].Type;
	}

	switch(type) {
	case 0:		/* enable/disable */
		sceDevConsPrintf(console, "%s&7",
			(value & 1) ? "Disable":"Enable ");
		break;
	case 1:		/* 2 digit none zero number */
		sceDevConsPrintf(console, "%2d&7", value);
		break;
	case 2:		/* 2 digit zero suppless number */
		sceDevConsPrintf(console, "%02d&7", value);
		break;
	case 3:		/* 4 digit none zero number */
		sceDevConsPrintf(console, "%4d&7", value);
		break;
	case 4:		/* for serial number */
		sceDevConsPrintf(console, "&7%c%08d&7",
			value >> 24, value & 0xFFFFFF);
		break;
	case 5:		/* name of week */
		sceDevConsPrintf(console, "%s&7", week2asc[value]);
		break;
	case 6:		/* for font address 7 digit hex-decimal number */
		sceDevConsPrintf(console, "400%04X&7", value);
		break;
	case 7:		/* on/off */
		sceDevConsPrintf(console, "%s&7",
			(value & 1) ? "On ":"Off ");
		break;
	case 8:		/* volume */
		sceDevConsPrintf(console, "%s&7",
			value ? (value==2 ? "Mute":"Low "):"Loud");
		break;
	case 9:		/* area jp/us/eu */
		sceDevConsPrintf(console, "%s&7",
			value ? (value==2 ? "EU":"US"):"JP");
		break;
	case 10:	/* valid/unknown */
		sceDevConsPrintf(console, "%s&7",
			(value & 1) ? "Valid  ":"Not yet");
		break;
	}
}


/*#pickInfoVal --- */
unsigned pickInfoVal(sceMcxTblInfo *info, int target) {

	switch(target) {
	case 0:		return info->AppliNo;
	case 1:		return info->Speaker;
	case 2:		return info->Infrared;
	case 3:		return info->Flash;
	case 4:		return info->Led;
	case 5:		return info->Serial;
	case 6:		return info->PYear;
	case 7:		return info->PMonth;
	case 8:		return info->PDay;
	case 9:		return info->PWeek;
	case 10:	return info->PHour;
	case 11:	return info->PMin;
	case 12:	return info->PSec;
	}
	return 0;
}


/*#putInfoVal --- */
unsigned putInfoVal(sceMcxTblInfo *info, int target, int val) {

	switch(target) {
	case 0:  info->AppliNo = val;	return sceMcxBitAppli;
	case 1:  info->Speaker = val;	return sceMcxBitSpeaker;
	case 2:  info->Infrared = val;	return sceMcxBitInfrared;
	case 3:  info->Flash = val;	return sceMcxBitFlash;
	case 4:  info->Led = val;	return sceMcxBitLed;

	case 6:  info->PYear = val;	break;
	case 7:  info->PMonth = val;	break;
	case 8:  info->PDay = val;	break;
	case 9:  info->PWeek = val;	break;
	case 10: info->PHour = val;	break;
	case 11: info->PMin = val;	break;
	case 12: info->PSec = val;	break;
	}

	if(6 <= target && target <= 12) {
		return sceMcxBitDate;
	}
	return 0;
}


/*#mnGetInfo --- */
int mnGetInfo(int mode, int btn) {
	static int oldbtn, pos, ret;
	static sceMcxTblInfo info;
	static unsigned changed;
	int curbtn, val;
	int line, move;

	if(!mode) {
		line = StatYs;
		sceDevConsLocate(console, StatXs, line++);
		sceDevConsPrintf(console, "Running PDA application");
		sceDevConsLocate(console, StatXs, line++);
		sceDevConsPrintf(console, "Speaker");
		sceDevConsLocate(console, StatXs, line++);
		sceDevConsPrintf(console, "Infrared");
		sceDevConsLocate(console, StatXs, line++);
		sceDevConsPrintf(console, "Flash write");
		sceDevConsLocate(console, StatXs, line++);
		sceDevConsPrintf(console, "LED on");
		sceDevConsLocate(console, StatXs, line++);
		sceDevConsPrintf(console, "Serial No");
		sceDevConsLocate(console, StatXs, line++);
		sceDevConsPrintf(console, "Date       -  -   (   )   :  :");

		sceMcxGetInfo(mcport, mcslot, &info);
		sceMcxSync(0, 0, &ret);
		if(ret) {
			dispError(ret);
			return 0;
		}

		for(pos = 0; pos < 13; ++pos) {
			dispPdaStat(pickInfoVal(&info, pos), pos, !pos);
		}

		oldbtn = btn;
		changed = 0;
		pos = 0;
	}

	curbtn = btn & (oldbtn ^ btn);		/* up-edge だけを検出 */
	oldbtn = btn;
	move = -1;

	if(curbtn & SCE_PADstart) {
		move = -2;
		sceMcxSetInfo(mcport, mcslot, &info, changed);
		sceMcxSync(0, 0, &ret);
	}
	else if(curbtn & SCE_PADselect) {
		move = -2;
	}
	else if(curbtn & (SCE_PADLup | SCE_PADLleft)) {
		if((curbtn & SCE_PADLleft) && 6 < pos) {
			move = pos - 1;
			if(move == 9) {
				move = 8;
			}
		}
		else if(9 <= pos) {
			move = 6;
		}
		else if(5 <= pos) {
			move = 4;
		}
		else if(0 < pos) {
			move = pos -1;
		}
	}
	else if(curbtn & (SCE_PADLdown | SCE_PADLright)) {
		if((curbtn & SCE_PADLright) && 6 <= pos && pos < 12) {
			move = pos + 1;
			if(move == 9) {
				move = 10;
			}
		}
		else if(pos <= 3) {
			move = pos + 1;
		}
		else if(pos <= 5) {
			move = 6;
		}
		else if(pos <= 9) {
			move = 10;
		}
		else if(pos <= 11) {
			move = 12;
		}
	}

	if(move != -1) {
		dispPdaStat(pickInfoVal(&info, pos), pos, 0);

		if(move < -1) {
			return 0;
		}
		pos = move;
		dispPdaStat(pickInfoVal(&info, pos), pos, 1);
	}

	/* 設定値変更 */
	if(curbtn & (SCE_PADR1 | SCE_PADL1 | SCE_PADR2 | SCE_PADL2)) {
		val = pickInfoVal(&info, pos);

		if(curbtn & SCE_PADR1) {
			++val;
		}
		if(curbtn & SCE_PADL1) {
			val += 10;
		}
		if(curbtn & SCE_PADR2) {
			--val;
		}
		if(curbtn & SCE_PADL2) {
			val -= 10;
		}

		if(val < pdaStat[pos].Min) {
			val = pdaStat[pos].Min;
		}
		else if(val > pdaStat[pos].Max) {
			val = pdaStat[pos].Max;
		}

		if(val != pickInfoVal(&info, pos)) {
			changed |= putInfoVal(&info, pos, val);

			dispPdaStat(val, pos, 1);
		}
	}

	return 1;
}


/*#pickUifsVal --- */
unsigned pickUifsVal(sceMcxTblUifs *info, int target) {

	switch(target) {
	case 0:		return info->AHour;
	case 1:		return info->AMin;
	case 2:		return info->Alarm;
	case 3:		return info->KeyLock;
	case 4:		return info->Volume;
	case 5:		return info->AreaCode;
	case 6:		return info->RtcSet;
	case 7:		return (int)(info->Font);
	}
	return 0;
}


/*#putUifsVal --- */
void putUifsVal(sceMcxTblUifs *info, int target, int val) {

	switch(target) {
	case 0: info->AHour = val;	break;
	case 1: info->AMin = val;	break;
	case 2: info->Alarm = val;	break;
	case 3: info->KeyLock = val;	break;
	case 4: info->Volume = val;	break;
	case 6: info->RtcSet = val;	break;
	}
}


/*#mnEditUifs --- */
int mnEditUifs(int mode, int btn) {
	static int oldbtn, pos, ret, phase;
	static sceMcxTblUifs info;
	int curbtn, val;
	int line, move;

	if(!mode) {
		line = StatYs;
		sceDevConsLocate(console, StatXs, line++);
		sceDevConsPrintf(console, "Alarm       :   ");
		sceDevConsLocate(console, StatXs, line++);
		sceDevConsPrintf(console, "key lock");
		sceDevConsLocate(console, StatXs, line++);
		sceDevConsPrintf(console, "Volume");
		sceDevConsLocate(console, StatXs, line++);
		sceDevConsPrintf(console, "Area code");
		sceDevConsLocate(console, StatXs, line++);
		sceDevConsPrintf(console, "Rtc set");
		sceDevConsLocate(console, StatXs, line++);
		sceDevConsPrintf(console, "Font addr");

		phase = 0;
		if(ret) {
			dispError(ret);
			return 0;
		}

		oldbtn = btn;
		pos = 0;
	}

	if(phase < 3) {
		++phase;
		return 1;
	}
	else if(phase == 3) {
		++phase;
		sceMcxGetUifs(mcport, mcslot, &info);
		sceMcxSync(0, 0, &ret);
		if(ret) {
			dispError(ret);
			return 0;
		}
		for(pos = 0; pos < 8; ++pos) {
			dispPdaStat(pickUifsVal(&info, pos), pos+20,
				(pos==5 || pos==7) ? -1 : !pos);
		}
		pos = 0;
	}

	curbtn = btn & (oldbtn ^ btn);
	oldbtn = btn;
	move = -1;

	if(curbtn & SCE_PADstart) {
		move = -2;
		sceMcxSetUifs(mcport, mcslot, &info);
		sceMcxSync(0, 0, &ret);
	}
	else if(curbtn & SCE_PADselect) {
		move = -2;
	}
	else if(curbtn & (SCE_PADLup | SCE_PADLleft)) {
		if(pos == 6) {
			move = 4;
		}
		else if(pos > 0) {
			move = pos -1;
		}
	}
	else if(curbtn & (SCE_PADLdown | SCE_PADLright)) {
		if(pos < 4) {
			move = pos + 1;
		}
		else if(pos == 4) {
			move = 6;
		}
	}

	if(move != -1) {
		dispPdaStat(pickUifsVal(&info, pos), pos+20, 0);

		if(move < -1) {
			return 0;
		}
		pos = move;
		dispPdaStat(pickUifsVal(&info, pos), pos+20, 1);
	}

	/* 設定値変更 */
	if(curbtn & (SCE_PADR1 | SCE_PADL1 | SCE_PADR2 | SCE_PADL2)) {
		val = pickUifsVal(&info, pos);

		if(curbtn & SCE_PADR1) {
			++val;
		}
		if(curbtn & SCE_PADL1) {
			val += 10;
		}
		if(curbtn & SCE_PADR2) {
			--val;
		}
		if(curbtn & SCE_PADL2) {
			val -= 10;
		}

		if(val < pdaStat[pos].Min) {
			val = uifsStat[pos].Min;
		}
		else if(val > uifsStat[pos].Max) {
			val = uifsStat[pos].Max;
		}

		if(val != pickUifsVal(&info, pos)) {
			putUifsVal(&info, pos, val);
			dispPdaStat(val, pos +20, 1);
		}
	}

	return 1;
}


/*#mnDispTrans --- */
int mnDispTrans(int mode, int btn) {
	static int oldbtn;
	static int pos = 1, dir, time;
	int curbtn, ret;

	if(!mode) {
		oldbtn = btn;
		if(pos < 0) {
			pos += 10;
		}
	}

	curbtn = btn & (oldbtn ^ btn);
	oldbtn = btn;

	ret = 1;
	if(curbtn & SCE_PADstart) {
		if(!pos) {
			sceMcxHideTrans(mcport, mcslot);
		}
		else {
			sceMcxShowTrans(mcport, mcslot, dir, time);
		}

		sceMcxSync(0, 0, &ret);
		if(ret) {
			dispError(ret);
		}
		pos -= 10;
	}
	else if(curbtn & SCE_PADselect) {
		pos -= 10;
	}
	else if(curbtn & (SCE_PADLright | SCE_PADLup)) {
		pos = (pos + 1) % 3;
	}
	else if(curbtn & (SCE_PADLleft | SCE_PADLdown)) {
		pos = (pos + 2) % 3;
	}
	else if(pos && curbtn&(SCE_PADL1 | SCE_PADL2 | SCE_PADR1 | SCE_PADR2)){
		if(pos == 1) {
			dir = !dir;
		}
		else {
			if(curbtn & SCE_PADL1) {
				time += 10;
			}
			else if(curbtn & SCE_PADL2) {
				time -= 10;
			}
			else if(curbtn & SCE_PADR1) {
				time += 1;
			}
			else if(curbtn & SCE_PADR2) {
				time -= 1;
			}
			if(time < 0) {
				time = 0;
			}
			else if(time > 255) {
				time = 255;
			}
		}
	}

	sceDevConsLocate(console, StatXs, StatYs);
	sceDevConsPrintf(console, "[&%dstart&7]  dir: &%d%s&7-->&%d%s&7  time: &%d%3d&7",
		pos > 0 ? 4:7,
		pos==1 ? 4:7,  dir?"PS2":"PDA",
		pos==1 ? 4:7, !dir?"PS2":"PDA",
		pos==2 ? 4:7, time);
	sceDevConsLocate(console, StatXs, StatYs +2);
	sceDevConsPrintf(console, "[&%dstop&7]", !pos ? 4:7); 


	if(pos < 0) {
		return 0;
	}
	return 1;
}


/*#mnReadDev --- */
int mnReadDev(int mode, int btn) {
	static int oldbtn;
	static int part, pos, devno, parasize, contsize;
	int curbtn, val, update, ret;

	update = val = 0;
	if(!mode) {
		oldbtn = btn;
		part = 0;
		update = 1;
	}

	curbtn = btn & (oldbtn ^ btn);		/* up-edge だけを検出 */
	oldbtn = btn;

	if(curbtn & SCE_PADstart) {
		sceMcxReadDev(mcport, mcslot, devno, paramBuff, parasize,
			sectBuff, contsize);
		sceMcxSync(0, 0, &ret);
		if(ret) {
			dispError(ret);
			return 0;
		}

		update = 1;
	}
	else if(curbtn & SCE_PADselect) {
		part = -1;
		update = -1;
	}
	else if(curbtn & SCE_PADLup) {
		if(!part) {
			if(!parasize) {
				part = 2;
			}
			else {
				part = 3;
				if((parasize -1) % 0x10 >= pos) {
					pos = (parasize -1)/ 0x10 * 0x10 + pos;
				}
				else if(parasize >= 0x10) {
					pos = ((parasize -1)/ 0x10 -1)* 0x10 + pos;
				}
				else {
					pos = parasize -1;
				}
			}
		}
		else if(part < 3) {
			part = (part + 3) % 4;
		}
		else {
			if(pos > 0x10) {
				pos -= 0x10;
			}
			else {
				part = 2;
			}
		}
		update = 1;
	}
	else if(curbtn & SCE_PADLdown) {
		if(part < 2) {
			++part;
		}
		else if(part == 2) {
			if(parasize) {
				part = 3;
				if(pos >= parasize) {
					pos = parasize -1;
				}
			}
			else {
				part = 0;
			}
		}
		else if(part == 3) {
			if(pos + 0x10 < parasize) {
				pos += 0x10;
			}
			else {
				pos %= 0x10;
				part = 0;
			}
		}
		update = 1;
	}
	else if(curbtn & SCE_PADLright) {
		if(part < 2) {
			++part;
		}
		else if(part == 2) {
			if(parasize) {
				part = 3;
				pos = 0;
			}
			else {
				part = 0;
			}
		}
		else if(part == 3) {
			if(pos < parasize -1) {
				++pos;
			}
			else {
				pos %= 0x10;
				part = 0;
			}
		}
		update = 1;
	}
	else if(curbtn & SCE_PADLleft) {
		if(!part) {
			if(!parasize) {
				part = 2;
			}
			else {
				part = 3;
				pos = parasize -1;
			}
		}
		else if(part < 3) {
			part = (part + 3) % 4;
		}
		else {
			if(pos > 0) {
				--pos;
			}
			else {
				part = 2;
			}
		}
		update = 1;
	}
	else if(curbtn & (SCE_PADL1 |SCE_PADL2 |SCE_PADR1 |SCE_PADR2)){
		switch(part) {
		case 0:	val = devno;		break;
		case 1:	val = parasize;		break;
		case 2:	val = contsize;		break;
		case 3:	val = paramBuff[pos];	break;
		}

		if(curbtn & SCE_PADL1) {
			val += 0x10;
		}
		else if(curbtn & SCE_PADL2) {
			val -= 0x10;
		}
		else if(curbtn & SCE_PADR1) {
			++val;
		}
		else if(curbtn & SCE_PADR2) {
			--val;
		}

		if(part==1 || part==2) {
			val &= 0x7F;
		}
		else {
			val &= 0xFF;
		}

		switch(part) {
		case 0:	devno = val;		break;
		case 1:	parasize = val;		break;
		case 2:	contsize = val;		break;
		case 3:	paramBuff[pos] = val;	break;
		}
		update = 1;
	}

	if(update) {
		sceDevConsClearBox(console, 0, MaxVertTitle+1, 80,13);
		sceDevConsLocate(console, 0, MaxVertTitle+1);
		sceDevConsPrintf(console, "device No.:&%d%X&7  param size:&%d%X&7  data size:&%d%X&7   \n\n",
			!part ? 4:5, devno,
			part==1 ? 4:5, parasize,
			part==2 ? 4:5, contsize);

		dispPageCont(paramBuff, (unsigned char *)parasize,
			part == 3 ? pos:-1);
		dispPageCont(sectBuff, (unsigned char *)contsize, -1);

		if(update < 0) {
			return 0;
		}
		update = 0;
	}

	return 1;
}


/*#mnWriteDev --- */
int mnWriteDev(int mode, int btn) {
	static int oldbtn;
	static int part, pos, devno, parasize, contsize;
	int curbtn, val, update, ret;

	update = val = 0;
	if(!mode) {
		oldbtn = btn;
		part = 0;
		update = 1;
	}

	curbtn = btn & (oldbtn ^ btn);		/* up-edge だけを検出 */
	oldbtn = btn;

	if(curbtn & SCE_PADstart) {
		sceMcxWriteDev(mcport, mcslot, devno, paramBuff, parasize,
			sectBuff, contsize);
		sceMcxSync(0, 0, &ret);
		if(ret) {
			dispError(ret);
			return 0;
		}

		part = -1;
		update = -1;
	}
	else if(curbtn & SCE_PADselect) {
		part = -1;
		update = -1;
	}
	else if(curbtn & SCE_PADLup) {
		if(!part && !contsize) {
			part = 4;
		}

		if(!part) {
			part = 4;
			if((contsize -1) % 0x10 >= pos) {
				pos = (contsize -1)/ 0x10 * 0x10 + pos;
			}
			else if(contsize >= 0x10) {
				pos = ((contsize -1)/ 0x10 -1)* 0x10 + pos;
			}
			else {
				pos = contsize -1;
			}
		}
		else if(part == 4) {
			if(pos > 0x10) {
				pos -= 0x10;
			}
			else if(!parasize) {
				part = 2;
			}
			else {
				part = 3;
				if((parasize -1) % 0x10 >= pos) {
					pos = (parasize -1)/ 0x10 * 0x10 + pos;
				}
				else if(parasize >= 0x10) {
					pos = ((parasize -1)/ 0x10 -1)* 0x10 + pos;
				}
				else {
					pos = parasize -1;
				}
			}
		}
		else if(part < 3) {
			part = (part + 3) % 4;
		}
		else {
			if(pos > 0x10) {
				pos -= 0x10;
			}
			else {
				part = 2;
			}
		}
		update = 1;
	}
	else if(curbtn & SCE_PADLdown) {
		if(part == 2 && !parasize) {
			part = 3;
		}

		if(part < 2) {
			++part;
		}
		else if(part == 2) {
			part = 3;
			if(pos >= parasize) {
				pos = parasize -1;
			}
		}
		else if(part == 3) {
			if(pos + 0x10 < parasize) {
				pos += 0x10;
			}
			else {
				pos %= 0x10;
				part = 4;
				if(!contsize) {
					part = 0;
				}
			}
		}
		else if(part == 4) {
			if(pos + 0x10 < contsize) {
				pos += 0x10;
			}
			else {
				pos %= 0x10;
				part = 0;
			}
		}
		update = 1;
	}
	else if(curbtn & SCE_PADLright) {
		if(part < 2) {
			++part;
		}
		else if(part == 2) {
			if(parasize) {
				part = 3;
				pos = 0;
			}
			else if(contsize) {
				part = 4;
				pos = 0;
			}
			else {
				part = 0;
				pos %= 0x10;
			}
		}
		else if(part == 3) {
			if(pos < parasize -1) {
				++pos;
			}
			else if(contsize) {
				part = 4;
				pos = 0;
			}
			else {
				part = 0;
				pos %= 0x10;
			}
		}
		else if(part == 4) {
			if(pos < contsize -1) {
				++pos;
			}
			else {
				part = 0;
				pos %= 0x10;
			}
		}
		update = 1;
	}
	else if(curbtn & SCE_PADLleft) {
		if(!part) {
			if(contsize) {
				part = 4;
				pos = contsize -1;
			}
			else if(parasize) {
				part = 3;
				pos = parasize -1;
			}
			else {
				part = 2;
			}
		}
		else if(part < 3) {
			part = (part + 3) % 4;
		}
		else if(part == 3) {
			if(pos > 0) {
				--pos;
			}
			else {
				part = 2;
			}
		}
		else if(part == 4) {
			if(pos > 0) {
				--pos;
			}
			else if(parasize) {
				part = 3;
				pos = parasize -1;
			}
			else {
				part = 2;
			}
		}
		update = 1;
	}
	else if(curbtn & (SCE_PADL1 |SCE_PADL2 |SCE_PADR1 |SCE_PADR2)){
		switch(part) {
		case 0:	val = devno;		break;
		case 1:	val = parasize;		break;
		case 2:	val = contsize;		break;
		case 3:	val = paramBuff[pos];	break;
		case 4:	val = sectBuff[pos];	break;
		}

		if(curbtn & SCE_PADL1) {
			val += 0x10;
		}
		else if(curbtn & SCE_PADL2) {
			val -= 0x10;
		}
		else if(curbtn & SCE_PADR1) {
			++val;
		}
		else if(curbtn & SCE_PADR2) {
			--val;
		}

		if(part==1 || part==2) {
			val &= 0x7F;
		}
		else {
			val &= 0xFF;
		}

		switch(part) {
		case 0:	devno = val;		break;
		case 1:	parasize = val;		break;
		case 2:	contsize = val;		break;
		case 3:	paramBuff[pos] = val;	break;
		case 4:	sectBuff[pos] = val;	break;
		}
		update = 1;
	}
	/* all erase write data */
	else if(curbtn & SCE_PADRdown) {
		memset(sectBuff, 0, 128);
		update = 1;
	}

	if(update) {
		sceDevConsClearBox(console, 0, MaxVertTitle+1, 80,13);
		sceDevConsLocate(console, 0, MaxVertTitle+1);
		sceDevConsPrintf(console, "device No.:&%d%X&7  param size:&%d%X&7  data size:&%d%X&7   \n\n",
			!part ? 4:5, devno,
			part==1 ? 4:5, parasize,
			part==2 ? 4:5, contsize);

		dispPageCont(paramBuff, (unsigned char *)parasize,
			part == 3 ? pos:-1);
		dispPageCont(sectBuff, (unsigned char *)contsize,
			part == 4 ? pos:-1);

		if(update < 0) {
			return 0;
		}
		update = 0;
	}

	return 1;
}


/*#mnEditMem --- */
int mnEditMem(int mode, int btn) {
	static unsigned char pageinp[8]="0000000", origbuff[128];
	static int oldbtn;
	static int phase, page, curs = -1;
	int curbtn, ret;

	if(!mode) {
		oldbtn = btn;
		phase = 0;
		page = 0;

		sceDevConsLocate(console, 0, MaxVertTitle +2);
		sceDevConsPrintf(console, "offset = ");
		inpstr(pageinp, 7, 0, 2, btn);
	}

	curbtn = btn & (oldbtn ^ btn);
	oldbtn = btn;

	if(curbtn & SCE_PADselect) {
		if(phase) {
			phase = 0;
		}
		else {
			sceDevConsLocate(console, 9, MaxVertTitle +2);
			inpstr(pageinp, -7, -1, 2, btn);
			return 0;
		}
	}

	ret = 0;
	switch(phase) {
	case 0:
		sceDevConsLocate(console, 0,MaxVertTitle +2);
		sceDevConsPrintf(console, "offset = ");
		page = inpstr(pageinp, 7, -1, 2, btn);
		if(curbtn & SCE_PADstart) {
			sceMcxGetMem(mcport, mcslot, sectBuff, page, 0x80);
			sceMcxSync(0, 0, &ret);
			if(ret) {
				break;
			}
			curs = -1;
			memcpy(origbuff, sectBuff, 128);
			++phase;
		}
		break;
	case 2:
		if(curbtn & SCE_PADstart) {
			sceMcxSetMem(mcport,mcslot,sectBuff,page,0x80);
			sceMcxSync(0, 0, &ret);

			if(ret) {
				dispError(ret);
				break;
			}
			memcpy(origbuff, sectBuff, 128);
		}
		else if(curbtn & SCE_PADLup) {
			if(curs > 0x10) {
				curs -= 0x10;
			}
			else if(curs >= 0) {
				curs = ~curs;
			}
			else {
				curs = ~curs + 0x70;
			}
		}
		else if(curbtn & SCE_PADLdown) {
			if(curs < 0) {
				curs = ~curs;
			}
			else if(curs < 0x70) {
				curs += 0x10;
			}
			else {
				curs = ~(curs & 0xF);
			}
		}
		else if(curbtn & SCE_PADLright) {
			if(curs < 0) {
				curs = 0;
			}
			else if(curs < 0x7F) {
				++curs;
			}
			else {
				curs = ~0xF;
			}
		}
		else if(curbtn & SCE_PADLleft) {
			if(curs < 0) {
				curs = 0x7F;
			}
			else {
				--curs;
			}
		}
		else if(curbtn & (SCE_PADL1 |SCE_PADL2 |SCE_PADR1 |SCE_PADR2)){
			if(curs < 0) {
				if(curbtn & SCE_PADL1) {
					page += 0x800;
				}
				else if(curbtn & SCE_PADL2) {
					page -= 0x800;
				}
				else if(curbtn & SCE_PADR1) {
					page += 0x80;
				}
				else if(curbtn & SCE_PADR2) {
					page -= 0x80;
				}
				page &= 0xFFFFFFF;

				sceMcxGetMem(mcport,mcslot,sectBuff,page,0x80);
				sceMcxSync(0, 0, &ret);

				/* 読み込みエラー無視 */
				ret = 0;
				if(ret) {
					break;
				}
				memcpy(origbuff, sectBuff, 128);
			}
			/* edit 状態に入っているとき */
			else {
				if(curbtn & SCE_PADL1) {
					sectBuff[curs] += 0x10;
				}
				else if(curbtn & SCE_PADL2) {
					sectBuff[curs] -= 0x10;
				}
				else if(curbtn & SCE_PADR1) {
					++sectBuff[curs];
				}
				else if(curbtn & SCE_PADR2) {
					--sectBuff[curs];
				}
				sectBuff[curs] &= 0xFF;
			}
		}
		else {
			break;
		}
	case 1:
		if(phase==1) {
			++phase;
		}

		sceDevConsClearBox(console, 0, MaxVertTitle+1, 80,13);
		sceDevConsLocate(console, 0, MaxVertTitle+1);
		sceDevConsPrintf(console, "current offset &%d%X&7",
			curs < 0 ? 4:5, page);
		if(page >> 24 == 8) {
			sceDevConsPrintf(console, " (card sector &%d%X&7)",
				curs < 0 ? 4:5, (page & 0x1FFFF) / 0x80);
		}
		sceDevConsPrintf(console, "\n\n");

		dispPageCont(sectBuff, origbuff, curs);
		break;
	}

	if(ret < 0) {
		dispError(ret);
		return 0;
	}
	return 1;
}


/*#mnSetLed --- */
int mnSetLed(int mode, int btn) {
	static int oldbtn;
	static int light;
	int curbtn, ret;

	if(!mode) {
		oldbtn = btn;
		light = !light;
	}

	curbtn = btn & (oldbtn ^ btn);
	oldbtn = btn;

	sceDevConsLocate(console, StatXs, StatYs +2);

	if(curbtn & SCE_PADstart) {
		sceMcxSetLed(mcport, mcslot, light);
		sceMcxSync(0, 0, &ret);
		if(ret) {
			dispError(ret);
		}
		sceDevConsPrintf(console, "[&%don&7/&%doff&7]",
			light? 5:0, !light? 5:0);
		return 0;
	}
	else if(curbtn & SCE_PADselect) {
		sceDevConsPrintf(console, "[&%don&7/&%doff&7]",
			light? 5:0, !light? 5:0);
		return 0;
	}
	else if(curbtn & (SCE_PADLleft | SCE_PADLright)) {
		light = !light;
	}

	sceDevConsPrintf(console, "[&%don&7/&%doff&7]",light? 4:0,!light? 4:0);

	return 1;
}


/* ----------------------- */
/* commonly used functions */
/* ----------------------- */


/*#dispError --- */
void dispError(int ret) {

	sceDevConsAttribute(console, 6);
	switch(ret) {
	case -2:
		sceDevConsMessage(console,15,10," No Format ");
		break;
	case -3:
		sceDevConsMessage(console,15,10," Memory Card Full ");
		break;
	case -4:
		sceDevConsMessage(console,15,10," Duplicated Entry ");
		break;
	case -5:
		sceDevConsMessage(console,15,10," Closed file handler ");
		break;
	default:
		sceDevConsMessage(console, 15,10, "No Card");
	}
	sceDevConsAttribute(console, 7);
}


/*#dispPageCont --- */
void dispPageCont(unsigned char *buff, unsigned char *orig, int curs) {
	int count, pos, color;
	unsigned char data;

	sceDevConsPrintf(console, "offs +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F 0123456789ABCDEF\n");
	sceDevConsPrintf(console, "---------------------------------------------------------------------\n");

	for(count=0; count<8; ++count) {
		sceDevConsPrintf(console, "%03X: ", count*16);
		for(pos=0; pos<16; ++pos) {
			color = 7;
			if(curs == (count * 16 + pos)) {
				color = 4;
			}
			else if((unsigned)orig < 128) {
				if((count * 16 + pos) < (unsigned)orig) {
					color = 5;
				}
				else {
					color = 0;
				}
			}
			else if(buff[count * 16 +pos] !=orig[count * 16 +pos]){
				color = 6;
			}
			else {
				color = 5;
			}

			sceDevConsPrintf(console, "&%d%02X&7 ",
				color, buff[count*16+pos]);
		}
		for(pos=0; pos<16; ++pos) {
			color = 7;
			if(curs == (count * 16 + pos)) {
				color = 4;
			}
			else if((unsigned)orig < 128) {
				if((count * 16 + pos) < (unsigned)orig) {
					color = 5;
				}
				else {
					color = 0;
				}
			}
			else if(buff[count * 16 +pos] !=orig[count * 16 +pos]){
				color = 6;
			}
			else {
				color = 5;
			}

			data = buff[count*16+pos];
			if(data < ' ' || 0x7F<= data) {
				data = '.';
			}
			sceDevConsPrintf(console, "&%d%c&7", color, data);
		}
		sceDevConsPrintf(console, "\n");
	}
}


/* #inpstr --- pos:>0 カーソル位置初期化、<0 前回の位置と同じ
	入力最大文字数が maxlen なので文字配列は maxlen +1 以上必要
	maxlen < 0 の場合は終了処理で unselect 色で表示するだけ
	digit --- 0:文字列入力、1:10進入力後、10進値を返す、2:16進数入力
		3:boolean入力 2進数としての値を返す */
int inpstr(unsigned char *buff, int maxlen, int pos, int digit, int btn) {
	static int curs, oldbtn;
	int count, curbtn, ret;

	/* 入力確定後にアンセレクト色で描画し直すとき */
	if(maxlen < 0) {
		sceDevConsAttribute(console, 5);
		sceDevConsPrintf(console, buff);
		sceDevConsAttribute(console, 7);
		return 0;
	}

	/* カーソルの初期位置を設定するとき */
	if(pos >=0) {
		oldbtn = btn;
		curs = pos;
	}

	curbtn = btn & (oldbtn ^ btn);		/* up-edge だけを検出 */
	oldbtn = btn;

	if(curbtn & SCE_PADLleft) {
		--curs;
	}
	else if(curbtn & SCE_PADLright) {
		++curs;
	}

	if(curs < 0) {
		curs = 0;
	}
	else if(curs >= maxlen) {
		curs = maxlen -1;
	}
	if(curs > strlen(buff)) {
		curs = strlen(buff);
	}

	buff[maxlen] = 0;

	if(curbtn & (SCE_PADLup | SCE_PADLdown | SCE_PADRup | SCE_PADRleft | SCE_PADRright) && !buff[curs]) {
		buff[curs +1] = 0;
	}
	/* 10進数入力モード */
	if(digit == 1) {
		if(curbtn & SCE_PADLup) {
			++buff[curs];
			if(!isdigit(buff[curs])) {
				buff[curs] = '0';
			}
		}
		if(curbtn & SCE_PADLdown) {
			--buff[curs];
			if(!isdigit(buff[curs])) {
				buff[curs] = '9';
			}
		}
	}
	/* 16進数入力モード */
	else if(digit == 2) {
		if(curbtn & SCE_PADLup) {
			++buff[curs];
			if(buff[curs]==('9'+1)) {
				buff[curs] = 'A';
			}
			if(!isxdigit(buff[curs])) {
				buff[curs] = '0';
			}
		}
		if(curbtn & SCE_PADLdown) {
			--buff[curs];
			if(buff[curs]==('A'-1)) {
				buff[curs] = '9';
			}
			if(!isxdigit(buff[curs])) {
				buff[curs] = 'F';
			}
		}
	}
	/* ○／× 入力モード */
	else if(digit == 3) {
		if(curbtn & (SCE_PADLup | SCE_PADLdown)) {
			if(buff[curs]=='o') {
				buff[curs] = 'x';
			}
			else {
				buff[curs] = 'o';
			}
		}
	}
	else {
		/* 41[A-Z]5A --> 61[a-z]7A --> 30[0123456789:;<=>?@]40
		-->20[ !"#$%&'()*+,-./]2F --> 5B[[\]^_`]60 --> 7B[{|}~]7E */
		if(curbtn & SCE_PADLup) {
			++buff[curs];
			switch(buff[curs]-1) {
			case 0x5A:	/* 大文字の終りなら小文字に */
				buff[curs] = 0x61; break;
			case 0x7A:	/* 小文字の終りなら数字に */
				buff[curs] = 0x30; break;
			case 0x40:	/* @ の次は 空白 */
				buff[curs] = 0x20; break;
			case 0x2F:	/* / の次は [ */
				buff[curs] = 0x5B; break;
			case 0x60:	/* ` の次は { */
				buff[curs] = 0x7B; break;
			case 0x7E:	/* ~ の次は 大文字 */
				buff[curs] = 0x41; break;
			}
			if(buff[curs] < 0x20 || buff[curs] >= 0x7F) {
				buff[curs] = 0x30;
			}
		}
		/* 41[A-Z]5A <-- 61[a-z]7A <-- 30[0123456789:;<=>?@]40
		<--20[ !"#$%&'()*+,-./]2F <-- 5B[[\]^_`]60 <-- 7B[{|}~]7E */
		else if(curbtn & SCE_PADLdown) {
			--buff[curs];
			switch(buff[curs]+1) {
			case 0x61:	/* 小文字の終りなら大文字に */
				buff[curs] = 0x5A; break;
			case 0x30:	/* 数文字の終りなら小字に */
				buff[curs] = 0x7A; break;
			case 0x20:	/* 空白の次は @ */
				buff[curs] = 0x40; break;
			case 0x5B:	/* [ の次は / */
				buff[curs] = 0x2F; break;
			case 0x7B:	/* { の次は ` */
				buff[curs] = 0x60; break;
			case 0x41:	/* 大文字 の次は ~ */
				buff[curs] = 0x7E; break;
			}
			if(buff[curs] < 0x20 || buff[curs] >= 0x7F) {
				buff[curs] = 'z';
			}
		}
		/* □ボタンで強制的に 0 にする */
		if(curbtn & SCE_PADRleft) {
			buff[curs] = 0x30;
		}
		/* △ボタンで強制的に A にする */
		else if(curbtn & SCE_PADRup) {
			buff[curs] = 0x41;
		}
		/* ○ボタンで強制的に a にする */
		else if(curbtn & SCE_PADRright) {
			buff[curs] = 0x61;
		}
	}

	/* 文字列入力のときのみ ×ボタンでバックスペース */
	if(curbtn & SCE_PADRdown && curs && !digit) {
		--curs;
		for(count=curs; count < maxlen; ++count) {
			buff[count] = buff[count+1];
		}
	}
	/* 文字列入力のときのみ R1ボタンで空白をインサート */
	else if(curbtn & SCE_PADR1 && !digit) {
		for(count= maxlen -1; count > curs; --count) {
			buff[count] = buff[count -1];
		}
		buff[maxlen] = 0;
		buff[curs] = ' ';
	}

	/* \0 の部分は黒色のアンダーバーで表示 */
	for(count=0; count < maxlen; ++count) {
		if(count==curs && ((frame % 30) < 10)) {
			sceDevConsPut(console, '_', 7);
		}
		else if(count < strlen(buff)) {
			sceDevConsPut(console, buff[count], 4);
		}
		else {
			sceDevConsPut(console, '_', 0);
		}
	}

	sceDevConsAttribute(console, 7);

	/* 要求されている形に変換して返す */
	if(digit == 1) {
		return atoi(buff);
	}
	else if(digit == 2) {
		return atox(buff);
	}
	else if(digit == 3) {
		ret = 0;
		for(count = 0; count < maxlen; ++count) {
			ret = (ret << 1) | (buff[count] != 'x');
		}
		return ret;
	}
	return 0;
}


/*#atox --- */
unsigned atox(unsigned char *data) {
	unsigned ret;

	ret = 0;
	while(isxdigit(*data)) {
		if(isdigit(*data)) {
			ret = ret * 16 + *data - '0';
		}
		else {
			ret = ret * 16 + toupper(*data) - 'A' +10;
		}
		++data;
	}

	return ret;
}


/*#loadIopModule --- */
void loadIopModule() {

	sceSifInitRpc(0);

	sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx", 0,0);
#if UseMultiTap
	sceSifLoadModule("host0:/usr/local/sce/iop/modules/mtapman.irx", 0,0);
#endif

	sceSifLoadModule("host0:/usr/local/sce/iop/modules/mcman.irx", 0, 0);
	sceSifLoadModule("host0:/usr/local/sce/iop/modules/mcserv.irx", 0, 0);
	sceSifLoadModule("host0:/usr/local/sce/iop/modules/mcxman.irx", 0, 0);
	sceSifLoadModule("host0:/usr/local/sce/iop/modules/mcxserv.irx", 0, 0);

	sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx", 0,0);
}

