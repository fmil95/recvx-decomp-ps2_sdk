/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 1.6
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
 *                            disp.h
 *                     header file for Display
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.10           12.17.1999      umemura     the first version
 *       0.20           01.19.2000      umemura     members are modified
 */
#ifndef _DISP_H_
#define _DISP_H_

#include <libgraph.h>

// ////////////////////////////////////////////////////////////////
//
// Display
//
typedef struct _Display {
    sceGsDispEnv dispenv;
    int fbp0;
    int fbp1;
    int fb_w;
    int fb_h;
} Display;

// ////////////////////////////////////////////////////////////////
//
// Functions
//
void dispCreate(
    Display *d,
    int fb_w,
    int fb_h
);
void dispDelete(Display *d);
void dispSetTags(
    Display *d, u_int *tags, void *image, int index,
    int disp_x, int disp_y,
    int disp_w, int disp_h,
    int image_w, int image_h
);
void dispSwitch(Display *d, int id);
void dispClear(Display *d, u_int val);
void startDisplay(int waitEven);
void endDisplay();

#endif _DISP_H_
