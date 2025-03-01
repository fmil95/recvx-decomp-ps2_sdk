/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *
 *              Emotion Engine Library Sample Program
 *
 *                          - mipmap -
 *
 *                         Version 0.3.0
 *                             S-JIS	
 *
 *                     Name : mathfunc.c
 *                     Description : various math function
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.3.0          Jun, 6,1999     munekis     first version
 */

#include<math.h>
#include<stdio.h>
#include"mathfunc.h"

/* mathematical functions */
// ===========================================================================
// �s��v�Z�̊֐��Q(libvu0���)
//
// �s��̔z��`���͈ȉ��̂悤�ɂȂ��Ă��܂�. (libvu0�Ƃ͓]�u)
// 
//     |v[0][0] v[0][1] v[0][2] v[0][3]|
//  m =|v[1][0] v[1][1] v[1][2] v[1][3]|
//     |v[2][0] v[2][1] v[2][2] v[2][3]|
//     |v[3][0] v[3][1] v[3][2] v[3][3]|
// 


// ==========================================================================
//  sceSamp0ApplyMatrix		�x�N�g���Ƀ}�g���b�N�X����Z����B
//  �`��
//  	void sceSamp0ApplyMatrix(sceSamp0FVECTOR v0, sceSamp0FMATRIX m0, sceSamp0FVECTOR v1)
//  ����
//  	v1		����:�x�N�g��
//  	m0		����:�}�g���b�N�X
//  	v0		�o��:�x�N�g��
// 
//  ���
//  	�}�g���b�N�Xm�Ƀx�N�g��v0���E�����Z���āAv1�ɗ^����
// 
//  		v0=m0*v1
//  �Ԃ�l
//  	�Ȃ�
// --------------------------------------------------------------------------
void sceSamp0ApplyMatrix(sceSamp0FVECTOR v0, sceSamp0FMATRIX m0,
			 sceSamp0FVECTOR v1)
{
  sceSamp0FVECTOR v;
  v[0] = v1[0]*m0[0][0] + v1[1]*m0[0][1] + v1[2]*m0[0][2] + v1[3]*m0[0][3];
  v[1] = v1[0]*m0[1][0] + v1[1]*m0[1][1] + v1[2]*m0[1][2] + v1[3]*m0[1][3];
  v[2] = v1[0]*m0[2][0] + v1[1]*m0[2][1] + v1[2]*m0[2][2] + v1[3]*m0[2][3];
  v[3] = v1[0]*m0[3][0] + v1[1]*m0[3][1] + v1[2]*m0[3][2] + v1[3]*m0[3][3];
  sceSamp0CopyVector(v0, v);
}

// ==========================================================================
// 
//  sceSamp0MulMatrix		�Q�}�g���b�N�X�̐ς����߂�
//  �`��
//  	void sceSamp0MulMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, sceSamp0FMATRIX m2)
//  ����
//  	m1,m2		����:�}�g���b�N�X
//  	m0		�o��:�}�g���b�N�X
//  ���
//  	�}�g���b�N�Xm1�Ƀ}�g���b�N�Xm2���E�����Z���āAm0�ɗ^����
// 
//  		m0=m1*m2
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0MulMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1,
		       sceSamp0FMATRIX m2)
{

  int i, j;
  sceSamp0FMATRIX m;
   for(i=0; i<4; i++){
     for(j=0; j<4; j++){
       m[i][j] = m1[i][0] * m2[0][j] + m1[i][1] * m2[1][j]
	 + m1[i][2] * m2[2][j] + m1[i][3] * m2[3][j];
     }
   }
   sceSamp0CopyMatrix(m0, m);

}

// ==========================================================================
// 
//  sceSamp0OuterProduct		�Q�̃x�N�g���̊O�ς����߂�
//  �`��
//  	void sceSamp0OuterProduct(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, sceSamp0FVECTOR v2)
//  ����
//  	v1,v2		����:�x�N�g��
//  	v0		�o��:�x�N�g��
//  ���
//  	�x�N�g��v1,v2�̊O�ς����߂�v0�ɗ^����
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0OuterProduct(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1,
			  sceSamp0FVECTOR v2)
{
  v0[0] = v1[1] * v2[2] - v1[2] * v2[1];
  v0[1] = v1[2] * v2[0] - v1[0] * v2[2];
  v0[2] = v1[0] * v2[1] - v1[1] * v2[0];
  v0[3] = 0;
}

// ==========================================================================
// 
//  sceSamp0InnerProduct		�Q�̃x�N�g���̓��ς����߂�
//  �`��
//  	float sceSamp0InnerProduct(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1)
//  ����
//  	v0,v1		����:�x�N�g��
//  ���
//  	�x�N�g��v0,v1�̓��ς����߂�
//  �Ԃ�l
//  	����
// 
// --------------------------------------------------------------------------
float sceSamp0InnerProduct(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1)
{
  return (v0[0]*v1[0] + v0[1]*v1[1] + v0[2]*v1[2]);
}

// ==========================================================================
//  sceSamp0Normalize		�x�N�g���̐��K�����s��
//  �`��
//  	void sceSamp0Normalize(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1)
//  ����
//  	v1		����:�x�N�g��
//  	v0		�o��:�x�N�g��
//  ���
//  	�x�N�g��v0�̐��K�����s��v1�ɗ^����
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0Normalize(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1)
{
  float norm, dnorm;
  norm = sqrt( v1[0]*v1[0] + v1[1]*v1[1] + v1[2]*v1[2] );
  if(norm > 0){
    dnorm = 1.0/norm;
    v0[0] = v1[0]*dnorm;
    v0[1] = v1[1]*dnorm;
    v0[2] = v1[2]*dnorm;
    v0[3] = v1[3];
  }
}

// ==========================================================================
// 
//  sceSamp0TransposeMatrix	�}�g���b�N�X�̓]�u�s������߂�
//  �`��
//  	void sceSamp0TransposeMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1)
//  ����
//  	m1		����:�}�g���b�N�X
//  	m0		�o��:�}�g���b�N�X
//  ���
//  	�}�g���b�N�Xm1�̓]�u�s�������m0�ɗ^����
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0TransposeMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1)
{
  int i,j;
  sceSamp0FMATRIX t;
  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      t[i][j] = m1[j][i];
    }
  }
  sceSamp0CopyMatrix(m0, t);
}

// ==========================================================================
// sceSamp0InversMatrix	�t�s�� (4x4) �����߂�B�i�s��͐����Ɖ��肷��j
//  �`��
// 	void sceSamp0InversMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1);
//  ����
//  	m1		����:�}�g���b�N�X(��] & ���s�ړ�)
//  	m0		�o��:�}�g���b�N�X
//  ���
//  	�}�g���b�N�Xm1�̋t�s������߂āA�}�g���b�N�Xm0�ɗ^����
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0InversMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1)
{
  int i,j;
  sceSamp0FMATRIX m;
  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      m[i][j] = m1[j][i];
    }
  }
  m[0][3] = -(m1[0][3]*m[0][0] + m1[1][3]*m[0][1] + m1[2][3]*m[0][2]);
  m[1][3] = -(m1[0][3]*m[1][0] + m1[1][3]*m[1][1] + m1[2][3]*m[1][2]);
  m[2][3] = -(m1[0][3]*m[2][0] + m1[1][3]*m[2][1] + m1[2][3]*m[2][2]);
  m[3][0] = m[3][1] = m[3][2] = 0.0;
  m[3][3] = 1;
  sceSamp0CopyMatrix(m0, m);
}

// ==========================================================================
// sceSamp0AddVector	4������Z (ADD/xyzw)
//  �`��
// 	void sceSamp0AddVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, sceSamp0FVECTOR v2)
//  ����
//  	v1,v2		����:�x�N�g��
//  	v0		�o��:�x�N�g��
//  ���
//  	�x�N�g��v1�̊e�v�f�ƃx�N�g��v2�̊e�v�f���e�X���Z����B
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0AddVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, sceSamp0FVECTOR v2)
{
  v0[0] = v1[0] + v2[0];
  v0[1] = v1[1] + v2[1];
  v0[2] = v1[2] + v2[2];
  v0[3] = v1[3] + v2[3];
}

// ==========================================================================
// sceSamp0SubVector		4���񌸎Z (SUB/xyzw)
//  �`��
// 	void sceSamp0SubVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, sceSamp0FVECTOR v2)
//  ����
//  	v1,v2		����:�x�N�g��
//  	v0		�o��:�x�N�g��
//  ���
//  	�x�N�g��v1�̊e�v�f�ƃx�N�g��v2�̊e�v�f���e�X���Z����B
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0SubVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, sceSamp0FVECTOR v2)
{
  v0[0] = v1[0] - v2[0];
  v0[1] = v1[1] - v2[1];
  v0[2] = v1[2] - v2[2];
  v0[3] = v1[3] - v2[3];
}

// ==========================================================================
// sceSamp0MulVector	4����ώZ�iMUL/xyzw: ���ςł͂Ȃ����Ƃɒ��Ӂj
//  �`��
// 	void sceSamp0MulVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, sceSamp0FVECTOR v2)
//  ����
//  	v1,v2		����:�x�N�g��
//  	v0		�o��:�x�N�g��
//  ���
//  	�x�N�g��v1�̊e�v�f�ƃx�N�g��v2�̊e�v�f���e�X��Z����B
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0MulVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, sceSamp0FVECTOR v2)
{
  v0[0] = v1[0] * v2[0];
  v0[1] = v1[1] * v2[1];
  v0[2] = v1[2] * v2[2];
  v0[3] = v1[3] * v2[3];
}

// ==========================================================================
// sceSamp0ScaleVector	�X�J���[�l����Z���� (MULx/xyzw)
//  �`��
// 	void sceSamp0ScaleVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, float t)
//  ����
//  	v1		����:�x�N�g��
//  	t		����:�X�J���[
//  	v0		�o��:�x�N�g��
//  ���
//  	�x�N�g��v1�ɃX�J���[t����Z���ăx�N�g��v0�ɗ^����
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0ScaleVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, float t)
{
  v0[0] = v1[0]*t;
  v0[1] = v1[1]*t;
  v0[2] = v1[2]*t;
  v0[3] = v1[3]*t;
}

// ==========================================================================
// sceSamp0TransMatrix	�s��̕��s�ړ�
//  �`��
// 	void sceSamp0TransMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, sceSamp0FVECTOR tv);
//  ����
//  	tv		����:�ړ��x�N�g��
//  	m1		����:�}�g���b�N�X
//  	m0		�o��:�}�g���b�N�X
//  ���
//  	�^����ꂽ�x�N�g�����A�}�g���b�N�X�𕽍s�ړ�����
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0TransMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, sceSamp0FVECTOR tv)
{
  int i,j;
  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      m0[i][j] = m1[i][j];
    }
  }
  m0[0][3] = m1[0][3] + tv[0];
  m0[1][3] = m1[1][3] + tv[1];
  m0[2][3] = m1[2][3] + tv[2];
  m0[3][3] = m1[3][3] + tv[3];
}

// ==========================================================================
// sceSamp0CopyVector	  �x�N�g���̕��� (lq �� sq �̑g�����Balignment check �p��)
//  �`��
// 	void sceSamp0CopyVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1);
//  ����
//  	v1		����:�x�N�g��
//  	v0		�o��:�x�N�g��
//  ���
//  	�x�N�g��v1���x�N�g��v0�ɃR�s�[����B
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0CopyVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1)
{
  v0[0] = v1[0];
  v0[1] = v1[1];
  v0[2] = v1[2];
  v0[3] = v1[3];
}


// ==========================================================================
// sceSamp0CopyMatrix	     �s��̕��� (lq �� sq �̑g�����Balignment check �p��)
//  �`��
// 	void sceSamp0CopyMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1);
//  ����
//  	m1		����:�}�g���b�N�X
//  	m0		�o��:�}�g���b�N�X
//  ���
//  	�}�g���b�N�Xm1���}�g���b�N�Xm0�ɃR�s�[����B
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0CopyMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1)
{
  int i,j;
  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      m0[i][j] = m1[i][j];
    }
  }
}

// ==========================================================================
// sceSamp0FTOI0Vector 	FTOI0
//  �`��
// 	void sceSamp0FTOI0Vector(sceSamp0IVECTOR v0, sceSamp0FVECTOR v1);
//  ����
//  	v1		����:�x�N�g��
//  	v0		�o��:�x�N�g��
//  ���
//  	�^����ꂽ�x�N�g���̗v�f�𐮐��ɕϊ�����
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0FTOI0Vector(sceSamp0IVECTOR v0, sceSamp0FVECTOR v1)
{
  v0[0] = (int)v1[0];
  v0[1] = (int)v1[1];
  v0[2] = (int)v1[2];
  v0[3] = (int)v1[3];
}

// ==========================================================================
// sceSamp0ITOF0Vector 	ITOF0
//  �`��
// 	void sceSamp0ITOF0Vector(sceSamp0FVECTOR v0, sceSamp0IVECTOR v1);
//  ����
//  	v1		����:�x�N�g��
//  	v0		�o��:�x�N�g��
//  ���
//  	�^����ꂽ�x�N�g���̗v�f�𐮐��ɕϊ�����
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0ITOF0Vector(sceSamp0FVECTOR v0, sceSamp0IVECTOR v1)
{
  v0[0] = (float)v1[0];
  v0[1] = (float)v1[1];
  v0[2] = (float)v1[2];
  v0[3] = (float)v1[3];

}

// ==========================================================================
// sceSamp0UnitMatrix	�P�ʍs��
//  �`��
// 	void sceSamp0UnitMatrix(sceSamp0FMATRIX m0);
//  ����
//  	m1		����:�}�g���b�N�X
//  	m0		�o��:�}�g���b�N�X
//  ���
//  	�^����ꂽ�s���P�ʍs��ɕϊ�����
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0UnitMatrix(sceSamp0FMATRIX m0)
{
  int i,j;
  for(i=0; i<4; i++){
    for(j=0; j<4; j++){
      m0[i][j] = (i==j)? 1.0 : 0;
    }
  }
}

// ==========================================================================
// sceSamp0RotMatrixZ	�y���𒆐S�Ƃ����s��̉�]
//  �`��
// 	void sceSamp0RotMatrixZ(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, float rz);
//  ����
//  	rz		����:��]�p(�L���͈�:-�� �` ��)
//  	m1		����:�}�g���b�N�X
//  	m0		�o��:�}�g���b�N�X
//  ���
//  	��]�prt���w���𒆐S�Ƃ�����]�}�g���b�N�X�����߂āA
// 	�}�g���b�N�Xm1�ɍ��������Z���āA���̌��ʂ��}�g���b�N�X
// 	m0�ɗ^����
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0RotMatrixZ(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, float rz)
{
  sceSamp0FMATRIX rot;
  sceSamp0UnitMatrix(rot);
  /*  rz = rz * (3.141592/180.0);*/
  rot[0][0] = cosf(rz); rot[0][1] = -sinf(rz);
  rot[1][0] = sinf(rz); rot[1][1] = cosf(rz);
  sceSamp0MulMatrix(m0, rot, m1);
}

// ==========================================================================
// sceSamp0RotMatrixX	�w���𒆐S�Ƃ����s��̉�]
//  �`��
// 	void sceSamp0RotMatrixX(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, float ry);
//  ����
//  	rx		����:��]�p(�L���͈�:-�� �` ��)
//  	m1		����:�}�g���b�N�X
//  	m0		�o��:�}�g���b�N�X
//  ���
//  	��]�prt���w���𒆐S�Ƃ�����]�}�g���b�N�X�����߂āA
// 	�}�g���b�N�Xm1�ɍ��������Z���āA���̌��ʂ��}�g���b�N�X
// 	m0�ɗ^����
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0RotMatrixX(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, float rx)
{
  sceSamp0FMATRIX rot;
  sceSamp0UnitMatrix(rot);
  /*  rx = rx * (3.141592/180.0);*/
  rot[1][1] = cosf(rx); rot[1][2] = -sinf(rx);
  rot[2][1] = sinf(rx); rot[2][2] = cosf(rx);
  sceSamp0MulMatrix(m0, rot, m1);
}

// ==========================================================================
// sceSamp0RotMatrixY	�x���𒆐S�Ƃ����s��̉�]
//  �`��
// 	void sceSamp0RotMatrixY(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, float ry);
//  ����
//  	ry		����:��]�p(�L���͈�:-�� �` ��)
//  	m1		����:�}�g���b�N�X
//  	m0		�o��:�}�g���b�N�X
//  ���
//  	��]�prt���x���𒆐S�Ƃ�����]�}�g���b�N�X�����߂āA
// 	�}�g���b�N�Xm1�ɍ��������Z���āA���̌��ʂ��}�g���b�N�X
// 	m0�ɗ^����
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0RotMatrixY(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, float ry)
{
  sceSamp0FMATRIX rot;
  sceSamp0UnitMatrix(rot);
  /*  ry = ry * (3.141592/180.0);*/
  rot[0][0] = cosf(ry); rot[2][0] = sinf(ry);
  rot[0][2] = -sinf(ry); rot[2][2] = cosf(ry);
  sceSamp0MulMatrix(m0, rot, m1);
}

// ==========================================================================
// sceSamp0RotMatrix	X,Y,Z���𒆐S�Ƃ����s��̉�]
//  �`��
// 	void sceSamp0RotMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, sceSamp0FVECTOR rot);
//  ����
//  	rot		����:��]�p�x�N�g��(�L���͈�:-�� �` ��)
//  	m1		����:�}�g���b�N�X
//  	m0		�o��:�}�g���b�N�X
//  ���
//  	��]�p�x�N�g��rot���X,Y,Z�����ꂼ��𒆐S�Ƃ�����]�}�g���b�N�X
//       �����߁A�}�g���b�N�Xm1�ɍ��������Z���āA���̌��ʂ��}�g���b�N�X
// 	m0�ɗ^����DZ ,Y, X ���̏��ɉ�]����D
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0RotMatrix(sceSamp0FMATRIX m0, sceSamp0FMATRIX m1, sceSamp0FVECTOR rot)
{
  sceSamp0RotMatrixZ(m0, m1, rot[2]);
  sceSamp0RotMatrixY(m0, m0, rot[1]);
  sceSamp0RotMatrixX(m0, m0, rot[0]);
}

// ==========================================================================
// sceSamp0ClampVector	�x�N�g���̃N�����v
//  �`��
// 	void sceSamp0ClampVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, float min, float max)
//  ����
//  	v0		�o��:�x�N�g��
//  	v1		����:�x�N�g��
//  	min		����:��������(�؎̂ĉ���)
//  	max		����:��������(�؎̂ď��)
//  ���
//  	�x�N�g��v1�̊e�v�f�� min������ max������Ƃ��ăN�����v��
//     v0�ɗ^����D
//  �Ԃ�l
//  	�Ȃ�
// 
// --------------------------------------------------------------------------
void sceSamp0ClampVector(sceSamp0FVECTOR v0, sceSamp0FVECTOR v1, float min, float max)
{
  sceSamp0FVECTOR vect;
  /*
  v0[0] = v1[0]<min ? min : (v1[0]>max ? max : v1[0]);
  v0[1] = v1[1]<min ? min : (v1[1]>max ? max : v1[1]);
  v0[2] = v1[2]<min ? min : (v1[2]>max ? max : v1[2]);
//  v0[2] = v1[2]<min ? min : (v1[2]>max ? max : v1[2]);
  v0[3] = v1[3]<min ? min : (v1[3]>max ? max : v1[3]);
  */
  vect[0] = v1[0]<min ? min : (v1[0]>max ? max : v1[0]);
  vect[1] = v1[1]<min ? min : (v1[1]>max ? max : v1[1]);
  vect[2] = v1[2]<min ? min : (v1[2]>max ? max : v1[2]);
  vect[3] = v1[3]<min ? min : (v1[3]>max ? max : v1[3]);
  sceSamp0CopyVector(v0,vect);
}


// ==========================================================================
// sceSamp0CameraMatrix    ���[���h���W�n�ɑ΂���J�������W�n���`
// 
// 	p:  camera position (in the world)
// 	zd: eye direction
// 	yd: vertical direction
// 	xd = zd x yd:  wing direction
// 	yd = yd x xd:  real veritical direction
// 
// 	        | xd0 yd0 zd0 p0 |
// 	M = inv | xd1 yd1 zd1 p1 |
// 	        | xd2 yd2 zd2 p2 |
// 	        |   0   0   0  1 |
void sceSamp0CameraMatrix(sceSamp0FMATRIX m, sceSamp0FVECTOR p, sceSamp0FVECTOR zd, sceSamp0FVECTOR yd)
{
	sceSamp0FMATRIX	m0;
	sceSamp0FVECTOR	xd;

	sceSamp0UnitMatrix(m0);
	sceSamp0OuterProduct(xd, yd, zd);
	sceSamp0Normalize(m0[0], xd);
	sceSamp0Normalize(m0[2], zd);
	sceSamp0OuterProduct(m0[1], m0[2], m0[0]);
	sceSamp0TransposeMatrix(m0, m0);
	sceSamp0TransMatrix(m0, m0, p);
	sceSamp0InversMatrix(m, m0);
}


// ==========================================================================
// sceSamp0RotCameraMatrix    ���[���h���W�n�ɑ΂���J�������W�n���`(��]�L)
// 
// 	p:  original camera position (in the world)
// 	zd: original eye direction
// 	yd: original vertical direction
//     rot: rotation vector
// 	direction : real eye direction
// 	vertical  : real veritical direction
//     position  : real camera position
// 
void sceSamp0RotCameraMatrix(sceSamp0FMATRIX m, sceSamp0FVECTOR p, sceSamp0FVECTOR zd, sceSamp0FVECTOR yd, sceSamp0FVECTOR rot)
{
	sceSamp0FMATRIX	work;
	sceSamp0FVECTOR direction, vertical, position;

	sceSamp0UnitMatrix(work);
	sceSamp0RotMatrixX(work,work,rot[0]);
	sceSamp0RotMatrixY(work,work,rot[1]);
	sceSamp0RotMatrixZ(work,work,rot[2]);
	sceSamp0ApplyMatrix(direction, work, zd);
	sceSamp0ApplyMatrix(vertical, work, yd);
	sceSamp0ApplyMatrix(position, work, p);
	sceSamp0CameraMatrix(m, position, direction, vertical);

}

// ==========================================================================
// sceSamp0NormalLightMatrix    ���s����(�R��)��������x�N�g���s��𐶐�
// 
//  make normal light matrix
// 	l0-l2:	light direction
// 	    | l0x l0y l0z 0 |
// 	M = | l1x l1y l1z 0 |
// 	    | l2x l1y l2z 0 |
// 	    |   0   0   0 1 |

void sceSamp0NormalLightMatrix(sceSamp0FMATRIX m, sceSamp0FVECTOR l0, sceSamp0FVECTOR l1, sceSamp0FVECTOR l2)
{
        sceSamp0FVECTOR  t;
        sceSamp0ScaleVector(t, l0, -1); sceSamp0Normalize(m[0], t);
        sceSamp0ScaleVector(t, l1, -1); sceSamp0Normalize(m[1], t);
        sceSamp0ScaleVector(t, l2, -1); sceSamp0Normalize(m[2], t);

	m[3][0] = m[3][1] = m[3][2] = 0;
	m[3][3] = 1.0;
}

// =========================================================================
// sceSamp0LighrColorMatrix   ���s����(�R��)�̃J���[�s��𐶐�
// 
//  make normal light matrix
// 	c0-c2:	light color
// 	a:	ambient color
// 
// 	    | c0r c1r c2r ar |
// 	M = | c0g c1g c2g ag |
// 	    | c0b c1b c2b ab |
// 	    |   0   0   0  0 |
// 
void sceSamp0LightColorMatrix(sceSamp0FMATRIX m, sceSamp0FVECTOR c0, sceSamp0FVECTOR c1, sceSamp0FVECTOR c2, sceSamp0FVECTOR a)
{
	sceSamp0CopyVector(m[0], c0);
	sceSamp0CopyVector(m[1], c1);
	sceSamp0CopyVector(m[2], c2);
	sceSamp0CopyVector(m[3],  a);
	sceSamp0TransposeMatrix(m, m);
}

// =========================================================================
//  sceSamp0NormalColorVector     �e���_�̃��C�e�B���O��̐F���v�Z
// 
//       local_light:  light direction(matrix)
//       light_color:  light color(matrix)
//       v0:   vertex normal vector
//       c0:   vertex color
// 
//       vect = local_light * v0;   (inner product)
//       0.0 < vect < 1.0;          (clamp)
//       c0   = local_color * vect; (inner product)
// 
void sceSamp0NormalColorVector(sceSamp0IVECTOR c0,
		sceSamp0FMATRIX local_light, sceSamp0FMATRIX light_color,
			       sceSamp0FVECTOR v0, sceSamp0FVECTOR c1){
  sceSamp0FVECTOR vect;
  sceSamp0ApplyMatrix(vect, local_light, v0);
  sceSamp0ClampVector(vect, vect, 0.0, 1.0);
  sceSamp0ApplyMatrix(vect, light_color, vect);
  sceSamp0MulVector(vect, c1, vect);
  sceSamp0ClampVector(vect, vect, 0.0, 255.0);
  sceSamp0FTOI0Vector(c0, vect);
}

// ========================================================================
// sceSamp0ViewScreenMatrix      ���_���W�n����X�N���[�����W�n�ւ̕ϊ��s��
// 
// 	scrz:		distance to screen
// 	ax, ay:		aspect ratio
// 	cx, cy:		center
// 	zmin, zmax:	Z-buffer range
// 	nearz, farz:	near, far edge
// 
void sceSamp0ViewScreenMatrix(sceSamp0FMATRIX m, float scrz, float ax, float ay,
	       float cx, float cy, float zmin, float zmax, float nearz, float farz)
{
	float	az, cz;
	sceSamp0FMATRIX	mt;

	cz = (-zmax * nearz + zmin * farz) / (-nearz + farz);
	az  = farz * nearz * (-zmin + zmax) / (-nearz + farz);

	//     | scrz    0  0 0 |
	// m = |    0 scrz  0 0 |
	//     |    0    0  0 1 |
	//     |    0    0  1 0 |
	sceSamp0UnitMatrix(m);
	m[0][0] = scrz;
	m[1][1] = scrz;
	m[2][2] = 0;
	m[3][3] = 0;
	m[3][2] = 1.0;
	m[2][3] = 1.0;

	//     | ax  0  0 cx |
	// m = |  0 ay  0 cy |
	//     |  0  0 az cz |
	//     |  0  0  0  1 |
	sceSamp0UnitMatrix(mt);
	mt[0][0] = ax;	mt[1][1] = ay;	mt[2][2] = az;
	mt[0][3] = cx;	mt[1][3] = cy;	mt[2][3] = cz;
	sceSamp0MulMatrix(m, mt, m);
	return;
}


// ==========================================================================
// sceSamp0RotTransPers	�����ϊ�
//  �`��
// 	void sceSamp0RotTransPers(sceSamp0IVECTOR v0, sceSamp0FMATRIX m0, sceSamp0FVECTOR v1);
//  ����
//  	v0		�o��:�x�N�g��(�X�N���[�����W)
//  	m0		����:�}�g���b�N�X
//  	v1		����:�x�N�g��
//  	mode		����:v0[2],v0[3]�̒l�̐؂�ւ�
//  			     0:32bit�����Ȃ��Œ菬���_(28.4)
//  			     1:32bit�����Ȃ��Œ菬���_(32.0)
//  ���
//  	�^����ꂽ���_���X�N���[�����W�ɓ����ϊ�����B
// 	�o��v0�̒l�́Av0[0],v0[1]�́A32bit�����t���Œ菬���_(28.4)�A
// 	v0[2],v0[3]��mode=1�̂Ƃ��́A32bit�����Ȃ��Œ菬���_(28.4)�A
// 	mode=0�̂Ƃ��́A32bit�����Ȃ��Œ菬���_(32.0)��Ԃ��B
// 	GIF��PACKED���[�h��XYZF2,XYZF3��p����ꍇ�ɂ́Amode=0�ɂ�
// 	��ƁAPACK���ɐ��������؂�o�����̂ŗL�p�ł���B
// 
//  �Ԃ�l
//  	�����ϊ���̃x�N�g���̑�S�v�f(W�l)�̋t��(1/W)��Ԃ��D
//       �e�N�X�`�����W STQ��Q�l�ɗp���邽�߁D
// 
// --------------------------------------------------------------------------
float sceSamp0RotTransPers(sceSamp0IVECTOR v0, sceSamp0FMATRIX m0, sceSamp0FVECTOR v1, int mode)
{

  float divw;
  sceSamp0FVECTOR retv;

  sceSamp0ApplyMatrix(retv, m0, v1);
  divw = 1.0 / retv[3];
  retv[0] = retv[0]*divw;
  retv[1] = retv[1]*divw;
  retv[2] = retv[2]*divw;
  /*    retv[3] = retv[3]*divw;   */
  if(mode==0){
    v0[0] = (int)(retv[0]*16.0);
    v0[1] = (int)(retv[1]*16.0);
    v0[2] = (int)(retv[2]*16.0);
    v0[3] = (int)(retv[3]*16.0);
  }else{
    v0[0] = (int)(retv[0]*16.0);
    v0[1] = (int)(retv[1]*16.0);
    v0[2] = (int)(retv[2]);
    v0[3] = (int)(retv[3]);
  }
  return divw;
}


// output vector contents
void sceSamp0DumpVector(char *s, sceSamp0FVECTOR v)
{
	printf(s);
	printf("\t[%8d,%8d,%8d,%8d]\n\r",
	       (int)v[0], (int)v[1], (int)v[2], (int)v[3]);
}

// output matrix contents
void sceSamp0DumpMatrix(char *s, sceSamp0FMATRIX m)
{
	puts(s);
	printf("\t[%8d,%8d,%8d,%8d]\n\r",
	       (int)(m[0][0]*10), (int)(m[0][1]*10),
	       (int)(m[0][2]*10), (int)(m[0][3]*10));
	printf("\t[%8d,%8d,%8d,%8d]\n\r",
	       (int)(m[1][0]*10), (int)(m[1][1]*10),
	       (int)(m[1][2]*10), (int)(m[1][3]*10));
	printf("\t[%8d,%8d,%8d,%8d]\n\r",
	       (int)(m[2][0]*10), (int)(m[2][1]*10),
	       (int)(m[2][2]*10), (int)(m[2][3]*10));
	printf("\t[%8d,%8d,%8d,%8d]\n\r",
	       (int)(m[3][0]*10), (int)(m[3][1]*10),
	       (int)(m[3][2]*10), (int)(m[3][3]*10));
}

