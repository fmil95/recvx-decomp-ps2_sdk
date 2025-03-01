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
 *      Name : parse.h
 *      Description : 
 * 
 *      Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      1.0.0          Dec,3,1999     munekis     1st version
 */


#ifndef __PARSE_H__
#define __PARSE_H__


// --- struct for parsing data ---
typedef struct {
    u_int size;
    u_int index;
    u_char *data;
} parse_data_t;



// --- prototypes ---
int GetToken(parse_data_t*, char*);

#endif
