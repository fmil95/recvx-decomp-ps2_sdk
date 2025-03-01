/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                        - refmap-448 -
 *
 *                        Version 0.4.0
 *
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : mathfunc.h
 *                     Description : params for mathfunc.
 *
 *       Version   Date           Design              Log
 *  --------------------------------------------------------------------
 *       0.3.0      Jun, 4,1999    munekis            first version
 *       0.4.0      Jul,11,2000    sasaki             modified for AA sample 
 */


#ifndef __MATHFUNC_H__
#define __MATHFUNC_H__

typedef int		sceSamp0IVECTOR[4] __attribute__((aligned (16)));
typedef float		sceSamp0FVECTOR[4] __attribute__((aligned (16)));
typedef float		sceSamp0FMATRIX[4][4] __attribute__((aligned (16)));

// prototypes
void  sceSamp0DumpVector(char *s, sceSamp0FVECTOR v);
void  sceSamp0DumpMatrix(char *s, sceSamp0FMATRIX m);
void  sceSamp0CopyVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1);
void  sceSamp0FTOI0Vector(sceSamp0IVECTOR v0, sceSamp0FVECTOR v1);
void  sceSamp0ITOF0Vector(sceSamp0FVECTOR v0, sceSamp0IVECTOR v1);
void  sceSamp0ScaleVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, float s);
void  sceSamp0AddVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, sceSamp0FVECTOR v2);
void  sceSamp0SubVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, sceSamp0FVECTOR v2);
void  sceSamp0MulVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, sceSamp0FVECTOR v2);
float sceSamp0InnerProduct(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1);
void  sceSamp0OuterProduct(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, sceSamp0FVECTOR v2);
void sceSamp0Normalize(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1);
void  sceSamp0ApplyMatrix(sceSamp0FVECTOR v0, sceSamp0FMATRIX m, sceSamp0FVECTOR v1);
void  sceSamp0UnitMatrix(sceSamp0FMATRIX m);
void  sceSamp0CopyMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1);
void  sceSamp0TransposeMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1);
void  sceSamp0MulMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, sceSamp0FMATRIX m2);
void  sceSamp0InversMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1);
void  sceSamp0RotMatrixX(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, float rx);
void  sceSamp0RotMatrixY(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, float ry);
void  sceSamp0RotMatrixZ(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, float rz);
void  sceSamp0RotMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, sceSamp0FVECTOR rot);
void  sceSamp0TransMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, sceSamp0FVECTOR tv);
void  sceSamp0CameraMatrix(sceSamp0FMATRIX m, sceSamp0FVECTOR p, sceSamp0FVECTOR zd, sceSamp0FVECTOR yd);
void  sceSamp0RotCameraMatrix(sceSamp0FMATRIX m, sceSamp0FVECTOR p, sceSamp0FVECTOR zd, sceSamp0FVECTOR yd, sceSamp0FVECTOR rot);
void  sceSamp0NormalLightMatrix(sceSamp0FMATRIX m, sceSamp0FVECTOR l0, sceSamp0FVECTOR l1, sceSamp0FVECTOR l2);
void  sceSamp0LightColorMatrix(sceSamp0FMATRIX m, sceSamp0FVECTOR c0, 
						sceSamp0FVECTOR c1, sceSamp0FVECTOR c2, sceSamp0FVECTOR a);
void  sceSamp0ViewScreenMatrix(sceSamp0FMATRIX m, float scrz, float ax, float ay, 
	float cx, float cy, float zmin, float zmax, float nearz, float farz);
void sceSamp0ClampVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, float min, float max);
float sceSamp0RotTransPers(sceSamp0IVECTOR v0, sceSamp0FMATRIX m0, sceSamp0FVECTOR v1, int mode);


// --- defines ---
#define PI 3.14159265358979323846f
#define RAD_TO_DEG(x) (x * 180.0f / PI)
#define DEG_TO_RAD(x) (x * PI / 180.0f)

#endif // __MATHFUNC_H__
