/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *	pstypes
 *	Version:  1.00
 *	SJIS
 * *	  Copyright (C) 1999 Sony Computer Entertainment Inc.
 *	All Rights Reserved.
 *
 */
#ifndef __PSTYPES_H__
#define __PSTYPES_H__

typedef int LONG;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef BYTE* LPBYTE;
typedef WORD* LPWORD;
typedef DWORD* LPDWORD;
typedef int BOOL;
typedef unsigned long QWORD;

typedef struct {
	short x;
	short y;
	short w;
	short h;
} PSRECT;

typedef struct {
	DWORD mode;		/* pixel mode */
	PSRECT crect;		/* CLUT rectangle on frame buffer */
	LPDWORD caddr;		/* CLUT address on main memory */
	PSRECT prect;		/* texture image rectangle on frame buffer */
	LPDWORD paddr;		/* texture image address on main memory */
} TIM_IMAGE;

#define	TIMID	(0x10)
typedef	struct {
	DWORD id;
	DWORD flag;
	DWORD pbnum;
	WORD px;
	WORD py;
	WORD pw;
	WORD ph;
} TIM16;

#define setPSRECT(p0, x0, y0, w0, h0)	{(p0)->x = (x0); (p0)->y = (y0); (p0)->w = (w0); (p0)->h = (h0); };
#define setXY0(p0, x, y)	{(p0)->x0 = (x); (p0)->y0 = (y); }
#define setWH(p0, x, y)		{(p0)->w = (x); (p0)->h = (y); }
#define setUV(p0, x, y)		{(p0)->u = (x); (p0)->v = (y); }
#define setRGB0(p0, r, g, b)	(p0)->r0 = (r); (p0)->g0 = (g); (p0)->b0 = (b)

typedef struct {
	WORD x0;
	WORD y0;
	WORD w;
	WORD h;
	WORD u;
	WORD v;
	BYTE r0;
	BYTE g0;
	BYTE b0;
	BYTE a0;
	float fog;
	int tbase;
	QWORD tex0;
} SPRTX;

#endif 
