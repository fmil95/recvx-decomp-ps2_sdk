/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                       - mpeg on memory -
 *
 *                         Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            mpegmem.h
 *                    header file for mpegmem
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.10           Aug.14.1999     umemura     the first version
 */

#ifndef _MPEGMEM_H_
#define _MPEGMEM_H_

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

void setLoadImageTags(u_int *tags, void *image,
	int x, int y, int  width, int height);
void setLoadImageTagsTile(u_int *tags, void *image,
	int x, int y, int  width, int height);
void loadImage(u_int *tags);
int vblankHandler(int val);
void startDisplay(int waitEven);
void endDisplay();
void syncDisplay(int frame);

#endif /* _MPEGMEM_H_ */

