/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *               Graphics Framework Sample Program
 *                  
 *              ---- Scene Viewer Version 0.40 ----
 * 
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                          Name : packbuf.c
 *
 *            Version   Date           Design      Log
 *  --------------------------------------------------------------------
 *            0.30      Apr,4,2000     thatake     Beta0
 *            0.40      Jun,16,2000     	   +animation, clut
 */

#include <eekernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <eeregs.h>
#include <libvu0.h>
#include <malloc.h>
#include "defines.h"
#include "packbuf.h"

void Vu1PacketBufInit(PACKET_BUF *packet, int size, u_int mask){
  packet->pHead = (u_int*)memalign(64, bound(sizeof(u_long128)*size, 64));
  if (packet->pHead == NULL){
      ErrPrintf("Vu1PacketBufInit: cannot malloc\n");
      return;
  }
  packet->pHead=(u_int *)(mask|(u_int)packet->pHead);
  packet->pCurr = packet->pHead;
  packet->size=size;
}

void Vu1PacketBufReset(PACKET_BUF *packet){
  packet->pCurr = packet->pHead;
}

int Vu1PacketBufOverflow(PACKET_BUF *packet){
    if ((int)packet->pCurr > (int)packet->pHead + (packet->size*sizeof(u_long128))){
	return 1;				// too many packets
    } else{
	return 0;				// OK
    }
}

u_int* Vu1PacketAddMic(u_int* pCurr, u_int* micpack)
{
  qword* curr = (qword*)pCurr;

  if (micpack){
      SET_QW((*curr), 0x0, 0x0, ((u_int)micpack)&0x0fffffff, 0x50000000); // call
      curr++;
      SET_QW((*curr), 0x0, 0x0, 0x0, 0x70000000); // end
  }
  return (u_int*)curr;
}

u_int* Vu1PacketAddObj(u_int* pCurr, u_int* objpack)
{
  qword* curr = (qword*)pCurr;

  if (objpack){
      SET_QW((*curr), 0x0, 0x0, ((u_int)objpack)&0x0fffffff, 0x50000000); // call
      curr++;
      SET_QW((*curr), 0x0, 0x0, 0x0, 0x70000000); // end
  }
  return (u_int*)curr;
}

void Vu1PacketKickDma(u_int* kickaddr)
{
    *D1_QWC  = 0x00;
    *D1_TADR = Paddr(kickaddr);
    *D_STAT = 2;
    *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
}

