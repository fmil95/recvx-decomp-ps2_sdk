/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *
 *                         Version 0.5.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : joint matrix interpolation runtime
 *                     Description : axis interpolate functions
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.5.0          Aug,16,1999     aoki
 */
#include <eekernel.h>                    
#include <regdef.h>
#include <eeregs.h>
#include <libvu0.h>
#include <math.h>
#include "axis.h"

/* 
 * 軸 {x,y,z} (x^2+y^2+z^2=1)周りに角度 dの回転マトリクス m は、
 *	s=sin(d), c=cos(d) として, 以下のように計算できる
 */
/*
 *  m =  {[ (1-c) x^2 + c,	   (1-c) x y + z s,	   (1-c) x z - y s ],
 *	  [ (1-c) x y - z s,	   (1-c) y^2 + c,	   (1-c) y z + x s ],
 *	  [ (1-c) x z + y s,	   (1-c) y z - x s, 	   (1-c) z^2 + c ]}
 *
 */
void a2m(sceVu0FMATRIX m, sceVu0FVECTOR a)
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
