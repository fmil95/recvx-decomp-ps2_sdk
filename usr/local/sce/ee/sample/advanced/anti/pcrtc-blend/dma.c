/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
#include <stdlib.h>
#include <stdio.h>
#include <eekernel.h>
#include <eeregs.h>
#include <math.h>
#include <libgraph.h>
#include <libdma.h>
#include <libpkt.h>
#include <libvu0.h>
#include <libdev.h>
#include <sifdev.h>
#include <eestruct.h>
#include "base.h"

unsigned int GetCOUNT(void);
void WaitDMA(void);
void WaitVU1(void);
void load_data_packet(u_int *packet);
void dma_packet_VIF1_Tag(u_int *packet);
void dma_packet_VIF1_noTag(u_int *packet);


void dma_packet_VIF1_Tag(u_int *tagpacket)
{
       /* DMA START 1  (VIF1)  */
		while(*D2_CHCR & 0x0100);
		*D_STAT = 2;      /* Chanel(1) Interrupt status clear */    
		*D1_QWC  = 0x00;  /* Chanel(1) QWC clear */
		*D1_TADR = (u_int)tagpacket & 0x0fffffff;
		FlushCache(0);
	/* 1:fromMemory,(1<<2):Chain,(0<<4):nonPush,
		   (1<<6):Tag,(0<<7):nonIRQ,(1<<8):DMAstart */     
		*D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
		WaitDMA();
	/* DMA END  1 */
		while(*VIF1_STAT & 0x1f000003);
		WaitVU1();
	/* VIF END  */
	       while(*GIF_STAT & 0x0c00);
}

void dma_packet_VIF1_noTag(u_int *tagpacket)
{
       /* DMA START 1  (VIF1)  */
		while(*D2_CHCR & 0x0100);
		*D_STAT = 2;      /* Chanel(1) Interrupt status clear */    
		*D1_QWC  = 0x00;  /* Chanel(1) QWC clear */
		*D1_TADR = (u_int)tagpacket & 0x0fffffff;
		FlushCache(0);
	/* 1:fromMemory,(1<<2):Chain,(0<<4):nonPush,
		   (0<<6):noTag,(0<<7):nonIRQ,(1<<8):DMAstart */     
		*D1_CHCR = 1 | (1<<2) | (0<<4) | (0<<6) | (0<<7) | (1<<8);
		WaitDMA();
	/* DMA END  1 */
		while(*VIF1_STAT & 0x1f000003);
		WaitVU1();
	/* VIF END  */
	       while(*GIF_STAT & 0x0c00);
}

void DirectDMApath3(int sq, u_int tx)
{
	/* DMA ch GIF */
	*D2_QWC = (sq);
	*D2_MADR = tx & 0x0fffffff;
	*D2_CHCR = 1 | (1<<8);  /* 1:fromMemory (1<<8):DMAstart */ 
	while((*D2_CHCR & 0x0100) || (*GIF_STAT & 0x0c00));
	/* DMA */
}


unsigned int GetCOUNT(void)
{
        unsigned int i;

        asm(" mfc0 %0, $09" : "=r"(i));

        return i;
}

void WaitDMA(void)
{
	asm("	WDMA:");
	asm("	bc0f WDMA");
	asm("	nop");
}

void WaitVU1(void)
{
	asm("	WVU1:");
	asm("	bc2t WVU1");
	asm("	nop");
}

