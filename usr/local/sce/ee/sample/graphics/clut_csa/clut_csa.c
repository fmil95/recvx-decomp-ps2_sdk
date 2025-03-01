/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - clut_csa -
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
 *       0.4.0          Jul,12,1999     munekis    1st version
 */


#include <eekernel.h>
#include <eeregs.h>
#include <eestruct.h>
#include <libgraph.h>
#include <libdma.h>


extern u_int My_clut4_1[]    __attribute__((section(".vudata")));
extern u_int My_clut4_2[]    __attribute__((section(".vudata")));
extern u_int My_clut4_3[]    __attribute__((section(".vudata")));
extern u_int My_indeximg4[]  __attribute__((section(".vudata")));


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
void make_packet(Q_WORDDATA *pbuf, u_int);
void load_clut_to_fb(void);
void tex_load(void);

#define INDEXIMG_ADDRESS_4   6720
#define CLUT_ADDRESS_4_1     6848
#define CLUT_ADDRESS_4_2     6852
#define CLUT_ADDRESS_4_3     6856

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


        // --- transfer indexed texture & clut data to frame buffer ---
	tex_load();
        load_clut_to_fb();

	frame = 0;
	init_packet(packet_buf);

	while(1){
		odev = !sceGsSyncV(0);
		sceGsSetHalfOffset((frame&1)?(&db.draw1):(&db.draw0),
			2048, 2048, odev);
		FlushCache(WRITEBACK_DCACHE);
		sceGsSwapDBuff(&db, frame);

		while(DGET_D2_CHCR() & 0x0100);

		// --- draw right rectangle(4bits indexed texture mapped) ----
                make_packet(packet_buf, frame);
		FlushCache(WRITEBACK_DCACHE);
		sceDmaSend(d2, packet_buf);

		frame++;
		sceGsSyncPath(0, 0);
	}

}



void load_clut_to_fb() {

	sceGsSetDefLoadImage(&cp, CLUT_ADDRESS_4_1, 1, SCE_GS_PSMCT32, 0, 0, 16, 16);
	FlushCache(WRITEBACK_DCACHE);
	sceGsExecLoadImage(&cp, (u_long128 *)My_clut4_1);
	sceGsSyncPath(0, 0);

	sceGsSetDefLoadImage(&cp, CLUT_ADDRESS_4_2, 1, SCE_GS_PSMCT32, 0, 0, 16, 16);
	FlushCache(WRITEBACK_DCACHE);
	sceGsExecLoadImage(&cp, (u_long128 *)My_clut4_2);
	sceGsSyncPath(0, 0);

	sceGsSetDefLoadImage(&cp, CLUT_ADDRESS_4_3, 1, SCE_GS_PSMCT32, 0, 0, 16, 16);
	FlushCache(WRITEBACK_DCACHE);
	sceGsExecLoadImage(&cp, (u_long128 *)My_clut4_3);
	sceGsSyncPath(0, 0);

}




void make_packet(Q_WORDDATA *pbuf, u_int frame) {

        u_int csa, n_clut, clut_address;
        
        csa = frame % 256;
        csa = csa / 16;
        n_clut = frame % 768;
        n_clut = n_clut / 256;

        // set cbp address 
        if(n_clut == 0) {
                clut_address = CLUT_ADDRESS_4_1;
        } else if(n_clut == 1) {
                clut_address = CLUT_ADDRESS_4_2;
        } else if(n_clut == 2) {
                clut_address = CLUT_ADDRESS_4_3;
        }

        // if clut_address is different from cbp0, load clut from clut_address to temporary buffer(cld == 0x100).
        pbuf[3].ul64[0] = SCE_GS_SET_TEX0(INDEXIMG_ADDRESS_4, 4, SCE_GS_PSMT8, 8, 8, 1,
                1, clut_address, SCE_GS_PSMCT32, 0, 0, 4);

        // No clut load to temporary buffer, only set csa.(cld == 0x00)
	pbuf[4].ul64[0] = SCE_GS_SET_TEX0(INDEXIMG_ADDRESS_4, 4, SCE_GS_PSMT4, 8, 8, 1,
		1, clut_address, SCE_GS_PSMCT32, 0, csa, 0);



}

void init_packet(Q_WORDDATA *pbuf) {

	pbuf[0].ul128 = (u_long128)0;
	pbuf[0].ui32[0] = 0x1000000d;		// DMA cnt

	pbuf[1].ul64[0] = SCE_GIF_SET_TAG(5, 0, 0, 0, 0, 1);
	pbuf[1].ul64[1] = 0xe;

	pbuf[2].ul64[0] = 0;
	pbuf[2].ul64[1] = SCE_GS_TEXFLUSH;

        pbuf[3].ul64[1]= SCE_GS_TEX0_1;

        pbuf[4].ul64[1]= SCE_GS_TEX0_1;

	pbuf[5].ul64[0] = SCE_GS_SET_TEX1(0, 0, SCE_GS_LINEAR, SCE_GS_LINEAR, 0, 0, 0);
	pbuf[5].ul64[1] = SCE_GS_TEX1_1;

	pbuf[6].ul64[0] = SCE_GS_SET_CLAMP(1, 1, 0, 0, 0, 0);
	pbuf[6].ul64[1] = SCE_GS_CLAMP_1;

	pbuf[7].ul64[0] = SCE_GIF_SET_TAG(1, 1, 1,
		SCE_GS_PRIM_SPRITE | SCE_GS_PRIM_TME, 0, 6);
	pbuf[7].ul64[1] = 0x412412;

	pbuf[8].ui32[0] = 0;			// S
	pbuf[8].ui32[1] = 0;			// T
	pbuf[8].ui32[2] = 0x3f800000;		// Q(1.0f)
	pbuf[8].ui32[3] = 0;

	pbuf[9].ui32[0] = 0xff;			// R
	pbuf[9].ui32[1] = 0x40;			// G
	pbuf[9].ui32[2] = 0x20;			// B
	pbuf[9].ui32[3] = 0x80;			// A

	pbuf[10].ui32[0] = (2048 - 150) << 4;	// X
	pbuf[10].ui32[1] = (2038 - 70) << 4;	// Y
	pbuf[10].ui32[2] = 1 << 4;		// Z
	pbuf[10].ui32[3] = 0;			// F

	pbuf[11].ui32[0] = 0x3f800000;		// S
	pbuf[11].ui32[1] = 0x3f800000; 	        // T
	pbuf[11].ui32[2] = 0x3f800000;		// Q(1.0f)
	pbuf[11].ui32[3] = 0;

	pbuf[12].ui32[0] = 0xff;		// R
	pbuf[12].ui32[1] = 0x40;		// G
	pbuf[12].ui32[2] = 0x20;		// B
	pbuf[12].ui32[3] = 0x80;		// A

	pbuf[13].ui32[0] = (2048 + 150) << 4;	// X
	pbuf[13].ui32[1] = (2038 + 70) << 4;	// Y
	pbuf[13].ui32[2] = 1 << 4;		// Z
	pbuf[13].ui32[3] = 0;			// F

	pbuf[14].ul128 = (u_long128)0;
	pbuf[14].ui32[0] = 0x70000000;		// DMA end

}

void tex_load(void)
{
        // --- load 4bit indexed texture data ---
	sceGsSetDefLoadImage(&tp, INDEXIMG_ADDRESS_4, 4, SCE_GS_PSMT4, 0, 0, 256, 256);
	FlushCache(WRITEBACK_DCACHE);
	sceGsExecLoadImage(&tp, (u_long128 *)My_indeximg4);
	sceGsSyncPath(0, 0);
}





