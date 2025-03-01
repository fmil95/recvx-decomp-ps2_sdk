/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* $Id: sysmem.h,v 1.3 1999/10/12 09:06:48 tei Exp $ */

/*
 *                     I/O Processor System Services
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         sysmem.h
 *                         system memory manager
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.00           1999/10/12      tei
 */

#ifndef _SYSMEM_H
#define _SYSMEM_H

#ifndef NULL
#define NULL (void*)0
#endif

extern void *AllocSysMemory(int type, unsigned long size, void *addr);
extern int FreeSysMemory(void *area);
extern unsigned long QueryMemSize();
extern unsigned long QueryMaxFreeMemSize();
extern unsigned long QueryTotalFreeMemSize();
extern void *QueryBlockTopAddress(void *addr);
extern unsigned long QueryBlockSize(void *addr);

extern void *Kprintf(const char *format, ...);

#endif /* _SYSMEM_H */
