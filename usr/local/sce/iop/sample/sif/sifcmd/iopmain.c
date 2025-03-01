/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - <file> -
 *
 *                         Version <0.10>
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            <main.c>
 *                     <sif cmd test program>
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.10            Mar,26,1999     hakamatani  first version
 *      0.90            Aug,23,1999     hakamatani  first version
 */

/* ================================================================
 *
 *      Program
 *
 * ================================================================ */

#ifdef R5900
#include <eekernel.h>
#include <stdio.h>
#include <sif.h>
#include <sifcmd.h>
#include <sifrpc.h>

#define _SIF_I_DEF
#define _SIF_DI  DI()
#define _SIF_EI  EI()
sceSifCmdHdr	sch __attribute__((aligned(16)));
sceSifCmdSRData srd __attribute__((aligned(16)));

#else

#include <stdio.h>
#include <loadcore.h>
#include <intrman.h>
#include <sif.h>
#include <sifcmd.h>

#define _SIF_I_DEF int oldisEI
#define _SIF_DI  CpuSuspendIntr(&oldisEI)
#define _SIF_EI  CpuResumeIntr(oldisEI) 

sceSifCmdHdr	sch;
sceSifCmdSRData srd;

ModuleInfo Module = {"SIFCMD_TEST", 0x0101 };

#endif

static void  test_f0(void *,void *);
static volatile int   test0;
static void  test_f1(void *,void *);
static volatile int   test1;

#define BSIZE 0x10
static sceSifCmdData cmdbuffer[BSIZE];

int start()
{
#ifdef R5900
#else
#endif
  unsigned int id;
  _SIF_I_DEF;

#ifdef R5900
  /* init */
  // –{—ˆ‚È‚ç InitCmd ‚¾‚ªAIOP ‘¤‚ª“®‚©‚È‚¢‚½‚ß‚É InitRpc
  // sceSifInitCmd();
  sceSifInitRpc(0);

#else
  FlushDcache();

  /* init */
  sceSifInitCmd();
#endif


  /* set local buffer & functions */
  _SIF_DI;
  sceSifSetCmdBuffer( &cmdbuffer[0], BSIZE);
  sceSifAddCmdHandler( 0, (void *) test_f0, (void *) &test0);
  sceSifAddCmdHandler( 1, (void *) test_f1, (void *) &test1);
  _SIF_EI;

  test0 = 0;
  test1 = 0;

  /* sync other cpu */
  srd.rno = 31;
  srd.value = 1;
  sceSifSendCmd(SIF_CMDC_SET_SREG, &srd, sizeof(srd), 0 , 0, 0);
  while(sceSifGetSreg(31) == 0);

  /* call test_f0 */
  sch.opt = (unsigned int) 10;
  id = sceSifSendCmd(0, &sch, sizeof(sch), 0 , 0, 0);
  while(sceSifDmaStat(id) >= 0);
  
  /* call test_f1 */
  sch.opt = (unsigned int) 20;
  id = sceSifSendCmd(1, &sch, sizeof(sch), 0 , 0, 0);
  while(sceSifDmaStat(id) >= 0);

  /* loop */
  while((test0 == 0) || (test1 == 0));
  
  printf("test0 = %d test1 = %d\n", test0, test1);

  while(1);

}

void test_f0(void *p,void *q)
{
  sceSifCmdHdr *h = (sceSifCmdHdr *) p;

  *(int *)q = (int)h->opt;
  
  return;
}

void test_f1(void *p,void *q)
{
  sceSifCmdHdr *h = (sceSifCmdHdr *) p;

  *(int *)q = (int)h->opt;
  
  return;
}

/* ----------------------------------------------------------------
 *	End on File
 * ---------------------------------------------------------------- */
/* DON'T ADD STUFF AFTER THIS */
