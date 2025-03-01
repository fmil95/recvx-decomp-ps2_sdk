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
 *                          Name : framebuf.h
 *
 *            Version   Date           Design      Log
 *  --------------------------------------------------------------------
 *            0.40      Jun,16,2000    aoki 	   
 */



/* initial texel base point (forward) */
#define INIT_TBP 0x1a40
/* initial CLUT base point (backward) */
#define INIT_CBP 0x4000

/* GS size for resident texture */
#define TEXEL_RES_SIZE 0x1000
#define CLUT_RES_SIZE 0x40


#define FW_GS_ALIGN_ANY (-1)
#define FW_GS_ALIGN_PAGE (1)

extern u_int getClutGsSize(u_int psm);
extern u_int getTexGsSize(u_int bw, u_int psm, u_int height);
extern u_int getCbp(u_int gssize, int align, int resident);
extern u_int getTbp(u_int gssize, int resident);
