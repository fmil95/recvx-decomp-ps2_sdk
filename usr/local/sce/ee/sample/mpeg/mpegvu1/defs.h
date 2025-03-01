/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                       - mpeg csc on vu1 -
 *
 *                         Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            defs.h
 *           global definitions for mpeg streaming program
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.10           12.17.1999      umemura     the first version
 *       0.20           01.17.2000      umemura     vu1
 */
#ifndef _DEFS_H_
#define _DEFS_H_

#include <eeregs.h>
#include <eetypes.h>

#define UNCMASK 0x0fffffff
#define UNCBASE 0x20000000

#define DISP_WIDTH 640
#define DISP_HEIGHT 448
#define MAX_WIDTH 720
#define MAX_HEIGHT 480

#define N_LDTAGS (MAX_WIDTH/16 * MAX_HEIGHT/16 * 6 + 10)
#define TS_NONE (-1)

#define bound(val, x) ((((val) + (x) - 1) / (x))*(x))
#define min(x, y) (((x) > (y))? (y): (x))
#define max(x, y) (((x) < (y))? (y): (x))
#define bss_align(val) \
    __attribute__ ((aligned(val))) __attribute__ ((section (".bss")))

void ErrMessage(char *message);
void switchThread();
void proceedAudio();

extern inline void *DmaAddr(void *val)
{
    return (void*)((u_int)val & UNCMASK);
}

extern inline void *UncAddr(void *val)
{
    return (void*)(((u_int)val & UNCMASK)|UNCBASE);
}

#endif // _DEFS_H_
