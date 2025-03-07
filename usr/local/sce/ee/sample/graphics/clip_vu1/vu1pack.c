/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - clip_vu1 -
 *
 *                         Version 0.3.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : vu1pack.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.3.0         
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

extern   sceVu0FVECTOR camera_p;
extern   sceVu0FVECTOR camera_zd;
extern   sceVu0FVECTOR camera_yd;
extern   sceVu0FVECTOR camera_rot;

extern   sceVu0FVECTOR light0;
extern   sceVu0FVECTOR light1;
extern   sceVu0FVECTOR light2;

extern   sceVu0FVECTOR color0;
extern   sceVu0FVECTOR color1;
extern   sceVu0FVECTOR color2;
extern   sceVu0FVECTOR ambient;

extern   sceVu0FVECTOR obj_trans;
extern   sceVu0FVECTOR obj_rot;

extern   sceVu0FMATRIX world_view;
extern   sceVu0FMATRIX view_screen;
extern   sceVu0FMATRIX world_screen;
extern   sceVu0FMATRIX local_screen;
extern   sceVu0FMATRIX local_world;
extern   sceVu0FMATRIX normal_light;
extern   sceVu0FMATRIX light_color;
extern   sceVu0FMATRIX local_light;
extern   sceVu0FMATRIX local_color;

extern   sceVu0FMATRIX work;
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
  head->current->dmatag.ui32[0] = 5 | ((0x5)<<28);// call 0x5 
  head->current->dmatag.ui32[1] = ((u_int)obj)&0x0fffffff;
  head->current->dmatag.ui32[2] = 0x10000000;// FLUSHE NOPより速い？
  head->current->dmatag.ui32[3] = SCE_VIF1_SET_UNPACK(0,4,0x0c,0);// V4_32
  (head->current+1)->dmatag.ul128 = 0x0;
  (head->current+1)->dmatag.ui32[0] = 0 | ((0x7)<<28);// end 0x7

  // local -> world (rotate)matrix
  sceVu0RotMatrix(head->current->Local_world, unit_mat, obj_rot);

  // local -> world (rotate&translate)matrix
  sceVu0TransMatrix(head->current->Local_world, 
		    head->current->Local_world, obj_trans);

  head->current->Vifend.ui32[0] = SCE_VIF1_SET_MSCALF(0, 0);// MSCALF 0
  head->current->Vifend.ui32[1] = SCE_VIF1_SET_BASE(20, 0);// BASE 0
  head->current->Vifend.ui32[2] = SCE_VIF1_SET_OFFSET(502, 0);// OFFSET 512
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
    dma->envhead.dmatag.ui32[0] = 21 | ((0x7)<<28);// element num 21 ,end 0x7
    dma->envhead.dmatag.ui32[2] = 0x10000000;// FLUSHE
    dma->envhead.dmatag.ui32[3] = SCE_VIF1_SET_UNPACK(0,20,0x0c,0);// V4_32
    dma->envhead.Normal_light[3][3] = 1.0;
    dma->envhead.Vifend.ui32[0] = SCE_VIF1_SET_NOP(0);
    dma->envhead.Vifend.ui32[1] = SCE_VIF1_SET_BASE(21, 0);// BASE 0
    dma->envhead.Vifend.ui32[2] = SCE_VIF1_SET_OFFSET(501, 0);// OFFSET 512
    dma->envhead.Vifend.ui32[3] = SCE_VIF1_SET_NOP(0);

    return &dma->envhead;
}


