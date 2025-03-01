/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - multi movie -
 *
 *                         Version 1.20
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            ezcube.h
 *                    header file for ezcube3
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.00           Aug. 13.1999    ywashizu    the first version
 *       1.20           Oct, 15,1999    ywashizu    new IPU movie format
 */


#ifndef _EZCUBE_H
#define _EZCUBE_H

#include <stdlib.h>
#include <stdio.h>

#include <libgraph.h>
#include <libdma.h>
#include <libpkt.h>
#include <libvu0.h>
#include <libipu.h>

#include "ezmovie.h"

#define PACKETSIZE 0x10000
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 224
#define PI (3.141592f)

#define SETVECTOR(v,x,y,z,w) ((v)[0]=x,(v)[1]=y,(v)[2]=z,(v)[3]=w)

#define CUBE_HEIGHT_H 80.0f
#define CUBE_WIDTH_H  80.0f
#define CUBE_DEPTH_H  80.0f

#define WALL_HEIGHT_H 150.0f
#define WALL_WIDTH_H  200.0f
#define WALL_DEPTH_H  250.0f

#define TRANS_X 0.0f
#define TRANS_Y 0.0f
#define TRANS_Z 0.0f
#define TRANS_W 0.0f

#define ROT_X 0.8f
#define ROT_Y 0.8f
#define ROT_Z 0.0f
#define ROT_W 0.0f

#define TEX0 6720
#define TEX1 (6720 + 1280) 
#define TEX2 (6720 + 1280*2)
#define TEX3 (6720 + 1280*3)
#define TEX4 (6720 + 1280*4)
#define TEX5 (6720 + 1280*5)

#define CUBE_AA1 0                        // Anti-Aliasing for cube OFF 
#define WALL_AA1 1                        // Anti-Aliasing for frame ON 

#define DTD(flags)              (((flags) >> 2) & 0x1)
#define IPU_CTRL_VAL(flags)     (((flags) & ~0x00000004) << 16)

#define MAX_MBX		(MAX_WIDTH/16)
#define MAX_MBY		(MAX_HEIGHT/16)
#define PK_SIZE	(MAX_MBX * MAX_MBY * 6 + 10)
#define DMASIZE	(1024*1024 - 16)

typedef struct{
    int datasize;
    u_int width;
    u_int height;
    u_int nframes;
    u_int frame_cnt;
    u_int mbx;
    u_int mby;
    u_int origin_x;
    u_int origin_y;
    u_int texbp;
    u_int texbw;
    int tw;
    int th;
    sceIpuDmaEnv env;
}IDEC_MOVIE;

void SetRGB(sceVu0IVECTOR* c0, sceVu0FMATRIX LocalLightMatrix, 
	    sceVu0FMATRIX LightColorMatrix, sceVu0FVECTOR* v1, 
	    sceVu0FVECTOR* c1);

void create_normal(sceVu0FVECTOR normal, sceVu0FVECTOR v0, 
		   sceVu0FVECTOR v1, sceVu0FVECTOR v2);

void light_rgb(sceVu0IVECTOR c0, sceVu0FMATRIX LocalLightMatrix, 
	       sceVu0FMATRIX LightColorMatrix, sceVu0FVECTOR v0, 
	       sceVu0FVECTOR v1, sceVu0FVECTOR v2, sceVu0FVECTOR c1);

void SetCubeVertex(sceVu0FVECTOR* v1);

void SetWallVertex(sceVu0FVECTOR* wallF);

void SetObjectColor(sceVu0FVECTOR* c1);

void create_viewworld(sceVu0FMATRIX ViewWorldMatrix, sceVu0FVECTOR camera_rot,
		      sceVu0FVECTOR camera_p, sceVu0FVECTOR camera_zd, 
		      sceVu0FVECTOR camera_yd);

void set_square_packet(sceVif1Packet* packet,  sceVu0IVECTOR c,
			sceVu0IVECTOR v0, sceVu0IVECTOR v1, sceVu0IVECTOR v2,
			sceVu0IVECTOR v3, IDEC_MOVIE movie);

void set_line_packet(sceVif1Packet* packet, sceVu0IVECTOR c,
		     sceVu0IVECTOR v0, sceVu0IVECTOR v1);

void SetGsRegs(int alpha_on, int tex_on);

void ReadIPUHeader(IDEC_MOVIE* movie, u_char* fileData, u_int texbp);

u_int getFlags(void);

#endif _EZCUBE_H
