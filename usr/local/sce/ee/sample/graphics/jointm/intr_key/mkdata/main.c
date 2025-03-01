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
 *                     Description : main procedure
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.5.0          Aug,16,1999     aoki
 */

#include <eekernel.h>                    
#include <eeregs.h>
#include <sifdev.h>
#include <libvu0.h>
#include <libdma.h>
#include <libgraph.h>
#include <stdio.h>
#include "axis.h"

#define ERR_STOP while(1)

extern void writeMatrix(sceVu0FMATRIX m, int num);
extern void writeAxis(sceVu0FVECTOR a, sceVu0FVECTOR t, u_int n);
extern void matseq2axisseq(sceVu0FMATRIX m[], int num);

char printtmp[128];
int fd;

void DevInit()
{
    sceDmaReset(1);

}

void writetofile(int num)
{
    if (sceWrite(fd,printtmp,num) < 0) {
        printf ("Can't Write file \n");
        ERR_STOP;
    }
}

extern sceVu0FMATRIX matrix[10][6];
extern int keynum;
extern int objnum;
extern unsigned long count[];

int main()
{
    int i;
 
    while (sceOpen("host:axisseq.dat",SCE_CREAT|SCE_WRONLY|SCE_TRUNC) < 0) {
        printf ("Can't read file axisseq.dat\n");
    }
    for (i=0; i<objnum; i++){
	matseq2axisseq(matrix[i], i);
    }
    
    sceClose(fd);
    return 0;
}

void matseq2axisseq(sceVu0FMATRIX m[], int num)
{
    sceVu0FMATRIX m0;
    sceVu0FMATRIX m1;
    sceVu0FMATRIX m2;
    sceVu0FVECTOR a0, t0;
    u_int n;
    int i;

    sceVu0CopyMatrix(m0, m[0]);
    writeMatrix(m0,num);			// write start matrix

    sprintf(printtmp,"DIFFNAME%02d[]={\n",num);
    writetofile(15);
    
    for (i=0; i<keynum; i++){
	n=count[i];				// read interpolate count (n)
	sceVu0CopyMatrix(m1, m[i+1]);			// read end matrix (m1)
	mat2axis(a0, t0, m0, m1, n);		// calc axis (a0)

	axis2mat(m2, m0, a0, t0, n);		// calc runtime end matrix (m2)
	chkerror(m1, m2);			// check error (original-runtime)
	writeAxis(a0, t0, n);			// write axis data
	sceVu0CopyMatrix(m0,m2);		// start matrix for next loop
    }
    
    sprintf(printtmp,"};\n");
    writetofile(3);
}

void writeMatrix(sceVu0FMATRIX m, int num)
{
    int i,j;
    
    sprintf(printtmp,"MATRIXNAME%02d={\n", num);
    writetofile(15);
    for (i=0; i<4; i++){
	sprintf(printtmp,"    {");
	writetofile(5);
	for (j=0; j<4; j++){
	    sprintf(printtmp,"%10.6f, ", m[i][j]);
	    writetofile(12);
	}
	sprintf(printtmp,"    },\n");
	writetofile(7);
    }
    sprintf(printtmp,"};\n");
    writetofile(3);
}



void writeAxis(sceVu0FVECTOR a, sceVu0FVECTOR t, u_int n)
{
    int i;

    sprintf(printtmp,"{{");
    writetofile(2);
    for (i=0; i<4; i++){
	sprintf(printtmp,"%10.6f, ", a[i]);
	writetofile(12);
    }
    sprintf(printtmp,"},\n    {");
    writetofile(8);

    for (i=0; i<3; i++){
	sprintf(printtmp,"%14e, ", t[i]);
	writetofile(16);
    }
    sprintf(printtmp,"},%4d},\n",n);
    writetofile(9);

}
