/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *
 *                         Version 1.1
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : joint matrix interpolation mkdata
 *                     Description : axis interpolatin functions
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.5.0          Aug,16,1999     aoki
 *       1.1            Oct,13,1999     aoki	remove error-size output
 */

#include <eekernel.h>                    
#include <regdef.h>
#include <eeregs.h>
#include <libvu0.h>
#include <libgraph.h>
#include <math.h>
#include "matrix.h"
#include "axis.h"
extern int printf(const char *str, ...);

static void m2a(sceVu0FVECTOR a, sceVu0FMATRIX m);

void mat2axis(sceVu0FVECTOR a, sceVu0FVECTOR t, sceVu0FMATRIX m0, sceVu0FMATRIX m1, u_int n)
{
    sceVu0FMATRIX md;
    int i;
    float f;

    sceVu0TransposeMatrix(md, m0);
    for (i=0; i<3; i++) md[i][3]=0.0f;		// cancel translation
    sceVu0MulMatrix(md,m1,md);			// md*m0=m1, md=m1*m0^(-1)
    
    m2a(a,md);					// calc axis from rotation matrix
    for (i=0; i<4; i++) t[i]=m1[3][i]-m0[3][i];	// calc translation diff

    f=(float)n;
    a[3]/=f;
    sceVu0DivVector(t,t,f);

}

static void m2a(sceVu0FVECTOR a, sceVu0FMATRIX m)
{
    int i;

    sceVu0FMATRIX r1, eig, ieig, xrot;
    sceVu0FVECTOR vt;

    /*  ŒÅ—LƒxƒNƒgƒ‹‚Æ·•ª‰ñ“]Ž²‚Í“¯‚¶Œü‚« */
    if (EigenVector(a, vt, m)<0){
	for (i=0; i<4; i++) a[i]=0.0f;
    } else{
/*  ‰ñ“]Šp“x‚ð‹‚ß‚é */
/*  rotm ‚ð X Ž²Žü‚è‚Ì‰ñ“]ƒ}ƒgƒŠƒNƒX xrot ‚É•ÏŠ·‚·‚é */

	EigenVec2Mat(eig, a, vt);		/*  eig * XŽ² = ·•ª‰ñ“]Ž² (a) */
	sceVu0TransposeMatrix(ieig, eig);	/*  ieig = eig^(-1)  */
	sceVu0MulMatrix(r1, m, ieig);
	sceVu0MulMatrix(xrot, eig, r1);		/* xrot= eig * m * ieig */
	a[3]=atan2(xrot[1][2], xrot[1][1]);	/*  a[3] ‚ª·•ª‰ñ“]Šp */
	
    }
}

/* 
 *
 * Ž² {x,y,z} Žü‚è‚ÉŠp“x d‚Ì‰ñ“]ƒ}ƒgƒŠƒNƒX m ‚ÍAs=sin(d), c=cos(d) ‚Æ‚µ‚Ä
 *
 *  m =  {[ (1-c) x^2 + c,	   (1-c) x y + z s,	   (1-c) x z - y s ],
 *	  [ (1-c) x y - z s,	   (1-c) y^2 + c,	   (1-c) y z + x s ],
 *	  [ (1-c) x z + y s,	   (1-c) y z - x s, 	   (1-c) z^2 + c ]}
 *
 *	‚ÆŒvŽZ‚Å‚«‚é
 */
static void a2m(sceVu0FMATRIX m, sceVu0FVECTOR a)
{
    sceVu0FVECTOR xyz[3],s;
    int i,j;
    float c;

    sceVu0UnitMatrix(m);

    c=sinf(a[3]);
    sceVu0ScaleVector(s,a,c);

    c=cosf(a[3]);

    for (i=0; i<3; i++){
	sceVu0ScaleVector(xyz[i],a,a[i]);
    }

    for (i=0; i<3; i++){
	for (j=0; j<3; j++){
	    if (i==j){
		m[i][j]=c;
	    } else{
		if (((3+(i-j))%3)==1){
		    m[i][j]= -s[3-(i+j)];
		} else{
		    m[i][j]= s[3-(i+j)];
		}
	    }
	}
    }
    

    c=1.0f-c;
    for (i=0; i<3; i++){
	sceVu0ScaleVector(xyz[i],xyz[i],c);
    }
    
    for (i=0; i<3; i++){
	for (j=0; j<3; j++){
	    m[i][j]+=xyz[i][j];
	}
    }

}

void axis2mat(sceVu0FMATRIX m2, sceVu0FMATRIX m0, sceVu0FVECTOR a0, sceVu0FVECTOR t0, u_int n)
{
    sceVu0FMATRIX mr;
    sceVu0FVECTOR t;
    u_int i;

    a2m(mr, a0);			// calc Rotation Matrix from axis
    
    sceGsSyncV(0);

    sceVu0CopyMatrix(m2, m0);		// start rotation matrix

    for (i=0; i<3; i++){
	t[i]=m0[3][i];			// start translation
	m2[3][i]=0;			// cancel translation
    }
    t[3]=0.0f;

    for (i=0; i<n; i++){
	sceVu0MulMatrix(m2,mr,m2);	// next rotation matrix
	sceVu0AddVector(t,t,t0);	// next translation
    }

    sceVu0TransMatrix(m2,m2,t);		// add translation to rotation matrix

}

// check error (between original and runtime)
void chkerror(sceVu0FMATRIX m0, sceVu0FMATRIX m1)
{
    sceVu0FMATRIX md;
    sceVu0FVECTOR a;
    sceVu0FVECTOR t;
    int i;
    float f;

    sceVu0TransposeMatrix(md, m0);
    for (i=0; i<3; i++) md[i][3]=0.0f;		// cancel translation
    sceVu0MulMatrix(md,m1,md);			// md*m0=m1, md=m1*m0^(-1)

    
    m2a(a,md);					// calc axis from rotation matrix
    for (i=0; i<4; i++) t[i]=m1[3][i]-m0[3][i];	// calc translation diff

    f=0.0f;
    for (i=0; i<3; i++) f+=t[i]*t[i];

    f=sqrtf(f);
//    printf("error size: rotate=%f, translation=%f\n", a[3], f);

}
