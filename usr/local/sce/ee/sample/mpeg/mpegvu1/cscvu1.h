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
 *                            cscvu1.h
 *              header file for video output buffer
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.10           01.19.2000      aoki        the first version
 *       0.10           01.19.2000      umemura     API is modified
 */
#ifndef _CSCVU1_H_
#define _CSCVU1_H_

#include <eetypes.h>

#define CSCVU1_LASTKICK 0x7FC

#define CSCVU1_INTER_FRAME 0
#define CSCVU1_FIELD       1
#define CSCVU1_PROG_FRAME  2

#define PROG_TAG_SIZE		bound(25661 * 16/4 + 2, 16)
#define INTER_TAG_SIZE		bound(25661 * 16/4 + 2, 16)
#define TOP_TAG_SIZE		bound(20261 * 16/4 + 2, 16)
#define BOT_TAG_SIZE		bound(20261 * 16/4 + 2, 16)

// ////////////////////////////////////////////////////////////////
//
//  Color space conversion on VU1
//
typedef struct {
    u_int *micro[3][2];
} CscVu1;

// ////////////////////////////////////////////////////////////////
//
//  Functions
//
void cscVu1Init(CscVu1 *csc);
void cscVu1SetTag(u_int *tags,
			int type, void *image, int width, int height);
void cscVu1Xyz2offset(CscVu1 *csc, int type,
			int isBottom, int xoff, int yoff);
void cscVu1Kick(u_int *tag);

// Dma tag
extern u_int vu1_init __attribute__((section(".vudata")));

// color trans matrix
extern u_int vu1_ColMatrix __attribute__((section(".vudata")));

// color offset
extern u_int vu1_Trans __attribute__((section(".vudata")));

// XYZ2 offset address
extern u_int vu1_XYZ2Offset __attribute__((section(".vudata")));

// micro Program for top field of interlace frame
extern u_int load_yuvfrfl0_mpg __attribute__((section(".vudata")));

// micro Program for bottom field of interlace frame
extern u_int load_yuvfrfl1_mpg __attribute__((section(".vudata")));

// micro Program for top field of progressive frame
extern u_int load_yuvprg0_mpg __attribute__((section(".vudata")));

// micro Program for bottom field for progressive mode
extern u_int load_yuvprg1_mpg __attribute__((section(".vudata")));

// micro Program for top field for field mode
extern u_int load_yuvfl0_mpg __attribute__((section(".vudata")));

// micro Program for bottom field for field mode
extern u_int load_yuvfl1_mpg __attribute__((section(".vudata")));

#endif // _CSCVU1_H_
