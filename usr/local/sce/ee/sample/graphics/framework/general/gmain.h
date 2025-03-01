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
 *                          Name : gmain.h
 *
 *            Version   Date           Design      Log
 *  --------------------------------------------------------------------
 *            0.30      Apr,4,2000     thatake     Beta0
 *            0.40      Jun,16,2000     	   +animation, clut
 */

#ifndef _GMAIN_H_
#define _GMAIN_H_

/* global functions in gmain.c */
extern void g_clear_model();
extern void g_finish(void);
extern void g_init();
extern void g_main(u_int  frame, u_int paddata);
extern void g_read_model(u_int *data);
extern void g_reset_camera(void);
extern void g_sync(u_int frame, int odev);
extern void DrawStrings(int x, int y, char *fmt, ...);

#endif /* !_GMAIN_H_ */
