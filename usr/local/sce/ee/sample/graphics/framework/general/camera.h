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
 *                          Name : camera.h
 *
 *            Version   Date           Design      Log
 *  --------------------------------------------------------------------
 *            0.30      Apr,4,2000     thatake     Beta0
 *            0.40      Jun,16,2000     	   +animation, clut
 */

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <libvu0.h>

typedef struct {
    sceVu0FVECTOR pos;
    sceVu0FVECTOR zdir;
    sceVu0FVECTOR ydir;
    sceVu0FVECTOR rot;
} CameraPos;

typedef struct{
    CameraPos cp;
    sceVu0FVECTOR clip_volume;
    sceVu0FMATRIX world_view;
    sceVu0FMATRIX view_screen;
    sceVu0FMATRIX view_clip;
    sceVu0FMATRIX world_screen;
    sceVu0FMATRIX world_clip;
    sceVu0FMATRIX clip_screen;
}CAMERA;

#if 0
typedef struct{
    sceVu0FVECTOR pos;
    sceVu0FVECTOR zdir;
    sceVu0FVECTOR ydir;
    sceVu0FVECTOR rot;
    sceVu0FVECTOR clip_volume;
    sceVu0FMATRIX world_view;
    sceVu0FMATRIX view_screen;
    sceVu0FMATRIX view_clip;
    sceVu0FMATRIX world_screen;
    sceVu0FMATRIX world_clip;
    sceVu0FMATRIX clip_screen;
}CAMERA;
#endif /* 0 */


extern void Camera_SetCamera_fromCameraPos(CAMERA *cam, CameraPos *cam_pos);

extern void sceVu0RotCameraMatrix(sceVu0FMATRIX m, CameraPos *cam_pos);

extern void SetVu0ViewScreenClipMatrix(sceVu0FMATRIX m, sceVu0FMATRIX mc,
				       sceVu0FMATRIX mcs,
				       sceVu0FVECTOR clip_vol,
				       float scrz, float ax, float ay, 
				       float cx, float cy, 
				       float zmin, float zmax, 
				       float nearz, float farz);

extern void Camera_SetClipScreen(CAMERA *cam, float screen_dist, 
			  float xaspect, float yaspect, 
			  float gs_xsize, float gs_ysize,
			  float zmini, float zmax, 
			  float nearz, float farz);


#define Camera_SetRotate(cam, cam_rot) \
  sceVu0CopyVector((cam)->cp.rot, cam_rot);

#define Camera_SetPosition(cam, cam_pos) \
  sceVu0CopyVector((cam)->cp.pos, cam_pos);

#define Camera_SetClipVolume(cam, clip_vol) \
  sceVu0CopyVector((cam)->clip_volume, clip_vol);

#define Camera_SetDirection(cam, cam_dir, cam_ver) \
  sceVu0CopyVector((cam)->cp.zdir, cam_dir); \
  sceVu0CopyVector((cam)->cp.ydir, cam_ver); 


#define Camera_SetMatrix(cam) \
  sceVu0RotCameraMatrix((cam)->world_view, &(cam)->cp); \
  sceVu0MulMatrix((cam)->world_screen,(cam)->view_screen,(cam)->world_view); \
  sceVu0MulMatrix((cam)->world_clip,(cam)->view_clip,(cam)->world_view);

#define Camera_SetScreen(cam, screen_dist, \
		      xaspect, yaspect, \
		      gs_xsize, gs_ysize, \
		      zmini, zmax, \
		      nearz, farz) \
  sceVu0ViewScreenMatrix((cam)->view_screen, \
			   screen_dist, xaspect, yaspect, \
			   gs_xsize, gs_ysize, zmini, zmax, \
			   nearz, farz); 

#endif /* !_CAMERA_H_ */
