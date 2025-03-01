/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *
 *                         Version 0.5.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : joint matrix interpolation mkdata
 *                     Description : header file for axis.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.5.0          Aug,16,1999     aoki
 */

extern void mat2axis(sceVu0FVECTOR a0, sceVu0FVECTOR t0, sceVu0FMATRIX m0, sceVu0FMATRIX m1, u_int n);
extern void axis2mat(sceVu0FMATRIX m2, sceVu0FMATRIX m0, sceVu0FVECTOR a0, sceVu0FVECTOR t0, u_int n);
extern void chkerror(sceVu0FMATRIX m1, sceVu0FMATRIX m2);
