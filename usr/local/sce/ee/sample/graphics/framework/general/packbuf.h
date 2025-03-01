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
 *                          Name : packbuf.h
 *
 *            Version   Date           Design      Log
 *  --------------------------------------------------------------------
 *            0.30      Apr,4,2000     thatake     Beta0
 *            0.40      Jun,16,2000     	   +animation, clut
 */

#ifndef _PACKETBUF_H_
#define _PACKETBUF_H_

#include <eetypes.h>

typedef struct{
  u_int* pHead;
  u_int* pCurr;
    int size;
}PACKET_BUF;

extern void Vu1PacketBufInit(PACKET_BUF *packet, int size, u_int mask);
extern void Vu1PacketBufReset(PACKET_BUF *packet);
extern int Vu1PacketBufOverflow(PACKET_BUF *packet);

extern u_int* Vu1PacketAddMic(u_int* pCurr, u_int* micpack);
extern u_int* Vu1PacketAddObj(u_int* pCurr, u_int* objpack);

extern void Vu1PacketKickDma(u_int* kickaddr);


#define SET_QW(qw, three, two, one, zero)  \
        (qw)[3]=three; (qw)[2]=two; (qw)[1]=one; (qw)[0]=zero;
/*
#define COPY_QW(qw0, qw1)  \
        qw0[3]=qw1[3]; qw0[2]=qw1[2]; qw0[1]=qw1[1]; qw0[0]=qw1[0]; 
	*/

#define COPY_QW(v0, v1) \
        asm __volatile__(" 
        lq    $6,0x0(%1) 
        sq    $6,0x0(%0)
        ": : "r" (v0) , "r" (v1) : "$6");  

#define DMA_CNT(curr, size) \
  (curr)[0] = 0x10000000 | (u_int)size; \
  (curr)[1] = 0x0; \
  (curr)[2] = SCE_VIF1_SET_NOP(0); \
  (curr)[3] = SCE_VIF1_SET_NOP(0); 

#define DMA_REF(curr, src_addr, size) \
  (curr)[0] = 0x30000000 | (u_int)size; \
  (curr)[1] = (u_int)src_addr & 0x7fffffff; \
  (curr)[2] = SCE_VIF1_SET_NOP(0); \
  (curr)[3] = SCE_VIF1_SET_NOP(0);

#define DMA_CALL(curr, index, size) \
  (curr)[0] = 0x50000000 | (u_int)size; \
  (curr)[1] = (u_int)index; \
  (curr)[2] = SCE_VIF1_SET_NOP(0); \
  (curr)[3] = SCE_VIF1_SET_NOP(0);

#define DMA_RET(curr, size) \
  (curr)[0] = 0x60000000 | (u_int)(size); \
  (curr)[1] = 0x0; \
  (curr)[2] = SCE_VIF1_SET_NOP(0); \
  (curr)[3] = SCE_VIF1_SET_NOP(0);

#define ADD_QWDATA(curr, qwdata) \
  COPY_QW(curr, qwdata);

#define ADD_UNPACK(curr, field, dest_addr, size) \
  (curr)[field] = SCE_VIF1_SET_UNPACK(dest_addr, size, 0x0c, 0);

#define ADD_UNPACKR(curr, field, dest_addr, size) \
  (curr)[field] = SCE_VIF1_SET_UNPACKR(dest_addr, size, 0x0c, 0);

#define ADD_MSCNT(curr, field) \
  (curr)[field] = SCE_VIF1_SET_MSCNT(0);

#define ADD_MSCAL(curr, field, start) \
  (curr)[field] = SCE_VIF1_SET_MSCAL(start, 0);

#define ADD_FLUSH(curr, field) \
  (curr)[field] = SCE_VIF1_SET_FLUSH(0);

#define ADD_BASE(curr, field, num) \
  (curr)[field] = SCE_VIF1_SET_BASE(num, 0);

#define ADD_OFFSET(curr, field, num) \
  (curr)[field] = SCE_VIF1_SET_OFFSET(num, 0);

#define ADD_STCYCL(curr, field, wl, cl) \
  (curr)[field] = SCE_VIF1_SET_STCYCL(wl, cl, 0);

#define ADD_STROW(curr, field) \
  (curr)[field] = SCE_VIF1_SET_STROW(0);

#define ADD_DIRECT(curr, field, num) \
  (curr)[field] = SCE_VIF1_SET_DIRECT(num, 0);


#define REF_UNPACK(curr, src_addr, dest_addr, size) \
  DMA_REF(curr, src_addr, size); \
  ADD_UNPACK(curr, 3, dest_addr, size);

#define REF_UNPACKR(curr, src_addr, dest_addr, size) \
  DMA_REF(curr, src_addr, size); \
  ADD_UNPACKR(curr, 3, dest_addr, size);

#define CALL_NOP(curr, index, size) \
  DMA_CALL(curr, index, size);

#define RET_NOP(curr, size) \
  DMA_RET(curr, size);

#define RET_BASE_OFFSET(curr, base, offset) \
  DMA_RET(curr, 0); \
  ADD_BASE(curr, 2, base); \
  ADD_OFFSET(curr, 3, offset);

#define CNT_MSCAL_FLUSH(curr, size, start) \
  DMA_CNT(curr, size); \
  ADD_MSCAL(curr,2, start); \
  ADD_FLUSH(curr,3 ); 

#define CNT_FLUSH(curr, size) \
  DMA_CNT(curr, size); \
  ADD_FLUSH(curr,3 ); 


#define CNT_MSCNT(curr, size) \
  DMA_CNT(curr, size); \
  ADD_MSCNT(curr, 3); 

#define CNT_UNPACKR(curr, dest_addr, size) \
  DMA_CNT(curr, size); \
  ADD_UNPACKR(curr, 3, dest_addr, size);

#define CNT_UNPACK(curr, dest_addr, size) \
  DMA_CNT(curr, size); \
  ADD_UNPACK(curr, 3, dest_addr, size);

#define CNT_STCYCL(curr, size, wl, cl) \
  DMA_CNT(curr, size); \
  ADD_STCYCL(curr, 2, wl, cl ); 

#define CNT_STCYCL_ROW(curr, size, wl, cl) \
  DMA_CNT(curr, size); \
  ADD_STCYCL(curr, 2, wl, cl ); \
  ADD_STROW(curr, 3);

#define REF_STCYCL_UNPACK(curr, src_addr, dest_addr, size, wl, cl) \
  DMA_REF(curr, src_addr, size); \
  ADD_STCYCL(curr, 2, wl, cl); \
  ADD_UNPACK(curr, 3, dest_addr, size);

#define REF_STCYCL_UNPACKR(curr, src_addr, dest_addr, size, wl, cl) \
  DMA_REF(curr, src_addr, size); \
  ADD_STCYCL(curr, 2, wl, cl); \
  ADD_UNPACKR(curr, 3, dest_addr, size);

#define CNT_STCYCL_UNPACKR(curr, dest_addr, size, wl, cl) \
  DMA_CNT(curr, size); \
  ADD_STCYCL(curr, 2, wl, cl); \
  ADD_UNPACKR(curr, 3, dest_addr, size);

#define CNT_DIRECT(curr, size) \
  DMA_CNT(curr, size); \
  ADD_DIRECT(curr, 3, (size));

#define REF_DIRECT(curr, src_addr, size) \
  DMA_REF(curr, src_addr, size); \
  ADD_DIRECT(curr, 3, size);


#endif /* !_PACKETBUF_H_ */
