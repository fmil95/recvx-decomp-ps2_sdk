/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
/******************************************************/
/************** matrix&vector utilities ***************/
/******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <eetypes.h>

void printmtx(float mtx[4][4]);
void printvct(float vct[4]);
void unit_matrix(float mtx[4][4]);
void TransposeMatrix3(float mtx[4][4],float tmtx[4][4]);
void MulMatrix(float a[4][4],float b[4][4],float m[4][4]);
void ApplyMatrix(float m[4][4],float a[4],float b[4]);
void OuterProduct(float v0[4],float v1[4],float v2[4]);
void VectorNormal(float *v);
void CopyVect(float v1[4], float v0[4]);
float VectDiff(float v0[4], float v1[4]);
float VectLength(float v[4]);
void set_VUvector(u_int *VUad,float vect[4]);
void set_VUmatrix(u_int *VUad,float mtx[4][4]);
void set_sincos_Taylor_coeff(u_int *VUad);
 

void printmtx(float mtx[4][4])
{
/*
	printf("[%08x,%08x,%08x,%08x]\n",*(u_int*)&mtx[0][0],
					 *(u_int*)&mtx[0][1],
					 *(u_int*)&mtx[0][2],
					 *(u_int*)&mtx[0][3]);
	printf("[%08x,%08x,%08x,%08x]\n",*(u_int*)&mtx[1][0],
					 *(u_int*)&mtx[1][1],
					 *(u_int*)&mtx[1][2],
					 *(u_int*)&mtx[1][3]);
	printf("[%08x,%08x,%08x,%08x]\n",*(u_int*)&mtx[2][0],
					 *(u_int*)&mtx[2][1],
					 *(u_int*)&mtx[2][2],
					 *(u_int*)&mtx[2][3]);
	printf("[%08x,%08x,%08x,%08x]\n",*(u_int*)&mtx[3][0],
					 *(u_int*)&mtx[3][1],
					 *(u_int*)&mtx[3][2],
					 *(u_int*)&mtx[3][3]);
*/
/*
	printf("[%08f,%08f,%08f,%08f]\n",mtx[0][0],mtx[0][1],mtx[0][2],mtx[0][3]);
	printf("[%08f,%08f,%08f,%08f]\n",mtx[1][0],mtx[1][1],mtx[1][2],mtx[1][3]);
	printf("[%08f,%08f,%08f,%08f]\n",mtx[2][0],mtx[2][1],mtx[2][2],mtx[2][3]);
	printf("[%08f,%08f,%08f,%08f]\n",mtx[3][0],mtx[3][1],mtx[3][2],mtx[3][3]);
*/
	printf("[%08d,%08d,%08d,%08d]\n",(int)mtx[0][0],(int)mtx[0][1],(int)mtx[0][2],(int)mtx[0][3]);
	printf("[%08d,%08d,%08d,%08d]\n",(int)mtx[1][0],(int)mtx[1][1],(int)mtx[1][2],(int)mtx[1][3]);
	printf("[%08d,%08d,%08d,%08d]\n",(int)mtx[2][0],(int)mtx[2][1],(int)mtx[2][2],(int)mtx[2][3]);
	printf("[%08d,%08d,%08d,%08d]\n",(int)mtx[3][0],(int)mtx[3][1],(int)mtx[3][2],(int)mtx[3][3]);
}

void printvct(float vct[4])
{
	printf("(%08x,%08x,%08x,%08x)\n",*(u_int*)&vct[0],
					 *(u_int*)&vct[1],
					 *(u_int*)&vct[2],
					 *(u_int*)&vct[3]);
}

void TransposeMatrix3(float mtx[4][4],float tmtx[4][4])
{
	tmtx[0][0]= mtx[0][0];    //transpose matrix
	tmtx[0][1]= mtx[1][0];
	tmtx[0][2]= mtx[2][0];
	tmtx[1][0]= mtx[0][1];
	tmtx[1][1]= mtx[1][1];
	tmtx[1][2]= mtx[2][1];
	tmtx[2][0]= mtx[0][2];
	tmtx[2][1]= mtx[1][2];
	tmtx[2][2]= mtx[2][2];
}

void unit_matrix(float mtx[4][4])
{
        mtx[0][0]= 1.0f; mtx[0][1]=0.0f; mtx[0][2]=0.0f; mtx[0][3]=0.0f;
        mtx[1][0]= 0.0f; mtx[1][1]=1.0f; mtx[1][2]=0.0f; mtx[1][3]=0.0f;
        mtx[2][0]= 0.0f; mtx[2][1]=0.0f; mtx[2][2]=1.0f; mtx[2][3]=0.0f;
        mtx[3][0]= 0.0f; mtx[3][1]=0.0f; mtx[3][2]=0.0f; mtx[3][3]=1.0f;
}

void MulMatrix(float a[4][4],float b[4][4],float m[4][4])
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

void ApplyMatrix(float m[4][4],float a[4], float b[4])
{
	b[0]= m[0][0]*a[0] + m[0][1]*a[1] + m[0][2]*a[2] + m[0][3]*a[3];
	b[1]= m[1][0]*a[0] + m[1][1]*a[1] + m[1][2]*a[2] + m[1][3]*a[3];
	b[2]= m[2][0]*a[0] + m[2][1]*a[1] + m[2][2]*a[2] + m[2][3]*a[3];
	b[3]= m[3][0]*a[0] + m[3][1]*a[1] + m[3][2]*a[2] + m[3][3]*a[3];
}

void OuterProduct(float v0[4], float v1[4], float v2[4])
{
    v2[0]= v0[1]*v1[2]-v0[2]*v1[1];
    v2[1]= v0[2]*v1[0]-v0[0]*v1[2];
    v2[2]= v0[0]*v1[1]-v0[1]*v1[0];
}

void VectorNormal(float *vct)
{
	float abs;

        abs= sqrtf(vct[0]*vct[0]+vct[1]*vct[1]+vct[2]*vct[2]);
        vct[0] /=abs;
        vct[1] /=abs;
        vct[2] /=abs;
}

void CopyVect(float v1[4], float v0[4])
{
        v1[0]= v0[0];
        v1[1]= v0[1];
        v1[2]= v0[2];
        v1[3]= v0[3];
}

float VectLength(float v[4])
{
        return( sqrtf( v[0]*v[0]+v[1]*v[1]+v[2]*v[2] ) );
}

float VectDiff(float v0[4], float v1[4])
{
        return( fabs(v0[0]-v1[0])+fabs(v0[1]-v1[1])+fabs(v0[2]-v1[2]) );
}

void set_VUvector(u_int *VUad,float vect[4])
{
        VUad[0] = *(u_int *)&vect[0];
        VUad[1] = *(u_int *)&vect[1];
        VUad[2] = *(u_int *)&vect[2];
        VUad[3] = *(u_int *)&vect[3];
}

void set_VUmatrix(u_int *VUad,float mtx[4][4])
{
        VUad[0] = *(u_int *)&mtx[0][0];
        VUad[1] = *(u_int *)&mtx[1][0];
        VUad[2] = *(u_int *)&mtx[2][0];
        VUad[3] = *(u_int *)&mtx[3][0];
        VUad[4] = *(u_int *)&mtx[0][1];
        VUad[5] = *(u_int *)&mtx[1][1];
        VUad[6] = *(u_int *)&mtx[2][1];
        VUad[7] = *(u_int *)&mtx[3][1];
        VUad[8] = *(u_int *)&mtx[0][2];
        VUad[9] = *(u_int *)&mtx[1][2];
        VUad[10] = *(u_int *)&mtx[2][2];
        VUad[11] = *(u_int *)&mtx[3][2];
        VUad[12] = *(u_int *)&mtx[0][3];
        VUad[13] = *(u_int *)&mtx[1][3];
        VUad[14] = *(u_int *)&mtx[2][3];
        VUad[15] = *(u_int *)&mtx[3][3];
}

void set_sincos_Taylor_coeff(u_int *VUad)
{
        *(float*)(VUad)=0;
        *(float*)(VUad+1)=1.0;
        *(float*)(VUad+2)=1.0;
        *(float*)(VUad+3)=0;

        *(float*)(VUad+4*1)=0;
        *(float*)(VUad+4*1+1)= -1.0/(float)2;
        *(float*)(VUad+4*1+2)= -1.0/(float)(3*2);
        *(float*)(VUad+4*1+3)=0;

        *(float*)(VUad+4*2)=0;
        *(float*)(VUad+4*2+1)= 1.0/(float)(4*3*2);
        *(float*)(VUad+4*2+2)= 1.0/(float)(5*4*3*2);
        *(float*)(VUad+4*2+3)=0;

        *(float*)(VUad+4*3)=0;
        *(float*)(VUad+4*3+1)= -1.0/(float)(6*5*4*3*2);
        *(float*)(VUad+4*3+2)= -1.0/(float)(7*6*5*4*3*2);
        *(float*)(VUad+4*3+3)=0;

        *(float*)(VUad+4*4)=0;
        *(float*)(VUad+4*4+1)= 1.0/(float)(8*7*6*5*4*3*2);
        *(float*)(VUad+4*4+2)= 1.0/(float)(9*8*7*6*5*4*3*2);
        *(float*)(VUad+4*4+3)=0;

        *(float*)(VUad+4*5)=0;
        *(float*)(VUad+4*5+1)= -1.0;
        *(float*)(VUad+4*5+2)= 1.0;
        *(float*)(VUad+4*5+3)=0;
}

