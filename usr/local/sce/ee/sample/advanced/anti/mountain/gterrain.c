/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
/************************************
*
*	terrain data
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


extern void seed_rand_f(float v[4]);



//copy bigterr to terr 
void copy_terrain(mesh *bigterr, mesh8 *terr, int bi, int bj, int pit)
{
	int	i,j;


	terr->mount_max= bigterr->mount_max;
	terr->mount_sharp= bigterr->mount_sharp;
	terr->tedge= bigterr->tedge/(float)pit;

	for(i=0;i<9;i+=pit){
	  for(j=0;j<9;j+=pit){
	   terr->pos[i][j][0]= bigterr->pos[bi+i/pit][bj+j/pit][0];
	   terr->pos[i][j][1]= bigterr->pos[bi+i/pit][bj+j/pit][1];
	   terr->pos[i][j][2]= bigterr->pos[bi+i/pit][bj+j/pit][2];
	   terr->pos[i][j][3]= 1.0;
	  }
	}
} 

//init mesh 
void init_terrain(mesh *terr)
{
	int	i,j;

	for(i=0;i<terr->nmesh*4+1;i++){
	  for(j=0;j<terr->nmesh*4+1;j++){
	    seed_rand_f(terr->pos[i][j]);
	    terr->pos[i][j][1]= -(float)(rand()%MOUNT_MAX)*terr->mount_max;
	    if(terr->pos[i][j][1]>WATER_LEVEL) terr->pos[i][j][1]=WATER_LEVEL;
	    terr->pos[i][j][3]= 1.0;
	  }
	}
}

