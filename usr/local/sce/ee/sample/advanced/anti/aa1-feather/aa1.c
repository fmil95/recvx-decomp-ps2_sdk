/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                       - feather aa1 sample  -
 *
 *                         Version 0.2.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : aa1.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.1.0           Oct,07,1999     sasaki    modified from feather
 *      0.2.0           Jul,11,2000     sasaki    
 */

#include <eekernel.h>
#include <eeregs.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <libgraph.h>
#include <libdma.h>
#include <libvu0.h>
#include <libdev.h>
#include <libpad.h>
#include <sifdev.h>
#include <sifrpc.h>


#define PIf	3.14159f
#define       FlushDCache()   FlushCache(0)

extern unsigned int My_dma_start __attribute__((section(".vudata")));
extern unsigned int My_dma_start2 __attribute__((section(".vudata")));
extern unsigned int My_dma_end __attribute__((section(".vudata")));
extern unsigned int My_euler __attribute__((section(".vudata")));
extern unsigned int My_frame001 __attribute__((section(".vudata")));
extern unsigned int My_frame002 __attribute__((section(".vudata")));
extern unsigned int My_texenv001 __attribute__((section(".vudata")));
extern unsigned int My_island __attribute__((section(".vudata")));
extern unsigned int My_refract __attribute__((section(".dmadata")));
extern unsigned int My_lattice __attribute__((section(".dmadata")));
extern unsigned int My_test __attribute__((section(".vudata")));

extern void DevInit(void);
extern void cpqword(u_int *dst, u_int *src, u_int dmy);
unsigned int GetCOUNT(void);
void WaitDMA(void);
void WaitVU1(void);
void textureLoad(void);
extern void view2mtx(float view[3], float target[3]);
extern void island_init(void);
extern void rot_feather(int v_div, float ramda);
extern void clear_buffer(u_int flg);
extern void make_island_packet(float factor);
extern void make_feather_packet(int *q, int *r, int rept, int prim_flg, int aa1_flg);
extern void set_light_color(float mag);
extern void copy_frame(void);
extern void copy_frame1(void);
extern void copy_frame_new(void);
extern void copy_frame1_new(void);
extern void DumpVu1Reg(void);
extern void init_ftpos();
extern void init_feather_rot();
extern void tex_buf_set(u_int flg);

sceGsDBuff db;

u_int paddata;
u_char rdata[32];
u_long128 pad_dma_buf[scePadDmaBufferMax] __attribute__((aligned(64)));

int main()
{
	int i, k;
	int q, r;
	int q1, r1;
	u_int timer_start, timer_end, frame;
	u_int timer_mid1;
	u_int timer_mid2;
	int oddeven;
	float	view[3],target[3];
	float	view1[3], view2[3], view3[3];
	float factor = 0.0f;
	float factor1;
	float ramda;
	int ifirst;
	int	rept;
	int v_param, v_elev;
	int v_div;
	int n_fp, n_rest;
	int dn_fp;
	float light_mag;
	int aa1_flg, prim_flg;
	int fcopy_flg, fcopyn_flg;
	int aa1_flg_cont, prim_flg_cont;
	int fcopy_flg_cont, fcopyn_flg_cont;

        sceSifInitRpc(0);
        while(sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
                            0, NULL) < 0){
            printf("Can't load module sio2man\n");
        }
        while(sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
                            0, NULL) < 0){
            printf("Can't load module padman\n");
        }

	DevInit();

	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

	sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224,
		SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_NOCLEAR);

	textureLoad();

	init_ftpos();
	init_feather_rot();

	oddeven = ! sceGsSyncV(0);
	frame = 0;
	i = 0;
	ifirst = 0;
	v_param = 0; v_elev = 0;
	n_fp = 0; dn_fp = 1;
	light_mag = 1.0f;
	ramda = 0.0f;
	aa1_flg = 0; prim_flg = 0;
	fcopy_flg = 0; fcopyn_flg = 0;
	aa1_flg_cont = 0; prim_flg_cont = 0;
	fcopy_flg_cont = 0; fcopyn_flg_cont = 0;

	island_init();

	FlushDCache();
        // --- open pad ---
        scePadInit(0);
        scePadPortOpen(0, 0, pad_dma_buf);

	while(1){

		if (fcopy_flg == 1) {
			if (frame%2 == 1) {
				copy_frame();
			} else {
				copy_frame1();
			}
		}

		if (fcopyn_flg == 1) {
			if (frame%2 == 1) {
				copy_frame_new();
			} else {
				copy_frame1_new();
			}
		}

		sceGsSwapDBuff(&db, frame);

                // read PAD
                if(scePadRead(0, 0, rdata) > 0){
                    paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
                }
                else{
                    paddata = 0;
                }

                if(paddata & SCE_PADRdown) n_fp -= dn_fp;
                if(paddata & SCE_PADRup) light_mag -= 0.001f;
			if (light_mag < 0.0f) light_mag = 0.0f;
                if(paddata & SCE_PADRright) n_fp += dn_fp;
                if(paddata & SCE_PADRleft) light_mag += 0.001f;
			if (light_mag > 1.0f) light_mag = 1.0f;
                if(paddata & SCE_PADLdown) v_elev += 1;
                if(paddata & SCE_PADLup) v_elev -= 1;
                if(paddata & SCE_PADLright) ramda += 0.001f;
			if (ramda > 1.0f) ramda = 1.0f;
                if(paddata & SCE_PADLleft) ramda -= 0.001f;
			if (ramda < 0.0f) ramda = 0.0f;

                if(paddata & SCE_PADl) {
			if (aa1_flg_cont == 0) {
				if (aa1_flg == 0) {
					aa1_flg = 1;
				} else {
					aa1_flg = 0;
				}
			}
			aa1_flg_cont = 1;
		} else {
			aa1_flg_cont = 0;
		}

                if(paddata & SCE_PADo) {
			if (fcopyn_flg_cont == 0) {
				if (fcopyn_flg == 0) {
					fcopyn_flg = 1;
				} else {
					fcopyn_flg = 0;
				}
			}
			fcopyn_flg_cont = 1;
		} else {
			fcopyn_flg_cont = 0;
		}

                if(paddata & SCE_PADm) {
			if (prim_flg_cont == 0) {
				if (prim_flg == 0) {
					prim_flg = 1;
				} else {
					prim_flg = 0;
				}
			}
			prim_flg_cont = 1;
		} else {
			prim_flg_cont = 0;
		}

                if(paddata & SCE_PADn) {
			if (fcopy_flg_cont == 0) {
				if (fcopy_flg == 0) {
					fcopy_flg = 1;
				} else {
					fcopy_flg = 0;
				}
			}
			fcopy_flg_cont = 1;
		} else {
			fcopy_flg_cont = 0;
		}

		if(frame>=1000) frame -=1000;
		timer_start = GetCOUNT();
		sceGsSetHalfOffset((frame&1)?(&db.draw1):(&db.draw0),
			2048, 2048, oddeven);

		if (aa1_flg == 0) {
			(&My_test)[8] = 0x00000001; //PRMODECONT
		} else {
			(&My_test)[8] = 0x00000000; //PRMODECONT
		}

//******************************************************
		q = 0; r = 0;

		v_param++;
		if (v_param >= 16800) v_param = 0;
		if (v_elev >= 4200) v_elev = 0; if (v_elev < 0) v_elev = 0;
		if (n_fp > 2800) n_fp = 2800; if (n_fp < 100) n_fp = 100;
		dn_fp = n_fp/200; if (dn_fp <= 0) dn_fp = 1;

		srand(0);

		factor1 = 2.0f*sinf((float)v_param/600.0f*3.141593f)*sinf((float)v_param/50.0f*3.141593f);

		view[0]= -10000.0f*cosf((float)v_param/600.0f*3.141593f);
		view[1]= -11000.0f + 8000.0f*cosf((float)v_elev/150.0f*3.141593f);
		view[2]= -10000.0f*sinf((float)v_param/600.0f*3.141593f);

		target[0]= 0.0f;
		target[1]= 0.0f;
		target[2]= 0.0f;

//******************************************************
		view2mtx(view,target);

		set_light_color(light_mag);
		make_island_packet(factor);
		clear_buffer((u_int)1);

		tex_buf_set(0);

		if((frame&1)==0){
			(&My_frame001)[0] = 0x000a0046;
			(&My_frame001)[1] = 0xffffffff;
			(&My_frame001)[5] = 0x00007900+ (oddeven?8:0);
			(&My_texenv001)[0] = 0x280288c0;
			(&My_frame002)[0] = 0x000a0046;
			(&My_frame002)[1] = 0xffffffff;
			(&My_frame002)[5] = 0x00007900 + (oddeven?8:0);
		}else{
			(&My_frame001)[0] = 0x000a0000;
			(&My_frame001)[1] = 0xffffffff;
			(&My_frame001)[5] = 0x00007900 + (oddeven?8:0);
			(&My_texenv001)[0] = 0x28028000;
			(&My_frame002)[0] = 0x000a0000;
			(&My_frame002)[1] = 0xffffffff;
			(&My_frame002)[5] = 0x00007900 + (oddeven?8:0);
		}
		(&My_test)[1] = 0x00000000;	//ZMASK
		(&My_test)[4] = 0x0005020a;	//TEST

		while(*D2_CHCR & 0x0100);
		*D_STAT = 2;
		*D1_QWC  = 0x00;
		*D1_TADR = (u_int)&My_dma_start & 0x0fffffff;
		FlushDCache();
		*D1_CHCR = 1 | (1<<2) | (0<<4) | (0<<6) | (0<<7) | (1<<8);

		if (ifirst == 0) {
			ifirst = 1;
		} else {
			WaitDMA();
		}
		while(*D1_CHCR&(1<<8));

		while(*VIF1_STAT & 0x1f000003);
		WaitVU1();
		while(*GIF_STAT & 0x0c00);

//******************************************************

		if((frame&1)==0){
			(&My_frame001)[0] = 0x000a0046;
			(&My_frame001)[1] = 0x00000000;
			(&My_frame001)[5] = 0x00007900+ (oddeven?8:0);
			(&My_texenv001)[0] = 0x280288c0;
			(&My_frame002)[0] = 0x000a0046;
			(&My_frame002)[1] = 0x00000000;
			(&My_frame002)[5] = 0x00007900 + (oddeven?8:0);
		}else{
			(&My_frame001)[0] = 0x000a0000;
			(&My_frame001)[1] = 0x00000000;
			(&My_frame001)[5] = 0x00007900 + (oddeven?8:0);
			(&My_texenv001)[0] = 0x28028000;
			(&My_frame002)[0] = 0x000a0000;
			(&My_frame002)[1] = 0x00000000;
			(&My_frame002)[5] = 0x00007900 + (oddeven?8:0);
		}
		(&My_test)[1] = 0x00000001;	//ZMASK
		(&My_test)[4] = 0x0005020a;	//TEST

		while(*D2_CHCR & 0x0100);
		*D_STAT = 2;
		*D1_QWC  = 0x00;
		*D1_TADR = (u_int)&My_dma_start & 0x0fffffff;
		FlushDCache();
		*D1_CHCR = 1 | (1<<2) | (0<<4) | (0<<6) | (0<<7) | (1<<8);

		if (ifirst == 0) {
			ifirst = 1;
		} else {
			WaitDMA();
		}
		while(*D1_CHCR&(1<<8));

		while(*VIF1_STAT & 0x1f000003);
		WaitVU1();
		while(*GIF_STAT & 0x0c00);

//******************************************************

		if (prim_flg == 1) {
			(&My_test)[8] = 0x00000001; //PRMODECONT
		}
		(&My_test)[1] = 0x00000000;	//ZMASK
		(&My_test)[4] = 0x0005020a;	//TEST

		view1[0]= -10000.0f*cosf((float)v_param/210.0f*3.141593f);
		view1[1]= -11000.0f + 8000.0f*cosf((float)v_elev/150.0f*3.141593f);
		view1[2]= -10000.0f*sinf((float)v_param/210.0f*3.141593f);

		view2[0]= -10000.0f*cosf((float)v_param/240.0f*3.141593f);
		view2[1]= -11000.0f + 8000.0f*cosf((float)v_elev/150.0f*3.141593f);
		view2[2]= -10000.0f*sinf((float)v_param/240.0f*3.141593f);

		view3[0]= -10000.0f*cosf((float)v_param/1200.0f*3.141593f);
		view3[1]= -11000.0f + 8000.0f*cosf(v_elev/150.0f*3.141593f);
		view3[2]= -10000.0f*sinf((float)v_param/1200.0f*3.141593f);

		if ((v_param % 6) == 0) {
			v_div = 1;
		} else {
			v_div = 0;
		}
		rot_feather(v_div,ramda);
		tex_buf_set(1);

		rept=0;

		timer_mid1 = GetCOUNT();

		for (k=1; 100*k < n_fp; k++) {

			n_rest = n_fp - 100*k;
			q1 = q; r1 = r;
			make_feather_packet(&q,&r,rept,prim_flg, 0);
			clear_buffer((u_int)0);

			if (aa1_flg == 1) {
				if((frame&1)==0){
					(&My_frame001)[0] = 0x000a0046;
					(&My_frame001)[1] = 0xffffffff;
					(&My_frame001)[5] = 0x00007900+ (oddeven?8:0);
					(&My_texenv001)[0] = 0x280288c0;
					(&My_frame002)[0] = 0x000a0046;
					(&My_frame002)[1] = 0xffffffff;
					(&My_frame002)[5] = 0x00007900 + (oddeven?8:0);
				}else{
					(&My_frame001)[0] = 0x000a0000;
					(&My_frame001)[1] = 0xffffffff;
					(&My_frame001)[5] = 0x00007900 + (oddeven?8:0);
					(&My_texenv001)[0] = 0x28028000;
					(&My_frame002)[0] = 0x000a0000;
					(&My_frame002)[1] = 0xffffffff;
					(&My_frame002)[5] = 0x00007900 + (oddeven?8:0);
				}
			}

			if (k == 1) view2mtx(view1,target);
			if (k == 5) view2mtx(view2,target);
			if (k == 12) view2mtx(view3,target);

			while(*D2_CHCR & 0x0100);
			*D_STAT = 2;
			*D1_QWC  = 0x00;
			*D1_TADR = (u_int)&My_dma_start2 & 0x0fffffff;
			FlushDCache();
			*D1_CHCR = 1 | (1<<2) | (0<<4) | (0<<6) | (0<<7) | (1<<8);

			WaitDMA();

			while(*D1_CHCR&(1<<8));

			while(*VIF1_STAT & 0x1f000003);
			WaitVU1();
			while(*GIF_STAT & 0x0c00);

			if (aa1_flg == 1) {

				if((frame&1)==0){
					(&My_frame001)[0] = 0x000a0046;
					(&My_frame001)[1] = 0x00000000;
					(&My_frame001)[5] = 0x00007900+ (oddeven?8:0);
					(&My_texenv001)[0] = 0x280288c0;
					(&My_frame002)[0] = 0x000a0046;
					(&My_frame002)[1] = 0x00000000;
					(&My_frame002)[5] = 0x00007900 + (oddeven?8:0);
				}else{
					(&My_frame001)[0] = 0x000a0000;
					(&My_frame001)[1] = 0x00000000;
					(&My_frame001)[5] = 0x00007900 + (oddeven?8:0);
					(&My_texenv001)[0] = 0x28028000;
					(&My_frame002)[0] = 0x000a0000;
					(&My_frame002)[1] = 0x00000000;
					(&My_frame002)[5] = 0x00007900 + (oddeven?8:0);
				}

				q = q1; r = r1;
				make_feather_packet(&q,&r,rept,prim_flg, 1);
				while(*D2_CHCR & 0x0100);
				*D_STAT = 2;
				*D1_QWC  = 0x00;
				*D1_TADR = (u_int)&My_dma_start2 & 0x0fffffff;
				FlushDCache();
				*D1_CHCR = 1 | (1<<2) | (0<<4) | (0<<6) | (0<<7) | (1<<8);

				WaitDMA();

				while(*D1_CHCR&(1<<8));

				while(*VIF1_STAT & 0x1f000003);
				WaitVU1();
				while(*GIF_STAT & 0x0c00);

			}

//			DumpVu1Reg();
//			while(1);

			rept++;
		}

		timer_mid2 = GetCOUNT();

		frame++;

//******************************************************

		timer_end = GetCOUNT();
/*
	printf("%d\n",timer_mid1 - timer_start);
	printf("%d\n",timer_mid2 - timer_start);
*/
//	printf("%d\n",timer_end - timer_start);

		oddeven = !sceGsSyncV(0);

	}
}

unsigned int GetCOUNT(void)
{
        unsigned int i;

        asm(" mfc0 %0, $09" : "=r"(i));

        return i;
}

void WaitDMA(void)
{
	asm(" WDMA:");
	asm("	bc0f WDMA");
	asm("	nop");
}

void WaitVU1(void)
{
	asm(" WVU1:");
	asm("	bc2t WVU1");
	asm("	nop");
}

#include "sakura32.h"
#include "jimentex.h"

static unsigned int initx[] __attribute__((aligned(16))) = {
        0x00008005, 0x10000000, 0x0000000e, 0x00000000,
        0x00000001, 0x00000000, 0x00000046, 0x00000000,
        0x027f0000, 0x01df0000, 0x00000040, 0x00000000,
        0x00000001, 0x00000000, 0x0000001a, 0x00000000,
        0x00006c00, 0x00007100, 0x00000018, 0x00000000,
	0x000a0000, 0x00000000, 0x0000004c, 0x00000000,
};

void textureLoad(void)
{

	*D2_QWC = 0x205;
	*D2_MADR = (u_int)snow32 & 0x0fffffff;
	*D2_CHCR = 1 | (1<<8);

	while((*D2_CHCR & 0x0100) || (*GIF_STAT & 0x0c00));

	*D2_QWC = 0x2005;
	*D2_MADR = (u_int)surface & 0x0fffffff;
	*D2_CHCR = 1 | (1<<8);

	while((*D2_CHCR & 0x0100) || (*GIF_STAT & 0x0c00));

        *D2_QWC = 0x6;
        *D2_MADR = (u_int)initx & 0x0fffffff;
        *D2_CHCR = 1 | (1<<8);

        while((*D2_CHCR & 0x0100) || (*GIF_STAT & 0x0c00));

}

typedef struct {
        sceGifTag       giftag;
	u_int		dbuf[0x80];
} GsClearData __attribute__((aligned(16)));

void copy_frame(void)
{
	int i,nloop;
	GsClearData gcd;

	nloop = 25;

       	SCE_GIF_CLEAR_TAG(&gcd.giftag);
       	gcd.giftag.NLOOP = nloop;
       	gcd.giftag.EOP = 1;
       	gcd.giftag.NREG = 1;
       	gcd.giftag.REGS0 = 0xe; // A_D

	i=0;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000001a; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000001; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000004c; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x000a0046; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000047; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000040; gcd.dbuf[i+1] = (u_int)0x00df0000; gcd.dbuf[i] = (u_int)0x027f0000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000042; gcd.dbuf[i+1] = (u_int)0x00000040; gcd.dbuf[i] = (u_int)0x00000064; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000008; gcd.dbuf[i+1] = (u_int)0x0000037c; gcd.dbuf[i] = (u_int)0x009fc00a; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000018; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000046; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000001; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000049; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000006; gcd.dbuf[i+1] = (u_int)0x00000006; gcd.dbuf[i] = (u_int)0xa8028000; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000014; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000060; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000003b; gcd.dbuf[i+1] = (u_int)0x00000080; gcd.dbuf[i] = (u_int)0x00000080; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000001; gcd.dbuf[i+1] = (u_int)0x3f800000; gcd.dbuf[i] = (u_int)0x80808080; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000000; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0000015e; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000001; gcd.dbuf[i] = (u_int)0x00100000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e002800; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e102800; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000003f; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00100000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000001; gcd.dbuf[i] = (u_int)0x00100000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e102800; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e102800; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000003f; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000008; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000005; i+=4;

	FlushCache(0);
	
	*D2_QWC = gcd.giftag.NLOOP+1;
	
       	*D2_MADR = (u_int)&gcd.giftag & 0x0fffffff;
	FlushCache(0);
       	*D2_CHCR = (1 << 8) | 1;
	
	sceGsSyncPath(0,0);

}

void copy_frame1(void)
{
	int i,nloop;
	GsClearData gcd;

	nloop = 25;

       	SCE_GIF_CLEAR_TAG(&gcd.giftag);
       	gcd.giftag.NLOOP = nloop;
       	gcd.giftag.EOP = 1;
       	gcd.giftag.NREG = 1;
       	gcd.giftag.REGS0 = 0xe; // A_D

	i=0;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000001a; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000001; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000004c; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x000a0000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000047; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000040; gcd.dbuf[i+1] = (u_int)0x00df0000; gcd.dbuf[i] = (u_int)0x027f0000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000042; gcd.dbuf[i+1] = (u_int)0x00000040; gcd.dbuf[i] = (u_int)0x00000064; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000008; gcd.dbuf[i+1] = (u_int)0x0000037c; gcd.dbuf[i] = (u_int)0x009fc00a; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000018; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00080008; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000046; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000001; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000049; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000006; gcd.dbuf[i+1] = (u_int)0x00000006; gcd.dbuf[i] = (u_int)0xa80288c0; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000014; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000060; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000003b; gcd.dbuf[i+1] = (u_int)0x00000080; gcd.dbuf[i] = (u_int)0x00000080; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000001; gcd.dbuf[i+1] = (u_int)0x3f800000; gcd.dbuf[i] = (u_int)0x80808080; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000000; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0000015e; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000001; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e002800; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e002800; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000003f; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00100000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000001; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e102800; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e002800; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000003f; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000008; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000005; i+=4;

	FlushCache(0);
	
	*D2_QWC = gcd.giftag.NLOOP+1;
	
       	*D2_MADR = (u_int)&gcd.giftag & 0x0fffffff;
	FlushCache(0);
       	*D2_CHCR = (1 << 8) | 1;
	
	sceGsSyncPath(0,0);

}

void copy_frame_new(void)
{
	int i,nloop;
	GsClearData gcd;

	nloop = 25;

       	SCE_GIF_CLEAR_TAG(&gcd.giftag);
       	gcd.giftag.NLOOP = nloop;
       	gcd.giftag.EOP = 1;
       	gcd.giftag.NREG = 1;
       	gcd.giftag.REGS0 = 0xe; // A_D

	i=0;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000001a; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000001; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000004c; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x000a0046; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000047; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000040; gcd.dbuf[i+1] = (u_int)0x00df0000; gcd.dbuf[i] = (u_int)0x027f0000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000042; gcd.dbuf[i+1] = (u_int)0x00000040; gcd.dbuf[i] = (u_int)0x00000064; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000008; gcd.dbuf[i+1] = (u_int)0x0000037c; gcd.dbuf[i] = (u_int)0x009fc00a; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000018; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000046; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000001; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000049; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000006; gcd.dbuf[i+1] = (u_int)0x00000006; gcd.dbuf[i] = (u_int)0xa8028000; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000014; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000060; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000003b; gcd.dbuf[i+1] = (u_int)0x00000080; gcd.dbuf[i] = (u_int)0x00000080; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000001; gcd.dbuf[i+1] = (u_int)0x3f800000; gcd.dbuf[i] = (u_int)0x90909090; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000000; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0000015e; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000001; gcd.dbuf[i] = (u_int)0x00100000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e002800; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e102800; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000003f; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00100000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000001; gcd.dbuf[i] = (u_int)0x00100000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e102800; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e102800; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000003f; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000008; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000005; i+=4;

	FlushCache(0);
	
	*D2_QWC = gcd.giftag.NLOOP+1;
	
       	*D2_MADR = (u_int)&gcd.giftag & 0x0fffffff;
	FlushCache(0);
       	*D2_CHCR = (1 << 8) | 1;
	
	sceGsSyncPath(0,0);

}

void copy_frame1_new(void)
{
	int i,nloop;
	GsClearData gcd;

	nloop = 25;

       	SCE_GIF_CLEAR_TAG(&gcd.giftag);
       	gcd.giftag.NLOOP = nloop;
       	gcd.giftag.EOP = 1;
       	gcd.giftag.NREG = 1;
       	gcd.giftag.REGS0 = 0xe; // A_D

	i=0;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000001a; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000001; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000004c; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x000a0000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000047; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000040; gcd.dbuf[i+1] = (u_int)0x00df0000; gcd.dbuf[i] = (u_int)0x027f0000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000042; gcd.dbuf[i+1] = (u_int)0x00000040; gcd.dbuf[i] = (u_int)0x00000064; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000008; gcd.dbuf[i+1] = (u_int)0x0000037c; gcd.dbuf[i] = (u_int)0x009fc00a; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000018; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00080008; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000046; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000001; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000049; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000006; gcd.dbuf[i+1] = (u_int)0x00000006; gcd.dbuf[i] = (u_int)0xa80288c0; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000014; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000060; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000003b; gcd.dbuf[i+1] = (u_int)0x00000080; gcd.dbuf[i] = (u_int)0x00000080; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000001; gcd.dbuf[i+1] = (u_int)0x3f800000; gcd.dbuf[i] = (u_int)0x90909090; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000000; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0000015e; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000001; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e002800; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e002800; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000003f; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00100000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000001; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000003; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e102800; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000004; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x0e002800; i+=4;

gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x0000003f; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000000; i+=4;
gcd.dbuf[i+3] = (u_int)0x0; gcd.dbuf[i+2] = (u_int)0x00000008; gcd.dbuf[i+1] = (u_int)0x00000000; gcd.dbuf[i] = (u_int)0x00000005; i+=4;

	FlushCache(0);
	
	*D2_QWC = gcd.giftag.NLOOP+1;
	
       	*D2_MADR = (u_int)&gcd.giftag & 0x0fffffff;
	FlushCache(0);
       	*D2_CHCR = (1 << 8) | 1;
	
	sceGsSyncPath(0,0);

}

