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
 *                     Name : joint matrix interpolation runtime
 *                     Description : header file for matintr.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.5.0          Aug,16,1999     aoki
 */
typedef struct {
    sceVu0FVECTOR axis;
    sceVu0FVECTOR trans;
} MatIntrDiffData;

typedef struct {
    sceVu0FVECTOR axis;
    float trans[3];
    u_int cnt;
} MatIntrDiffData2;

typedef struct {
    MatIntrDiffData *diffdata;
    int diffdatanum;
} MatIntrData;

typedef struct {
    MatIntrData data;
    sceVu0FMATRIX currentR;
    sceVu0FMATRIX diffR;
    sceVu0FVECTOR currentT;
    sceVu0FVECTOR diffT;
    int cnt;
} MatIntr;


void MatIntrInit(MatIntr *mi, sceVu0FMATRIX startm, MatIntrDiffData2 *d, int num);
extern void MatIntrLoadMatrix(sceVu0FMATRIX m, MatIntr *mi);
void SetTransMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1, sceVu0FVECTOR tv);
