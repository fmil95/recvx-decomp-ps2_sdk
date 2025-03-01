/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                     3DIcon file converter.
 *
 *                         Version 1.0.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *      Name : iconconv.h
 *      Description : 
 * 
 *      Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      1.0.0          Dec,3,1999     munekis     1st version
 */

#ifndef __ICONCONV_H__
#define __ICONCONV_H__

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>



// ---- defines ----
#define VERSION_ID 0x00010000

// ---- macros ----
#define ITOF10(x) ((float)(x) / 1024.0f)
#define ITOF12(x) ((float)(x) / 4096.0f)
#define FTOI10(x) ((short)((x) * 1024.0f))
#define FTOI12(x) ((short)((x) * 4096.0f))



// ---- prototypes ----
int pad_fprintf(FILE *, char *, ...);
int usage(void);
int DumpIconFile(FILE*, u_char *, u_int);
int DumpBinaryIconFile(FILE*, u_char *, u_int);
int My_fwrite(void *, int, int, FILE *);

#endif
