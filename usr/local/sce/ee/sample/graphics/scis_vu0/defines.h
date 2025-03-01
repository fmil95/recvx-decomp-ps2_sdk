/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                    - scissoring CORE&VU0  -
 *
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : defines.h
 *
 *       Version      Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.5.0        Sep.22.1999     thatake     1st Version
 */

typedef union {
  u_long128   ul128;
  u_long      ul64[2];
  u_int       ui32[4];
}QWdata;

typedef struct {
  sceGifTag giftag;
  sceGsTest gs_test;
  long gs_test1addr;
  sceGsAlphaEnv gs_alpha;
  sceGsTexEnv gs_tex;
}TexEnv;

typedef struct{
  QWdata   buf[2048];
//  QWdata   *buf;
  u_int    size;
//  u_int    *pBase;
}GifDmaPacket;

typedef struct{
  u_int  *pDataHead; 
  u_int  magic;
  u_int  prim;
  u_int  blockNum;
  u_int  dataNum;
  u_int  psumNum;
  u_int  *vertexNum;
  u_int  *polyNum;
  float  **pData;
  GifDmaPacket *pack;
  GifDmaPacket *packbuf;
}ObjData;

typedef struct{
  sceVu0FVECTOR vertex;
  sceVu0FVECTOR normal;
  sceVu0FVECTOR color;
  sceVu0FVECTOR texUV;
  sceVu0FVECTOR clipV;
}Node;

typedef struct{
  Node node[10];
  int  nodeNum;
}NodeArray;

typedef struct{
  NodeArray  triangle;
  NodeArray  narray[2];
  NodeArray  *in;
  NodeArray  *out;
  int        rotflag;// 0 to 3
  int        flipflag;// 0 or 1
}NodeArraySet;

typedef struct{
  int              xyzflag;
  u_int            clipmask;
//  sceVu0FVECTOR    normal;
}ScissorPlane;

typedef struct{
  ScissorPlane plane[6];
  int          planeNum;
}ScissorPlaneArray;


