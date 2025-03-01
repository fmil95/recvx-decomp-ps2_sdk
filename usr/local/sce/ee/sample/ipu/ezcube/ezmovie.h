/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                        - easy movie -
 *
 *                         Version 0.30
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
 *       0.20           Aug.13.1999     ywashizu    new IPU movie format
 *       0.30           Oct,15,1999     ywashizu    new IPU movie format
 */

#ifndef _EZMOVIE_H_
#define _EZMOVIE_H_

// COMPARE_ON flag doesn't work with this sample program
/* #define COMPARE_ON*/

#define DISP_WIDTH 640
#define DISP_HEIGHT 480
#define MAX_WIDTH 640
#define MAX_HEIGHT 480
#define MAX_BsDataSize (10*1024*1024) // in bytes

#define PLAYCOUNT 5

#define BSFILE "host:1_6.ipu"

#define bound(val, x) ((((val) + (x) - 1) / (x))*(x))
#define min(x, y)	(((x) < (y))? (x): (y))

#define bss_align(val) \
    __attribute__ ((aligned(val))) __attribute__ ((section (".bss")))
#define data_align(val) __attribute__ ((aligned(val)))

#define DmaAddr(val) val
#define UncachedAddr(val) ((void*)((unsigned int)(val) | 0x20000000))

#endif /* _EZMOVIE_H_ */

