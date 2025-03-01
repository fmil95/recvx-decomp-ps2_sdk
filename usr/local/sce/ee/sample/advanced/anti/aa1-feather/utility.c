/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                       - feather aa1 sample  -
 *
 *                         Version 0.2.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : utility.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.1.0           Oct,07,1999     sasaki    modified from feather
 *      0.2.0           Jul,11,2000     sasaki    
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <libvu0.h>

#define FRAME_H		640.0f
#define FRAME_V		224.0f
#define LEFTUPPER_H	(2048.0f-320.0f)
#define LEFTUPPER_V	(2048.0f-112.0f)
#define TEXTURE_H	1024.0f
#define TEXTURE_V	256.0f
#define WAVE_X_MAX   132
#define WAVE_Z_MAX   132
#define SMAX		10
#define NSRC		8
#define LAMP		100.0f
#define FRAME		50.0f
#define REFRACT_RATE	0.05f
#define VIEWP_X		0.0f
#define VIEWP_Y		(-18000.0f)
#define VIEWP_Z		(-18000.0f)
#define TARGET_X	0.0f
#define TARGET_Y	0.0f
#define TARGET_Z	0.0f
#define LIGHTDIR_X	1.0f
#define LIGHTDIR_Y	-1.0f
#define LIGHTDIR_Z	1.0f
#define NWAVE		15.0f

#define ERR_STOP while(1)

void Vu0MulAddRef(float *v0, float *v1, float *v2, float a);
void VuApplyMatrixPers(float m[4][4], float a[4] , float b[4]);
void Euler(float a[3], float b[3], float c[3]);
void RotMatrix(float a[3], float m[4][4]);
void MulMatrix(float a[4][4],float b[4][4],float m[4][4]);
void ApplyMatrix(float m[4][4], float a[4] , float b[4]);
void VuApplyMatrix(float m[4][4], float a[4] , float b[4]);
void VectorNormal(float *v);
void OuterProduct(float *v0, float *v1, float *v2);
void SubOuterProduct(float *v0, float *v1, float *v2, float *v3);
void Vu0SubOuterProduct(float *v0, float *v1, float *v2, float *v3);
void get_rotmatrix_from_axis(float p, float z, float y, float x, float r[4][4]);
void get_normal(float LPOS[WAVE_X_MAX][WAVE_Z_MAX][3], float LNML[WAVE_X_MAX][WAVE_Z_MAX][3], int nlattx, int nlattz);
void get_SWmatrix_from_view(float v[3],float t[3], float SWMTX[4][4]);
void get_VuSWmatrix_from_view(float v[3],float t[3], float SWMTX[4][4]);


void Euler(theta,theta_cos,theta_sin)
float 	theta[3],theta_sin[3],theta_cos[3];
{
	int	i;
	float 	PAI= 3.14159f;

	/* 0<=theta<2.0*pai */
	for(i=0;i<3;i++){
	      	while(theta[i]<0.0f||theta[i]>=2.0f*PAI){
               	    if(theta[i]<0.0f) theta[i] += 2.0f*PAI;
               	    if(theta[i]>=2.0f*PAI) theta[i] -= 2.0f*PAI;
            	}
        }

	/* -0.5*pai<= theta_sin <= 0.5*pai */
	for(i=0;i<3;i++){
	    	theta_sin[i]= theta[i];
		if(theta[i]>0.5f*PAI && theta[i]<1.5f*PAI) theta_sin[i]= PAI - theta[i];
		if(theta[i]>=1.5f*PAI) theta_sin[i]= theta[i] - 2.0f*PAI;
	}

	/* -1.0*pai<= theta_cos <= 0 */
	for(i=0;i<3;i++){
		if(theta[i]<1.0f*PAI) theta_cos[i]= -theta[i];
		if(theta[i]>=1.0f*PAI) theta_cos[i]= theta[i] - 2.0f*PAI;
	}
}

void RotMatrix(r,mtx)
float r[3];
float mtx[4][4];
{
float c0,c1,c2;
float s0,s1,s2;

	c0= cosf(r[0]);
	c1= cosf(r[1]);
	c2= cosf(r[2]);
	s0= sinf(r[0]);
	s1= sinf(r[1]);
	s2= sinf(r[2]);

	mtx[0][0]= c1*c2;
	mtx[0][1]= s0*s1*c2 - s2*c0;
	mtx[0][2]= s1*c0*c2 + s0*s2;
	mtx[0][3]= 0.0f;

	mtx[1][0]= s2*c1;
	mtx[1][1]= s0*s1*s2 + c0*c2;
	mtx[1][2]= s1*c0*s2 - s0*c2;
	mtx[1][3]= 0.0f;

	mtx[2][0]= -s1;
	mtx[2][1]= s0*c1;
	mtx[2][2]= c0*c1;
	mtx[2][3]= 0.0f;

	mtx[3][0]= 0.0f;
	mtx[3][1]= 0.0f;
	mtx[3][2]= 0.0f;
	mtx[3][3]= 1.0f;
}

void MulMatrix(a,b,m)
float a[4][4];
float b[4][4];
float m[4][4];
{
	m[0][0]= a[0][0]*b[0][0]+a[0][1]*b[1][0]+a[0][2]*b[2][0]+a[0][3]*b[3][0];
	m[0][1]= a[0][0]*b[0][1]+a[0][1]*b[1][1]+a[0][2]*b[2][1]+a[0][3]*b[3][1];
	m[0][2]= a[0][0]*b[0][2]+a[0][1]*b[1][2]+a[0][2]*b[2][2]+a[0][3]*b[3][2];
	m[0][3]= a[0][0]*b[0][3]+a[0][1]*b[1][3]+a[0][2]*b[2][3]+a[0][3]*b[3][3];

	m[1][0]= a[1][0]*b[0][0]+a[1][1]*b[1][0]+a[1][2]*b[2][0]+a[1][3]*b[3][0];
	m[1][1]= a[1][0]*b[0][1]+a[1][1]*b[1][1]+a[1][2]*b[2][1]+a[1][3]*b[3][1];
	m[1][2]= a[1][0]*b[0][2]+a[1][1]*b[1][2]+a[1][2]*b[2][2]+a[1][3]*b[3][2];
	m[1][3]= a[1][0]*b[0][3]+a[1][1]*b[1][3]+a[1][2]*b[2][3]+a[1][3]*b[3][3];

	m[2][0]= a[2][0]*b[0][0]+a[2][1]*b[1][0]+a[2][2]*b[2][0]+a[2][3]*b[3][0];
	m[2][1]= a[2][0]*b[0][1]+a[2][1]*b[1][1]+a[2][2]*b[2][1]+a[2][3]*b[3][1];
	m[2][2]= a[2][0]*b[0][2]+a[2][1]*b[1][2]+a[2][2]*b[2][2]+a[2][3]*b[3][2];
	m[2][3]= a[2][0]*b[0][3]+a[2][1]*b[1][3]+a[2][2]*b[2][3]+a[2][3]*b[3][3];

	m[3][0]= a[3][0]*b[0][0]+a[3][1]*b[1][0]+a[3][2]*b[2][0]+a[3][3]*b[3][0];
	m[3][1]= a[3][0]*b[0][1]+a[3][1]*b[1][1]+a[3][2]*b[2][1]+a[3][3]*b[3][1];
	m[3][2]= a[3][0]*b[0][2]+a[3][1]*b[1][2]+a[3][2]*b[2][2]+a[3][3]*b[3][2];
	m[3][3]= a[3][0]*b[0][3]+a[3][1]*b[1][3]+a[3][2]*b[2][3]+a[3][3]*b[3][3];

}

#if 1

inline void Vu0MulAddRef(sceVu0FVECTOR a0, sceVu0FVECTOR a1, sceVu0FVECTOR a2, float REF)
{
	asm __volatile__
	("
		lqc2    vf4,0x0(%1)
		lqc2    vf5,0x0(%2)
		ctc2	%3,$vi21
		vaddax.xyzw	ACC,vf4,vf0
		vmaddi.xyzw	vf6,vf5,I
		sqc2    vf6,0x0(%0)
	": : "r" (a0) , "r" (a1) ,"r" (a2), "r" (REF): "memory");
}

inline void Vu0ApplyMatrixPers(sceVu0FVECTOR v0, sceVu0FMATRIX m0,sceVu0FVECTOR v1)
{
	asm __volatile__("
	lqc2    vf4,0x0(%1)
	lqc2    vf5,0x10(%1)
	lqc2    vf6,0x20(%1)
	lqc2    vf7,0x30(%1)
	lqc2    vf8,0x0(%2)
	vmulax.xyzw	ACC,   vf4,vf8
	vmadday.xyzw	ACC,   vf5,vf8
	vmaddaz.xyzw	ACC,   vf6,vf8
	vmaddw.xyzw	vf12,vf7,vf8
	vdiv	Q,vf00w,vf12z
	vwaitq
	vmulq.xy	vf12,vf12,Q
	sqc2    vf12,0x0(%0)
	": : "r" (v0) , "r" (m0) ,"r" (v1): "memory");
}

inline void Vu0ApplyMatrix(sceVu0FVECTOR v0, sceVu0FMATRIX m0,sceVu0FVECTOR v1)
{
	asm __volatile__("
	lqc2    vf4,0x0(%1)
	lqc2    vf5,0x10(%1)
	lqc2    vf6,0x20(%1)
	lqc2    vf7,0x30(%1)
	lqc2    vf8,0x0(%2)
	vmulax.xyzw	ACC,   vf4,vf8
	vmadday.xyzw	ACC,   vf5,vf8
	vmaddaz.xyzw	ACC,   vf6,vf8
	vmaddw.xyzw	vf12,vf7,vf8
	sqc2    vf12,0x0(%0)
	": : "r" (v0) , "r" (m0) ,"r" (v1): "memory");
}

inline void Vu0TransposeMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1)
{
	asm __volatile__("
	lq $8,0x0000(%1)
	lq $9,0x0010(%1)
	lq $10,0x0020(%1)
	lq $11,0x0030(%1)

	pextlw     $12,$9,$8
	pextuw     $13,$9,$8
	pextlw     $14,$11,$10
	pextuw     $15,$11,$10

	pcpyld     $8,$14,$12
	pcpyud     $9,$12,$14
	pcpyld     $10,$15,$13
	pcpyud     $11,$13,$15

	sq $8,0x0000(%0)
	sq $9,0x0010(%0)
	sq $10,0x0020(%0)
	sq $11,0x0030(%0)
	": : "r" (m0) , "r" (m1): "memory");
}

void VuApplyMatrixPers(m,a,b)
float	m[4][4];
float	a[4];
float	b[4];
{
	Vu0ApplyMatrixPers(b, m, a);
}

void VuApplyMatrix(m,a,b)
float	m[4][4];
float	a[4];
float	b[4];
{
	Vu0ApplyMatrix(b, m, a);
}

void ApplyMatrix(m,a,b)
float	m[4][4];
float	a[4];
float	b[4];
{
	sceVu0FMATRIX m0;

	Vu0TransposeMatrix(m0, m);
	Vu0ApplyMatrix(b, m0, a);
/*	b[0]= m[0][0]*a[0] + m[0][1]*a[1] + m[0][2]*a[2] + m[0][3]*a[3];
	b[1]= m[1][0]*a[0] + m[1][1]*a[1] + m[1][2]*a[2] + m[1][3]*a[3];
	b[2]= m[2][0]*a[0] + m[2][1]*a[1] + m[2][2]*a[2] + m[2][3]*a[3];
	b[3]= m[3][0]*a[0] + m[3][1]*a[1] + m[3][2]*a[2] + m[3][3]*a[3];*/

}
#endif

inline void Vu0Normalize(sceVu0FVECTOR v0, sceVu0FVECTOR v1)
{
	asm __volatile__("
	lqc2    vf4,0x0(%1)
	vmul.xyz vf5,vf4,vf4
	vaddy.x vf5,vf5,vf5
	vaddz.x vf5,vf5,vf5

	vsqrt Q,vf5x
	vwaitq
	vaddq.x vf5x,vf0x,Q
	vdiv  Q,vf0w,vf5x
	vsub.xyzw vf7,vf0,vf0		#vf7.xyzw=0;
	vwaitq

	vmulq.xyz  vf7,vf4,Q
	sqc2    vf7,0x0(%0)
	": : "r" (v0) , "r" (v1): "memory");
}

 void VectorNormal(float *vct)
{
	Vu0Normalize(vct, vct);
/*float abs;
        abs= sqrtf(vct[0]*vct[0]+vct[1]*vct[1]+vct[2]*vct[2]);
        vct[0] /=abs;
        vct[1] /=abs;
        vct[2] /=abs;
*/
}

inline void Vu0OuterProduct(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2)
{
	asm __volatile__
	("
		lqc2    vf4,0x0(%1)
		lqc2    vf5,0x0(%2)
		vopmula.xyz	ACC,vf4,vf5
		vopmsub.xyz	vf6,vf5,vf4
		vsub.w vf6,vf6,vf6		#vf6.xyz=0;
		sqc2    vf6,0x0(%0)
	": : "r" (v0) , "r" (v1) ,"r" (v2): "memory");
}

inline void OuterProduct(float *v0, float *v1, float *v2)
{
	Vu0OuterProduct(v2, v0, v1);
/*    v2[0]= v0[1]*v1[2]-v0[2]*v1[1];
    v2[1]= v0[2]*v1[0]-v0[0]*v1[2];
    v2[2]= v0[0]*v1[1]-v0[1]*v1[0];
*/
}

inline void Vu0SubOuterProduct(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2, sceVu0FVECTOR v3 )
{
register u_int reg0 = 0;
	asm __volatile__
	("
		lqc2    vf4,0x0(%1)
		lqc2    vf5,0x0(%2)
		lqc2    vf7,0x0(%3)
		vsub.xyz	vf5,vf5,vf4
		vsub.xyz	vf4,vf4,vf7
		vopmula.xyz	ACC,vf5,vf4
		vopmsub.xyz	vf6,vf4,vf5
		vsub.w vf6,vf6,vf6		#vf6.xyz=0;
		li	%4,0x3827c5ac		#$5=0.00004
		ctc2	%4,$vi21			#I reg=$5
		vnop
		vmuli.xyz	vf6,vf6,I
		sqc2    vf6,0x0(%0)
	": : "r" (v0) , "r" (v1) ,"r" (v2), "r" (v3), "r" (reg0): "memory" );
}

inline void SubOuterProduct(float *v0, float *v1, float *v2, float *v3)
{
	Vu0SubOuterProduct(v3, v0, v1, v2);
}

/*get rotation matrix from rotation axis(normalized)*/
void get_rotmatrix_from_axis(phs,z,y,x,rmat)
float	phs;
float	z,y,x;
float	rmat[4][4];
{
	float	az,ay,ax;
	float	amat[4][4];
	float	iamat[4][4];
	float	rotx[4][4];
	float	arot[4][4];
	float	v0[4] __attribute__((aligned(16)));
	float	v1[4] __attribute__((aligned(16)));
	float	v2[4] __attribute__((aligned(16)));

	az= fabsf(z);
	ay= fabsf(y);
	ax= fabsf(x);
	if((az>=ay)&&(az>=ax)){
		v1[0]= -z;	v1[1]= 0.0f;	v1[2]= x;
	}
	if((ay>=ax)&&(ay>=az)){
		v1[0]= -y;	v1[1]= x;	v1[2]= 0.0f;
	}
	if((ax>=ay)&&(ax>=az)){	
		v1[0]= y;	v1[1]= -x;	v1[2]= 0.0f;
	}

	VectorNormal(v1);

	v0[0]= x;	v0[1]= y;	v0[2]= z;

	OuterProduct(v0,v1,v2);

	amat[0][0]= v0[0];	amat[0][1]= v1[0];	amat[0][2]= v2[0];	amat[0][3]= 0.0f;
	amat[1][0]= v0[1];	amat[1][1]= v1[1];	amat[1][2]= v2[1];	amat[1][3]= 0.0f;
	amat[2][0]= v0[2];	amat[2][1]= v1[2];	amat[2][2]= v2[2];	amat[2][3]= 0.0f;
	amat[3][0]= 0.0f;		amat[3][1]= 0.0f;		amat[3][2]= 0.0f;		amat[3][3]= 1.0f;

	iamat[0][0]= v0[0];	iamat[0][1]= v0[1];	iamat[0][2]= v0[2];	iamat[0][3]= 0.0f;
	iamat[1][0]= v1[0];	iamat[1][1]= v1[1];	iamat[1][2]= v1[2];	iamat[1][3]= 0.0f;
	iamat[2][0]= v2[0];	iamat[2][1]= v2[1];	iamat[2][2]= v2[2];	iamat[2][3]= 0.0f;
	iamat[3][0]= 0.0f;		iamat[3][1]= 0.0f;		iamat[3][2]= 0.0f;		iamat[3][3]= 1.0f;

	rotx[0][0]= 1.0f;	rotx[0][1]= 0.0f;		rotx[0][2]= 0.0f;		rotx[0][3]= 0.0f;
	rotx[1][0]= 0.0f;		rotx[1][1]= cosf(phs);	rotx[1][2]= -sinf(phs);	rotx[1][3]= 0.0f;
	rotx[2][0]= 0.0f;		rotx[2][1]= sinf(phs);	rotx[2][2]= cosf(phs);	rotx[2][3]= 0.0f;
	rotx[3][0]= 0.0f;		rotx[3][1]= 0.0f;		rotx[3][2]= 0.0f;		rotx[3][3]= 1.0f;

	MulMatrix(amat,rotx,arot);

	MulMatrix(arot,iamat,rmat);
}

/*calculate normal vector on lattice*/
void get_normal(lpos,lnml,NLATTX,NLATTZ)
float   lpos[WAVE_X_MAX][WAVE_Z_MAX][3];
float   lnml[WAVE_X_MAX][WAVE_Z_MAX][3];
int	NLATTX,NLATTZ;
{
	int	i,j;
	float	v0[4] __attribute__((aligned(16)));
	float	v1[4] __attribute__((aligned(16)));
	float	nv[4] __attribute__((aligned(16)));

	/*innner vertex*/
	for(i=1;i<NLATTZ-1;i++){
	    for(j=1;j<NLATTX-1;j++){
		v0[0]= lpos[i][j+1][0]-lpos[i][j-1][0];
		v0[1]= lpos[i][j+1][1]-lpos[i][j-1][1];
		v0[2]= lpos[i][j+1][2]-lpos[i][j-1][2];
		v1[0]= lpos[i+1][j][0]-lpos[i-1][j][0];
		v1[1]= lpos[i+1][j][1]-lpos[i-1][j][1];
		v1[2]= lpos[i+1][j][2]-lpos[i-1][j][2];
		OuterProduct(v0,v1,nv);
		VectorNormal(nv);
		lnml[i][j][0]= nv[0];
		lnml[i][j][1]= nv[1];
		lnml[i][j][2]= nv[2];
	    }
	}
	/*edge 0,2*/
	for(i=1;i<NLATTZ-1;i++){
	    v0[0]= lpos[i][1][0]-lpos[i][0][0];
	    v0[1]= lpos[i][1][1]-lpos[i][0][1];
	    v0[2]= lpos[i][1][2]-lpos[i][0][2];
	    v1[0]= lpos[i+1][0][0]-lpos[i-1][0][0];
	    v1[1]= lpos[i+1][0][1]-lpos[i-1][0][1];
	    v1[2]= lpos[i+1][0][2]-lpos[i-1][0][2];
	    OuterProduct(v0,v1,nv);
	    VectorNormal(nv);
	    lnml[i][0][0]= nv[0];
	    lnml[i][0][1]= nv[1];
	    lnml[i][0][2]= nv[2];

	    v0[0]= lpos[i][NLATTX-1][0]-lpos[i][NLATTX-2][0];
	    v0[1]= lpos[i][NLATTX-1][1]-lpos[i][NLATTX-2][1];
	    v0[2]= lpos[i][NLATTX-1][2]-lpos[i][NLATTX-2][2];
	    v1[0]= lpos[i+1][NLATTX-1][0]-lpos[i-1][NLATTX-1][0];
	    v1[1]= lpos[i+1][NLATTX-1][1]-lpos[i-1][NLATTX-1][1];
	    v1[2]= lpos[i+1][NLATTX-1][2]-lpos[i-1][NLATTX-1][2];
	    OuterProduct(v0,v1,nv);
	    VectorNormal(nv);
	    lnml[i][NLATTX-1][0]= nv[0];
	    lnml[i][NLATTX-1][1]= nv[1];
	    lnml[i][NLATTX-1][2]= nv[2];
	}
	/*edge 1,3*/
	for(j=1;j<NLATTX-1;j++){
	    v0[0]= lpos[0][j+1][0]-lpos[0][j-1][0];
	    v0[1]= lpos[0][j+1][1]-lpos[0][j-1][1];
	    v0[2]= lpos[0][j+1][2]-lpos[0][j-1][2];
	    v1[0]= lpos[1][j][0]-lpos[0][j][0];
	    v1[1]= lpos[1][j][1]-lpos[0][j][1];
	    v1[2]= lpos[1][j][2]-lpos[0][j][2];
	    OuterProduct(v0,v1,nv);
	    VectorNormal(nv);
	    lnml[0][j][0]= nv[0];
	    lnml[0][j][1]= nv[1];
	    lnml[0][j][2]= nv[2];

	    v0[0]= lpos[NLATTZ-1][j+1][0]-lpos[NLATTZ-1][j-1][0];
	    v0[1]= lpos[NLATTZ-1][j+1][1]-lpos[NLATTZ-1][j-1][1];
	    v0[2]= lpos[NLATTZ-1][j+1][2]-lpos[NLATTZ-1][j-1][2];
	    v1[0]= lpos[NLATTZ-1][j][0]-lpos[NLATTZ-2][j][0];
	    v1[1]= lpos[NLATTZ-1][j][1]-lpos[NLATTZ-2][j][1];
	    v1[2]= lpos[NLATTZ-1][j][2]-lpos[NLATTZ-2][j][2];
	    OuterProduct(v0,v1,nv);
	    VectorNormal(nv);
	    lnml[NLATTZ-1][j][0]= nv[0];
	    lnml[NLATTZ-1][j][1]= nv[1];
	    lnml[NLATTZ-1][j][2]= nv[2];
	}
	/*corner 0*/
	v0[0]= lpos[0][1][0]-lpos[0][0][0];
	v0[1]= lpos[0][1][1]-lpos[0][0][1];
	v0[2]= lpos[0][1][2]-lpos[0][0][2];
	v1[0]= lpos[1][0][0]-lpos[0][0][0];
	v1[1]= lpos[1][0][1]-lpos[0][0][1];
	v1[2]= lpos[1][0][2]-lpos[0][0][2];
	OuterProduct(v0,v1,nv);
	VectorNormal(nv);

	lnml[0][0][0]= nv[0];
	lnml[0][0][1]= nv[1];
	lnml[0][0][2]= nv[2];

	/*corner 1*/
	v0[0]= lpos[0][NLATTX-1][0]-lpos[0][NLATTX-2][0];
	v0[1]= lpos[0][NLATTX-1][1]-lpos[0][NLATTX-2][1];
	v0[2]= lpos[0][NLATTX-1][2]-lpos[0][NLATTX-2][2];
	v1[0]= lpos[1][NLATTX-1][0]-lpos[0][NLATTX-1][0];
	v1[1]= lpos[1][NLATTX-1][1]-lpos[0][NLATTX-1][1];
	v1[2]= lpos[1][NLATTX-1][2]-lpos[0][NLATTX-1][2];

	OuterProduct(v0,v1,nv);
	VectorNormal(nv);
	lnml[0][NLATTX-1][0]= nv[0];
	lnml[0][NLATTX-1][1]= nv[1];
	lnml[0][NLATTX-1][2]= nv[2];

	/*corner 2*/
	v0[0]= lpos[NLATTZ-1][1][0]-lpos[NLATTZ-1][0][0];
	v0[1]= lpos[NLATTZ-1][1][1]-lpos[NLATTZ-1][0][1];
	v0[2]= lpos[NLATTZ-1][1][2]-lpos[NLATTZ-1][0][2];
	v1[0]= lpos[NLATTZ-1][0][0]-lpos[NLATTZ-2][0][0];
	v1[1]= lpos[NLATTZ-1][0][1]-lpos[NLATTZ-2][0][1];
	v1[2]= lpos[NLATTZ-1][0][2]-lpos[NLATTZ-2][0][2];
	OuterProduct(v0,v1,nv);
	VectorNormal(nv);
	lnml[NLATTZ-1][0][0]= nv[0];
	lnml[NLATTZ-1][0][1]= nv[1];
	lnml[NLATTZ-1][0][2]= nv[2];

	/*corner 3*/
	v0[0]= lpos[NLATTZ-1][NLATTX-1][0]-lpos[NLATTZ-1][NLATTX-2][0];
	v0[1]= lpos[NLATTZ-1][NLATTX-1][1]-lpos[NLATTZ-1][NLATTX-2][1];
	v0[2]= lpos[NLATTZ-1][NLATTX-1][2]-lpos[NLATTZ-1][NLATTX-2][2];
	v1[0]= lpos[NLATTZ-1][NLATTX-1][0]-lpos[NLATTZ-2][NLATTX-1][0];
	v1[1]= lpos[NLATTZ-1][NLATTX-1][1]-lpos[NLATTZ-2][NLATTX-1][1];
	v1[2]= lpos[NLATTZ-1][NLATTX-1][2]-lpos[NLATTZ-2][NLATTX-1][2];
	OuterProduct(v0,v1,nv);
	VectorNormal(nv);
	lnml[NLATTZ-1][NLATTX-1][0] = nv[0];
	lnml[NLATTZ-1][NLATTX-1][1] = nv[1];
	lnml[NLATTZ-1][NLATTX-1][2] = nv[2];
}

void get_SWmatrix_from_view(view,target,swmtx)
float	view[3];
float	target[3];
float	swmtx[4][4];
{
	float	v[4] __attribute__((aligned(16)));
	float	bn;

	if(target[0]==view[0]&&target[1]==view[1]&&target[2]==view[2]){
		printf("Singular point of view2mtx: distance=0\n");
        	swmtx[0][0]= 1.0f;
        	swmtx[0][1]= 0.0f;
        	swmtx[0][2]= 0.0f;
        	swmtx[0][3]= 0.0f;

        	swmtx[1][0]= 0.0f;
        	swmtx[1][1]= 1.0f;
        	swmtx[1][2]= 0.0f;
        	swmtx[1][3]= 0.0f;

        	swmtx[2][0]= 0.0f;
        	swmtx[2][1]= 0.0f;
        	swmtx[2][2]= 1.0f;
        	swmtx[2][3]= 1.0f;

        	swmtx[3][0]= 0.0f;
        	swmtx[3][1]= 0.0f;
        	swmtx[3][2]= 0.0f;
        	swmtx[3][3]= 1.0f;
		ERR_STOP;
	}

	v[0]= target[0]-view[0];	
	v[1]= target[1]-view[1];	
	v[2]= target[2]-view[2];

	VectorNormal(v);
	bn= sqrtf(v[0]*v[0]+v[2]*v[2]);

	if(bn==0){
		printf("Singular point of view2mtx: ON Y-axis\n");
        	swmtx[0][0]= 1.0f;
        	swmtx[0][1]= 0.0f;
        	swmtx[0][2]= 0.0f;
        	swmtx[0][3]= 0.0f;

        	swmtx[1][0]= 0.0f;
        	swmtx[1][1]= 1.0f;
        	swmtx[1][2]= 0.0f;
        	swmtx[1][3]= 0.0f;

        	swmtx[2][0]= 0.0f;
        	swmtx[2][1]= 0.0f;
        	swmtx[2][2]= 1.0f;
        	swmtx[2][3]= sqrtf((target[1]-view[1])*(target[1]-view[1]));

        	swmtx[3][0]= 0.0f;
        	swmtx[3][1]= 0.0f;
        	swmtx[3][2]= 0.0f;
        	swmtx[3][3]= 1.0f;
		ERR_STOP;
	}

        swmtx[0][0]= v[2]/bn;
        swmtx[0][1]= 0.0f;
        swmtx[0][2]= -v[0]/bn;
        swmtx[0][3]= 0.0f;

        swmtx[1][0]= -v[1]*v[0]/bn;
        swmtx[1][1]= (v[2]*v[2]+v[0]*v[0])/bn;
        swmtx[1][2]= -v[1]*v[2]/bn;
        swmtx[1][3]= 0.0f;

        swmtx[2][0]= v[0];
        swmtx[2][1]= v[1];
        swmtx[2][2]= v[2];
        swmtx[2][3]= sqrtf((target[0]-view[0])*(target[0]-view[0])
                          +(target[1]-view[1])*(target[1]-view[1])
                          +(target[2]-view[2])*(target[2]-view[2]));

        swmtx[3][0]= 0.0f;
        swmtx[3][1]= 0.0f;
        swmtx[3][2]= 0.0f;
        swmtx[3][3]= 1.0f;
}

/*Transpose of get_SWmatrix_from_view*/
void get_VuSWmatrix_from_view(view,target,swmtx)
float	view[3];
float	target[3];
float	swmtx[4][4];
{
	float	v[4] __attribute__((aligned(16)));
	float	bn;

	if(target[0]==view[0]&&target[1]==view[1]&&target[2]==view[2]){
		printf("Singular point of view2mtx: distance=0\n");
		swmtx[0][0]= 1.0f;
		swmtx[0][1]= 0.0f;
		swmtx[0][2]= 0.0f;
		swmtx[0][3]= 0.0f;

		swmtx[1][0]= 0.0f;
		swmtx[1][1]= 1.0f;
		swmtx[1][2]= 0.0f;
		swmtx[1][3]= 0.0f;

		swmtx[2][0]= 0.0f;
		swmtx[2][1]= 0.0f;
		swmtx[2][2]= 1.0f;
		swmtx[2][3]= 0.0f;

		swmtx[3][0]= 0.0f;
		swmtx[3][1]= 0.0f;
		swmtx[3][2]= 1.0f;
		swmtx[3][3]= 1.0f;
		ERR_STOP;
	}

	v[0]= target[0]-view[0];	
	v[1]= target[1]-view[1];	
	v[2]= target[2]-view[2];

	VectorNormal(v);
	bn= sqrtf(v[0]*v[0]+v[2]*v[2]);

	if(bn==0){
		printf("Singular point of view2mtx: ON Y-axis\n");
		swmtx[0][0]= 1.0f;
		swmtx[0][1]= 0.0f;
		swmtx[0][2]= 0.0f;
		swmtx[0][3]= 0.0f;

		swmtx[1][0]= 0.0f;
		swmtx[1][1]= 1.0f;
		swmtx[1][2]= 0.0f;
		swmtx[1][3]= 0.0f;

		swmtx[2][0]= 0.0f;
		swmtx[2][1]= 0.0f;
		swmtx[2][2]= 1.0f;
		swmtx[2][3]= 0.0f;

		swmtx[3][0]= 0.0f;
		swmtx[3][1]= 0.0f;
		swmtx[3][2]= sqrtf((target[1]-view[1])*(target[1]-view[1]));
		swmtx[3][3]= 1.0f;
		ERR_STOP;
	}

	swmtx[0][0]= v[2]/bn;
	swmtx[0][1]= -v[1]*v[0]/bn;
	swmtx[0][2]= v[0];
	swmtx[0][3]= 0.0f;

	swmtx[1][0]= 0.0f;
	swmtx[1][1]= (v[2]*v[2]+v[0]*v[0])/bn;
	swmtx[1][2]= v[1];
	swmtx[1][3]= 0.0f;

	swmtx[2][0]= -v[0]/bn;
	swmtx[2][1]= -v[1]*v[2]/bn;
	swmtx[2][2]= v[2];
	swmtx[2][3]= 0.0f;

	swmtx[3][0]= 0.0f;
	swmtx[3][1]= 0.0f;
	swmtx[3][2]= sqrtf((target[0]-view[0])*(target[0]-view[0])
			  +(target[1]-view[1])*(target[1]-view[1])
			  +(target[2]-view[2])*(target[2]-view[2]));
	swmtx[3][3]= 1.0f;
}

