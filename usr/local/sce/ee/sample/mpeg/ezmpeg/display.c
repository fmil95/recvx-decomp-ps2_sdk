/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                        - easy mpeg -
 *
 *                         Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            display.c
 *                   functions for displaying images
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.10           Aug.14.1999     umemura     the first version
 */

#include <stdio.h>
#include <libgraph.h>

volatile int isCountVblank = 0;
volatile int vblankCount = 0;

// //////////////////////////////////////
// 
//  Vblank handler
// 
int vblankHandler(int val)
{
    if (isCountVblank) {
    	vblankCount++;
    }
    ExitHandler();
    return 0;
}

// //////////////////////////////////////
// 
//  Wait until even/odd field
//  Start to count vblank
// 
void startDisplay(int waitEven)
{
    // wait untill even field
    while (sceGsSyncV(0) == waitEven)
    	;

    isCountVblank = 1;
    vblankCount = 0;
}

// //////////////////////////////////////
// 
//  Stop to count vblank
// 
void endDisplay()
{
    isCountVblank =  0;
}

// //////////////////////////////////////
// 
//  Wait until expected vblank count
// 
void syncDisplay(int frame)
{
    if (frame < (vblankCount >> 1)) {
        printf("[frame drop] frame %d\n", frame);
	vblankCount = (frame << 1);
    }
    // wait until two vblanks happen
    while (frame > (vblankCount >> 1))
    	;
}

