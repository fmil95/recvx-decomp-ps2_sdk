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
 *                          Name : hier.h
 *
 *            Version   Date           Design      Log
 *  --------------------------------------------------------------------
 *            0.40      Jun,21,2000    kaneko
 */
#ifndef _HIER_H_
#define _HIER_H_

#include "vu1frame.h"



extern void LoadMatrix(sceVu0FMATRIX m);
extern void PushMatrix(sceVu0FMATRIX m);
extern void PopMatrix(sceVu0FMATRIX m);
extern void LoadRotMatrix(sceVu0FMATRIX m);
extern void PushRotMatrix(sceVu0FMATRIX m);
extern void PopRotMatrix(sceVu0FMATRIX m);
extern void SetHierarchyMatrix(HIERARCHY_FRAME *hf, int i);
extern void SetHierarchy(HIERARCHY_FRAME *hf);
extern void SetShare(SHAPE_FRAME *sf);

void HierarchyInit(HIERARCHY_FRAME *hf, Hierarchy *hierarchy, int nhierarchy, SHAPE_FRAME *sf);

#endif /* !_HIER_H_ */
