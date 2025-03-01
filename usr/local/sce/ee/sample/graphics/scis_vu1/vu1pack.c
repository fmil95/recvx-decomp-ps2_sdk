/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                       - scissoring VU1 -
 *
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : vu1pack.c
 *
 *       Version      Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.5.0        Sep,22,1999     thatake     1st Version
 */

#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <libdev.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <libvu0.h>
#include <sifdev.h>

#include "defines.h"

extern   sceVu0FVECTOR obj_trans;
extern   sceVu0FVECTOR obj_rot;
extern   sceVu0FMATRIX unit_mat;

void InitVu1DmaPacket(Vu1DmaPacket *dma, 
		      Vu1ObjPacket *head0, Vu1ObjPacket *head1)
{
  dma->pkt_switch = 0;
  dma->objhead[0] = head0;
  dma->objhead[1] = head1;
  dma->current = dma->objhead[0];
}

Vu1ObjPacket* GetVu1ObjHead(Vu1DmaPacket *dma)
{
  return dma->objhead[dma->pkt_switch];
}

Vu1ObjPacket* AddVu1ObjPacket(Vu1DmaPacket *head, u_int *obj, 
			      sceVu0FVECTOR obj_rot, 
			      sceVu0FVECTOR obj_trans)
{
  head->current->dmatag.ul128 = 0x0;
  head->current->dmatag.ui32[0] = 7 | ((0x5)<<28);// DMAtag [call 0x5] 
  head->current->dmatag.ui32[1] = ((u_int)obj)&0x0fffffff;
  head->current->dmatag.ui32[2] = 0x10000000;// FLUSHE
  head->current->dmatag.ui32[3] = SCE_VIF1_SET_UNPACK(0,4,0x0c,0);// V4_32
  (head->current+1)->dmatag.ul128 = 0x0;
  (head->current+1)->dmatag.ui32[0] = 0 | ((0x7)<<28);// end 0x7

  // local -> world (rotate)matrix
  sceVu0RotMatrix(head->current->Local_world, unit_mat, obj_rot);

  // local -> world (rotate&translate)matrix
  sceVu0TransMatrix(head->current->Local_world, 
		    head->current->Local_world, obj_trans);

  head->current->viftag.ul128 = 0x0;
  head->current->viftag.ui32[3] 
    = SCE_VIF1_SET_UNPACK(25, 1, 0x0c, 0);// free area 25-29

  // proparty of object & Magic code
  head->current->Obj_prim[0] = 0x800b;
  head->current->Obj_prim[1] = 0x300ec000;
  head->current->Obj_prim[2] = 0x0412;
  head->current->Obj_prim[3] = 0x0;

  head->current->Vifend.ui32[0] = SCE_VIF1_SET_MSCALF(0, 0);// MSCALF 0
  head->current->Vifend.ui32[1] = SCE_VIF1_SET_BASE(141, 0);// BASE 0
  head->current->Vifend.ui32[2] = SCE_VIF1_SET_OFFSET(400, 0);// OFFSET 512
  head->current->Vifend.ui32[3] = SCE_VIF1_SET_NOP(0);
  head->current += 1;

  return (head->current)-1;
}

Vu1ObjPacket* CloseVu1ObjPacket(Vu1DmaPacket *dma)
{
  dma->pkt_switch = !dma->pkt_switch;
  dma->current = dma->objhead[dma->pkt_switch];
  return dma->objhead[!dma->pkt_switch];
}

Vu1EnvPacket* GetVu1EnvHead(Vu1DmaPacket *dma)
{
  return &dma->envhead;
}

Vu1EnvPacket* CloseVu1EnvPacket(Vu1DmaPacket *dma)
{
    dma->envhead.dmatag.ul128 = 0x0;
    dma->envhead.dmatag.ui32[0] = 25 | ((0x7)<<28);// element num 25 ,end 0x7
    dma->envhead.dmatag.ui32[2] = 0x10000000;// FLUSHE
    dma->envhead.dmatag.ui32[3] = SCE_VIF1_SET_UNPACK(0,24,0x0c,0);// V4_32
    dma->envhead.Normal_light[3][3] = 1.0;
    dma->envhead.Vifend.ui32[0] = SCE_VIF1_SET_NOP(0);
    dma->envhead.Vifend.ui32[1] = SCE_VIF1_SET_BASE(141, 0);// BASE 21 - 491
    dma->envhead.Vifend.ui32[2] = SCE_VIF1_SET_OFFSET(400, 0);// OFFSET 491-961
    dma->envhead.Vifend.ui32[3] = SCE_VIF1_SET_NOP(0);

    return &dma->envhead;
}


