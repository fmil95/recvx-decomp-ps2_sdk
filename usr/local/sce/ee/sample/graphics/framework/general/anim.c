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
 *                          Name : anim.c
 *
 *            Version   Date           Design      Log
 *  --------------------------------------------------------------------
 *            0.40      Jun,21,2000    kaneko
 */
#include "defines.h"
#include "vu1frame.h"
#include "hier.h"
#include "anim.h"
#include "eval.h"


static int keypoint(int frame, int *kf, int nkey)
{
  int i;
  int ret;

  ret = 0;
  for(i=0;i<nkey;i++){
    if((frame>=kf[i]) && (frame<=kf[i+1])){
      ret = i;
      break;
    }
  }
  return ret;
}

void SetAnimation(u_int frame, HIERARCHY_FRAME *hf)
{
  int nkey,hier;
  int frm, key, end;
  int *kf;
  float *kv;

  float t;
  float a,b;
  float f;
  int i,j;

  for(i=0;i<nanimation;i++){
    nkey = animation[i].nkey;
    hier = animation[i].hierarchy;
    kf = keyframe[animation[i].keyframe];
    kv = keyvalue[animation[i].keyvalue];
    end = kf[nkey-1];
    
    frm = (int)(frame % end);
    key = keypoint(frm, kf, nkey);

    t = (float)(frm - kf[key])/(float)(kf[key+1] - kf[key]);

    /* Trans */
    for(j=0;j<3;j++){
      a = kv[key*8 + j];
      b = kv[(key+1)*8 + j];
      f = linear(t, a, b);
      hf->hierarchy[hier].trans[j] = f;
    }
    hf->hierarchy[hier].trans[3] = 0.0f;

    /* Rot */
    for(j=0;j<3;j++){
      a = kv[key*8 + 4 + j];
      b = kv[(key+1)*8 + 4 + j];
      f = linear(t, a, b);
      hf->hierarchy[hier].rot[j] = f;
    }
    hf->hierarchy[hier].rot[3] = 0.0f;
  }
}
