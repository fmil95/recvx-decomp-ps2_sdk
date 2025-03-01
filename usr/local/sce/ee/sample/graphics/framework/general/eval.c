/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *               Graphics Framework Sample Program
 *                  
 *              ---- Scene Viewer Version 0.40 ----
 * 
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                          Name : eval.c
 *
 *            Version   Date           Design      Log
 *  --------------------------------------------------------------------
 *            0.40      Jun,21,2000    kaneko
 */
#include <stdio.h>
#include <stdlib.h>
#include <eekernel.h>
#include <eeregs.h>
#include <math.h>
#include <libgraph.h>
#include <libdma.h>
#include <malloc.h>



 
float linear(float t, float a, float b)
{
  float f;

  f = (1.0f - t)*a + t*b;
  return f;
}
