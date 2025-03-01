/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - vu0 -
 *
 *                         Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.10
 */

.global SampleCubeDataHead
.global SampleCubeDataBlock0

.data
.align 4
/*
SceSampleCubeDataHead:
.word 0x<MAGIC>, 0x00000000, 0x<block num|PRIM>, 0x<data num>
*/
SampleCubeDataHead:
.word 0x00000001, 0x00000000, 0x00010014, 0x0000000e

SampleCubeDataBlock0:
.word 0x0000000e, 0x00000000, 0x00000000, 0x00000000

/* SceSampleDataBlock0 vertex */
.float -5.0, -5.0, 5.0, 1.0
.float -5.0, -5.0, -5.0, 1.0
.float -5.0, 5.0, 5.0, 1.0
.float -5.0, 5.0, -5.0, 1.0
.float 5.0, 5.0, -5.0, 1.0
.float -5.0, -5.0, -5.0, 1.0
.float 5.0, -5.0, -5.0, 1.0
.float -5.0, -5.0, 5.0, 1.0
.float 5.0, -5.0, 5.0, 1.0
.float -5.0, 5.0, 5.0, 1.0
.float 5.0, 5.0, 5.0, 1.0
.float 5.0, 5.0, -5.0, 1.0
.float 5.0, -5.0, 5.0, 1.0
.float 5.0, -5.0, -5.0, 1.0

/* SceSampleDataBlock0 normal */
.float -1.0, 0.0, 0.0, 1.0
.float -1.0, 0.0, 0.0, 1.0
.float -1.0, 0.0, 0.0, 1.0
.float -1.0, 0.0, 0.0, 1.0
.float 0.0, 1.0, 0.0, 1.0
.float 0.0, 0.0, -1.0, 1.0
.float 0.0, 0.0, -1.0, 1.0
.float 0.0, -1.0, 0.0, 1.0
.float 0.0, -1.0, 0.0, 1.0
.float 0.0, 0.0, 1.0, 1.0
.float 0.0, 0.0, 1.0, 1.0
.float 0.0, 1.0, 0.0, 1.0
.float 1.0, 0.0, 0.0, 1.0
.float 1.0, 0.0, 0.0, 1.0

/* SceSampleDataBlock0 texture */
.float 0.0, 0.0, 1.0, 0.0
.float 0.0, 1.0, 1.0, 0.0
.float 1.0, 0.0, 1.0, 0.0
.float 1.0, 1.0, 1.0, 0.0
.float 0.0, 1.0, 1.0, 0.0
.float 1.0, 0.0, 1.0, 0.0
.float 0.0, 0.0, 1.0, 0.0
.float 1.0, 1.0, 1.0, 0.0
.float 0.0, 1.0, 1.0, 0.0
.float 1.0, 0.0, 1.0, 0.0
.float 0.0, 0.0, 1.0, 0.0
.float 0.0, 1.0, 1.0, 0.0
.float 1.0, 0.0, 1.0, 0.0
.float 1.0, 1.0, 1.0, 0.0

/*SceSampleDataBlock0 color */
.float 255.0, 255.0, 255.0, 127.0
.float 255.0, 255.0, 255.0, 127.0
.float 255.0, 255.0, 255.0, 127.0
.float 255.0, 255.0, 255.0, 127.0
.float 255.0, 255.0, 255.0, 127.0
.float 255.0, 255.0, 255.0, 127.0
.float 255.0, 255.0, 255.0, 127.0
.float 255.0, 255.0, 255.0, 127.0
.float 255.0, 255.0, 255.0, 127.0
.float 255.0, 255.0, 255.0, 127.0
.float 255.0, 255.0, 255.0, 127.0
.float 255.0, 255.0, 255.0, 127.0
.float 255.0, 255.0, 255.0, 127.0
.float 255.0, 255.0, 255.0, 127.0


