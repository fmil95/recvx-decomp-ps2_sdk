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
 *                          Name : defines.h
 *
 *            Version   Date           Design      Log
 *  --------------------------------------------------------------------
 *            0.30      Apr,4,2000     thatake     Beta0
 *            0.40      Jun,16,2000     	   +animation, clut
 */

#define MODELSIZE_MAX (1024*1024*16)	    /* max model data file size (bytes) */

#define KICK_SIZE 256			    /* max order table size (qwords)  */
#define PACK_SIZE 65536			    /* max model data flow list size (qwords) */
#define SHAPE_PACK_SIZE (MODELSIZE_MAX/256) /* max sub-model_data_flow_list size (qwords) */

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 224


#define ERR_STOP          while(1)

extern char err_msg[];
extern void ErrPrintf(char *fmt, ...);

#define PI 3.14159265f

#define CACHED 0
#define UNCACHED 0x20000000
#define UNCACHED_ACCELARATED 0x30000000
#define PADDR_MASK 0x0fffffff

#define Cached(x) ((u_int)(x)|CACHED)
#define Uncached(x) ((u_int)(x)|UNCACHED)
#define UncachedA(x) ((u_int)(x)|UNCACHED_ACCELARATED)
#define Paddr(x) ((u_int)(x)&PADDR_MASK)

#define bound(x,a) ((((int)(x)+(a)-1)/(a))*(a))
