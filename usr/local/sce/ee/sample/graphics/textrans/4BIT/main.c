/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - textrans -
 *
 *                        Version 1.3.0
 *
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : 4BIT/main.c
 *                     Description : main procedures
 *
 *       Version   Date           Design              Log
 *  --------------------------------------------------------------------
 *       1.3.0      Dec, 6,1999    munekis            first version
 */



#include <eekernel.h>
#include <eeregs.h>
#include <eestruct.h>
#include <libgraph.h>
#include <libdma.h>
#include <sifdev.h>
#include <stdio.h>
#include <stdlib.h>

sceGsDBuff db;

typedef union {
	u_long128	ul128;
	u_long		ul64[2];
	u_int		ui32[4];
} Q_WORDDATA;

Q_WORDDATA packet_buf[1024];
sceDmaChan *d2;

sceGsLoadImage cp, tp;
sceGsStoreImage gs_simage;

void init_packet(Q_WORDDATA *pbuf);
void make_packet(Q_WORDDATA *pbuf, u_int);
void load_clut_to_fb(void);
void load_tex_to_mem(void);
void tex_load(void);
void fb_all_clear(void);

#define INDEXIMG_ADDRESS_4       6720
#define INDEXIMG_ADDRESS_4_REF   6720
#define CLUT_ADDRESS_4     8000


u_long128 tex_buffer[256 * 256 / 2 / 16];


#define ERR_STOP while(1);


// ---------------------------------------------------------------------------

int main() {

	short odev;
	int frame, count, i;

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


        init_packet(packet_buf);

        // --- transfer indexed texture & clut data to frame buffer ---
        fb_all_clear();
	load_tex_to_mem();
        load_clut_to_fb();

	*T0_MODE = T_MODE_CLKS_M | T_MODE_CUE_M;

	while(1){
		*T0_COUNT = 0;
		for(i = 0; i < 100; i++) 
			tex_load();
		count = *T0_COUNT;
		printf("count = %d\n", count);


		odev = !sceGsSyncV(0);

		sceGsSetHalfOffset((frame&1)?(&db.draw1):(&db.draw0),
			2048, 2048, odev);
		FlushCache(WRITEBACK_DCACHE);
		sceGsSwapDBuff(&db, frame);
		sceGsSyncPath(0, 0);


		// --- draw right rectangle(4bits indexed texture mapped) ----
		FlushCache(WRITEBACK_DCACHE);
		sceDmaSend(d2, packet_buf);

		frame++;
		sceGsSyncPath(0, 0);
	}


	ERR_STOP;
}


// ---------------------------------------------------------------------------

void load_tex_to_mem() {

    int fd;
#ifdef __32BIT__
    char filename[] = "host:tex4_32.raw";
#else
    char filename[] = "host:tex4.raw";
#endif

    fd = sceOpen(filename, SCE_RDONLY);
    sceRead(fd, (void*)tex_buffer, 256 * 256 / 2);
    sceClose(fd);
    return;
}


// ---------------------------------------------------------------------------

void load_clut_to_fb() {

        int fd;
        u_long128 buffer[16 * 4 / 16];

        fd = sceOpen("host:clut4.raw", SCE_RDONLY);
        sceRead(fd, (void*)buffer, 16 * 4);

	sceGsSetDefLoadImage(&cp, CLUT_ADDRESS_4, 1, SCE_GS_PSMCT32, 0, 0, 8, 2);
	FlushCache(WRITEBACK_DCACHE);
	sceGsExecLoadImage(&cp, buffer);
	sceGsSyncPath(0, 0);
        sceClose(fd);

        return;
}





void init_packet(Q_WORDDATA *pbuf) {

	pbuf[0].ul128 = (u_long128)0;
	pbuf[0].ui32[0] = 0x1000000d;		// DMA cnt

	pbuf[1].ul64[0] = SCE_GIF_SET_TAG(5, 0, 0, 0, 0, 1);
	pbuf[1].ul64[1] = 0xe;

	pbuf[2].ul64[0] = 0;
	pbuf[2].ul64[1] = SCE_GS_TEXFLUSH;

        pbuf[3].ul64[1]= 0;
        pbuf[3].ul64[1]= SCE_GS_TEXFLUSH;

        pbuf[4].ul64[1]= SCE_GS_TEX0_1;
	pbuf[4].ul64[0] = SCE_GS_SET_TEX0(INDEXIMG_ADDRESS_4_REF, 4, SCE_GS_PSMT4, 8, 8, 1, 1, CLUT_ADDRESS_4, SCE_GS_PSMCT32, 0, 0, 1);

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

	pbuf[10].ui32[0] = (2048 - 200) << 4;	// X
	pbuf[10].ui32[1] = (2048 - 100) << 4;	// Y
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

	pbuf[13].ui32[0] = (2048 + 200) << 4;	// X
	pbuf[13].ui32[1] = (2048 + 100) << 4;	// Y
	pbuf[13].ui32[2] = 1 << 4;		// Z
	pbuf[13].ui32[3] = 0;			// F

	pbuf[14].ul128 = (u_long128)0;
	pbuf[14].ui32[0] = 0x70000000;		// DMA end

}

void tex_load(void)
{

#ifdef __32BIT__
	sceGsSetDefLoadImage(&tp, INDEXIMG_ADDRESS_4, 2, SCE_GS_PSMCT32, 0, 0, 128, 64);
#else
        sceGsSetDefLoadImage(&tp, INDEXIMG_ADDRESS_4, 4, SCE_GS_PSMT4, 0, 0, 256, 256) ;
#endif
	FlushCache(WRITEBACK_DCACHE);
        sceGsExecLoadImage(&tp, tex_buffer);
	sceGsSyncPath(0, 0);
}



void fb_all_clear() {

     u_long128 *zero;
     u_int* pz;
     u_int i;

     zero = (u_long128*) malloc(256 * 256 * 4);
     pz = (u_int*) zero;    
     for(i = 0; i < 256 * 256; i++) zero[i] = 0x00000000;

     for(i = 0; i < 16; i++) {
         sceGsSetDefLoadImage(&tp, i * 1024, 4, SCE_GS_PSMCT32, 0, 0, 256, 256);
	 FlushCache(WRITEBACK_DCACHE);
	 sceGsExecLoadImage(&tp, zero);
	 sceGsSyncPath(0, 0);
     }

     free(zero);

     return;
}






