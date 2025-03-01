/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                        - easy movie -
 *
 *                         Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            ezmovie.h
 *                    header file for ezmovie
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.10           Mar.29.1999     umemura     the first version
 */

#ifndef _EZMOVIE_H_
#define _EZMOVIE_H_

#define DISP_WIDTH  640
#define DISP_HEIGHT 448
#define MAX_WIDTH   720
#define MAX_HEIGHT  480

#define bound(val, x) ((((val) + (x) - 1) / (x))*(x))
#define min(x, y)	(((x) < (y))? (x): (y))

#define bss_align(val) \
    __attribute__ ((aligned(val))) __attribute__ ((section (".bss")))
#define data_align(val) __attribute__ ((aligned(val)))

#define UNCMASK 0x0fffffff
#define UNCBASE 0x20000000
#define DmaAddr(val) \
    ((void*)((unsigned int)(val) & UNCMASK))
#define UncachedAddr(val) \
    ((void*)(((unsigned int)(val) & UNCMASK) | UNCBASE))

#endif /* _EZMOVIE_H_ */

