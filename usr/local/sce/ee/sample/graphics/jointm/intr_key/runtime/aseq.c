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
 *                     Description : read axis sequence data
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.5.0          Aug,16,1999     aoki
 */

#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <libdev.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <sifdev.h>
#include <libvu0.h>
#include "matintr.h"

#define MATRIXNAME00 sceVu0FMATRIX aseqstart0
#define DIFFNAME00 MatIntrDiffData2 aseqdiff0
#define MATRIXNAME01 sceVu0FMATRIX aseqstart1
#define DIFFNAME01 MatIntrDiffData2 aseqdiff1
#define MATRIXNAME02 sceVu0FMATRIX aseqstart2
#define DIFFNAME02 MatIntrDiffData2 aseqdiff2
#define MATRIXNAME03 sceVu0FMATRIX aseqstart3
#define DIFFNAME03 MatIntrDiffData2 aseqdiff3
#define MATRIXNAME04 sceVu0FMATRIX aseqstart4
#define DIFFNAME04 MatIntrDiffData2 aseqdiff4
#define MATRIXNAME05 sceVu0FMATRIX aseqstart5
#define DIFFNAME05 MatIntrDiffData2 aseqdiff5
#define MATRIXNAME06 sceVu0FMATRIX aseqstart6
#define DIFFNAME06 MatIntrDiffData2 aseqdiff6
#define MATRIXNAME07 sceVu0FMATRIX aseqstart7
#define DIFFNAME07 MatIntrDiffData2 aseqdiff7
#define MATRIXNAME08 sceVu0FMATRIX aseqstart8
#define DIFFNAME08 MatIntrDiffData2 aseqdiff8
#define MATRIXNAME09 sceVu0FMATRIX aseqstart9
#define DIFFNAME09 MatIntrDiffData2 aseqdiff9


int keynum=5;
#include "axisseq.dat"
