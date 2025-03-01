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
 *                          Name : vu1frame.h
 *
 *            Version   Date           Design      Log
 *  --------------------------------------------------------------------
 *            0.30      Apr,4,2000     thatake     Beta0
 *            0.40      Jun,16,2000    aoki 	   +animation, clut
 */
#ifndef _VU1FRAME_H_
#define _VU1FRAME_H_

#include <libvu0.h>
#include "packbuf.h"

typedef struct{
    sceVu0FMATRIX world_screen;  
    sceVu0FMATRIX world_clip;    
    sceVu0FMATRIX clip_screen;   
    sceVu0FMATRIX init_data;     
    sceVu0FMATRIX light_vector0; 
    sceVu0FMATRIX light_point0;  
    sceVu0FMATRIX light_color0;  
    sceVu0FMATRIX light_vector1; 
    sceVu0FMATRIX light_point1;  
    sceVu0FMATRIX light_color1;  
    sceVu0FMATRIX light_vector2; 
    sceVu0FMATRIX light_point2;  
    sceVu0FMATRIX light_color2;  
    u_int*  pPackData;
    u_int*  pRawData;
    u_int   myID;
}MICRO_FRAME;

typedef struct{
    sceVu0FMATRIX local_world; /* vu1mem 0  */
    sceVu0FMATRIX light_rot;   /* vu1mem 4  */
    sceVu0IVECTOR giftag0;     /* vu1mem 8  */
    sceVu0IVECTOR giftag1;     /* vu1mem 9  */
    sceVu0IVECTOR prim0;       /* vu1mem 10 */
    sceVu0IVECTOR prim1;       /* vu1mem 11 */
    sceVu0FMATRIX calc_mat;    /* vu1mem 12 */
    struct _hier **hier;
    int hierNum;
    u_int*  pRawData;
    int vertNum;
    int texNum;
    int mateNum;
    int geomNum;
    u_int texID;
    u_int matID;
    u_int myID;
    sceVu0FVECTOR** pGeomVertex;
    sceVu0FVECTOR** pGeomNormal;
#if 0
    qword** pGeomVertex;
    qword** pGeomNormal;
#endif /* 0 */
    sceVu0FVECTOR bound_box[2];
}Shape;

typedef struct{
  int num;
  Shape *shape;
    u_int*  pPackData;
    u_int VertexTotal;
}SHAPE_FRAME;

typedef struct{
    qword GIFtag;
    qword Bitblt;
    qword Trxpos;
    qword Trxreg;
    qword Trxdir;
} TextTransRegs;    

typedef struct{
    qword GIFtexflush;
    qword Texflush;
    qword GIFtex01;
    qword Tex01;
    TextTransRegs texTransRegs;
    TextTransRegs clutTransRegs;
    qword texGIFtag;
    u_int *texpData;
    u_int texBp;	  /* TBP (texel base addr at GS) */
    u_int texGsSize;	  /* blocks (aligned page break) */
    u_int texTransSize;   /* qwords */
    qword clutGIFtag;
    u_int *clutpData;
    u_int clutBp;	   /* CBP (clut base addr at GS) */
    u_int clutGsSize;      /* blocks */
    u_int clutTransSize;   /* qwords */
    u_int*  pPackData;
    int resident;
    u_int myID;
    u_int pad0, pad1;
}Image;

typedef struct{
  int num;
  Image *image;
  float TransSizeTotal;
}TEXTURE_FRAME;


typedef struct _Sharedvn{
  u_int	bv,lv;		/* shared vertex info */
  u_int	bn,ln;		/* shared normal info */
}Sharedvn;


typedef struct _Hierarchy{
  sceVu0FVECTOR		trans;
  sceVu0FVECTOR		rot;
  sceVu0FVECTOR		scale;
    int shapeid;
    int parent;
    int child;		/* not yet */
    int sibling;	/* not yet */
  Sharedvn			sharedvn;
}Hierarchy;


typedef struct _hier{
    sceVu0FMATRIX local_world; /* vu1mem 0  */
    sceVu0FMATRIX light_rot;   /* vu1mem 4  */
    u_int myID;
} Hier;

typedef struct{
  int num;
    Hierarchy *hierarchy;
    Hier *hier;
}HIERARCHY_FRAME;

typedef struct _Animation{
  int	hierarchy;
  int	nkey;
  int	keyframe;
  int   keyvalue;
}Animation;


typedef struct _Share{
  int   srcidx;	/* src index array */
  int	dstidx;	/* dst index array */
  int   geomID;	/* vertex array ptr */
  int	num;		/* num of vertex index */
}Share;

/* for world-position */
extern Hier hierarchy_root;

/* for model data handling */
extern sceVu0FVECTOR *srcvertex;
extern sceVu0FVECTOR *srcnormal;
extern sceVu0FVECTOR *dstvertex;
extern sceVu0FVECTOR *dstnormal;

extern int nsrcvertex;
extern int nsrcnormal;
extern int ndstvertex;
extern int ndstnormal;

extern int nhierarchy;
extern Hierarchy *hierarchy;

extern int nanimation;
extern Animation *animation;

extern int **keyframe;
extern int nkeyframe;

extern float **keyvalue;
extern int nkeyvalue;

extern Share *sharevertex;
extern int nsharevertex;
extern int shareshapeidvertex;
extern int *indexvertex;

extern Share *sharenormal;
extern int nsharenormal;
extern int shareshapeidnormal;
extern int *indexnormal;


/* global functions */
extern void Vu1MicroSetWorldScreen(MICRO_FRAME *mic, sceVu0FMATRIX mat);
extern void Vu1MicroSetWorldClip(MICRO_FRAME *mic, sceVu0FMATRIX mat);
extern void Vu1MicroSetClipScreen(MICRO_FRAME *mic, sceVu0FMATRIX mat);
extern void Vu1MicroSetInitData(MICRO_FRAME *mic, sceVu0FMATRIX mat);
extern void Vu1MicroSetLightVect(MICRO_FRAME *mic,int field,
            sceVu0FVECTOR vect0,sceVu0FVECTOR vect1,sceVu0FVECTOR vect2);
extern void Vu1MicroSetLightPoint(MICRO_FRAME *mic,int field,
            sceVu0FVECTOR point0, sceVu0FVECTOR point1, sceVu0FVECTOR point2);
extern void Vu1MicroSetLightColor(MICRO_FRAME *mic,int field,
            sceVu0FVECTOR color0, sceVu0FVECTOR color1, 
            sceVu0FVECTOR color2, sceVu0FVECTOR color3); 

extern void Vu1ShapeGetGiftag(qword giftag, u_int attribute, u_int packsize);
extern int Vu1MicroInit(MICRO_FRAME *mic);
extern u_int* Vu1MicroMakePacket(MICRO_FRAME *mic, u_int *packbuf, 
                                  u_int *rawdata, int worksize);

extern void Vu1ShapeSetLocalWorld(Shape *shape, sceVu0FMATRIX mat);
extern void Vu1ShapeSetLightRot(Shape *shape, sceVu0FMATRIX mat);
extern void Vu1ShapeSetGiftag(Shape *shape,int field, sceVu0IVECTOR vec);
extern void Vu1ShapeSetPrim(Shape *shape, int field , sceVu0IVECTOR vec);
extern void Vu1ShapeSetCalcMat(Shape *shape, sceVu0FMATRIX mat);

extern int Vu1ShapeInit(SHAPE_FRAME *sf, u_int *data);
extern u_int* Vu1ResidentTextureMakePacket(TEXTURE_FRAME *tex, u_int* pWrite);
extern u_int* Vu1ShapeMakePacket(Shape *shape, TEXTURE_FRAME *tex, u_int* pWrite, int packsize);
extern u_int* Vu1ShapeFrameMakePacket(SHAPE_FRAME *sf, TEXTURE_FRAME *tex, u_int* pWrite, u_int worksize, HIERARCHY_FRAME *hf, PACKET_BUF *sp);

extern int Vu1TexInit(TEXTURE_FRAME *tex, u_int* data);


#endif /* !_VU1FRAME_H_ */
