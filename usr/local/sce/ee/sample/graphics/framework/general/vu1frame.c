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
 *                          Name : vu1frame.c
 *
 *            Version   Date           Design      Log
 *  --------------------------------------------------------------------
 *            0.30      Apr,4,2000     thatake     Beta0
 *            0.40      Jun,16,2000    aoki	   +clut
 */

#include <eekernel.h>
#include <stdlib.h>
#include <libdev.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <libvu0.h>
#include <malloc.h>


#include "defines.h"
#include "packbuf.h"
#include "vu1frame.h"
#include "framebuf.h"
#include "hier.h"


/* for world-position */
Hier hierarchy_root;

/* global pointer/number for model handling */
sceVu0FVECTOR *srcvertex=NULL;
sceVu0FVECTOR *srcnormal=NULL;
sceVu0FVECTOR *dstvertex=NULL;
sceVu0FVECTOR *dstnormal=NULL;

int nsrcvertex=0;
int nsrcnormal=0;
int ndstvertex=0;
int ndstnormal=0;

int nhierarchy=0;
Hierarchy *hierarchy=NULL;
int nanimation;
Animation *animation=NULL;

int **keyframe=NULL;
int nkeyframe=0;
float **keyvalue=NULL;
int nkeyvalue=0;

Share *sharevertex=NULL;
int nsharevertex=0;
int shareshapeidvertex= -1;
int *indexvertex=NULL;
Share *sharenormal=NULL;
int nsharenormal=0;
int shareshapeidnormal= -1;
int *indexnormal=NULL;


/* ///////////////////////////////////////////////////////
//
//   VU1micro_code packetting & data_management function
//
/////////////////////////////////////////////////////// */

inline void Vu1MicroSetWorldScreen(MICRO_FRAME *mic, sceVu0FMATRIX mat)
{  sceVu0CopyMatrix(mic->world_screen, mat);}

inline void Vu1MicroSetWorldClip(MICRO_FRAME *mic, sceVu0FMATRIX mat)
{  sceVu0CopyMatrix(mic->world_clip, mat);}

inline void Vu1MicroSetClipScreen(MICRO_FRAME *mic, sceVu0FMATRIX mat)
{  sceVu0CopyMatrix(mic->clip_screen, mat);}

inline void Vu1MicroSetInitData(MICRO_FRAME *mic, sceVu0FMATRIX mat)
{  sceVu0CopyMatrix(mic->init_data, mat);}

void Vu1MicroSetLightVect(MICRO_FRAME *mic, int field, 
				 sceVu0FVECTOR vect0, 
			  sceVu0FVECTOR vect1,sceVu0FVECTOR vect2)
{
  switch(field){
  case 0 :sceVu0NormalLightMatrix(mic->light_vector0, vect0, vect1, vect2);
    mic->light_vector0[3][3] = 1.0f;    break;
  case 1 :sceVu0NormalLightMatrix(mic->light_vector1, vect0, vect1, vect2);
    mic->light_vector1[3][3] = 1.0f;    break;
  case 2 :sceVu0NormalLightMatrix(mic->light_vector2, vect0, vect1, vect2);
    mic->light_vector2[3][3] = 1.0f;    break;
  }
}

void Vu1MicroSetLightPoint(MICRO_FRAME *mic,int field, sceVu0FVECTOR point0, 
			  sceVu0FVECTOR point1, sceVu0FVECTOR point2)
{  
  sceVu0FVECTOR zero = {0.0f, 0.0f, 0.0f, 0.0f};
  switch(field){
  case 0 : 
    sceVu0CopyVector(mic->light_point0[0], point0);
    sceVu0CopyVector(mic->light_point0[1], point1);
    sceVu0CopyVector(mic->light_point0[2], point2);
    sceVu0CopyVector(mic->light_point0[3], zero);
    break;
  case 1 :
    sceVu0CopyVector(mic->light_point1[0], point0);
    sceVu0CopyVector(mic->light_point1[1], point1);
    sceVu0CopyVector(mic->light_point1[2], point2);
    sceVu0CopyVector(mic->light_point0[3], zero);
    break;
  case 2 :
    sceVu0CopyVector(mic->light_point2[0], point0);
    sceVu0CopyVector(mic->light_point2[1], point1);
    sceVu0CopyVector(mic->light_point2[2], point2);
    sceVu0CopyVector(mic->light_point0[3], zero);
    break;
  }
}

void Vu1MicroSetLightColor(MICRO_FRAME *mic,int field, 
			   sceVu0FVECTOR color0, sceVu0FVECTOR color1,
			   sceVu0FVECTOR color2, sceVu0FVECTOR color3)
{  
  switch(field){
  case 0 :sceVu0LightColorMatrix(mic->light_color0, 
				 color0, color1, color2, color3); break;
  case 1 :sceVu0LightColorMatrix(mic->light_color1, 
				 color0, color1, color2, color3); break;
  case 2 :sceVu0LightColorMatrix(mic->light_color2, 
				 color0, color1, color2, color3); break;
  }
}

int Vu1MicroInit(MICRO_FRAME *mic)
{
  sceVu0FMATRIX unit, zero;

  sceVu0UnitMatrix(zero);
  zero[0][0] = 0.0f;  zero[1][1] = 0.0f;  
  zero[2][2] = 0.0f;  zero[3][3] = 0.0f;
  sceVu0UnitMatrix(unit);

  Vu1MicroSetWorldScreen(mic, unit);
  Vu1MicroSetWorldClip(mic, unit);
  Vu1MicroSetClipScreen(mic, unit);
  Vu1MicroSetInitData(mic, unit);
  Vu1MicroSetLightVect(mic, 0, unit[0], unit[1], unit[2]);
  Vu1MicroSetLightVect(mic, 1, unit[0], unit[1], unit[2]);
  Vu1MicroSetLightVect(mic, 2, unit[0], unit[1], unit[2]);
  Vu1MicroSetLightPoint(mic, 0, zero[0], zero[0], zero[0]);
  Vu1MicroSetLightPoint(mic, 1, zero[0], zero[0], zero[0]);
  Vu1MicroSetLightPoint(mic, 2, zero[0], zero[0], zero[0]);
  Vu1MicroSetLightColor(mic, 0, zero[0], zero[0], zero[0], zero[0]);
  Vu1MicroSetLightColor(mic, 1, zero[0], zero[0], zero[0], zero[0]);
  Vu1MicroSetLightColor(mic, 2, zero[0], zero[0], zero[0], zero[0]);

  return 0;
}

u_int* Vu1MicroMakePacket(MICRO_FRAME *mic, u_int *packbuf, 
			  u_int *rawdata, int worksize)
{
  qword *curr = (qword*)packbuf;
  mic->pPackData = packbuf;
  mic->pRawData = rawdata;

  
  /*               EEmem_addr        size */
  CALL_NOP(*curr,  mic->pRawData,     0  ); curr++; 
  /*                            EEmem_addr      VU1mem size  wl  cl */
  REF_STCYCL_UNPACK(*curr, (&mic->world_screen),  16,   52,   1,  1 ); curr++;
  /*                     base_addr       offset_size */
  RET_BASE_OFFSET(*curr,    120,      (1024-120-worksize)/2   ); curr++;

  return (u_int*)curr;
}


/* ///////////////////////////////////////////////////////
//
//  Shape_data packetting & data_management function
//
/////////////////////////////////////////////////////// */

inline void Vu1ShapeSetLocalWorld(Shape *shape, sceVu0FMATRIX mat)
{  sceVu0CopyMatrix(shape->local_world, mat); }

inline void Vu1ShapeSetLightRot(Shape *shape, sceVu0FMATRIX mat)
{  sceVu0CopyMatrix(shape->light_rot, mat); }

inline void Vu1ShapeSetGiftag(Shape *shape, int field, sceVu0IVECTOR vec)
{
  switch(field){
  case 0 : COPY_QW(shape->giftag0, vec);break;
  case 1 : COPY_QW(shape->giftag1, vec);break;
  }
}

inline void Vu1ShapeSetPrim(Shape *shape, int field , sceVu0IVECTOR vec)
{
  switch(field){
  case 0 : COPY_QW(shape->prim0, vec);break;
  case 1 : COPY_QW(shape->prim1, vec);break;
  }
}

inline void Vu1ShapeSetCalcMat(Shape *shape, sceVu0FMATRIX mat)
{  sceVu0CopyMatrix(shape->calc_mat, mat); }

int Vu1ShapeInit(SHAPE_FRAME *sf, u_int *data)
{
    int i, j, k;
    Shape *shape;
    u_int *currR = data;
    sceVu0FMATRIX unit;
    sceVu0IVECTOR ivect = {0, 0, 0, 0};
    u_int size;
    u_int *p;
    int geomNum;
    int geomidx;

    sf->num = currR[3];
    sf->shape = (Shape *)memalign(16, sizeof(Shape)*sf->num);
    if(sf->shape == NULL){
	ErrPrintf("cannot malloc shape\n");
	return -1;
    }
    sf->pPackData = NULL;
    sf->VertexTotal = 0;

    currR+=4;  /* skip top header(shapeNum) */

    for(i=0;i<sf->num;i++){
	shape= &sf->shape[i];
	shape->hierNum=currR[2];
	shape->hier = (Hier **)memalign(16, sizeof(Hier *)*shape->hierNum);
	if(shape->hier == NULL){
	    ErrPrintf("cannot malloc shape->hier\n");
	    return -1;
	}
	shape->hierNum=0;
	shape->pRawData = currR;

	shape->myID = currR[0];
	size=currR[1];
	shape->mateNum = currR[3];

	sceVu0UnitMatrix(unit);
	Vu1ShapeSetLocalWorld(shape, unit);
	Vu1ShapeSetLightRot(shape, unit);
	Vu1ShapeSetGiftag(shape, 0, ivect);
	Vu1ShapeSetGiftag(shape, 1, ivect);
	Vu1ShapeSetPrim(shape, 0, ivect);
	Vu1ShapeSetPrim(shape, 1, ivect);
	Vu1ShapeSetCalcMat(shape, unit);
	shape->vertNum = 0;
	shape->texNum = 0;
	shape->geomNum = 0;
	shape->texID = 0;
	shape->matID = 0;

	// count geomNum;
	p=currR+4;
	for (j=0; j<shape->mateNum; j++){	// each Material
	    geomNum=p[1];
	    shape->geomNum+=geomNum;		// count geomNum
	    p+=12;
	    for (k=0; k<geomNum; k++){		// each geometry
		p+=p[1]+4;			// geomSize + geom-header
	    }
	}

	// allocate pGeomVertex, pGeomNormal
	shape->pGeomVertex=(sceVu0FVECTOR **)memalign(16, sizeof(sceVu0FVECTOR *)*shape->geomNum);
	if(shape->pGeomVertex == NULL){
	    ErrPrintf("cannot malloc shape->pGeomVertex\n");
	    return -1;
	}
	shape->pGeomNormal=(sceVu0FVECTOR **)memalign(16, sizeof(sceVu0FVECTOR *)*shape->geomNum);
	if(shape->pGeomNormal == NULL){
	    ErrPrintf("cannot malloc shape->pGeomNormal\n");
	    return -1;
	}

	// set pGeomVertex, pGeomNormal
	p=currR+4;
	geomidx=0;
	for (j=0; j<shape->mateNum; j++){	// each Material
	    geomNum=p[1];
	    p+=12;				// skip material header, info
	    for (k=0; k<geomNum; k++){		// each geometry
      		shape->pGeomVertex[geomidx]=(sceVu0FVECTOR *)(p+4);
      		shape->pGeomNormal[geomidx]=(sceVu0FVECTOR *)(p+4+(p[3]*4)); // vertex num= p[3]
		geomidx++;
		p+=p[1]+4;			// skip geom-header + geomSize
	    }
	}


	currR+=(size+4); // shape header + body size
    }
    return 0;
}

void Vu1ShapeGetGiftag(qword giftag, u_int attribute, u_int packsize)
{
  u_int prim = 0x3ff & attribute;
  u_int type = 0x07 & attribute;
  /*  u_int shade = (attribute>>3) & 0x01; */
  u_int shade = 0x01; /* Gouraud on */
  u_int tex = (attribute>>4) & 0x01; 
  u_int fog = (attribute>>5) & 0x01; 
  /*  u_int anti = (attribute>>7) & 0x01; */
  u_int stuv = (attribute>>8) & 0x01;
  
  u_int rgb_regs = 0x01;
  u_int xyz_regs = 0x04; 
  u_int stuv_regs = 0x02; 
  u_int nloop = 0;
  u_int nregs = 0;

  union{
    u_long128   ul128;
    u_long      ul64[2];
    u_int       ui32[4];
  }Giftag;

  if(stuv==0){ stuv_regs = 0x02;  } 
  else{ stuv_regs = 0x03; } 
  if(fog==0){xyz_regs = 0x04;} 
  else{xyz_regs = 0x04;} 
  /* 0x04 and 0x05 packing mode for z-value is not similar, */
  /* so we select 0x04 and fix 24bit z-value dyanmic range. */

  switch(type){
  case 0 : break; /* point */
  case 1 : break; /* line */
  case 2 : break; /* line strip */
  case 3 : nregs = 3; nloop = packsize;
    Giftag.ul64[1] = ((rgb_regs)<<4) | ((stuv_regs)<<0) | ((xyz_regs)<<8);
    break; /* Triangle */
  case 4 : nregs = 3; nloop = packsize;
    Giftag.ul64[1] = ((rgb_regs)<<4) | ((stuv_regs)<<0) | ((xyz_regs)<<8);
    break; /* Triangle Strip */
  case 5 : break; /* Triangle Fan */
  case 6 : break; /* Sprite */
  case 7 : nregs = 0; attribute &= 0xff8; break; 
  }

  prim |= (u_long)shade << 3; 
  prim |= (u_long)tex << 4; 
  Giftag.ul64[0] = ((u_long)(prim)<<47) | (0x01L<<46);
  Giftag.ui32[1] |= (((u_long)(nregs))<<28);
  Giftag.ui32[0] |= 0x8000 | nloop;

  giftag[0] = Giftag.ui32[0]; giftag[1] = Giftag.ui32[1];
  giftag[2] = Giftag.ui32[2]; giftag[3] = Giftag.ui32[3];

}


u_int* Vu1VertexMakePacket(u_int** ppWrite, u_int** ppRead, 
			   sceVu0FVECTOR *bound,
			   int vertNum, int packNum, u_int prim, u_int MODE)
{

  int   i;
  qword giftag;
  qword primitive;
  qword  *currW = (qword*)*ppWrite;
  u_int  *currR = (u_int*)*ppRead;
  float *rawdata = (float*)currR;
  float *curr_ver = rawdata;
  float *curr_nor = rawdata + vertNum*4*1;
  float *curr_st  = rawdata + vertNum*4*2;
  float *curr_col = rawdata + vertNum*4*3;
    
  primitive[0] = prim;

  Vu1ShapeGetGiftag(giftag, prim, packNum);
  /*                        VU1mem size  wl  cl   */
  CNT_STCYCL_UNPACKR(*currW,  0,     4,   1,  1  ); currW++;
  ADD_QWDATA(*currW,  giftag); currW++;
  ADD_QWDATA(*currW,  primitive); currW++;
  ADD_QWDATA(*currW,  primitive); currW++;
  ADD_QWDATA(*currW,  primitive); currW++;


/*                  size   wl    cl */
  CNT_STCYCL(*currW,  0,    1,    4 ); currW++;
  /*                EEmem_addr  VU1mem   size  */
  REF_UNPACKR(*currW, curr_ver,    4,   packNum); currW++;
  REF_UNPACKR(*currW, curr_nor,    5,   packNum); currW++;
  REF_UNPACKR(*currW, curr_st,     6,   packNum); currW++;
  REF_UNPACKR(*currW, curr_col,    7,   packNum); currW++;
  /*               size */
  CNT_MSCNT(*currW,  0  ); currW++;

  currR += 4*packNum;

  /* calculate bounding box */
  for(i=0; i<packNum; i++){
    if(bound[0][0]<curr_ver[i*4+0]) bound[0][0] = curr_ver[i*4+0];
    if(bound[0][1]<curr_ver[i*4+1]) bound[0][1] = curr_ver[i*4+1];
    if(bound[0][2]<curr_ver[i*4+2]) bound[0][2] = curr_ver[i*4+2];
    if(bound[1][0]>curr_ver[i*4+0]) bound[1][0] = curr_ver[i*4+0];
    if(bound[1][1]>curr_ver[i*4+1]) bound[1][1] = curr_ver[i*4+1];
    if(bound[1][2]>curr_ver[i*4+2]) bound[1][2] = curr_ver[i*4+2];
  }

  *ppWrite = (u_int*)currW;
  *ppRead = (u_int*)currR;

  return (u_int*)currW;

}

u_int* Vu1GeomMakePacket(u_int**ppWrite, u_int** ppRead, 
			 sceVu0FVECTOR *bound,
			 int packNum, u_int MODE)
{
  qword  *currW = (qword*)*ppWrite;
  u_int  *currR = (u_int*)*ppRead;
  u_int  vNum = *(currR + 3);
  u_int  rest_vNum = vNum;
  u_int  prim = *(currR + 2) & 0x3ff;

  currR += 4; /* skip the header of geometory */

  

  while(rest_vNum > packNum){
      Vu1VertexMakePacket((u_int**)&currW, &currR, bound,
			  vNum, packNum, prim, 0);
      switch(prim & 0x7){
	case(0x03) : rest_vNum -= packNum; break;
	case(0x04) : 
	  currR -= 2; 
	  rest_vNum -= (packNum-2); break;
      }
  }

  Vu1VertexMakePacket((u_int**)&currW, &currR, bound,
		      vNum, rest_vNum, prim, 0);
  currR += 4*3*vNum; /* skip normal/st/color field */

  *ppWrite = (u_int*)currW;
  *ppRead = (u_int*)currR;

  return (u_int*)currW;

}

u_int* Vu1ResidentTextureMakePacket(TEXTURE_FRAME *tex, u_int* pWrite)
{
  Image *image;
  qword  *currW = (qword*)pWrite;
  int texID;

  if(tex != NULL){
      for (texID=0; texID<tex->num; texID++){
	  image= tex->image;
	  if (image[texID].resident){
	      image[texID].pPackData = (u_int*)currW;
	      CNT_FLUSH(*currW, 0); currW++;
	      REF_DIRECT(*currW,  image[texID].texTransRegs.GIFtag, 5); currW++;
	      REF_DIRECT(*currW,  image[texID].texGIFtag, 1); currW++;
	      REF_DIRECT(*currW,  image[texID].texpData, image[texID].texTransSize); currW++;
	      tex->TransSizeTotal+=image[texID].texTransSize/65536.0f;

	      if (image[texID].clutTransSize>0){
		  REF_DIRECT(*currW,  image[texID].clutTransRegs.GIFtag, 5); currW++;
		  REF_DIRECT(*currW,  image[texID].clutGIFtag, 1); currW++;
		  REF_DIRECT(*currW,  image[texID].clutpData, image[texID].clutTransSize); currW++;
		  tex->TransSizeTotal+=image[texID].clutTransSize/65536.0f;
	      }
	    
	      REF_DIRECT(*currW, image[texID].GIFtexflush, 2); currW++;
	      RET_NOP(*currW,  0  ); currW++;
	  } else{
	      image[texID].pPackData = NULL;
	  }
      }
  }

  return (u_int*)currW;
}


u_int* Vu1MateMakePacket(Shape* shape, TEXTURE_FRAME *tex, 
			 u_int** ppWrite, u_int** ppRead, 
			 int packNum, u_int MODE)
{
  Image *image;
  int k;
  qword  *currW = (qword*)*ppWrite;
  u_int  *currR = (u_int*)*ppRead;
  int geomNum = *(currR + 1);
  int texID  =  *(currR + 2);
  int texNum =  *(currR + 3);


  currR += 12; /* skip the header(3qword) of material */

  if(tex != NULL){
    image = tex->image;
    if((texNum)&&(texID<tex->num)){
	shape->texNum++;
	CNT_FLUSH(*currW, 0); currW++;


	if (image[texID].resident){
  	    /* no texflush for resident texture */
  	    REF_DIRECT(*currW, image[texID].GIFtex01, 2); currW++;
	} else{
  	    /* add texture transfer for non-resident texture */
	    REF_DIRECT(*currW,  image[texID].texTransRegs.GIFtag, 5); currW++;
	    REF_DIRECT(*currW,  image[texID].texGIFtag, 1); currW++;
	    REF_DIRECT(*currW,  image[texID].texpData, image[texID].texTransSize); currW++;

	    tex->TransSizeTotal+=image[texID].texTransSize/65536.0f;

	    if (image[texID].clutTransSize>0){
		REF_DIRECT(*currW,  image[texID].clutTransRegs.GIFtag, 5); currW++;
		REF_DIRECT(*currW,  image[texID].clutGIFtag, 1); currW++;
		REF_DIRECT(*currW,  image[texID].clutpData, image[texID].clutTransSize); currW++;
		tex->TransSizeTotal+=image[texID].clutTransSize/65536.0f;
	    }
  	    /* with texflush for non-resident texture */
	    REF_DIRECT(*currW, image[texID].GIFtexflush, 4); currW++;
	}
    }
  }

  shape->geomNum += geomNum;


  for(k=0; k<geomNum; k++){
      shape->vertNum += *(currR + 3);

      Vu1GeomMakePacket((u_int**)&currW, &currR, shape->bound_box, packNum, 0);
  } /* end of the loop of geomNum */

  *ppWrite = (u_int*)currW;
  *ppRead = (u_int*)currR;

  return (u_int*)currW;

}

u_int* Vu1ShapeFrameMakePacket(SHAPE_FRAME *sf, TEXTURE_FRAME *tex, u_int* pWrite, u_int worksize, HIERARCHY_FRAME *hf, PACKET_BUF *sp)
{
    int    i, j;
  /*                                   doublebuf initdata kicktag per_vertex */
    qword  *currW = (qword*)pWrite;
    int    packsize = ((1024-120-worksize) /  2      - 4     - 1 )   /   7 ;
    packsize -= packsize%3;
    
    sf->pPackData = (u_int*)currW;
    sf->VertexTotal = 0;

    for (i=0; i<sf->num; i++){
	if (sf->shape[i].hierNum==0){
	    REF_STCYCL_UNPACK(*currW, &hierarchy_root.local_world, 0, 8, 1, 1); currW++;

	    currW=(qword *)Vu1PacketAddObj((u_int *)currW, sp->pCurr);
	    sp->pCurr=Vu1ShapeMakePacket(&sf->shape[i], tex, sp->pCurr, packsize);
	    sf->VertexTotal +=sf->shape[i].vertNum;
	}
	for (j=0; j<sf->shape[i].hierNum; j++){
	    REF_STCYCL_UNPACK(*currW, &sf->shape[i].hier[j]->local_world, 0, 8, 1, 1); currW++;

	    currW=(qword *)Vu1PacketAddObj((u_int *)currW, sp->pCurr);
	    sp->pCurr=Vu1ShapeMakePacket(&sf->shape[i], tex, sp->pCurr, packsize);
	    sf->VertexTotal +=sf->shape[i].vertNum;
	}
    }

    RET_NOP(*currW,  0  ); currW++;

    return (u_int *)currW;
}

u_int* Vu1ShapeMakePacket(Shape *shape, TEXTURE_FRAME *tex, u_int* pWrite, int packsize)
{
  int    i;
  qword  *currW = (qword*)pWrite;
  u_int  *currR = shape->pRawData;
  sceVu0FVECTOR row = {1.0f, 1.0f, 1.0f, 1.0f};



    shape->prim0[0] = packsize * 4;
    shape->prim1[0] = packsize;

    currR += 4; /* skip shape header(1qword) of shape */

    /* Init Vu1 local-obj header */
    /*                            EEmem_addr      VU1mem size wl  cl */
    REF_UNPACK(*currW,  shape->giftag0,  8,  8); currW++;
    /*                     size  VU1mem */
    CNT_MSCAL_FLUSH(*currW,  0,     0   ); currW++;
    /*                    size   wl  cl */
    CNT_STCYCL_ROW(*currW,  1,    1,  1 ); currW++;
    COPY_QW(currW, row); currW++;

    for(i=0; i<shape->mateNum; i++){

	Vu1MateMakePacket(shape, tex, (u_int**)&currW, &currR, packsize, 0);
    } 

    sceVu0SubVector(shape->bound_box[0], shape->bound_box[0], 
		    shape->bound_box[1]);
    
    shape->bound_box[0][0] += shape->bound_box[0][1];
    shape->bound_box[0][0] += shape->bound_box[0][2];
    shape->bound_box[0][0] /= 3.0f;


    RET_NOP(*currW,  0  ); currW++;

    return (u_int*)currW;
}


/* ///////////////////////////////////////////////////////
//
//  Initialize GStexture environment
//
/////////////////////////////////////////////////////// */
int Vu1TexInit(TEXTURE_FRAME *tex, u_int* data)
{
  int i;
  Image *image;
  u_int *currR = data;
  u_int texWidth, texHeight;
  u_int psm;
  u_int tbw;
  u_int clutWidth, clutHeight;
  u_int cpsm=0;
  u_int cbw=0;


  tex->num = *(currR + 3);
  tex->image = (Image *)memalign(16, sizeof(Image)*tex->num);
  if(tex->image == NULL){
    ErrPrintf("cannot malloc image\n");
    return -1;
  }
  tex->TransSizeTotal=0.0f;
  image = tex->image;

  currR += 4;

  for(i=0;i<tex->num;i++){
    image[i].myID = i;
    
    /* read and set whether this texture is resident */
    image[i].resident=currR[2]&1;

    /* read image data and set GS Tex01 reg */
    image[i].Tex01[0] = currR[0];
    image[i].Tex01[1] = currR[1]; 
    image[i].Tex01[2]=  SCE_GS_TEX0_1;
    image[i].Tex01[3] = 0;
    psm= (image[i].Tex01[0]>>GS_TEX0_PSM_O) & 0x3f;

    currR+=4;

    image[i].texTransSize= (currR[0]+3)/4;
    texHeight = currR[2] >> 16;
    texWidth = currR[2]& 0xffff;
    if ( (psm==SCE_GS_PSMT8) || (psm==SCE_GS_PSMT4)){
	tbw= 2*((texWidth+127)/128);
    } else{
	tbw= (texWidth+63)/64;
    }

    image[i].texGsSize= getTexGsSize(tbw, psm, texHeight);
    image[i].texBp = getTbp(image[i].texGsSize, image[i].resident);

    image[i].Tex01[0]&= ~((u_int)(GS_TEX0_TBP0_M|GS_TEX0_TBW_M));
    image[i].Tex01[0]|= (image[i].texBp << GS_TEX0_TBP0_O);
    image[i].Tex01[0]|= (tbw<< GS_TEX0_TBW_O);

    image[i].clutTransSize = (currR[1]+3)/4;

    if (image[i].clutTransSize){
	cpsm= (image[i].Tex01[1]>>(GS_TEX0_CPSM_O-32)) & 0xf;
	clutHeight = currR[3] >> 16;
	clutWidth = currR[3]& 0xffff;
	cbw=1;

	image[i].clutGsSize= getClutGsSize(psm);
	image[i].clutBp = getCbp(image[i].clutGsSize, FW_GS_ALIGN_ANY, image[i].resident);

	image[i].Tex01[1]&= ~((GS_TEX0_CBP_M|GS_TEX0_CLD_M) >> 32);
	image[i].Tex01[1]|= (image[i].clutBp<<(GS_TEX0_CBP_O-32));
	image[i].Tex01[1]|= (2<<(GS_TEX0_CLD_O-32));
    } else{
	clutWidth = 0;
	clutHeight = 0;
	image[i].clutGsSize=0;    
    }

    /* GIFtag for Texflush */
    SET_QW(image[i].GIFtexflush, 0x0, 0x0000000e, 0x10000000, 0x00008001 );

    /* set GS reg Texflush */
    SET_QW(image[i].Texflush,    0x0, 0x0000003f, 0x00000000, 0x00000000 );
    
    /* GIFtag for Tex01 */
    SET_QW(image[i].GIFtex01, 0x0, 0x0000000e, 0x10000000, 0x00008001 );

    /* GIFtag and GSregs for texel host->local transfer */
    SET_QW(image[i].texTransRegs.GIFtag, 0x0, 0x0000000e, 0x10000000, 0x00008004 );
    SET_QW(image[i].texTransRegs.Bitblt, 0x0, 0x00000050, 
	   ((psm<<(GS_BITBLTBUF_DPSM_O-32))
	    |(tbw<<(GS_BITBLTBUF_DBW_O-32))
	    | (image[i].texBp<<(GS_BITBLTBUF_DBP_O-32))),
	   0x00000000 );
    SET_QW(image[i].texTransRegs.Trxpos, 0x0, 0x00000051, 0x00000000, 0x00000000 );
    SET_QW(image[i].texTransRegs.Trxreg, 0x0, 0x00000052, texHeight, texWidth);
    SET_QW(image[i].texTransRegs.Trxdir, 0x0, 0x00000053, 0x00000000, 0x00000000 );

    /* GIFtag and GSregs for clut host->local transfer */
    if (image[i].clutTransSize>0){
	SET_QW(image[i].clutTransRegs.GIFtag, 0x0, 0x0000000e, 0x10000000, 0x00008004 );
	SET_QW(image[i].clutTransRegs.Bitblt, 0x0, 0x00000050, 
	       ((cpsm<<(GS_BITBLTBUF_DPSM_O-32))
		|(cbw<<(GS_BITBLTBUF_DBW_O-32))
		| (image[i].clutBp<<(GS_BITBLTBUF_DBP_O-32))),
	       0x00000000 );
	SET_QW(image[i].clutTransRegs.Trxpos, 0x0, 0x00000051, 0x00000000, 0x00000000 );
	SET_QW(image[i].clutTransRegs.Trxreg, 0x0, 0x00000052, clutHeight, clutWidth);
	SET_QW(image[i].clutTransRegs.Trxdir, 0x0, 0x00000053, 0x00000000, 0x00000000 );
    }

    currR += 4;

    image[i].texpData = currR;

    /* GIFtag for texel data transfer to GS */
    SET_QW(image[i].texGIFtag, 0x0, 0x0, 0x08000000, image[i].texTransSize|0x8000 );

    currR += image[i].texTransSize*4;

    /* GIFtag for clut data transfer to GS */
    if (image[i].clutTransSize){
	image[i].clutpData = currR;
	SET_QW(image[i].clutGIFtag, 0x0, 0x0, 0x08000000, image[i].clutTransSize|0x8000 );

	currR += image[i].clutTransSize*4;
    }
    
  }

  return 0;
}
