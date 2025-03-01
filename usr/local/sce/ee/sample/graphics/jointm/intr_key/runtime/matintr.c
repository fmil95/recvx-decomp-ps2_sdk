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
 *                     Description : keyframe interpolation procedures
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.5.0          Aug,16,1999     aoki
 */

#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <eeregs.h>
#include <sifdev.h>
#include <libvu0.h>
#include <libgraph.h>
#include "matintr.h"
#include "axis.h"

extern int NextDiff(MatIntr *mi);

extern sceVu0FVECTOR zerovec;


void MatIntrLoadMatrix(sceVu0FMATRIX m, MatIntr *mi)
{

    if (mi->cnt<=0) {
	int v;

	v=NextDiff(mi);				// get next sequence
	if (v<0){
	    sceVu0TransMatrix(m, mi->currentR, mi->currentT);
	    return;			// no more sequence!
	}
    }

    sceVu0MulMatrix(mi->currentR, mi->diffR, mi->currentR);
    sceVu0AddVector(mi->currentT, mi->diffT, mi->currentT);
    mi->cnt--;

    sceVu0TransMatrix(m, mi->currentR, mi->currentT);
}

int NextDiff(MatIntr *mi)
{
    int *l;

    mi->data.diffdatanum--;

    if (mi->data.diffdatanum<0){
	mi->data.diffdatanum= 0;
	return -1;				// no more diff data
    }

    // calculate diff rotation matrix
    a2m(mi->diffR, mi->data.diffdata->axis);

    // diff translation
    sceVu0CopyVectorXYZ(mi->diffT, mi->data.diffdata->trans);
    mi->diffT[3]=0.0f;

    // diff count
    l= (int *)&(mi->data.diffdata->trans[3]);
    mi->cnt= *l;
    mi->data.diffdata++;

    return 1;
}


void MatIntrInit(MatIntr *mi, sceVu0FMATRIX startm, MatIntrDiffData2 *d, int num)
{
    int i;

    mi->data.diffdata=(MatIntrDiffData *)d;
    mi->data.diffdatanum=num;

    SetTransMatrix(mi->currentR, startm, zerovec);
    for (i=0; i<4; i++) mi->currentT[i]= startm[3][i];
    mi->cnt=0;
}

// m0=m1, m0[3][0..3]=tv[0..3]
void SetTransMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1, sceVu0FVECTOR tv)
{
        asm __volatile__("
        lqc2    vf4,0x0(%2)
        lq    $7,0x0(%1)
        lq    $8,0x10(%1)
        lq    $9,0x20(%1)
        vmove.xyzw        vf5,vf4
        sq    $7,0x0(%0)
        sq    $8,0x10(%0)
        sq    $9,0x20(%0)
        sqc2    vf5,0x30(%0)
        ": : "r" (m0) , "r" (m1), "r" (tv):"$7","$8","$9");
}
