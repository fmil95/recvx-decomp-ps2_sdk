/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - multi movie -
 *
 *                         Version 1.20
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            ezmovie.h
 *                    header file for ezcube3
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.00           July.21.1999    ywashizu    the first version
 *       1.10           Aug. 13.1999    ywashizu    new IPU movie format
 *       1.20           Oct, 15,1999    ywashizu    new IPU movie format
 */

#ifndef _EZMOVIE_H_
#define _EZMOVIE_H_

// COMPARE_ON flag doesn't work with this sample program
/* #define COMPARE_ON*/

#define DISP_WIDTH 640
#define DISP_HEIGHT 480

#define MAX_WIDTH 640
#define MAX_HEIGHT 480
#define MAX_BsDataSize (3*1024*1024)

#define EZFILE0 "host:ez.ipu"
#define EZFILE1 "host:rugby.ipu"
#define EZFILE2 "host:yama.ipu"
#define EZFILE3 "host:456.ipu"
#define EZFILE4 "host:1_6.ipu"
#define EZFILE5 "host:wada.ipu"

#define bound(val, x) ((((val) + (x) - 1) / (x))*(x))
#define min(x, y)	(((x) < (y))? (x): (y))

#define bss_align(val) \
    __attribute__ ((aligned(val))) __attribute__ ((section (".bss")))
#define data_align(val) __attribute__ ((aligned(val)))

#define DmaAddr(val) val
#define UncachedAddr(val) ((void*)((unsigned int)(val) | 0x20000000))

#endif /* _EZMOVIE_H_ */

