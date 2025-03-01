/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* $Id: timerman.h,v 1.4 2000/03/01 08:56:15 tei Exp $ */

/*
 *                     I/O Processor System Services
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         timerman.h
 *                         Hard timer manager defines
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.00           1999/10/12      tei
 */

#ifndef _TIMERMAN_H_
#define _TIMERMAN_H_

int	AllocHardTimer(int source, int size, int prescale);
int	ReferHardTimer(int source, int size, int mode, int modemask);
/* source */
#define TC_SYSCLOCK	1
#define	TC_PIXEL	2
#define	TC_HLINE	4
#define	TC_HOLD		8

#define	iSetTimerMode(timid, mode)	SetTimerMode((timid), (mode))
#define	iGetTimerStatus(timid)		GetTimerStatus(timid)
#define	iSetTimerCounter(timid,count)	SetTimerCounter((timid),(count))
#define	iGetTimerCounter(timid)		GetTimerCounter(timid)
#define	iSetTimerCompare(timid,compare) SetTimerCompare((timid),(compare))
#define	iGetTimerCompare(timid)		GetTimerCompare(timid)
#define	iSetHoldMode(holdnum, mode)	SetHoldMode((holdnum),(mode))
#define	iGetHoldMode(holdnum)		GetHoldMode(holdnum)
#define	iGetHoldReg(holdnum)		GetHoldReg(holdnum)

int		FreeHardTimer(int timid);
void		SetTimerMode(int timid, int mode);
long		GetTimerStatus(int timid);
void		SetTimerCounter(int timid, unsigned long count);
unsigned long	GetTimerCounter(int timid);
void		SetTimerCompare(int timid, unsigned long compare);
unsigned long	GetTimerCompare(int timid);
void		SetHoldMode(int holdnum, int mode);
unsigned long	GetHoldMode(int holdnum);
unsigned long	GetHoldReg(int holdnum);

int		GetHardTimerIntrCode(int timid);

/* Timer Counter mode/status register
 *   GATF = 0x00000001 [ 0:1] -- Gate flag 0:ignore gate 1:check gate
 *   GATM = 0x00000006 [ 1:2] -- Gate mode
 *   ZRET = 0x00000008 [ 3:1] -- Zero retrun
 *    CMP = 0x00000010 [ 4:1] -- Comp intr
 *   OVFL = 0x00000020 [ 5:1] -- Over intr
 *   REPT = 0x00000040 [ 6:1] -- Repeat intr
 *   LEVL = 0x00000080 [ 7:1] -- irq is level
 *   EXTC = 0x00000100 [ 8:1] -- Ext signal count
 *   PSCL = 0x00000200 [ 9:1] -- Pre-Scaler for timer-2
 *   INTF = 0x00000400 [10:1] -- (status) Interrupt flag
 *   EQUF = 0x00000800 [11:1] -- (status) Compare equal
 *   OVFF = 0x00001000 [12:1] -- (status) Overflow flag
 *   NTPS = 0x00006000 [13:2] -- New Pre-Scaler for timer-4,5
 */
#define tGATF_0       	(0<<0)
#define tGATF_1       	(1<<0)
#define tGATM(x)      	((x)<<1)
#define  tGATE_ON_count			0
#define  tGATE_ON_ClearSart		1
#define  tGATE_ON_Clear_OFF_Start	2
#define  tGATE_ON_Start			3
#define tZRET_0       	(0<<3)
#define tZRET_1       	(1<<3)
#define tCMP_0        	(0<<4)
#define tCMP_1        	(1<<4)
#define tOVFL_0       	(0<<5)
#define tOVFL_1       	(1<<5)
#define tREPT_0       	(0<<6)
#define tREPT_1       	(1<<6)
#define tLEVL_0       	(0<<7)
#define tLEVL_1       	(1<<7)
#define tEXTC_0       	(0<<8)
#define tEXTC_1       	(1<<8)
#define tPSCL_0       	(0<<9)
#define tPSCL_1       	(1<<9)
#define tINTF_0       	(0<<10)
#define tINTF_1       	(1<<10)
#define tEQUF_0       	(0<<11)
#define tEQUF_1       	(1<<11)
#define tOVFF_0       	(0<<12)
#define tOVFF_1       	(1<<12)
#define tNTPS(x)      	((x)<<13)

/* Fully Qualified Bits
 ( tGATF_0|tGATM(x)|tZRET_0|tCMP_0|tOVFL_0|tREPT_0|tLEVL_0|tEXTC_0|tPSCL_0|\
   tNTPS(x) )
 */

/* HOLD MODE register
 *   TRG0 = 0x00000001 [ 0:1] -- Trigger edge  0:falling edge, 1: rising edge
 *   TMD0 = 0x00000002 [ 1:1] -- Trigger mode  0:first trigger, 1: everty trigger
 *   HMD0 = 0x00000004 [ 2:1] -- Hold mode  0:disable, 1:enable
 *   ATC0 = 0x00000008 [ 3:1] -- Auto clear mode 0:disable, 1:enable
 *   TRG1 = 0x00000010 [ 4:1] -- 
 *   TMD1 = 0x00000020 [ 5:1] -- 
 *   HMD1 = 0x00000040 [ 6:1] -- 
 *   ATC1 = 0x00000080 [ 7:1] -- 
 *   TRG2 = 0x00000100 [ 8:1] -- 
 *   TMD2 = 0x00000200 [ 9:1] -- 
 *   HMD2 = 0x00000400 [10:1] -- 
 *   ATC2 = 0x00000800 [11:1] -- 
 *   TRG3 = 0x00001000 [12:1] -- 
 *   TMD3 = 0x00002000 [13:1] -- 
 *   HMD3 = 0x00004000 [14:1] -- 
 *   ATC3 = 0x00008000 [15:1] -- 
 */
#define hTRG_0       	(0<<0)
#define hTRG_1       	(1<<0)
#define hTMD_0       	(0<<1)
#define hTMD_1       	(1<<1)
#define hHMD_0       	(0<<2)
#define hHMD_1       	(1<<2)
#define hATC_0       	(0<<3)
#define hATC_1       	(1<<3)

#endif /* _TIMERMAN_H_ */
