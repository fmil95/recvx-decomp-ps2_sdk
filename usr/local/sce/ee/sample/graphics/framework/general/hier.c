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
 *                          Name : hier.c
 *
 *            Version   Date           Design      Log
 *  --------------------------------------------------------------------
 *            0.40      Jun,21,2000    kaneko
 */

#include <eekernel.h>
#include <stdio.h>
#include <eeregs.h>
#include <libvu0.h>
#include <malloc.h>
#include "defines.h"
#include "vu1frame.h"
#include "hier.h"






#define STACKSIZE 30
static int current = 0;
static sceVu0FMATRIX curmatrix = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
static sceVu0FMATRIX stackmatrix[STACKSIZE];


void LoadMatrix(sceVu0FMATRIX m)
{
  sceVu0CopyMatrix(m,curmatrix);
}

void PushMatrix(sceVu0FMATRIX m)
{
  sceVu0CopyMatrix(stackmatrix[current], m);
  sceVu0CopyMatrix(curmatrix, m);
  current++;
  if(current >= 30){
    ErrPrintf("over 30 stack depth\n");
    return;
  }
}

void PopMatrix(sceVu0FMATRIX m)
{
  current--;
  if(current < 0){
    ErrPrintf("under 0 stack depth\n");
    return;
  }

  sceVu0CopyMatrix(m, stackmatrix[current]);

  if(!current){
    sceVu0UnitMatrix(curmatrix);
  }
  else{
    sceVu0CopyMatrix(curmatrix, m);
  }
}




static int currot = 0;
static sceVu0FMATRIX currotmatrix = { {1.0, 0.0, 0.0, 0.0}, {0.0, 1.0, 0.0, 0.0}, {0.0, 0.0, 1.0, 0.0}, {0.0, 0.0, 0.0, 1.0} };
static sceVu0FMATRIX stackrotmatrix[STACKSIZE];

void LoadRotMatrix(sceVu0FMATRIX m)
{
  sceVu0CopyMatrix(m,currotmatrix);
}

void PushRotMatrix(sceVu0FMATRIX m)
{
  sceVu0CopyMatrix(stackrotmatrix[currot], m);
  sceVu0CopyMatrix(currotmatrix, m);
  currot++;
  if(currot >= 30){
    ErrPrintf("over 30 stack depth\n");
    return;
  }
}

void PopRotMatrix(sceVu0FMATRIX m)
{
  currot--;
  if(current < 0){
    ErrPrintf("under 0 stack depth\n");
    return;
  }

  sceVu0CopyMatrix(m, stackrotmatrix[currot]);

  if(!currot){
    sceVu0UnitMatrix(currotmatrix);
  }
  else{
    sceVu0CopyMatrix(currotmatrix, m);
  }
}





void ApplyVertex(sceVu0FMATRIX m, Sharedvn *s)
{
  int i;
  
  for(i=s->bv;i<s->bv+s->lv;i++){
    sceVu0ApplyMatrix(dstvertex[i], m, srcvertex[i]);

  }
}

void ApplyNormal(sceVu0FMATRIX m, Sharedvn *s)
{
  int i;
  
  for(i=s->bn;i<s->bn+s->ln;i++){
    sceVu0ApplyMatrix(dstnormal[i], m, srcnormal[i]);
  }
}




void SetShareV(Share *s, int id, sceVu0FVECTOR **dstbase)
{
  int i;
  int idx;

  for(i=0;i<s->num;i++){
      idx=sharevertex[id].srcidx+i;
      sceVu0CopyVector((dstbase[s->dstidx])[i], dstvertex[(indexvertex[idx])]);
  }


}

void SetShareN(Share *s, int id, sceVu0FVECTOR **dstbase)
{
  int i;
  int idx;


  for(i=0;i<s->num;i++){
      idx=sharenormal[id].srcidx+i;
      sceVu0CopyVector((dstbase[s->dstidx])[i], dstnormal[(indexnormal[idx])]);
  }

}


void SetShare(SHAPE_FRAME *sf)
{
  int i;

  for(i=0;i<nsharevertex;i++){
    SetShareV(&sharevertex[i], 
	      i,
	      sf->shape[shareshapeidvertex].pGeomVertex);
  }
  for(i=0;i<nsharenormal;i++){
    SetShareN(&sharenormal[i], 
	      i,
	      sf->shape[shareshapeidnormal].pGeomNormal);
  }
}

inline void HierSetLocalWorld(Hier *hier, sceVu0FMATRIX mat)
{  sceVu0CopyMatrix(hier->local_world, mat); }

inline void HierSetLightRot(Hier *hier, sceVu0FMATRIX mat)
{  sceVu0CopyMatrix(hier->light_rot, mat); }





void SetHierarchyMatrix(HIERARCHY_FRAME *hf, int id)
{
  int i;
  static sceVu0FMATRIX m2;
  static sceVu0FMATRIX m0;


  /* Normalize hf->hierarchy[id].rot angle [-PI..PI] */
  {
      sceVu0FVECTOR v;
      int npi;

      sceVu0ScaleVectorXYZ(v,hf->hierarchy[id].rot, 1.0f/PI);
      for (i=0; i<3; i++){
	  
	  npi=(int)v[i];
	  if (npi<0) npi-=1;
	  else npi+=1;
	  npi/=2;
	  if (npi!=0){
	      hf->hierarchy[id].rot[i]-= npi*(2.0f*PI);
	  }
      }
  }

  {
      sceVu0FMATRIX m1;

      sceVu0UnitMatrix(m1);
      sceVu0RotMatrixY(m1, m1, hf->hierarchy[id].rot[1]);	/* for LightWave3D */
      sceVu0RotMatrixX(m1, m1, hf->hierarchy[id].rot[0]);	/* for LightWave3D */
      sceVu0RotMatrixZ(m1, m1, hf->hierarchy[id].rot[2]);	/* for LightWave3D */
      LoadRotMatrix(m2);
      sceVu0MulMatrix(m2, m2, m1);
      sceVu0TransMatrix(m1, m1, hf->hierarchy[id].trans);
      LoadMatrix(m0);
      sceVu0MulMatrix(m0, m0, m1);
  }

  if(hf->hierarchy[id].shapeid != -1){

      HierSetLightRot(&hf->hier[id], m2);
      HierSetLocalWorld(&hf->hier[id], m0);


    if(hf->hierarchy[id].sharedvn.lv>0){
      ApplyVertex(m0, &hf->hierarchy[id].sharedvn);
      ApplyNormal(m2, &hf->hierarchy[id].sharedvn);
    }
  }

  for(i=0;i<hf->num;i++){
    if((hf->hierarchy[i].parent != -1) && (hf->hierarchy[i].parent == id)){
      PushRotMatrix(m2);
      PushMatrix(m0);
      SetHierarchyMatrix(hf, i);
      PopMatrix(m0);
      PopRotMatrix(m2);
    }
  }
}

void SetHierarchy(HIERARCHY_FRAME *hf)
{
    int i;

    if (hf){
	for(i=0;i<hf->num;i++){
	    if(hf->hierarchy[i].parent == -1){
		SetHierarchyMatrix(hf, i);
	    }
	}
    }
}

void HierarchyNodeInit(HIERARCHY_FRAME *hf, int id, SHAPE_FRAME *sf)
{
  int i;

  if(hf->hierarchy[id].shapeid != -1){
      Shape *shape;

      shape= &sf->shape[hf->hierarchy[id].shapeid];
      shape->hier[shape->hierNum++]= &hf->hier[id];
  }

  for(i=0;i<hf->num;i++){
    if((hf->hierarchy[i].parent != -1) && (hf->hierarchy[i].parent == id)){
      HierarchyNodeInit(hf, i, sf);
    }
  }
}

void HierarchyInit(HIERARCHY_FRAME *hf, Hierarchy *hierarchy, int nhierarchy, SHAPE_FRAME *sf)
{
  int i;

  hf->hierarchy=hierarchy;
  hf->num=nhierarchy;
  hf->hier = (Hier *)memalign(64, bound(sizeof(Hier)*hf->num, 64));
  if (hf->hier==NULL){				// malloc error
      hf->num=0; hf->hierarchy=NULL;
      ErrPrintf("cannot malloc hier\n");
      return;
  }

  for(i=0;i<hf->num;i++){
      hf->hier[i].myID=i;
      if(hf->hierarchy[i].parent == -1){
	  HierarchyNodeInit(hf, i, sf);
      }
  }
}

