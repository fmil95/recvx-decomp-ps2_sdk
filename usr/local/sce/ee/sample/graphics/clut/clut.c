/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - clut -
 *
 *                         Version 0.3.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : clut.c
 *                     Description : main procedures
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.3.0           Jun,11,1999     munekis    1st version
 */


#include <eekernel.h>
#include <eeregs.h>
#include <eestruct.h>
#include <libgraph.h>
#include <libdma.h>


extern u_int My_clut8[];
extern u_int My_indeximg8[];
extern u_int My_clut4[];
extern u_int My_indeximg4[];


sceGsDBuff db;

typedef union {
	u_long128	ul128;
	u_long		ul64[2];
	u_int		ui32[4];
} Q_WORDDATA;

Q_WORDDATA packet_buf[1024];
sceDmaChan *d2;

sceGsLoadImage cp, tp;

void init_packet(Q_WORDDATA *pbuf);
void make_packet1(Q_WORDDATA *pbuf);
void make_packet2(Q_WORDDATA *pbuf);
void init_clut_index(void);
void update_clut(int);
void tex_load(void);

#define INDEXIMG_ADDRESS_8 6720
#define CLUT_ADDRESS_8     6976
#define INDEXIMG_ADDRESS_4 6980
#define CLUT_ADDRESS_4     7108

int main() {

	short odev;
	int frame;

	sceDmaReset(1);
	sceGsResetPath();
	d2 = sceDmaGetChan(2);

	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
	sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224, 
		SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
        *(u_long *)&db.clear0.rgbaq =
                SCE_GS_SET_RGBAQ(0x10, 0x10, 0xa0, 0x80, 0x3f800000);
        *(u_long *)&db.clear1.rgbaq =
                SCE_GS_SET_RGBAQ(0x10, 0x10, 0xa0, 0x80, 0x3f800000);

	tex_load();

	frame = 0;

	init_packet(packet_buf);
        init_clut_index();

	while(1){

                // update clut table and send them to GS.
                update_clut(frame);

		odev = !sceGsSyncV(0);
		sceGsSetHalfOffset((frame&1)?(&db.draw1):(&db.draw0),
			2048, 2048, odev);
		FlushCache(WRITEBACK_DCACHE);
		sceGsSwapDBuff(&db, frame);

		make_packet1(packet_buf);
		while(DGET_D2_CHCR() & 0x0100);

		// --- draw left rectangle(8bits indexed texture mapped) ----
		FlushCache(WRITEBACK_DCACHE);
		sceDmaSend(d2, packet_buf);
		while(DGET_D2_CHCR() & 0x0100);

		// --- draw right rectangle(4bits indexed texture mapped) ----
                make_packet2(packet_buf);
		FlushCache(WRITEBACK_DCACHE);
		sceDmaSend(d2, packet_buf);

		frame++;
		sceGsSyncPath(0, 0);
	}

}

unsigned char index8[256];
void init_clut_index(void) {

        u_int i, j, idx;
        idx = 0;
        for(i = 0; i < 256; i+=32) {
                for(j = i; j < i+8; j++) 
	                index8[idx++] = (unsigned char)j;
                for(j = i+16; j < i+16+8; j++) 
	                index8[idx++] = (unsigned char)j;
                for(j = i+8;  j < i+8+8;  j++) 
	                index8[idx++] = (unsigned char)j;
                for(j = i+24; j < i+24+8; j++) 
	                index8[idx++] = (unsigned char)j;
	}

}


void update_clut(int frame) {

        u_int *pclut, index, i;

	// --- update 8bits clut ---
        pclut = (u_int*) My_clut8;
        i = (frame - 1) % 256;
        index = index8[i];
        pclut[index] = 0x80000000 | (i << 16) | (i << 8) | i;
        index = index8[frame % 256];
        pclut[index] = 0x800000ff;
      
        sceGsSetDefLoadImage(&cp, CLUT_ADDRESS_8, 1, SCE_GS_PSMCT32, 0, 0, 16, 16);
	FlushCache(WRITEBACK_DCACHE);
	sceGsExecLoadImage(&cp, (u_long128 *)My_clut8);
	sceGsSyncPath(0, 0);

	// --- update 4bits clut ---
        i = frame % 16;
        if(i != 0) return;

        pclut = (u_int*) My_clut4;
        index = ((frame >> 4) - 1) % 16;
        pclut[index] = 0x80000000 | (index << 20) | (index << 12) | (index << 4);
        index = (frame >> 4) % 16;
        pclut[index] = 0x800000ff;
        
	sceGsSetDefLoadImage(&cp, CLUT_ADDRESS_4, 1, SCE_GS_PSMCT32, 0, 0, 8, 2);
	FlushCache(WRITEBACK_DCACHE);
	sceGsExecLoadImage(&cp, (u_long128 *)My_clut4);
	sceGsSyncPath(0, 0);

}




void make_packet2(Q_WORDDATA *pbuf) {

	pbuf[3].ul64[0] = SCE_GS_SET_TEX0(INDEXIMG_ADDRESS_4, 4, SCE_GS_PSMT4, 8, 8, 1,
		1, CLUT_ADDRESS_4, SCE_GS_PSMCT32, 0, 0, 1);
	pbuf[9].ui32[0] = (2048 + 10) << 4;	// X
	pbuf[9].ui32[1] = (2038 - 70) << 4;	// Y

	pbuf[12].ui32[0] = (2048 + 300) << 4;	// X
	pbuf[12].ui32[1] = (2038 + 70) << 4;	// Y

}

void make_packet1(Q_WORDDATA *pbuf) {

	pbuf[3].ul64[0] = SCE_GS_SET_TEX0(INDEXIMG_ADDRESS_8, 4, SCE_GS_PSMT8, 8, 8, 1,
		1, CLUT_ADDRESS_8, SCE_GS_PSMCT32, 0, 0, 1);
	pbuf[9].ui32[0] = (2048 - 300) << 4;	// X
	pbuf[9].ui32[1] = (2038 - 70) << 4;	// Y

	pbuf[12].ui32[0] = (2048 - 10) << 4;	// X
	pbuf[12].ui32[1] = (2038 + 70) << 4;	// Y


}


void init_packet(Q_WORDDATA *pbuf) {

	pbuf[0].ul128 = (u_long128)0;
	pbuf[0].ui32[0] = 0x1000000c;		// DMA cnt

	pbuf[1].ul64[0] = SCE_GIF_SET_TAG(4, 0, 0, 0, 0, 1);
	pbuf[1].ul64[1] = 0xe;


	pbuf[2].ul64[0] = 0;
	pbuf[2].ul64[1] = SCE_GS_TEXFLUSH;

        pbuf[3].ul64[1]= SCE_GS_TEX0_1;

	pbuf[4].ul64[0] = SCE_GS_SET_TEX1(0, 0, SCE_GS_LINEAR, SCE_GS_LINEAR, 0, 0, 0);
	pbuf[4].ul64[1] = SCE_GS_TEX1_1;

	pbuf[5].ul64[0] = SCE_GS_SET_CLAMP(1, 1, 0, 0, 0, 0);
	pbuf[5].ul64[1] = SCE_GS_CLAMP_1;

	pbuf[6].ul64[0] = SCE_GIF_SET_TAG(1, 1, 1,
		SCE_GS_PRIM_SPRITE | SCE_GS_PRIM_TME, 0, 6);
	pbuf[6].ul64[1] = 0x412412;

	pbuf[7].ui32[0] = 0;			// S
	pbuf[7].ui32[1] = 0;			// T
	pbuf[7].ui32[2] = 0x3f800000;		// Q(1.0f)
	pbuf[7].ui32[3] = 0;

	pbuf[8].ui32[0] = 0xff;			// R
	pbuf[8].ui32[1] = 0x40;			// G
	pbuf[8].ui32[2] = 0x20;			// B
	pbuf[8].ui32[3] = 0x80;			// A

	pbuf[9].ui32[2] = 1 << 4;		// Z
	pbuf[9].ui32[3] = 0;			// F

	pbuf[10].ui32[0] = 0x3f800000;		// S
	pbuf[10].ui32[1] = 0x3f800000; 	        // T
	pbuf[10].ui32[2] = 0x3f800000;		// Q(1.0f)
	pbuf[10].ui32[3] = 0;

	pbuf[11].ui32[0] = 0xff;		// R
	pbuf[11].ui32[1] = 0x40;		// G
	pbuf[11].ui32[2] = 0x20;		// B
	pbuf[11].ui32[3] = 0x80;		// A

	pbuf[12].ui32[2] = 1 << 4;		// Z
	pbuf[12].ui32[3] = 0;			// F

	pbuf[13].ul128 = (u_long128)0;
	pbuf[13].ui32[0] = 0x70000000;		// DMA end

}

void tex_load(void)
{
	sceGsSetDefLoadImage(&tp, INDEXIMG_ADDRESS_8, 4, SCE_GS_PSMT8, 0, 0, 256, 256);
	FlushCache(WRITEBACK_DCACHE);
	sceGsExecLoadImage(&tp, (u_long128 *)My_indeximg8);
	sceGsSyncPath(0, 0);

	sceGsSetDefLoadImage(&tp, INDEXIMG_ADDRESS_4, 4, SCE_GS_PSMT4, 0, 0, 256, 256);
	FlushCache(WRITEBACK_DCACHE);
	sceGsExecLoadImage(&tp, (u_long128 *)My_indeximg4);
	sceGsSyncPath(0, 0);
}





