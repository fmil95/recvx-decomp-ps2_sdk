/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
/************************************
*
*	random 
*
*************************************/
#include <eekernel.h>
#include <eeregs.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <libdev.h>
#include <libgraph.h>
#include "constant.h"

/*short packet by VIF command*/
#define V4_32   0x6c
#define V3_32   0x68
#define V2_32   0x64
#define VIF_MASKENABLE 0x10

void seed_rand_f(float v[4]);
void seed_rand_i(int v[4]);

void seed_rand_f(float v[4])
{
	srand( ((int)v[0]*BIGN+(int)v[2])%1048576 );
}

void seed_rand_i(int v[4])
{
	srand( (v[0]*BIGN+v[2])%1048576 );
}

void make_random_table8(float rt[9][9][4])
{
	int	i,j;

	srand(1000000);
	for(i=0;i<8;i++){
	  for(j=0;j<8;j++){
		rt[i][j][0]= 0;
		rt[i][j][1]= (float)(-10000+rand()%20000)*0.00005;
		rt[i][j][2]= 0;
	  }
	}
	for(j=0;j<8;j++){
		rt[8][j][0]= 0;
		rt[8][j][1]= rt[0][j][1];
		rt[8][j][2]= 0;
	}
	for(i=0;i<9;i++){
		rt[i][8][0]= 0;
		rt[i][8][1]= rt[i][0][1];
		rt[i][8][2]= 0;
	}

}

void make_random_table16(float rt16[17][17][4])
{
	int	i,j;

	for(i=0;i<17;i++){
	  for(j=0;j<17;j++){
		rt16[i][j][0]= 0;
		rt16[i][j][1]= (float)(-10000+rand()%20000)*0.00005;
		rt16[i][j][2]= 0;
	  }
	}
	for(j=0;j<16;j++){
		rt16[16][j][0]= 0;
		rt16[16][j][1]= rt16[0][j][1];
		rt16[16][j][2]= 0;
	}
	for(i=0;i<17;i++){
		rt16[i][16][0]= 0;
		rt16[i][16][1]= rt16[i][0][1];
		rt16[i][16][2]= 0;
	}
}

void make_random_table16_1(float rt16[17][17][4],float rt16_1[17][17][4])
{
	int	i,j;

	for(i=0;i<17;i++){
	  for(j=0;j<17;j++){
		rt16_1[i][j][0]= rt16[(i%8)*2][(j%8)*2][0];
		rt16_1[i][j][1]= rt16[(i%8)*2][(j%8)*2][1];
		rt16_1[i][j][2]= rt16[(i%8)*2][(j%8)*2][2];
	  }
	}
}

void make_random_table16_2(float rt16[17][17][4],float rt16_2[17][17][4])
{
	int	i,j;

	for(i=0;i<17;i++){
	  for(j=0;j<17;j++){
		rt16_2[i][j][0]= rt16[(i%4)*4][(j%4)*4][0];
		rt16_2[i][j][1]= rt16[(i%4)*4][(j%4)*4][1];
		rt16_2[i][j][2]= rt16[(i%4)*4][(j%4)*4][2];
	  }
	}
}

void generate_random16_packet(u_int *dp, float   rt16[17][17][4])
{
	int	i,j;


        /*****************/
        /*random16 packet*/
        /*****************/

        //random
        for(i=0;i<17;i++){
          //DMAtag
	  if(i==16)
            *dp++ = 0x70000000 + (4+4*17)/4;              		//ID=end, QWC
	  else
            *dp++ = 0x10000000 + (4+4*17)/4;              		//ID=cont, QWC
          *dp++ = 0x00000000;                                     //main memory
          *dp++ = 0;      //next DMAtag on 128bit boundary
          *dp++ = 0;
	  //VIF
          *dp++ = SCE_VIF1_SET_NOP(0);
          *dp++ = SCE_VIF1_SET_NOP(0);
          *dp++ = SCE_VIF1_SET_NOP(0);
          *dp++ = SCE_VIF1_SET_UNPACK(100+17*i, 17, V4_32, 0);
          for(j=0;j<17;j++){
		*dp++ = *(u_int*)&rt16[i][j][0];
		*dp++ = *(u_int*)&rt16[i][j][1];
		*dp++ = *(u_int*)&rt16[i][j][2];
		*dp++ = *(u_int*)&rt16[i][j][3];
          }
        }
}

