/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                       - feather aa1 sample  -
 *
 *                         Version 0.2.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : make_packet.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.1.0           Oct,07,1999     sasaki    modified from feather
 *      0.2.0           Jul,11,2000     sasaki    delete unused variables
 */

/*
*
*	VU & GS test data generator
*
*/
#include <eekernel.h>
#include <eeregs.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <libdev.h>
#include <libgraph.h>

#define GetDp(id) ((id)?(0x70002000):(0x70000000))

extern unsigned int My_island __attribute__((section(".vudata")));
extern unsigned int My_clear_buffer __attribute__((section(".vudata")));
extern unsigned int My_tex_buffer __attribute__((section(".vudata")));
extern unsigned int My_zbuffer __attribute__((section(".vudata")));
extern unsigned int My_light_color __attribute__((section(".vudata")));

extern unsigned int My_refract __attribute__((section(".dmadata")));
extern unsigned int My_lattice __attribute__((section(".dmadata")));
extern unsigned int My_refract_end __attribute__((section(".dmadata")));
extern unsigned int My_lattice_end __attribute__((section(".dmadata")));

#define       FlushDCache()   FlushCache(0)

#define FRAME_H		640.0f
#define FRAME_V		224.0f
#define LEFTUPPER_H	(2048.0f-320.0f)
#define LEFTUPPER_V	(2048.0f-112.0f)
#define TEXTURE_H	1024.0f
#define TEXTURE_V	256.0f
#define WAVE_X_MAX   132
#define WAVE_Z_MAX   132
#define SMAX		10
#define NSRC		8
#define LAMP		100.0f
#define FRAME		50.0f
#define REFRACT_RATE	0.05f
#define VIEWP_X		10000.0f
#define VIEWP_Y		(-18000.0f)
#define VIEWP_Z		(-8000.0f)
#define TARGET_X	0.0f
#define TARGET_Y	0.0f
#define TARGET_Z	0.0f
#define LIGHTDIR_X	1.0f
#define LIGHTDIR_Y	-1.0f
#define LIGHTDIR_Z	1.0f
#define NWAVE		15.0f
#define PAI		3.14159f

extern void Euler(float a[3], float b[3], float c[3]);
extern void RotMatrix(float a[3], float m[4][4]);
extern void MulMatrix(float a[4][4],float b[4][4],float m[4][4]);
extern void ApplyMatrix(float m[4][4], float a[4] , float b[4]);
extern void VectorNormal(float *v);
extern void OuterProduct(float *v0, float *v1, float *v2);
extern void get_rotmatrix_from_axis(float p, float z, float y, float x, float r[4][4]);
extern void get_SWmatrix_from_view(float view[3], float target[3], float SWMTX[4][4]);
extern void get_VuSWmatrix_from_view(float view[3], float target[3], float SWMTX[4][4]);

	/*lattice vertex rotation center*/
float	lcen[WAVE_X_MAX][WAVE_Z_MAX][3] __attribute__((aligned(16)));

float	psmtx[4][4] __attribute__((aligned(16)));
float	pwmtx[4][4] __attribute__((aligned(16)));
float	swmtx[4][4] __attribute__((aligned(16)));
float	wlmtx[4][4] __attribute__((aligned(16)));
float	plmtx[4][4] __attribute__((aligned(16)));

extern unsigned int My_plmtx __attribute__((section(".vudata")));
extern unsigned int My_wlmtx __attribute__((section(".vudata")));


float	VUswmtx[4][4] __attribute__((aligned(16)));
float	lattx,lattz;
float	XCEN;
float	YCEN;
float	ZCEN;
//float	v[SMAX][3];
float	lview[WAVE_X_MAX][WAVE_Z_MAX][3];
float	view_cen[4];

static float	tpos[40][40][3];
static float    ttext[40][40][2];
static float	view[4] __attribute__((aligned(16)));

static float	ftpos[40000][4] __attribute__((aligned(16)));

//prepare island data on main memory
void make_island_packet(float factor)
{
	int	p;
	int	i,j,k;
	int	NLATTX,NLATTZ;		/*wave dimension*/
	int	NISLX,NISLZ;		/*island dimension*/
	u_long128	Giftag;
	float ypos;

	NLATTX = 129;
	NLATTZ = 129;

	NISLX= (NLATTX-1)/4+1;
	NISLZ= (NLATTZ-1)/4+1;

	p=0;
	factor = 1.5f;

	for(i=0;i<NISLZ-1;i++){
	    for(j=0;j<NISLX-1;j+=16){
		//p++;			//unpack
		p+=4;			//VIFnop,VIFnop,VIFnop,unpack

               ((u_long *)&Giftag)[0] = SCE_GIF_SET_TAG(34, 1, 1,
                        SCE_GS_PRIM_TRISTRIP | SCE_GS_PRIM_IIP | SCE_GS_PRIM_TME
                       ,0 , 4);
               ((u_long *)&Giftag)[1] = (u_long)0x412e;

                (&My_island)[p++] = ((u_int *)&Giftag)[0];
                (&My_island)[p++] = ((u_int *)&Giftag)[1];
                (&My_island)[p++] = ((u_int *)&Giftag)[2];
                (&My_island)[p++] = ((u_int *)&Giftag)[3];


		/*RGBA(fixed)*/
		//p++;			//unpack
		p+=4;			//VIFnop,VIFnop,VIFnop,unpack
		for(k=j;k<=j+16;k++){
		    (&My_island)[p++] = 0x43000000;
		    (&My_island)[p++] = 0x43000000;
		    (&My_island)[p++] = 0x43000000;
		    (&My_island)[p++] = 0x42900000;
		    (&My_island)[p++] = 0x43000000;
		    (&My_island)[p++] = 0x43000000;
		    (&My_island)[p++] = 0x43000000;
		    (&My_island)[p++] = 0x42900000;
		}
		//p += 4*34;

		/*XYZ*/
		//p++;			//unpack
		p+=4;			//VIFnop,VIFnop,VIFnop,unpack
		for(k=j;k<=j+16;k++){
			ypos = tpos[i][k][1]*factor + 1000.0f;
		    (&My_island)[p++] = *(u_int*)&tpos[i][k][0];
		    (&My_island)[p++] = *(u_int*)&ypos;
		    (&My_island)[p++] = *(u_int*)&tpos[i][k][2];
		    (&My_island)[p++] = 0x3f800000;
			ypos = tpos[i+1][k][1]*factor + 1000.0f;
		    (&My_island)[p++] = *(u_int*)&tpos[i+1][k][0];
		    (&My_island)[p++] = *(u_int*)&ypos;
		    (&My_island)[p++] = *(u_int*)&tpos[i+1][k][2];
		    (&My_island)[p++] = 0x3f800000;
		}

		/*NxNyNz*/
		//p++;			//unpack
		p+=4;			//VIFnop,VIFnop,VIFnop,unpack
		for(k=j;k<=j+16;k++){
		    (&My_island)[p++] = 0x00000000;
		    (&My_island)[p++] = 0x00000000;
		    (&My_island)[p++] = 0x3f800000;
		    (&My_island)[p++] = 0x3f800000;
		    (&My_island)[p++] = 0x00000000;
		    (&My_island)[p++] = 0x00000000;
		    (&My_island)[p++] = 0x3f800000;
		    (&My_island)[p++] = 0x3f800000;
		}

		/*UV(fixed)*/
		//p++;			//unpack
		p+=4;			//VIFnop,VIFnop,VIFnop,unpack
		for(k=j;k<=j+16;k++){
		    (&My_island)[p++] = *(u_int*)&ttext[i][k][0];
		    (&My_island)[p++] = *(u_int*)&ttext[i][k][1];
			p += 2;
		    (&My_island)[p++] = *(u_int*)&ttext[i+1][k][0];
		    (&My_island)[p++] = *(u_int*)&ttext[i+1][k][1];
			p += 2;
		}
		//p += 4*34;

		//p ++;			//MSCAL
		p+=4;			//VIFnop,VIFnop,VIFnop,MSCAL
	    }
	}
}

//prepare triangle feather data on main memory
void make_feather_packet(int *qq, int *rr, int rept, int prim_flg, int aa1_flg)
{
	register int	p,q,r,r2;
	register int	i,j,k;
	int	NLATTX,NLATTZ;		/*wave dimension*/
	int	NISLX,NISLZ;		/*island dimension*/
	u_long128	Giftag;
	float xpos,ypos,zpos;
	float val_one, val_half, val_zero;

	float u[4],v[4];
	float	xyzi[4] __attribute__((aligned(16)));
	register int reptf;

	register float *pos;
	register float *nrm;

	u_int	*Feather;

	reptf= rept;

	val_one = 1.0f;
	val_half = 0.5f;
	val_zero = 0.0f;

	/*Uncache Accellerated*/
	Feather= (u_int*)(((u_int)&My_island & 0x0fffffff) | 0x30000000);

	xyzi[0]=0;
	xyzi[1]=0;
	xyzi[2]=0;
	xyzi[3]=128.0;

	pos = (float *)GetDp(0);
	nrm = (float *)GetDp(1);

	NLATTX = 129;
	NLATTZ = 129;

	NISLX= (NLATTX-1)/4+1;
	NISLZ= (NLATTZ-1)/4+1;

	p=0; q=*qq; r=*rr; r2 = r;

	if (q%2 == 0) {
		u[0] = 0.0f; v[0] = 0.0f;
		u[1] = 0.0f; v[1] = 1.0f;
		u[2] = 1.0f; v[2] = 1.0f;
		u[3] = 1.0f; v[3] = 0.0f;
	} else {
		u[0] = 1.0f; v[0] = 1.0f;
		u[1] = 1.0f; v[1] = 0.0f;
		u[2] = 0.0f; v[2] = 0.0f;
		u[3] = 0.0f; v[3] = 1.0f;
	}

	if (prim_flg == 0)
        	((u_long *)&Giftag)[0] = SCE_GIF_SET_TAG(34, 1, 1,
                  SCE_GS_PRIM_TRI | SCE_GS_PRIM_IIP | SCE_GS_PRIM_TME
                 ,0 , 4);
	if (prim_flg == 1) {
		if (aa1_flg == 0) {
        		((u_long *)&Giftag)[0] = SCE_GIF_SET_TAG(34, 1, 1,
               		   SCE_GS_PRIM_LINESTRIP | SCE_GS_PRIM_IIP | SCE_GS_PRIM_TME
               		  ,0 , 4);
		} else {
        		((u_long *)&Giftag)[0] = SCE_GIF_SET_TAG(34, 1, 1,
               		   SCE_GS_PRIM_LINESTRIP | SCE_GS_PRIM_IIP | SCE_GS_PRIM_TME | SCE_GS_PRIM_AA1
               		  ,0 , 4);
		}
	}
        ((u_long *)&Giftag)[1] = (u_long)0x412e;

	FlushDCache();			//DCache Flush for Uncache Write

	for(i=0;i<32;i++){
	    for(j=0;j<32;j+=16){

	      if(reptf==0){
		//p++;			//unpack
		p+=4;			//VIFnop,VIFnop,VIFnop,unpack

                Feather[p++] = ((u_int *)&Giftag)[0];
                Feather[p++] = ((u_int *)&Giftag)[1];
                Feather[p++] = ((u_int *)&Giftag)[2];
                Feather[p++] = ((u_int *)&Giftag)[3];

		/*RGBA(fixed)*/
		//p++;			//unpack
		p+=4;			//VIFnop,VIFnop,VIFnop,unpack
		for(k=j;k<=j+16;k++){
		    Feather[p++] = 0x430c0000;
		    Feather[p++] = 0x430c0000;
		    Feather[p++] = 0x430c0000;
		    Feather[p++] = 0x00000000;
		    Feather[p++] = 0x41000000;
		    Feather[p++] = 0x41000000;
		    Feather[p++] = 0x41000000;
		    Feather[p++] = 0x00000000;
		}
	      }else{
		p += 4+4+4+4*34;
	      }


		/*XYZ*/
		//p++;			//unpack
		p+=4;			//VIFnop,VIFnop,VIFnop,unpack

		asm __volatile__("
			lqc2		vf28,0x0(%0)		#vf28=(128.0,0,0,0)
			lqc2		vf29,0x0(%0)		#vf29=(128.0,0,0,0)
			lqc2		vf30,0x0(%0)		#vf30=(128.0,0,0,0)
		": : "r"(xyzi): "memory");

		for(k=0;k<11;k++){

			xpos = ftpos[q][0] + pos[r++];
			ypos = ftpos[q][1] + pos[r++];
			zpos = ftpos[q][2] + pos[r++];
		    Feather[p++] = *(u_int*)&xpos;
		    Feather[p++] = *(u_int*)&ypos;
		    Feather[p++] = *(u_int*)&zpos;
		    Feather[p++] = 0x43000000;
			r++;
			xpos = ftpos[q][0] + pos[r++];
			ypos = ftpos[q][1] + pos[r++];
			zpos = ftpos[q][2] + pos[r++];
		    Feather[p++] = *(u_int*)&xpos;
		    Feather[p++] = *(u_int*)&ypos;
		    Feather[p++] = *(u_int*)&zpos;
		    Feather[p++] = 0x43000000;
			r++;
			xpos = ftpos[q][0] + pos[r++];
			ypos = ftpos[q][1] + pos[r++];
			zpos = ftpos[q][2] + pos[r++];
		    Feather[p++] = *(u_int*)&xpos;
		    Feather[p++] = *(u_int*)&ypos;
		    Feather[p++] = *(u_int*)&zpos;
		    Feather[p++] = 0x43000000;
			r++;
			q++;

			if (r == 960) r=0;
		}

		Feather[p++] = *(u_int*)&val_zero;
		Feather[p++] = *(u_int*)&val_zero;
		Feather[p++] = *(u_int*)&val_zero;
		Feather[p++] = 0x43000000;

		/*NxNyNz*/
		//p++;			//unpack
		p+=4;			//VIFnop,VIFnop,VIFnop,unpack
		for(k=0;k<11;k++){

		    Feather[p++] = *(u_int*)&nrm[r2++];
		    Feather[p++] = *(u_int*)&nrm[r2++];
		    Feather[p++] = *(u_int*)&nrm[r2++];
  		    Feather[p++] = 0x3f800000;
			r2++;
		    Feather[p++] = *(u_int*)&nrm[r2++];
		    Feather[p++] = *(u_int*)&nrm[r2++];
		    Feather[p++] = *(u_int*)&nrm[r2++];
  		    Feather[p++] = 0x3f800000;
			r2++;
		    Feather[p++] = *(u_int*)&nrm[r2++];
		    Feather[p++] = *(u_int*)&nrm[r2++];
		    Feather[p++] = *(u_int*)&nrm[r2++];
  		    Feather[p++] = 0x3f800000;
			r2++;

			if (r2 == 960) r2=0;

		}

		Feather[p++] = *(u_int*)&val_zero;
		Feather[p++] = *(u_int*)&val_zero;
		Feather[p++] = *(u_int*)&val_zero;
		Feather[p++] = 0x3f800000;

	      if(reptf==0){
		/*UV(fixed)*/
		//p++;			//unpack
		p+=4;			//VIFnop,VIFnop,VIFnop,unpack
		for(k=0;k<5;k++){

		    Feather[p++] = *(u_int*)&u[0];
		    Feather[p++] = *(u_int*)&v[0];
			p += 2;

		    Feather[p++] = *(u_int*)&u[1];
		    Feather[p++] = *(u_int*)&v[1];
			p += 2;

		    Feather[p++] = *(u_int*)&u[2];
		    Feather[p++] = *(u_int*)&v[2];
			p += 2;

		    Feather[p++] = *(u_int*)&u[2];
		    Feather[p++] = *(u_int*)&v[2];
			p += 2;

		    Feather[p++] = *(u_int*)&u[3];
		    Feather[p++] = *(u_int*)&v[3];
			p += 2;

		    Feather[p++] = *(u_int*)&u[0];
		    Feather[p++] = *(u_int*)&v[0];
			p += 2;

		}
		Feather[p++] = *(u_int*)&u[0];
		Feather[p++] = *(u_int*)&v[0];
			p += 2;

		Feather[p++] = *(u_int*)&u[1];
		Feather[p++] = *(u_int*)&v[1];
			p += 2;

		Feather[p++] = *(u_int*)&u[2];
		Feather[p++] = *(u_int*)&v[2];
			p += 2;

		Feather[p++] = *(u_int*)&u[2];
		Feather[p++] = *(u_int*)&v[2];
			p += 2;

	      }else{
		p += 4+4*34;
	      }

		//p ++;			//MSCAL
		p+=4;			//VIFnop,VIFnop,VIFnop,MSCAL
	    }
	}

	*qq = q; *rr = r;
	FlushDCache();			//DCache Flush for Uncache Write

}

//set view matrix
void view2mtx(view,target)
float	view[3];
float	target[3];
{

	/*Prim/Screen matrix						*/
	/*	=psmatrix2 of tgpu_a.pke				*/
	/* 	interlace mode 						*/
	/*       nearZ=64						*/
	/*       farZ=65536						*/
	/*       scrZ=512						*/
	/*       OFX=2048						*/
	/*       OFY=2048						*/
	/*       Zmin=0							*/
	/*  Zmax=16000000.0 (2^24=16777216‚É‚·‚é‚ÆŒvŽZŒë·‚Åoverflow‚·‚é)*/
	/*								*/
	/*       ZA= (16000000*64-0)/(64-65536)= -15640			*/
	/*       ZB= -(16000000*65536*64-0)/(64-65536)=1025001000	*/
	/*								*/
	psmtx[0][0]= 512.0f;	psmtx[0][1]=0.0f;	psmtx[0][2]=2048.0f;	psmtx[0][3]=0.0f;
	psmtx[1][0]= 0.0f;	psmtx[1][1]=256.0f;	psmtx[1][2]=2048.0f;	psmtx[1][3]=0.0f;
	psmtx[2][0]= 0.0f;	psmtx[2][1]=0.0f;	psmtx[2][2]= -15640.0f;	psmtx[2][3]=1025001000.0f;
	psmtx[3][0]= 0.0f;	psmtx[3][1]=0.0f;	psmtx[3][2]=1.0f;	psmtx[3][3]=0.0f;
	
	wlmtx[0][0]= 1.0f;	wlmtx[0][1]=0.0f;	wlmtx[0][2]=0.0f;	wlmtx[0][3]=0.0f;
	wlmtx[1][0]= 0.0f;	wlmtx[1][1]=1.0f;	wlmtx[1][2]=0.0f;	wlmtx[1][3]=0.0f;
	wlmtx[2][0]= 0.0f;	wlmtx[2][1]=0.0f;	wlmtx[2][2]=1.0f;	wlmtx[2][3]=0.0f;
	wlmtx[3][0]= 0.0f;	wlmtx[3][1]=0.0f;	wlmtx[3][2]=0.0f;	wlmtx[3][3]=1.0f;
	

	/*get Screen/World matrix from view point*/
	get_SWmatrix_from_view(view,target,swmtx);
	get_VuSWmatrix_from_view(view,target,VUswmtx);

	/*change VUswmtx for refraction STQ*/
	VUswmtx[0][0] *= 0.5;
	VUswmtx[1][0] *= 0.5;
	VUswmtx[2][0] *= 0.5;
	VUswmtx[3][0] *= 0.5;

	/*Prim/World matrix*/
	MulMatrix(psmtx,swmtx,pwmtx);

        /*Prim/Local Matrix*/
        MulMatrix(pwmtx,wlmtx,plmtx);

	/*store PRIM/LOCAL matrix in DMA packet*/
        (&My_plmtx)[0] = *(u_int *)&plmtx[0][0];
        (&My_plmtx)[1] = *(u_int *)&plmtx[1][0];
        (&My_plmtx)[2] = *(u_int *)&plmtx[2][0];
        (&My_plmtx)[3] = *(u_int *)&plmtx[3][0];
        (&My_plmtx)[4] = *(u_int *)&plmtx[0][1];
        (&My_plmtx)[5] = *(u_int *)&plmtx[1][1];
        (&My_plmtx)[6] = *(u_int *)&plmtx[2][1];
        (&My_plmtx)[7] = *(u_int *)&plmtx[3][1];
        (&My_plmtx)[8] = *(u_int *)&plmtx[0][2];
        (&My_plmtx)[9] = *(u_int *)&plmtx[1][2];
        (&My_plmtx)[10] = *(u_int *)&plmtx[2][2];
        (&My_plmtx)[11] = *(u_int *)&plmtx[3][2];
        (&My_plmtx)[12] = *(u_int *)&plmtx[0][3];
        (&My_plmtx)[13] = *(u_int *)&plmtx[1][3];
        (&My_plmtx)[14] = *(u_int *)&plmtx[2][3];
        (&My_plmtx)[15] = *(u_int *)&plmtx[3][3];

	/*store Screen/World matrix in DMA packet*/
        (&My_wlmtx)[0] = *(u_int *)&swmtx[0][0];
        (&My_wlmtx)[1] = *(u_int *)&swmtx[1][0];
        (&My_wlmtx)[2] = *(u_int *)&swmtx[2][0];
        (&My_wlmtx)[3] = *(u_int *)&swmtx[3][0];
        (&My_wlmtx)[4] = *(u_int *)&swmtx[0][1];
        (&My_wlmtx)[5] = *(u_int *)&swmtx[1][1];
        (&My_wlmtx)[6] = *(u_int *)&swmtx[2][1];
        (&My_wlmtx)[7] = *(u_int *)&swmtx[3][1];
        (&My_wlmtx)[8] = *(u_int *)&swmtx[0][2];
        (&My_wlmtx)[9] = *(u_int *)&swmtx[1][2];
        (&My_wlmtx)[10] = *(u_int *)&swmtx[2][2];
        (&My_wlmtx)[11] = *(u_int *)&swmtx[3][2];
        (&My_wlmtx)[12] = *(u_int *)&swmtx[0][3];
        (&My_wlmtx)[13] = *(u_int *)&swmtx[1][3];
        (&My_wlmtx)[14] = *(u_int *)&swmtx[2][3];
        (&My_wlmtx)[15] = *(u_int *)&swmtx[3][3];

	FlushDCache();

}


void mat_mult_vec(float result[4], float matrix[4][4], float vect[4])
{
	int i,j;

	for (i=0; i<4; i++) {
		result[i] = 0.f;
		for (j=0; j<4; j++) {
			result[i] += matrix[i][j]*vect[j];
		}
	}
}

void mat_mult_mat(float result[4][4], float matrix1[4][4], float matrix2[4][4])
{
	int i,j,k;

	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			result[i][j] = 0.f;
			for (k=0; k<4; k++) {
				result[i][j] += matrix1[i][k]*matrix2[k][j];
			}
		}
	}
}

void unit_matrix(float matrix[4][4])
{
	int i,j;

	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			if (i==j) {
				matrix[i][j] = 1.0f;
			} else {
				matrix[i][j] = 0.0f;
			}
		}
	}
}

void print_matrix(float matrix[4][4])
{
	int i,j;

	for (i=0; i<4; i++) {
		for (j=0; j<4; j++) {
			printf("%d ",(int)(matrix[i][j]*100.0f));
		}
		printf("\r\n");
	}
	printf("\r\n");
}

void cross_product(float v1[3], float v2[3], float vr[3])
{
	vr[2] = v1[0]*v2[1]-v2[0]*v1[1];
	vr[1] = v1[2]*v2[0]-v2[2]*v1[0];
	vr[0] = v1[1]*v2[2]-v2[1]*v1[2];
}

float dot_product(float x[3], float y[3])
{
	return(x[0]*y[0]+x[1]*y[1]+x[2]*y[2]);
}

int normalize(float v[3])
{
	float mather;

	mather = sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	if (mather > 1e-6f) {
		v[0] /= mather;
		v[1] /= mather;
		v[2] /= mather;
		return 0;
	}
	return -1;
}

float get_cosine(float x[3], float y[3])
{
	normalize(x);
	normalize(y);
	return(dot_product(x,y));
}

static float xrot_matrix[4][4] __attribute__((aligned(16)));
static float yrot_matrix[4][4] __attribute__((aligned(16)));
static float zrot_matrix[4][4] __attribute__((aligned(16)));
static float zrot_matrix2[4][4] __attribute__((aligned(16)));
static float tmp_matrix[4][4] __attribute__((aligned(16)));

static float tmp_pos[4] __attribute__((aligned(16)));
static float random_number, rn;
static float sintht, costht, theata;

static float tmp_vec[4] __attribute__((aligned(16)));

//intialize feather rotation data
void init_feather_rot()
{
	int i,j,p;
	float org_pos[4][4], org_nrm[4][4];
	float org_rnd[4];

	register float *pos;
	register float *nrm;

	pos = (float *)GetDp(0);
	nrm = (float *)GetDp(1);

	unit_matrix(xrot_matrix);
	unit_matrix(yrot_matrix);
	unit_matrix(zrot_matrix);

	rn = (float)(rand()%100000)*0.000003f + 0.65f;

	org_pos[0][0] = 1.f*rn; org_pos[0][1] = 0.f;     org_pos[0][2] = 0.f;    org_pos[0][3] = 1.f;
	org_pos[1][0] = 1.f*rn; org_pos[1][1] = 0.0f*rn; org_pos[1][2] = 1.f*rn; org_pos[1][3] = 1.f;
	org_pos[2][0] = 0.f;    org_pos[2][1] = 0.f;     org_pos[2][2] = 1.f*rn; org_pos[2][3] = 1.f;
	org_pos[3][0] = 0.f;    org_pos[3][1] = 0.f;     org_pos[3][2] = 0.f;    org_pos[3][3] = 1.f;

	org_nrm[0][0] = 0.f;  org_nrm[0][1] = -1.f;  org_nrm[0][2] = 0.f;  org_nrm[0][3] = 0.f;
	org_nrm[1][0] = 0.f;  org_nrm[1][1] = -1.f;  org_nrm[1][2] = 0.f;  org_nrm[1][3] = 0.f;
	org_nrm[2][0] = 0.f;  org_nrm[2][1] = -1.f;  org_nrm[2][2] = 0.f;  org_nrm[2][3] = 0.f;
	org_nrm[3][0] = 0.f;  org_nrm[3][1] = -1.f;  org_nrm[3][2] = 0.f;  org_nrm[3][2] = 0.f;

	p = 0;
	for (j=0; j<160; j++) {

		random_number = (float)(rand()%100000)*0.00001f;
		theata = 2.*3.141592654f*random_number;
		sintht = sinf(theata); costht = cosf(theata);
        	zrot_matrix[0][0] = costht; zrot_matrix[0][1] = sintht;
        	zrot_matrix[1][0] = -sintht; zrot_matrix[1][1] = costht;

		random_number = (float)(rand()%100000)*0.00001f;
		theata = 2.*3.141592654f*random_number;
		sintht = sinf(theata); costht = cosf(theata);
        	xrot_matrix[1][1] = costht; xrot_matrix[1][2] = sintht;
        	xrot_matrix[2][1] = -sintht; xrot_matrix[2][2] = costht;

		random_number = (float)(rand()%100000)*0.00001f;
		org_rnd[0] = random_number*0.8f + 0.1f;
		random_number = (float)(rand()%100000)*0.00001f;
		org_rnd[1] = random_number*0.8f + 0.1f;
		random_number = (float)(rand()%100000)*0.00001f;
		org_rnd[2] = random_number*0.8f + 0.1f;

		for (i=0; i<4; i++) {
			mat_mult_vec(tmp_vec,zrot_matrix,org_pos[i]);
			mat_mult_vec(org_pos[i],yrot_matrix,tmp_vec);
		}

		for (i=0; i<4; i++) {
			mat_mult_vec(tmp_vec,zrot_matrix,org_nrm[i]);
			mat_mult_vec(org_nrm[i],yrot_matrix,tmp_vec);
		}

		pos[p]   = org_pos[0][0]*100000.f;
		nrm[p++] = org_nrm[0][0];
		pos[p]   = org_pos[0][1]*100000.f;
		nrm[p++] = org_nrm[0][1];
		pos[p]   = org_pos[0][2]*100000.f;
		nrm[p++] = org_nrm[0][2];

		pos[p]   = org_rnd[0];
		nrm[p]   = 0.0f;
		p++;

		pos[p]   = org_pos[1][0]*100000.f;
		nrm[p++] = org_nrm[1][0];
		pos[p]   = org_pos[1][1]*100000.f;
		nrm[p++] = org_nrm[1][1];
		pos[p]   = org_pos[1][2]*100000.f;
		nrm[p++] = org_nrm[1][2];

		pos[p]   = org_rnd[1];
		nrm[p]   = 0.0f;
		p++;

		pos[p]   = org_pos[2][0]*100000.f;
		nrm[p++] = org_nrm[2][0];
		pos[p]   = org_pos[2][1]*100000.f;
		nrm[p++] = org_nrm[2][1];
		pos[p]   = org_pos[2][2]*100000.f;
		nrm[p++] = org_nrm[2][2];

		pos[p]   = org_rnd[2];
		nrm[p]   = 0.0f;
		p++;

	}

}

//update feather rotation
void rot_feather(int v_div, float ramda)
{
	int i,j,p,p2;
	float org_pos[4][4] __attribute__((aligned(16)));
	float org_nrm[4][4] __attribute__((aligned(16)));
	static float org_rnd[4] __attribute__((aligned(16)));
	static float org_rnd_old[4] __attribute__((aligned(16)));
	float org_rnd_ave[4] __attribute__((aligned(16)));
	float org_rnx[4] __attribute__((aligned(16)));

	register float *pos;
	register float *nrm;

	pos = (float *)GetDp(0);
	nrm = (float *)GetDp(1);

	unit_matrix(xrot_matrix);
	unit_matrix(yrot_matrix);
	unit_matrix(zrot_matrix);
	unit_matrix(zrot_matrix2);

	org_rnd_ave[0] = 0.0f;
	org_rnd_ave[1] = 0.0f;
	org_rnd_ave[2] = 0.0f;

	p = 0;

	for (j=0; j<80; j++) {

		p += 3;
		org_rnd_ave[0] += pos[p];

		p += 4;
		org_rnd_ave[1] += pos[p];

		p += 4;
		org_rnd_ave[2] += pos[p];

		p += 13;

	}

	org_rnd_ave[0] /= 80.f;
	org_rnd_ave[1] /= 80.f;
	org_rnd_ave[2] /= 80.f;

	p = 0;

	for (j=0; j<160; j++) {

		p2 = p;
		org_pos[0][0] = pos[p2];
		org_nrm[0][0] = nrm[p2++];
		org_pos[0][1] = pos[p2];
		org_nrm[0][1] = nrm[p2++];
		org_pos[0][2] = pos[p2];
		org_nrm[0][2] = nrm[p2++];
		org_rnd[0]    = pos[p2];
		org_rnx[0]    = nrm[p2];
		p2++;

		org_pos[1][0] = pos[p2];
		org_nrm[1][0] = nrm[p2++];
		org_pos[1][1] = pos[p2];
		org_nrm[1][1] = nrm[p2++];
		org_pos[1][2] = pos[p2];
		org_nrm[1][2] = nrm[p2++];
		org_rnd[1]    = pos[p2];
		org_rnx[1]    = nrm[p2];
		p2++;

		org_pos[2][0] = pos[p2];
		org_nrm[2][0] = nrm[p2++];
		org_pos[2][1] = pos[p2];
		org_nrm[2][1] = nrm[p2++];
		org_pos[2][2] = pos[p2];
		org_nrm[2][2] = nrm[p2++];
		org_rnd[2]    = pos[p2];
		org_rnx[2]    = nrm[p2];
		p2++;

		if (v_div == 1) {
			org_rnd[0] = (4.0f - (float)j/10000.0f)*org_rnd[0]*(1.f-org_rnd[0]);
			org_rnx[0] += 0.2f*(org_rnd[0]-org_rnd_old[0]);
			org_rnd_old[0] = (org_rnd_old[0] + org_rnd[0])/2.0f;
			org_rnx[0] *= 0.99f;
		}
		theata = org_rnx[0]*0.03f;
		sintht = sinf(theata); costht = cosf(theata);
        	zrot_matrix[0][0] = costht; zrot_matrix[0][1] = sintht;
        	zrot_matrix[1][0] = -sintht; zrot_matrix[1][1] = costht;

		if (v_div == 1) {
			org_rnd[1] = (3.99f - (float)j/10000.0f)*org_rnd[1]*(1.f-org_rnd[1]);
			org_rnx[1] += 0.2f*(org_rnd[1]-org_rnd_old[1]);
			org_rnd_old[1] = (org_rnd_old[1] + org_rnd[1])/2.0f;
			org_rnx[1] *= 0.99f;
		}
		theata = org_rnx[1]*0.03f;
		sintht = sinf(theata); costht = cosf(theata);
        	xrot_matrix[1][1] = costht; xrot_matrix[1][2] = sintht;
        	xrot_matrix[2][1] = -sintht; xrot_matrix[2][2] = costht;

		if (v_div == 1) {
			org_rnd[2] = (3.98f - (float)j/10000.0f)*org_rnd[2]*(1.f-org_rnd[2]);
			org_rnx[2] += 0.2f*(org_rnd[2]-org_rnd_old[2]);
			org_rnd_old[2] = (org_rnd_old[2] + org_rnd[2])/2.0f;
			org_rnx[2] *= 0.99f;
		}
		theata = org_rnx[2]*0.03f;
		sintht = sinf(theata); costht = cosf(theata);
		yrot_matrix[0][0] = costht; yrot_matrix[0][2] = sintht;
       		yrot_matrix[2][0] = -sintht; yrot_matrix[2][2] = costht;

		mat_mult_mat(tmp_matrix, xrot_matrix, yrot_matrix);


		asm __volatile__("
			lqc2		vf10,0x0(%0)
			lqc2		vf11,0x10(%0)
			lqc2		vf12,0x20(%0)
			lqc2		vf13,0x30(%0)
			lqc2		vf14,0x0(%1)
			lqc2		vf15,0x10(%1)
			lqc2		vf16,0x20(%1)
			lqc2		vf17,0x30(%1)
		": :  "r"(zrot_matrix), "r"(tmp_matrix): "memory");

		for (i=0; i<3; i++) {
			//mat_mult_vec(tmp_vec,zrot_matrix,org_pos[i]);
			//mat_mult_vec(org_pos[i],tmp_matrix,tmp_vec);

			asm __volatile__("
				lqc2		vf1,0x0(%0)
				vmulax.xyzw	ACC,vf10,vf1
				vmadday.xyzw	ACC,vf11,vf1
				vmaddaz.xyzw	ACC,vf12,vf1
				vmaddw.xyzw	vf2,vf13,vf1
				vmulax.xyzw	ACC,vf14,vf2
				vmadday.xyzw	ACC,vf15,vf2
				vmaddaz.xyzw	ACC,vf16,vf2
				vmaddw.xyzw	vf3,vf17,vf2
				sqc2		vf3,0x0(%0)
			": :  "r"(org_pos[i]): "memory" );
		}

		for (i=0; i<3; i++) {
			//mat_mult_vec(tmp_vec,zrot_matrix,org_nrm[i]);
			//mat_mult_vec(org_nrm[i],tmp_matrix,tmp_vec);

			asm __volatile__("
				lqc2		vf1,0x0(%0)
				vmulax.xyzw	ACC,vf10,vf1
				vmadday.xyzw	ACC,vf11,vf1
				vmaddaz.xyzw	ACC,vf12,vf1
				vmaddw.xyzw	vf2,vf13,vf1
				vmulax.xyzw	ACC,vf14,vf2
				vmadday.xyzw	ACC,vf15,vf2
				vmaddaz.xyzw	ACC,vf16,vf2
				vmaddw.xyzw	vf3,vf17,vf2
				sqc2		vf3,0x0(%0)
			": :  "r"(org_nrm[i]): "memory" );
		}

		pos[p]   = org_pos[0][0];
		nrm[p++] = org_nrm[0][0];
		pos[p]   = org_pos[0][1];
		nrm[p++] = org_nrm[0][1];
		pos[p]   = org_pos[0][2];
		nrm[p++] = org_nrm[0][2];
		pos[p]   = org_rnd[0];
		nrm[p]   = org_rnx[0];
		p++;

		pos[p]   = org_pos[1][0];
		nrm[p++] = org_nrm[1][0];
		pos[p]   = org_pos[1][1];
		nrm[p++] = org_nrm[1][1];
		pos[p]   = org_pos[1][2];
		nrm[p++] = org_nrm[1][2];
		pos[p]   = org_rnd[1];
		nrm[p]   = org_rnx[1];
		p++;

		pos[p]   = org_pos[2][0];
		nrm[p++] = org_nrm[2][0];
		pos[p]   = org_pos[2][1];
		nrm[p++] = org_nrm[2][1];
		pos[p]   = org_pos[2][2];
		nrm[p++] = org_nrm[2][2];
		pos[p]   = org_rnd[2];
		nrm[p]   = org_rnx[2];
		p++;

	}

}

//initialize feather position
void init_ftpos()
{
	int i;

	for (i=0; i<20000; i++) {

		unit_matrix(xrot_matrix);
		unit_matrix(yrot_matrix);
		unit_matrix(zrot_matrix);

		tmp_pos[0] = 0.0f; tmp_pos[1] = 0.0f; tmp_pos[2] = 0.0f;

		random_number = (float)(rand()%100000)*0.00001f;
		tmp_pos[0] += 16.0f*random_number - 8.0f;

		random_number = (float)(rand()%100000)*0.00001f;
		tmp_pos[2] += 16.0f*random_number - 8.0f;

		random_number = (float)(rand()%100000)*0.00001f;
		tmp_pos[1] += 8.0f*random_number - 5.0f;

		ftpos[2*i][0] = tmp_pos[0]*100000.f;
		ftpos[2*i][1] = tmp_pos[1]*100000.f;
		ftpos[2*i][2] = tmp_pos[2]*100000.f;

		ftpos[2*i+1][0] = tmp_pos[0]*100000.f;
		ftpos[2*i+1][1] = tmp_pos[1]*100000.f;
		ftpos[2*i+1][2] = tmp_pos[2]*100000.f;

	}

}

void island_init(void)
{
	int	i,j;
	int	NLATTX,NLATTZ;		/*wave dimension*/
	int	NISLX,NISLZ;		/*island dimension*/
	float   XHEN,YHEN,ZHEN;

	NLATTX = 129;
	NLATTZ = 129;
	XHEN = 10000.0f;
	YHEN = 0.0f;
	ZHEN = 10000.0f;

	/*draw lattice*/

	NISLX= (NLATTX-1)/4+1;
	NISLZ= (NLATTZ-1)/4+1;

        /*********************/
        /*  wave initialize  */
        /********************/
        lattx= XHEN*2.0f/(NLATTX-1);
        lattz= ZHEN*2.0f/(NLATTZ-1);

        /*initialize rotation center of lattice vertex*/
        for(i=0;i<NLATTZ;i++){
            for(j=0;j<NLATTX;j++){
                lcen[i][j][0] = XCEN - XHEN + lattx*j;
                lcen[i][j][1] = YCEN - YHEN ;
                lcen[i][j][2] = ZCEN - ZHEN + lattz*i;
            }
        }

        /*initialize land*/
	for (i=0; i<33; i++) {
		for (j=0; j<33; j++) {

			tpos[i][j][0] = (i-16)*350.0f;
	tpos[i][j][1] = sinf((float)i*0.3f + (float)(j-8))*cosf((float)j*0.5f)*100.0f + sinf((float)(i-10)*0.4f)*100.0f
                                        +cosf((float)(j-10)*0.2f)*cosf((float)(j-10)*0.2f)*200.0f;
			tpos[i][j][2] = (j-16)*350.0f;

			ttext[i][j][0] = i*0.03125f;
			ttext[i][j][1] = j*0.03125f;

		}
	}

	/*generate view vector*/
	for(i = 0; i < NLATTZ; i++){
		for(j = 0; j < NLATTX; j++){
			view[0] = VIEWP_X - lcen[i][j][0];
			view[1] = VIEWP_Y - lcen[i][j][1];
			view[2] = VIEWP_Z - lcen[i][j][2];

			VectorNormal(view);

			lview[i][j][0] = view[0];
			lview[i][j][1] = view[1];
			lview[i][j][2] = view[2];
		}
	}
	view_cen[0]= lview[64][64][0];
	view_cen[1]= lview[64][64][1];
	view_cen[2]= lview[64][64][2];
}

void clear_buffer(u_int times)
{
	if (times == 1) {
		(&My_clear_buffer)[4] = (u_int)0x00000006;
	} else {
		(&My_clear_buffer)[4] = (u_int)0x00000000;
	}

}

void tex_buf_set(u_int flg)
{
	if (flg == 0) {
		(&My_tex_buffer)[0] = (u_int)0xdc00a710;
		(&My_tex_buffer)[1] = (u_int)0x00000005;
	} else {
		(&My_tex_buffer)[0] = (u_int)0x54006580;
		(&My_tex_buffer)[1] = (u_int)0x00000005;
	}
	FlushDCache();
}

void set_light_color(float mag)
{
	(&My_light_color)[13] = *(u_int*)&mag;
	(&My_light_color)[14] = *(u_int*)&mag;
}

