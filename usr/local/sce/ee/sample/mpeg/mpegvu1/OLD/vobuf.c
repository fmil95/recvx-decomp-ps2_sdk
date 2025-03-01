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
 *                            vobuf.c
 *             functions for video output buffer
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.10           12.17.1999      umemura     the first version
 *       0.20           01.17.2000      umemura     vu1
 */
#include <eekernel.h>
#include "defs.h"
#include "vobuf.h"

// ////////////////////////////////////////////////////////////////
//
// Functions called from decoding thread
//
int voBufCreate(
    VoBuf *f,
    VoData *data,
    VoTag *tagProg,
    VoTag *tagInter,
    VoTag *tagTop,
    VoTag *tagBot,
    int size
)
{
    int i;

    f->data = data;
    f->tagProg = tagProg;
    f->tagInter = tagInter;
    f->tagTop = tagTop;
    f->tagBot = tagBot;

    f->tag = tagProg; // caution
    f->size = size;
    f->count = 0;
    f->write = 0;

    for (i = 0; i < size; i++) {
        f->tagProg[i].status = VOBUF_STATUS_EMPTY;
        f->tagInter[i].status = VOBUF_STATUS_EMPTY;
        f->tagTop[i].status = VOBUF_STATUS_EMPTY;
        f->tagBot[i].status = VOBUF_STATUS_EMPTY;
    }
}

void voBufDelete(VoBuf *f)
{
}

int voBufReset(VoBuf *f)
{
    f->count = 0;
    f->write = 0;
}

int voBufIsFull(VoBuf *f)
{
    return f->count == f->size;
}

void voBufIncCount(VoBuf *f)
{
    // disable interrupt
    DI();

    f->tag[f->write].status = VOBUF_STATUS_FULL;
    f->count++;
    f->write = (f->write + 1) % f->size;

    // enable interrupt
    EI();
}

VoData *voBufGetData(VoBuf *f)
{
    return voBufIsFull(f)? (VoData*)NULL: f->data + f->write;
}

// ////////////////////////////////////////////////////////////////
//
// Functions called from interrupt handler
//
int voBufIsEmpty(VoBuf *f)
{
    return f->count == 0;
}

VoTag *voBufGetTag(VoBuf *f)
{
    return voBufIsEmpty(f)? (VoTag*)NULL:
    	f->tag + ((f->write - f->count + f->size) % f->size);
}

void voBufDecCount(VoBuf *f)
{
    if (f->count > 0) {
	f->count--;
    }
}

