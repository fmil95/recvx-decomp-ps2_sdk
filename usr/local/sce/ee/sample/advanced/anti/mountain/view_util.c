/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
/********************************************************/
/************* rotation  & viewing utilities ************/
/********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <eetypes.h>

#define ERR_STOP while(1)

extern void VectorNormal(float *v);
extern void OuterProduct(float *v0, float *v1, float *v2);
extern void MulMatrix(float a[4][4],float b[4][4],float m[4][4]);

void Euler(float a[3], float b[3], float c[3]);
void RotMatrix(float a[3], float m[4][4]);
void get_rotmatrix_from_axis(float p, float z, float y, float x, float r[4][4]);
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
	amat[3][0]= 0.0f;	amat[3][1]= 0.0f;	amat[3][2]= 0.0f;	amat[3][3]= 1.0f;

	iamat[0][0]= v0[0];	iamat[0][1]= v0[1];	iamat[0][2]= v0[2];	iamat[0][3]= 0.0f;
	iamat[1][0]= v1[0];	iamat[1][1]= v1[1];	iamat[1][2]= v1[2];	iamat[1][3]= 0.0f;
	iamat[2][0]= v2[0];	iamat[2][1]= v2[1];	iamat[2][2]= v2[2];	iamat[2][3]= 0.0f;
	iamat[3][0]= 0.0f;	iamat[3][1]= 0.0f;	iamat[3][2]= 0.0f;	iamat[3][3]= 1.0f;

	rotx[0][0]= 1.0f;	rotx[0][1]= 0.0f;	rotx[0][2]= 0.0f;	rotx[0][3]= 0.0f;
	rotx[1][0]= 0.0f;	rotx[1][1]= cosf(phs);	rotx[1][2]= -sinf(phs);	rotx[1][3]= 0.0f;
	rotx[2][0]= 0.0f;	rotx[2][1]= sinf(phs);	rotx[2][2]= cosf(phs);	rotx[2][3]= 0.0f;
	rotx[3][0]= 0.0f;	rotx[3][1]= 0.0f;	rotx[3][2]= 0.0f;	rotx[3][3]= 1.0f;

	MulMatrix(amat,rotx,arot);

	MulMatrix(arot,iamat,rmat);
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

        swmtx[1][0]= -v[1]*v[0]/bn;
        swmtx[1][1]= (v[2]*v[2]+v[0]*v[0])/bn;
        swmtx[1][2]= -v[1]*v[2]/bn;

        swmtx[2][0]= v[0];
        swmtx[2][1]= v[1];
        swmtx[2][2]= v[2];

	swmtx[0][3]= -(view[0]*swmtx[0][0]+view[1]*swmtx[0][1]+view[2]*swmtx[0][2]);
	swmtx[1][3]= -(view[0]*swmtx[1][0]+view[1]*swmtx[1][1]+view[2]*swmtx[1][2]);
	swmtx[2][3]= -(view[0]*swmtx[2][0]+view[1]*swmtx[2][1]+view[2]*swmtx[2][2]);

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

