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
 *                     Name : joint matrix interpolation mkdata
 *                     Description : matrix functions
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
#include "matrix.h"

int EigenVector(sceVu0FVECTOR t, sceVu0FVECTOR v0, sceVu0FMATRIX m)
            			/* m: Rotation Matrix */
            			/* t: Eigen Vector */
            			/* v: perpendicular vector to t */

{
    sceVu0FMATRIX me;
    sceVu0FVECTOR v1;
    sceVu0FVECTOR nv;
    int i,j;
    int maxcol;
    float f;
    float maxf;

    sceVu0CopyMatrix(me,m);
    
    // �P�ʍs�������
    for (i=0; i<3; i++) me[i][i]-=1.0f;

    // ��Βl�ő�̏��s�񎮂����߂�
    maxf= -1.0f;
    for (i=0; i<3; i++){
	for (j=0; j<3; j++){
	    f = (me[(i+1)%3][(j+1)%3] * me[(i+2)%3][(j+2)%3])
		- (me[(i+1)%3][(j+2)%3] * me[(i+2)%3][(j+1)%3]);
	    f = fabs(f);
	    if (f>maxf){
		maxf=f;
		maxcol=i;
	    }
	}
    }

        // ��Βl�̈�ԑ傫�����s�񎮂ɏ]���Ĉꎟ�Ɨ��ȃx�N�g����v0,v1�ɓ����
    for (i=0; i<3; i++){
	v0[i]=me[(maxcol+1)%3][i];
	v1[i]=me[(maxcol+2)%3][i];
    }
    v0[3]=v1[3]=1.0f;

        // v0 ��v1�̊O�σx�N�g�������
    sceVu0OuterProduct(nv, v0, v1);

        // �O�σx�N�g���𐳋K������ (�ŗL�x�N�g��)
    sceVu0Normalize(t, nv);

#define MINMINOR 0.00000000001f
    if (maxf<MINMINOR){
		return -1;				// �l�����������ďo�͂ɐM���������Ȃ�
    } else{
	return 1;				// ����I��
    }
}

void EigenVec2Mat(sceVu0FMATRIX smat, sceVu0FVECTOR ev, sceVu0FVECTOR t)
{
    sceVu0FVECTOR mv0, mv1, mv2;
    int i;

    sceVu0UnitMatrix(smat);
    sceVu0Normalize(mv0, ev);
    sceVu0Normalize(mv1, t); 
    sceVu0OuterProduct(mv2,mv0,mv1);
    sceVu0Normalize(mv2,mv2);

    for (i=0; i<3; i++) smat[i][0]=mv0[i];
    for (i=0; i<3; i++) smat[i][1]=mv1[i];
    for (i=0; i<3; i++) smat[i][2]=mv2[i];

    return;
}

