/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - tmip -
 *
 *                         Version 0.10 
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            objpart.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 */

#include "object.h"
#define MAXPARTICLE	4096
#define MAXV		32
#define DRX		(10*3.14/180)
#define DRY		(20*3.14/180)
#define DRZ		(30*3.14/180)

static int	npart   = 32;
static int	step    = 13;
static VECTOR	rot     = {   0,   0,   0,0};
static VECTOR	drot    = { DRX, DRY, DRZ,0};
static VECTOR	dst[MAXPARTICLE];


void objDrvParticleAttributes(VECTOR r, int n, int s) 
{
	CopyVector(rot, r);
	if ((npart = n) > MAXPARTICLE)
		npart = MAXPARTICLE;
	step = s;
}

qword *objDrvParticle(qword *pbase, OBJECT *obj) 
{
	static int first = 1;
	int	i, j, k, nv;
	u_int	flag, t;
	VECTOR	v1, n, c[MAXV], v[MAXV]; 
	VECTOR	*orgc, *orgn, *orgv;
	MATRIX	rm, mm, ms, nl, nc; 

	// setup
	if (first) {
		int range = 50;
		first = 0;

		if (obj->box) {
			float t;
			for (i = 0; i < 8; i++) {
				for (k = 0; k < 3; k++) {
				t = obj->box[i][k]>0? obj->box[i][k]: -obj->box[i][k];
					if (range < t)
						range = t;
				}
			}
		}
		for (i = 0; i < npart; i++) {
			dst[i][0] = rand()%range-range/2;
			dst[i][1] = rand()%range-range/2;
			dst[i][2] = rand()%range-range/2;
			dst[i][3] = 1;
		}
	}

	// update matrix
	MulMatrix(ms, obj->view->ws,  obj->mw[0]);
	MulMatrix(nl, obj->light->nl, obj->mw[0]);
	setVector(nl[3], 0, 0, 0, 1);	// patch because MulMatrix3 is lacked
	MulMatrix(nc, obj->light->lc, nl);

	// update rotation
	UnitMatrix(mm);
	RotMatrixX(mm, mm, rot[0]);
	RotMatrixY(mm, mm, rot[1]);
	RotMatrixZ(mm, mm, rot[2]);

	UnitMatrix(rm);
	RotMatrixX(rm, rm, drot[0]);
	RotMatrixY(rm, rm, drot[1]);
	RotMatrixZ(rm, rm, drot[2]);


	// set vertex
	nv  = (int)obj->cvnt[0][3];
	orgc = &obj->cvnt[1];
	orgv = &obj->cvnt[1+nv];
	orgn = &obj->cvnt[1+2*nv];

	// set tag
	for (j = 0; j < npart; j++) {

		// setup local rotation
		if (j%step == 0) {

			MulMatrix(mm, rm, mm);
			for (k = 0; k < nv; k++) {
				ApplyMatrix(v[k], mm, orgv[k]);
				ApplyMatrix(n, mm, orgn[k]);
				ApplyMatrix(c[k], nc, n);
				MulVector(c[k], orgc[k], c[k]);
			
				if (c[k][0] < 0) c[k][0] = 0;
				if (c[k][1] < 0) c[k][1] = 0;
				if (c[k][2] < 0) c[k][2] = 0;
					
				// normalize color
				Int0Vector((int *)c[k], c[k]);
			}
		}

		// draw
		obj->tag[0] = 0x8000 | nv;
		CopyVector((float *)(*pbase++),   (float *)obj->tag);			
		for (flag = i = 0; i < nv; i++) {
			// distortion
			AddVector(v1, dst[j], v[i]);

			// rot-trans-pers
			ApplyMatrix(v1, ms, v1);

			// project
			DivVectorXYZ(v1, v1, v1[3]);
		
			// check flag
			if (obj->clip) {			     
				t = ClipScreen(v1)? 1: 0;
				flag = (flag<<1 | t);
			}

			// normalize vertex
			Int4Vector((int *)v1, v1);
		
			// clip
			t = flag? 0x8000: 0;
			v1[3] = *(float *)&t;

			// add to display list
			CopyVector((float *)(*pbase++), c[i]);
			CopyVector((float *)(*pbase++), v1);
		}
	}
	return(pbase);
}
