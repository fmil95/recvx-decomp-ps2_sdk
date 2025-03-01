/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *                    Basic Sample Program
 *                        Version 0.30
 *            S-JIS  
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.10          May,6,1999	shinohara
 */

#include<libvu0.h>

void sceVu0RotTransPersNClipCol(u_long128 *v0, sceVu0FMATRIX m0, sceVu0FVECTOR *v1, sceVu0FVECTOR *nv,
		sceVu0FVECTOR *st,sceVu0FVECTOR *c,sceVu0FMATRIX ll,sceVu0FMATRIX lc, int n)
{
	asm __volatile__("
	lqc2	vf4,0x0(%1)	#set local_world matrix[0]
	lqc2	vf5,0x10(%1)	#set local_world matrix[1]
	lqc2	vf6,0x20(%1)	#set local_world matrix[2]
	lqc2	vf7,0x30(%1)	#set local_world matrix[3]

	lqc2	$vf17,0x0(%6)	#set local_light matirix[0]
	lqc2	$vf18,0x10(%6)	#set local_light matirix[1]
	lqc2	$vf19,0x20(%6)	#set local_light matirix[2]

	lqc2	$vf21,0x0(%7)	#set light_color matrix[0]
	lqc2	$vf22,0x10(%7)	#set light_color matrix[1]	
	lqc2	$vf23,0x20(%7)	#set light_color matrix[2]
	lqc2	$vf20,0x30(%7)	#set light_color matrix[3]		


_rotTPNCC_loop:
	lqc2	vf8,0x0(%2)	#load XYZ
	lqc2	$vf24,0x0(%4)	#load NORMAL
	lqc2	$vf25,0x0(%5)	#load COLOR
	lqc2	$vf27,0x0(%8)	#load ST

	vmulax.xyzw     ACC, vf4,vf8
	vmadday.xyzw    ACC, vf5,vf8
	vmaddaz.xyzw    ACC, vf6,vf8
	vmaddw.xyzw      vf12,vf7,vf8
	vdiv    Q,vf0w,vf12w
	vwaitq
	vmulq.xyzw	vf12,vf12,Q
	vftoi4.xyzw	vf13,vf12

	vmulax.xyzw    ACC, $vf17,$vf24
	vmadday.xyzw   ACC, $vf18,$vf24
	vmaddz.xyzw     $vf24,$vf19,$vf24

	vmaxx.xyz	$vf24,$vf24,$vf0

	vmulax.xyzw    ACC, $vf21,$vf24
	vmadday.xyzw   ACC, $vf22,$vf24
	vmaddaz.xyzw   ACC, $vf23,$vf24
	vmaddw.xyzw     $vf24,$vf20,$vf0

	vmul.xyzw	$vf26,$vf24,$vf25

	vmulq.xyz	$vf28,$vf27,Q


	vmaxx.xyz	$vf26,$vf26,$vf0	
	lui	$2,0x437f
	ctc2	$2,$vi21
	vnop
	vnop
	vminii.xyz	$vf26,$vf26,I
	vftoi0.xyzw	$vf26,$vf26


	sqc2	$vf28,0x0(%0)		#store STQ 
	addi	%0,0x10
	sqc2	$vf26,0x0(%0)		#store RGBA 
	addi	%0,0x10
	sqc2	vf13,0x0(%0)		#store XYZ 
	addi	%0,0x10

	addi	%3,-1
	addi	%2,0x10
	addi	%4,0x10
	addi	%5,0x10
	addi	%8,0x10
	bne	$0,%3,_rotTPNCC_loop

	": : "r" (v0) , "r" (m0) ,"r" (v1), "r" (n) ,"r" (nv) ,"r" (c), "r" (ll) ,"r" (lc),"r"(st) :"$2","$3","$9");
}

// ==========================================================================
// sceVu0RotCameraMatrix    
// ワールド座標系に対するカメラ座標系を定義(回転有)
// 
//      p:  original camera position (in the world)
//      zd: original eye direction
//      yd: original vertical direction
//      rot: rotation vector
//      direction : real eye direction
//      vertical  : real veritical direction
//      position  : real camera position
// 
void sceVu0RotCameraMatrix(sceVu0FMATRIX m, sceVu0FVECTOR p, sceVu0FVECTOR zd,
 sceVu0FVECTOR yd, sceVu0FVECTOR rot)
{
        sceVu0FMATRIX work;
        sceVu0FVECTOR direction, vertical, position;

        sceVu0UnitMatrix(work);
        sceVu0RotMatrixX(work,work,rot[0]);
        sceVu0RotMatrixY(work,work,rot[1]);
        sceVu0RotMatrixZ(work,work,rot[2]);
        sceVu0ApplyMatrix(direction, work, zd);
        sceVu0ApplyMatrix(vertical, work, yd);
        sceVu0ApplyMatrix(position, work, p);
        sceVu0CameraMatrix(m, position, direction, vertical);
}

