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
 *      Name : error.c
 *      Description : 
 * 
 *      Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      1.0.0          Dec,3,1999     munekis     1st version
 */

#include <stdio.h>
#include "error.h"

int error_msg(char *fmt, ...) {
    va_list args;
    fprintf(stderr, "(file : %s line : %d)", __FILE__, __LINE__);
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);

    return 0;
}


int log_printf(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vfprintf(stdout, fmt, args);
    va_end(args);

    return 0;
}

