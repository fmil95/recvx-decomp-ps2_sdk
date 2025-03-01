/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 		Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
	                       			    All Rights Reserved.
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
  QWdata         dmatag;
  sceVu0FMATRIX  Local_world;
  sceVu0FMATRIX  Normal_light;
  sceVu0FMATRIX  World_screen;
  sceVu0FMATRIX  World_clip;
  sceVu0FMATRIX  Light_color;
//  sceVu0FMATRIX  Clip_screen;
  sceVu0FVECTOR  Point_light0;
  sceVu0FVECTOR  Point_light1;
  sceVu0FVECTOR  Point_color0;
  sceVu0FVECTOR  Point_color1;
  QWdata         Vifend;// MSCAL,BASE,OFFSET
}Vu1EnvPacket;

typedef struct{
  QWdata         dmatag;// DMAcall & VIFunpack
  sceVu0FMATRIX  Local_world;
  QWdata         viftag;// VIFunpack
//  sceVu0IVECTOR  Obj_prim;
  sceVu0FVECTOR  Light_point[3];
  sceVu0FVECTOR  Light_vector[3];
  QWdata         Vifend;// MSCAL,BASE,OFFSET
}Vu1ObjPacket;

typedef struct{
  Vu1ObjPacket  *objhead[2];
  Vu1ObjPacket  *current;
  Vu1EnvPacket  envhead;
  int           pkt_switch;
}Vu1DmaPacket;

typedef struct{
  QWdata   buf[512];
//  QWdata   *buf;
  u_int    size;
  u_int    *pBase;
}GifDmaPacket;
