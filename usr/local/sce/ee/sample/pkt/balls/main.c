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

#define SUNIT	   0x08		/* プリセットするボールの数*/
#define PACKETSIZE (0x100*SUNIT)
#define WORKSIZE   (0x80)
#define WORKBASE   0x70000000
#define SUPPER     (WORKBASE+WORKSIZE)
#define SLOWER     (WORKBASE+WORKSIZE+PACKETSIZE)

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 224
#define OFFX (((4096-SCREEN_WIDTH)/2)<<4)
#define OFFY (((4096-SCREEN_HEIGHT)/2)<<4)
#define WALL_X		((SCREEN_WIDTH-16)<<4)	/* reflection point */
#define WALL_Y		((SCREEN_HEIGHT-8)<<4)
#define MAXOBJ (2500*SUNIT)	/* 表示するボールの数の最大 */
#define STRING_SIZE 32		/* 表示する文字数の最大*/


sceGsDBuff db;
void init_config_system(void);
extern int texture_image[];

/*
 * スプライトの動きに関するバッファ
 */
typedef struct {
	u_short x, y;		/* 現在の位置 */
        u_short z, pad;
	u_short dx, dy;		/* 速度 */
        u_short pad1,pad2;
} POS;

typedef struct {
    u_long prim,rgbaq,tex0;
    u_long uv1,xyzf1;
    u_long uv2,xyzf2;
    u_long uv3,xyzf3;
    u_long uv4,xyzf4;
    u_long nop;
} SPRITE_TAG;


/* ボールのスタート位置と移動距離の設定 */
void  init_point(POS *pos,int n);
void BUG_WALKAROUND(sceVif1Packet *);

u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

int main(int argc, char* argv[]){
    u_int fr;			/* フレームカウンタ */
    sceVif1Packet packet[2];	/* DMAパケットハンドラ */
    int x,y,w,h,count,Uoffset;
    sceDmaEnv  env;		/* DMA 共通環境 */
    volatile sceDmaChan *sfrom;
    sceDmaChan *p1;
    POS *pp,*pos;
    int i,sindex,j,nobj=1;
    u_long giftag[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 1, 12),
			 0x0000f43434343610L };	/* スプライト用GIFタグ */
    u_long giftagAD[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1),
			 0x000000000000000eL };	/* A＋D用GIFタグ */
    float fog = 1.0f;
    SPRITE_TAG *sprite;		/* プリセットパケット構造体 */

    u_int paddata;
    u_char rdata[32];

    char string_buf[32];	/* 文字表示おためのストリングバッファ */
    u_long128 *print_buf;	/* 文字表示のためのDMAパケットバッファ */

    sceSifInitRpc(0);
    while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
			0, NULL) < 0){
	printf("Can't load module sio2man\n");
    }
    while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
			0, NULL) < 0){
	printf("Can't load module padman\n");
    }

    init_config_system();

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
    /* DMA toSPR チャンネルディスクリプタ取得 */
    sfrom = (volatile sceDmaChan *)sceDmaGetChan(SCE_DMA_toSPR);

    /* ball attribute buffer */
    pos = (POS *)malloc(sizeof(POS)*MAXOBJ);
    /* font display buffer フォントはドットによって表示されるため
       １文字128 qword 必要 */
    print_buf = (u_long128 *)(0x20000000 | (u_int)malloc(16*16/2*16*STRING_SIZE)); /* uncached area */

    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT, SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);

    init_point(pos,MAXOBJ);	/* initialize ball attribute buffer */

    db.clear0.rgbaq.R = 0x0;	/* clear color for dbuf0 */
    db.clear0.rgbaq.G = 0x80;
    db.clear0.rgbaq.B = 0x80;

    db.clear1.rgbaq.R = 0x0;	/* clear color for dbuf1 */
    db.clear1.rgbaq.G = 0x80;
    db.clear1.rgbaq.B = 0x80;

    FlushCache(0);		/* update memory */

    sceVif1PkReset(&packet[sindex]); /* Gs initialize packet */
				/* add load image packet */
    sceVif1PkRefLoadImage(&packet[sindex],6720,SCE_GS_PSMCT32,32*8/64,
			  (u_long128 *)texture_image,32*8*32/4,0,0,32*8,32);
    sceVif1PkCnt(&packet[sindex], 0);
    sceVif1PkOpenDirectCode(&packet[sindex], 0);
    sceVif1PkOpenGifTag(&packet[sindex], *(u_long128*)&giftagAD);
    sceVif1PkReserve(&packet[sindex],sceGsSetDefAlphaEnv((sceGsAlphaEnv *)packet[sindex].pCurrent, 0) * 4); /* add alpha environment packet */
    sceVif1PkCloseGifTag(&packet[sindex]);
    sceVif1PkCloseDirectCode(&packet[sindex]);
    sceVif1PkEnd(&packet[sindex], 0);
    sceVif1PkTerminate(&packet[sindex]);

    sceDmaSend(p1,(u_int *)(((u_int)packet[sindex].pBase&0x0fffffff)|0x80000000)); /* kick Gs initialize packet */
    sceGsSyncPath(0,0);		/* wait for Gs initialize packet end */
    /* make preset packet (double buffer on scratch pad ram) */
    for(sindex=0;sindex<2;sindex++) {
	sceVif1PkReset(&packet[sindex]);

	sceVif1PkCnt(&packet[sindex], 0);
	sceVif1PkOpenDirectCode(&packet[sindex], 0);
	sceVif1PkOpenGifTag(&packet[sindex], *(u_long128*)&giftag);
	for(i=0;i<SUNIT;i++) {
	    x = 100<<4;
	    y = 100<<4;
	    w = 32<<4;
	    h = 16<<4;
	    Uoffset = i*32;
	    sceVif1PkAddGsData(&packet[sindex],
			     SCE_GS_SET_PRIM(0x04,0,1,0,1,0,1,0,0));
	    sceVif1PkAddGsData(&packet[sindex],
			     SCE_GS_SET_RGBAQ(0x80,0x80,0x80,0x80,
					      *(u_int *)&fog));
	    sceVif1PkAddGsData(&packet[sindex],
			     SCE_GS_SET_TEX0(6720,4,SCE_GS_PSMCT32,8,5,1,
					     0,0,0,0,0,0));
	    sceVif1PkAddGsData(&packet[sindex],
			     SCE_GS_SET_UV((Uoffset<<4)+0,0));
	    sceVif1PkAddGsData(&packet[sindex],
			     SCE_GS_SET_XYZ(OFFX+x,OFFY+y,1));
	    sceVif1PkAddGsData(&packet[sindex],
			     SCE_GS_SET_UV((Uoffset+32)<<4,0));
	    sceVif1PkAddGsData(&packet[sindex],
			     SCE_GS_SET_XYZ(OFFX+x+w,OFFY+y,1));
	    sceVif1PkAddGsData(&packet[sindex],
			     SCE_GS_SET_UV(Uoffset<<4,32<<4));
	    sceVif1PkAddGsData(&packet[sindex],
			     SCE_GS_SET_XYZ(OFFX+x,OFFY+y+h,1));
	    sceVif1PkAddGsData(&packet[sindex],
			     SCE_GS_SET_UV((Uoffset+32)<<4,32<<4));
	    sceVif1PkAddGsData(&packet[sindex],
			     SCE_GS_SET_XYZ(OFFX+x+w,OFFY+y+h,1));
	    sceVif1PkAddGsData(&packet[sindex],0x00000000);
	}
	sceVif1PkCloseGifTag(&packet[sindex]);
	sceVif1PkCloseDirectCode(&packet[sindex]);
	BUG_WALKAROUND(&packet[sindex]); /* SCRATCH IN/OUT DMA BUG 回避 */
	sceVif1PkEnd(&packet[sindex], 0);
	sceVif1PkTerminate(&packet[sindex]);
    }
    sindex = 0;			/* scrach packet double buffer index */
    fr     = 0;			/* frame double buffer index */
    while(sceGsSyncV(0)==0); /* display next in odd field when interlace*/
    scePadInit(0);
    scePadPortOpen(0, 0, pad_dma_buf);
    
    do{
	*T0_COUNT = 0;		/* Hcounter reset */
	sceGsSwapDBuff(&db, fr);
	sfrom->madr = (u_int *)pos;

	if(scePadRead(0, 0, rdata) > 0){
	    paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
	}
	else{
	    paddata = 0;
	}

	if ((paddata&SCE_PADLup) && nobj<MAXOBJ/SUNIT) {
	    nobj++;
	}else if ((paddata&SCE_PADLdown) && nobj>1) {
	    nobj--;
	}

	for(j=0;j<nobj;j++) {
	    sceVif1PkReset(&packet[sindex]);

	    /* ball attributeをスクラッチメモリへ持ってくる */
	    sfrom->sadr = (u_int *)(WORKBASE&0x0fffffff);
	    sfrom->qwc  = (sizeof(POS)*SUNIT)/0x10;
	    sfrom->chcr.STR = 0x01; /* START */
	    pp = (POS *)WORKBASE;
	    sceVif1PkReserve(&packet[sindex],2 * 4); /* skip DMA/GIF TAG */
	    sceDmaSync((sceDmaChan *)sfrom,0,0);

	    sprite = (SPRITE_TAG *)packet[sindex].pCurrent;

	    for(i=0;i<SUNIT;i++) {
		if ((u_int)(x = (pp->x + pp->dx*fr) % (WALL_X*2)) >= WALL_X)
		    x = WALL_X*2 - x;
		if ((y = (pp->y + pp->dy*fr) % (WALL_Y*2)) >= WALL_Y)
		    y = WALL_Y*2 - y;
		pp++;

		w = 32<<4;
		h = 16<<4;

		/* touch geometory information on template packet */
		sprite->xyzf1 = SCE_GS_SET_XYZ(OFFX+x,OFFY+y,1);
		sprite->xyzf2 = SCE_GS_SET_XYZ(OFFX+x+w,OFFY+y,1);
		sprite->xyzf3 = SCE_GS_SET_XYZ(OFFX+x,OFFY+y+h,1);
		sprite->xyzf4 = SCE_GS_SET_XYZ(OFFX+x+w,OFFY+y+h,1);
		sprite++;
	    }

	    /*       sceGsSyncPath(0,0); wait for drawing */
	    /*       sceDmaSyncN(0); not impliment yet */
	    /*      sceDmaSyncN(0) のコードの代り */
	    {
		asm(" AAA:");
		asm("       bc0f AAA");
		asm("       nop");
		*D_STAT = 0x3ff;	/* CLEAR CIS */
	    }

	    sceDmaSend(p1,(u_int *)(((u_int)packet[sindex].pBase&0x0fffffff)|0x80000000));
	    sindex ^= 0x01;
	}
	fr++;

	count = *T0_COUNT;

	sprintf(string_buf,"BALLS = %d (H time %d)",nobj*8,count);
	/* print_buf上にFontImageを作成するためのDMAパケットを作る */
	sceDevFont(print_buf,OFFX+(8<<4),OFFY+(8<<4),0xffffffff,string_buf,0);
	sceGsSyncPath(0,0);
	sceDmaSend(p1,(u_int *)((u_int)print_buf&0x0fffffff));

	if(fr&0x01)		/* interrace half pixcel adjust */
	    sceGsSetHalfOffset(&db.draw1,2048,2048,sceGsSyncV(0)^0x01);
	else
	    sceGsSetHalfOffset(&db.draw0,2048,2048,sceGsSyncV(0)^0x01);
	FlushCache(0);
	sceGsSyncPath(0,0);
    }while(1);
    free(pos);
    free(print_buf);
    return 0;
}


void init_config_system(void)
{
    sceDevVif0Reset();
    sceDevVu0Reset();
    sceGsResetPath();
    sceDmaReset(1);
}

/* attribute intialize */
void init_point(POS *pos , int num)
{
	int	i;

	for (i = 0; i < num; i++) {
	  pos->x  = rand();		/* スタート座標 Ｘ */
	  pos->y  = rand();		/* スタート座標 Ｙ */
	  pos->z  = 0x02<<4;
	  pos->dx = (rand()%64) + 1;	/* 移動距離 Ｘ (1<=x<=4) */
	  pos->dy = (rand()%32) + 1;	/* 移動距離 Ｙ (1<=y<=4) */
	  pos++;
	}
}

/*
 * MEM->SCRATCH,SCRATCH->MEMのDMAを同時にキックする場合は
 * SCRATCH->MEMのDMAの１パケットのサイズが必ず４の倍数QWORDに
 * なるように調整する
 */
void BUG_WALKAROUND(sceVif1Packet *pkt)
{
    int dma_size,i;

    dma_size = ((u_int)pkt->pCurrent - (u_int)pkt->pBase - 0x10)/0x10;

    for(i=0;i<dma_size%4;i++)
	{
	    sceVif1PkAddCode(pkt,SCE_VIF1_SET_NOP(0));
	    sceVif1PkAddCode(pkt,SCE_VIF1_SET_NOP(0));
	    sceVif1PkAddCode(pkt,SCE_VIF1_SET_NOP(0));
	    sceVif1PkAddCode(pkt,SCE_VIF1_SET_NOP(0));
	}
}

