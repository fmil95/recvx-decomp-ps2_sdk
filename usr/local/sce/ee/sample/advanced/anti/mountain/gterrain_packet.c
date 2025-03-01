/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
/************************************
*
*	terrain packet
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


extern union {
        u_long128       l128;
        u_long          l64[2];
        u_int           i32[4];
} Giftag2;


/*short packet by VIF command*/
#define V4_32   0x6c
#define V3_32   0x68
#define V2_32   0x64
#define VIF_MASKENABLE 0x10


void generate_POS_packet(u_int *dp, mesh8 *terr, int si, int sj, int aalias)
{
	int	i,j;
	int	ndiv;

	ndiv= 8/terr->sdiv;

	/*****************/
	/*triangle packet*/
	/*****************/
	//DMAtag
	*dp++ = 0x70000000 + (12+4+12+(4+4*81)+4)/4;  		//ID=end, QWC
	*dp++ = 0x0000000;					//SPR(ScratchPad)
	*dp++ = 0;	//next DMAtag on 128bit boundary
	*dp++ = 0;

	//si,sj
	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_UNPACKR(0, 2, V4_32, 0);

	*dp++ = si;
	*dp++ = (si%2)*8;
	*dp++ = 0;
	*dp++ = 0;

	*dp++ = sj;
	*dp++ = (sj%2)*8;
	*dp++ = 0;
	*dp++ = 0;

	//GIFtag
	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_UNPACKR(2, 3, V4_32, 0);

	//TriangleStrip
	if(aalias==0){
	    //EOP=1
            Giftag2.l64[0] = SCE_GIF_SET_TAG(18, 1, 1, 
		SCE_GS_PRIM_TRISTRIP | SCE_GS_PRIM_IIP | SCE_GS_PRIM_FGE | SCE_GS_PRIM_TME  ,0,3);
	}else{
	    //EOP=0
            Giftag2.l64[0] = SCE_GIF_SET_TAG(18, 0, 1, 
		SCE_GS_PRIM_TRISTRIP | SCE_GS_PRIM_IIP | SCE_GS_PRIM_FGE | SCE_GS_PRIM_TME  ,0,3);
	}
        Giftag2.l64[1] = (u_long)0x412;

        *dp++ = Giftag2.i32[0];
        *dp++ = Giftag2.i32[1];
        *dp++ = Giftag2.i32[2];
        *dp++ = Giftag2.i32[3];

	//LineStrip EOP=0
        Giftag2.l64[0] = SCE_GIF_SET_TAG(17, 0, 1, 
		SCE_GS_PRIM_LINESTRIP | SCE_GS_PRIM_IIP | SCE_GS_PRIM_FGE | SCE_GS_PRIM_TME | SCE_GS_PRIM_AA1  ,0,3);
        Giftag2.l64[1] = (u_long)0x412;

        *dp++ = Giftag2.i32[0];
        *dp++ = Giftag2.i32[1];
        *dp++ = Giftag2.i32[2];
        *dp++ = Giftag2.i32[3];

	//LineStrip EOP=1
        Giftag2.l64[0] = SCE_GIF_SET_TAG(9, 1, 1, 
		SCE_GS_PRIM_LINESTRIP | SCE_GS_PRIM_IIP | SCE_GS_PRIM_FGE | SCE_GS_PRIM_TME | SCE_GS_PRIM_AA1  ,0,3);
        Giftag2.l64[1] = (u_long)0x412;

        *dp++ = Giftag2.i32[0];
        *dp++ = Giftag2.i32[1];
        *dp++ = Giftag2.i32[2];
        *dp++ = Giftag2.i32[3];

	//XYZ
	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_UNPACKR(5, 81, V4_32, 0);	


	for(i=0;i<9;i++){				//405 or 605
	  if((i%ndiv)==0){
	    for(j=0;j<9;j++){
	      if((j%ndiv)==0){
		*(float*)dp++ = terr->pos[i][j][0];
		*(float*)dp++ = terr->pos[i][j][1];
		*(float*)dp++ = terr->pos[i][j][2];
		*(float*)dp++ = terr->pos[i][j][3];
	      }else{
	      	dp +=4;
	      }
	    }
	  }else{
		dp += 4*9;
	  }
	}

	//MSCAL
    	*dp++ = SCE_VIF1_SET_MSCAL(1200, 0);	//fsub.vuasm for fractal subdivide
	*dp++ = 0;
	*dp++ = 0;
	*dp++ = 0;
}

void generate_STQ_packet(u_int *dp,  float texd)
{
	int	i,j;
	float	texn;


	/*****************/
	/*triangle packet*/
	/*****************/
	//DMAtag
	*dp++ = 0x70000000 + ((4+4*81)*2)/4;  		//ID=end, QWC
	*dp++ = 0;					//SPR(ScratchPad)
	*dp++ = 0;	
	*dp++ = 0;

	//STQ
	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_UNPACK(486, 81, V4_32, 0);	//405+81 -> 567

	texn= 8.0/texd;

	for(i=0;i<9;i++){
	  for(j=0;j<9;j++){
	   	*(float*)dp++ = (float)(i)/texn;
	   	*(float*)dp++ = (float)(j)/texn;
	   	*(float*)dp++ = 1.0;
	   	*(float*)dp++ = 1.0;
	  }
	}

	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_NOP(0);
	*dp++ = SCE_VIF1_SET_UNPACK(656, 81, V4_32, 0);	//575+81 -> 737

	texn= 8.0/texd;

	for(i=0;i<9;i++){
	  for(j=0;j<9;j++){
	   	*(float*)dp++ = (float)(i)/texn;
	   	*(float*)dp++ = (float)(j)/texn;
	   	*(float*)dp++ = 1.0;
	   	*(float*)dp++ = 1.0;
	  }
	}
}

