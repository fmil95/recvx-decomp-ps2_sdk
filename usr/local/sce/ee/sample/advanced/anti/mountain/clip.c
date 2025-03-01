/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
/************************************
*
*	clip
*
*************************************/
#include <eekernel.h>
#include <eeregs.h>
#include <stdlib.h>
#include <math.h>
#include <libdev.h>
#include <libgraph.h>
#include "terrain.h"
#include "constant.h"
#include "scale.h"


int view_clip_polygon(float pos[4][4], float pl[4][4], float nearZ, float farZ)
{
	int	i;
	float	s[4][4];
	int	Cmx=1,Cpx=1,Cmy=1,Cpy=1, Cmz=1, Cpz=1;

	for(i=0; i<4; i++){
	    s[i][0]= 	 pl[0][0]*pos[i][0]
			+pl[0][1]*pos[i][1]
			+pl[0][2]*pos[i][2]
			+pl[0][3];
	    s[i][1]= 	 pl[1][0]*pos[i][0]
			+pl[1][1]*pos[i][1]
			+pl[1][2]*pos[i][2]
			+pl[1][3];
	    s[i][3]= 	 pl[3][0]*pos[i][0]
			+pl[3][1]*pos[i][1]
			+pl[3][2]*pos[i][2]
			+pl[3][3];
	}


	for(i=0; i<4; i++){
	    if(s[i][3]>nearZ) Cmz=0;
	    if(s[i][3]<farZ) Cpz=0;
	    if(((-2048.0+320.0)*s[i][3]+s[i][0])>0) Cmx=0;
	    if((( 2048.0+320.0)*s[i][3]-s[i][0])>0) Cpx=0;
	    if(((-2048.0+112.0)*s[i][3]+s[i][1])>0) Cmy=0;
	    if((( 2048.0+112.0)*s[i][3]-s[i][1])>0) Cpy=0;
	}

	if(Cmx==1||Cpx==1||Cmy==1||Cpy==1||Cmz==1||Cpz==1) return(1);
	else return(0);
}


int GS_clip_polygon(float pos[4][4], float pl[4][4], float nearZ, float farZ)
{
	int	i;
	float	s[4][4];
	int	Cmx=0,Cpx=0,Cmy=0,Cpy=0, Cmz=0, Cpz=0;

	for(i=0; i<4; i++){
	    s[i][0]= 	 pl[0][0]*pos[i][0]
			+pl[0][1]*pos[i][1]
			+pl[0][2]*pos[i][2]
			+pl[0][3];
	    s[i][1]= 	 pl[1][0]*pos[i][0]
			+pl[1][1]*pos[i][1]
			+pl[1][2]*pos[i][2]
			+pl[1][3];
	    s[i][3]= 	 pl[3][0]*pos[i][0]
			+pl[3][1]*pos[i][1]
			+pl[3][2]*pos[i][2]
			+pl[3][3];
	}


	for(i=0; i<4; i++){
	    if(s[i][3]<CLIPZ1 ) Cmz=1;
	    if(s[i][3]>=CLIPZ1 && s[i][3]<nearZ && Cmz!=1 ) Cmz=2;
	    if(s[i][3]>=nearZ && s[i][3]<CLIPZ2 && Cmz!=1 && Cmz!=2) Cmz=4;
	    
	    if(((-2048.0+2048.0)*s[i][3]+s[i][0])<0) Cmx=1;
	    if((( 2048.0+2048.0)*s[i][3]-s[i][0])<0) Cpx=1;
	    if(((-2048.0+2048.0)*s[i][3]+s[i][1])<0) Cmy=1;
	    if((( 2048.0+2048.0)*s[i][3]-s[i][1])<0) Cpy=1;
	}

	if(Cmx==0&&Cpx==0&&Cmy==0&&Cpy==0&&Cmz==0&&Cpz==0) return(0);	//GS in
	else{
		if(Cmz==1) return(1);					//GS out CLIPZ1
		if(Cmz==2) return(2);					//GS out nearZ
		if(Cmz==4) return(4);					//GS out CLIPZ2
		if(Cmz==0) return(4);					//GS out X or Y
	}
	return(0);
}


int view_clip(mesh *terr, float pl[4][4], float nearZ, float farZ, int i, int j, int pit)
{
	int	k;
	float	s[8][4];
	int	Cmx=1,Cpx=1,Cmy=1,Cpy=1, Cmz=1, Cpz=1;
	int	ip,jp;
	float	bblk[8][4];

	ip= i+pit;
	jp= j+pit;

	    //terrain's bounding block
	    bblk[0][0]= terr->pos[i][j][0];
	    bblk[0][1]= 0;
	    bblk[0][2]= terr->pos[i][j][2];

	    bblk[1][0]= terr->pos[i][jp][0];
	    bblk[1][1]= 0;
	    bblk[1][2]= terr->pos[i][jp][2];

	    bblk[2][0]= terr->pos[ip][j][0];
	    bblk[2][1]= 0;
	    bblk[2][2]= terr->pos[ip][j][2];

	    bblk[3][0]= terr->pos[ip][jp][0];
	    bblk[3][1]= 0;
	    bblk[3][2]= terr->pos[ip][jp][2];

	    bblk[4][0]= terr->pos[i][j][0];
	    bblk[4][1]= -BBLOCKH;
	    bblk[4][2]= terr->pos[i][j][2];

	    bblk[5][0]= terr->pos[i][jp][0];
	    bblk[5][1]= -BBLOCKH;
	    bblk[5][2]= terr->pos[i][jp][2];

	    bblk[6][0]= terr->pos[ip][j][0];
	    bblk[6][1]= -BBLOCKH;
	    bblk[6][2]= terr->pos[ip][j][2];

	    bblk[7][0]= terr->pos[ip][jp][0];
	    bblk[7][1]= -BBLOCKH;
	    bblk[7][2]= terr->pos[ip][jp][2];

	    for(k=0; k<8; k++){
	        s[k][0]=  pl[0][0]*bblk[k][0]
			 +pl[0][1]*bblk[k][1]
			 +pl[0][2]*bblk[k][2]
			 +pl[0][3];
	        s[k][1]=  pl[1][0]*bblk[k][0]
			 +pl[1][1]*bblk[k][1]
			 +pl[1][2]*bblk[k][2]
			 +pl[1][3];
	        s[k][3]=  pl[3][0]*bblk[k][0]
			 +pl[3][1]*bblk[k][1]
			 +pl[3][2]*bblk[k][2]
			 +pl[3][3];
	    }

	    Cmx=1;
	    Cpx=1;
	    Cmy=1;
	    Cpy=1; 
	    Cmz=1; 
	    Cpz=1;

	    for(k=0; k<8; k++){
	    	if(s[k][3]>nearZ) Cmz=0;
	    	if(s[k][3]<farZ) Cpz=0;
	    	if(((-2048.0+320.0)*s[k][3]+s[k][0])>0) Cmx=0;
	    	if((( 2048.0+320.0)*s[k][3]-s[k][0])>0) Cpx=0;
	    	if(((-2048.0+112.0)*s[k][3]+s[k][1])>0) Cmy=0;
	    	if((( 2048.0+112.0)*s[k][3]-s[k][1])>0) Cpy=0;
	    }

	    if(Cmx==1||Cpx==1||Cmy==1||Cpy==1||Cmz==1||Cpz==1) return(1);
	    else return(0);
}


