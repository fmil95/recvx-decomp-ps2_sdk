/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* $Id: sif.h,v 1.7 1999/11/21 10:00:23 hakama Exp $ */

/*
 *                     I/O Processor System Services
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         sif.h
 *                         sif interface routines header.
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.00           1999/10/12
 */

#ifndef _SIF_H_DEFS
#define _SIF_H_DEFS

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

/* flag bit asigned */
#define SIF_INIT	(1 << 0)
#define SIF_INIT2	(1 << 1)
#define SIF_CMD_FLG	(1 << 17)

#define SIF_DBG_1	(1 << 2)
#define SIF_DBG_2	(1 << 3)
#define SIF_DBG_3	(1 << 4)

/* send direction & mode */
#define SIF_FROM_IO	0x0
#define SIF_TO_IO	0x1
#define SIF_FROM_IOP	0x0
#define SIF_TO_IOP	0x1
#define SIF_FROM_EE	0x0
#define SIF_TO_EE	0x1

#define SIF_DMA_INT	0x2
#define SIF_DMA_INT_I	0x2
#define SIF_DMA_INT_O	0x4

#define SIF_DMA_SPR	0x8

#define SIF_DMA_BSN	0x10
#define SIF_DMA_TAG	0x20

/* for ELF loader */
extern void sceSifAcceptData(void); /* EE only */

extern void sceSifInit(void);
extern int  sceSifCheckInit(void);  /* IOP only */

extern void sceSifSetDChain(void);

typedef struct {
	unsigned int	data;
	unsigned int	addr;
	unsigned int	size;
	unsigned int	mode;
} sceSifDmaData;

extern unsigned int sceSifSetDma(sceSifDmaData *sdd, int len);
extern int sceSifDmaStat(unsigned int id);
#if 0
extern unsigned int sceSifAddDmaQueue(sceSifDmaData *sdd, int len);
extern int sceSifGetIdStat(unsigned int id);
#endif

extern void sceSifSend(unsigned int data, unsigned int addr,
		       unsigned int size, unsigned int mode);
extern unsigned int sceSifIsSending(void);
extern void sceSifSendSync(void);

extern void sceSifSetSIF0DMA(unsigned int addr, unsigned int size,
			     unsigned int mode);
extern void sceSifSetSIF1DMA(unsigned int addr, unsigned int size,
			     unsigned int mode);
extern void sceSifSetSIF2DMA(unsigned int addr, unsigned int size,
			     unsigned int mode);
extern void sceSifSendSync0(void);
extern void sceSifSendSync1(void);
extern void sceSifSendSync2(void);
extern unsigned int sceSifIsSending0(void);
extern unsigned int sceSifIsSending1(void);
extern unsigned int sceSifIsSending2(void);

extern unsigned int sceSifGetMSflg(void);
extern unsigned int sceSifSetMSflg(unsigned int mask);
extern unsigned int sceSifGetSMflg(void);
extern unsigned int sceSifSetSMflg(unsigned int mask);
extern unsigned int sceSifGetMScom(void);
extern unsigned int sceSifSetMScom(unsigned int val);
extern unsigned int sceSifGetSMcom(void);
extern unsigned int sceSifSetSMcom(unsigned int val);
extern void sceSifIntrOther(void);

extern void RegisterSif0Handler(void (*handler)(void *), void *common);
extern void ReleaseSif0Handler(void);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* SIF_H_DEFS */

/* End of File */
