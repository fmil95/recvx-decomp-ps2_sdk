/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                 Run-Length encode/decode(2bytes based)
 *
 *                         Version 1.0.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *      Name : rle.h
 *      Description : 
 * 
 *      Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      1.0.0          Dec,3,1999     munekis     1st version
 */


#ifndef __RLE__
#define __RLE__

int RleDecode(u_char *in_data, u_int size, 
              u_char *out_data, u_int *out_size);
int RleEncode(u_char *in_data, u_int size, 
              u_char *out_data, u_int *out_size);

#endif
