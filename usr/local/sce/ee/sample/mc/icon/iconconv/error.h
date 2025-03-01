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
 *      Name : error.h
 *      Description : 
 * 
 *      Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      1.0.0          Dec,3,1999     munekis     1st version
 */


#ifndef __MY_ERROR_H__
#define __MY_ERROR_H__

#include <stdarg.h>


// ---- prototypes ----
int error_msg(char*, ...);
int log_printf(char*, ...);
#define ERRORMSG0(x)     error_msg(x)
#define ERRORMSG1(x, y)  error_msg(x, y)

// ---- macros ----
// ==== DEBUG 
#ifdef __DEBUG__

#define LOG0(x)     log_printf(x)
#define LOG1(x, y)  log_printf(x, y)

// ==== NO DEBUG 
#else 

#define LOG0(x)
#define LOG1(x, y) 

#endif



#endif // __MY_ERROR_H__



