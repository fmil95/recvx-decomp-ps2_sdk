/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
/**************************************************
*
*	generate terrain
*
*	
***************************************************/
#include <eekernel.h>
#include <eeregs.h>
#include <math.h>
#include <stdio.h>
#include <libgraph.h>
#include <libdma.h>
#include <libvu0.h>
#include <libdev.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <libpad.h>
#include "terrain.h"
#include "constant.h"
#include "scale.h"


#define FlushDCache()   FlushCache(0)
#define	ZMIN		0.0
#define	ZMAX		16000000.0 	//if(ZMAX==2^24=16777216.0) overflow? in float calculation
#define	SCRZ		256.0
#define OFX		2048.0
#define	OFY		2048.0

#define	ZA		((ZMAX*NEARZ-0)/(NEARZ-FARZ))
#define ZB 		(-(ZMAX*FARZ*NEARZ-0)/(NEARZ-FARZ))

extern unsigned int My_gs_clip __attribute__((section(".vudata")));
extern unsigned int My_GS_CLIP __attribute__((section(".vudata")));
extern unsigned int My_micro_dma_start __attribute__((section(".vudata")));
extern unsigned int My_dma_start __attribute__((section(".vudata")));
extern unsigned int My_frame_clear_cloud __attribute__((section(".vudata")));
extern unsigned int My_baseoffset_400_170 __attribute__((section(".vudata")));
extern unsigned int My_adcbit __attribute__((section(".vudata")));
extern unsigned int My_fogab __attribute__((section(".vudata")));
extern unsigned int My_FOGAB __attribute__((section(".vudata")));
extern unsigned int My_fog_color __attribute__((section(".vudata")));
extern unsigned int My_frame001 __attribute__((section(".vudata")));
extern unsigned int My_PRIM_LOCAL __attribute__((section(".vudata")));
extern unsigned int My_LIGHT_LOCAL __attribute__((section(".vudata")));
extern unsigned int My_LIGHT_COLOR __attribute__((section(".vudata")));
extern unsigned int My_prim_local __attribute__((section(".vudata")));
extern unsigned int My_light_local __attribute__((section(".vudata")));
extern unsigned int My_light_color __attribute__((section(".vudata")));
extern unsigned int My_max_color __attribute__((section(".vudata")));
extern unsigned int My_MAX_COLOR __attribute__((section(".vudata")));
extern unsigned int My_rgb_fix __attribute__((section(".vudata")));
extern unsigned int My_RGB_FIX __attribute__((section(".vudata")));

extern unsigned int My_sdiv __attribute__((section(".vudata")));
extern unsigned int My_SDIV __attribute__((section(".vudata")));
extern unsigned int My_tedge __attribute__((section(".vudata")));
extern unsigned int My_TEDGE __attribute__((section(".vudata")));
extern unsigned int My_water_level __attribute__((section(".vudata")));
extern unsigned int My_WATER_LEVEL __attribute__((section(".vudata")));
extern unsigned int My_1_second __attribute__((section(".vudata")));
extern unsigned int My_1_SECOND __attribute__((section(".vudata")));
extern unsigned int My_zpos __attribute__((section(".vudata")));
extern unsigned int My_ZPOS __attribute__((section(".vudata")));
extern unsigned int My_szpos __attribute__((section(".vudata")));
extern unsigned int My_SZPOS __attribute__((section(".vudata")));
extern unsigned int My_SISJ __attribute__((section(".vudata")));
extern unsigned int My_TEXD __attribute__((section(".vudata")));
extern unsigned int My_nmesh __attribute__((section(".vudata")));
extern unsigned int My_NMESH __attribute__((section(".vudata")));
extern unsigned int My_aalias __attribute__((section(".vudata")));
extern unsigned int My_AALIAS __attribute__((section(".vudata")));
extern unsigned int My_triangle __attribute__((section(".vudata")));
extern unsigned int My_TRIANGLE __attribute__((section(".vudata")));

extern unsigned int My_texture_3 __attribute__((section(".vudata")));
extern unsigned int My_texture_31 __attribute__((section(".vudata")));
extern unsigned int My_frame_buffer_set __attribute__((section(".vudata")));
extern unsigned int My_frame_buf __attribute__((section(".vudata")));
extern unsigned int My_llmtx __attribute__((section(".vudata")));
extern unsigned int My_plmtx __attribute__((section(".vudata")));

float	PSmtx[4][4] __attribute__((aligned(16)))	//PRIM/SCREEN matrix
		= {	{SCRZ,        0, OFX,  0}, 
			{0,    SCRZ*0.5, OFY,  0}, 
			{0,           0,  ZA, ZB}, 
			{0,           0, 1.0,  0} };
float	SWmtx[4][4];					//SCREEN/WORLD matrix
float   PWmtx[4][4] __attribute__((aligned(16)));	//PRIM/WORLD matrix

extern u_long *setTextureHeader(u_long *buffer, u_int dbp, u_int dbw, u_int dpsm, u_int rrw, u_int rrh, int col);
extern void readtex(char *fn, void *buffer, int size);

extern int DevInit(void);
extern void dma_kick_schain(u_int tadr);
extern void make_random_table16(float rt16[17][17][4]);
extern void generate_random16_packet(u_int Rpacket16[330][4], float   rt16[17][17][4]);
extern void make_random_table16_1(float rt16[17][17][4],float rt16_1[17][17][4]);
extern void make_random_table16_2(float rt16[17][17][4],float rt16_2[17][17][4]);
extern unsigned int GetCOUNT(void);
extern void get_SWmatrix_from_view(float v[3],float t[3], float SWMTX[4][4]);
extern void MulMatrix(float a[4][4],float b[4][4],float m[4][4]);
extern void set_VUmatrix(u_int *VUad,float mtx[4][4]);
extern void set_VUvector(u_int *VUad,float vect[4]);
extern void init_terrain(mesh *terr);
extern void generate_STQ_packet(u_int *dp,  float texd);
extern int view_clip(mesh *terr, float pl[4][4], float nearZ, float farZ, int i, int j, int pit);
extern void generate_POS_packet(u_int *dp, mesh8 *terr, int si, int sj, int aalias);
extern void WaitVU1(void);
extern void copy_terrain(mesh *bigterr, mesh8 *terr, int bi, int bj, int pit);

void textureLoad(void);
void terrain_by_view(mesh *bigterr, float view[3], float tedge);

sceGsDBuff db;


float	mount_max=0.7;
float	mount_sharp=2.7;

mesh	bigterr;			//outest terrain

u_int paddata;
u_char rdata[32];
u_long128 pad_dma_buf[scePadDmaBufferMax] __attribute__((aligned (64)));

int main()
{

	u_int 	frame;
	int 	oddeven;

	float	view[3]= {VIEWX, VIEWY, VIEWZ};
	float	target[3]= {TARGETX, TARGETY, TARGETZ};


	float	plmat[4][4];	//PRIM/LOCAL matrix
	float	wlmat[4][4]={	{1.0,	0,	0,	0},
				{  0, 1.0,	0,	0},
				{  0,	0,    1.0,	0},
				{  0,	0,	0,    1.0}};	//WORLD/LOCAL matrix for land

	float	llmat[4][4]={	{-0.5,	-0.5,	-0.5,	0},
				{   0,	   0,	   0,	0},
				{   0,	   0,	   0,	0},
				{   0,	   0,	   0,	0}}; 	//LIGHT/LOCAL matrix for land

	float	lcmat[4][4]={	{0.5,	0,	0,	0.3},
				{0.5,	0,	0,	0.3},
				{0.5,	0,	0,	0.3},
				{1.0,	0,	0,	0}};	//LIGHT/COLOR matrix for land

	float	maxcol[4]={255.0, 255.0, 255.0, 255.0};		//max COLOR 
	float	rgbfix[4]={200.0, 200.0, 200.0, 128.0};		//rgb fix color

	int	fcount=0;

	int	aalias=1;


	int	i,j;


	mesh8	terr;

	float	Rtable16[17][17][4] __attribute__((aligned(16)));
	float	Rtable16_1[17][17][4] __attribute__((aligned(16)));
	float	Rtable16_2[17][17][4] __attribute__((aligned(16)));
	u_int	Rpacket16[330][4] __attribute__((aligned(16)));
	u_int	Rpacket16_1[330][4] __attribute__((aligned(16)));
	u_int	Rpacket16_2[330][4] __attribute__((aligned(16)));

	int	A1,A2,A2S,A2E;
	int	B1,B2,B2S,B2E;
	int	C1,C2;

	int	triangle=1;

	float	POSpacket[400][4] __attribute__((aligned(16)));
	float	STQpacket[800][4] __attribute__((aligned(16)));

	float	vleng= 10000.0;
	float	vtheta= PIf*0.25;
	float	vphi= 0.0;

        sceSifInitRpc(0);
        while(sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
                            0, NULL) < 0){
            printf("Can't load module sio2man\n");
        }
        while(sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
                            0, NULL) < 0){
            printf("Can't load module padman\n");
        }

	DevInit();
	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

	sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224, SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_NOCLEAR);

	//texture load
	textureLoad();

	/*micro program load*/
	while(*D2_CHCR & 0x0100);
	while(*D1_CHCR & 0x0100);
	*D_STAT = 2;
	*D1_QWC  = 0x00;
	*D1_TADR = (u_int)&My_micro_dma_start & 0x0fffffff;
	FlushCache(0);
	*D1_CHCR = 1 | (1<<2) | (0<<4) | (0<<6) | (0<<7) | (1<<8);

	oddeven = ! sceGsSyncV(0);
	frame = 0;

	FlushCache(0);

        // open PAD Device
        scePadInit(0);
        scePadPortOpen(0, 0, pad_dma_buf);


	/*FOG parameter initialize*/
	*(float *)(&My_FOGAB+3)= (255.0*FogNear)/(FogNear-FogFar);
	*(float *)(&My_FOGAB+7)= -(255.0*FogFar*FogNear)/(FogNear-FogFar);


	/*GS clip area*/
	*(float*)&My_GS_CLIP=0.0;		//near left upper
	*(float*)(&My_GS_CLIP+1)=0.0;	
	*(float*)(&My_GS_CLIP+2)=0.0;	
	*(float*)(&My_GS_CLIP+3)=NEARZ;	

	*(float*)(&My_GS_CLIP+4)=4095.0;	//far right down 
	*(float*)(&My_GS_CLIP+5)=4095.0;	
	*(float*)(&My_GS_CLIP+6)=0.0;	
	*(float*)(&My_GS_CLIP+7)=FARZ;	

        *(float*)(&My_GS_CLIP+8)=4096.0;
        *(float*)(&My_GS_CLIP+9)=4096.0;
        *(float*)(&My_GS_CLIP+10)=1.0;
        *(float*)(&My_GS_CLIP+11)=-1.0;

        *(float*)(&My_GS_CLIP+12)=0.0;
        *(float*)(&My_GS_CLIP+13)=0.0;
        *(float*)(&My_GS_CLIP+14)=NEARZ;
        *(float*)(&My_GS_CLIP+15)=-FARZ;

	dma_kick_schain((u_int)&My_gs_clip);


	//make randam table & packet
	make_random_table16(Rtable16);
	generate_random16_packet(Rpacket16,Rtable16);

	make_random_table16_1(Rtable16,Rtable16_1);
	generate_random16_packet(Rpacket16_1,Rtable16_1);

	make_random_table16_2(Rtable16,Rtable16_2);
	generate_random16_packet(Rpacket16_2,Rtable16_2);

	//load Fractal subdivide constants to VU-MEM
	*(float *)&My_WATER_LEVEL= -600.0;
	dma_kick_schain((u_int)&My_water_level);
	dma_kick_schain((u_int)&My_1_second);
	dma_kick_schain((u_int)&My_zpos);
	dma_kick_schain((u_int)&My_szpos);

	while(1){


		if(frame>=4000) {frame -=4000; fcount++;}

		sceGsSetHalfOffset((frame&1)?(&db.draw1):(&db.draw0), 2048, 2048, oddeven);

		if((frame&1)==0){
			(&My_frame001)[0] = 0x000a0046;
			(&My_frame001)[5] = 0x00007900+ (oddeven?8:0);
		}else{
			(&My_frame001)[0] = 0x000a0000;
			(&My_frame001)[5] = 0x00007900 + (oddeven?8:0);
		}

		FlushCache(0);
		sceGsSwapDBuff(&db, frame);

                // read PAD
                if(scePadRead(0, 0, rdata) > 0){
                    paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
                }else{
                    paddata = 0;
                }


                if((paddata & SCE_PADk) && (paddata & SCE_PADRright))	{ mount_max += 0.01; }
                if((paddata & SCE_PADk) && (paddata & SCE_PADRleft))	{ mount_max -= 0.01; }
                if((paddata & SCE_PADk) && (paddata & SCE_PADRup))	{ mount_sharp += 0.01;}
                if((paddata & SCE_PADk) && (paddata & SCE_PADRdown))	{ mount_sharp -= 0.01;}

                if(!(paddata & SCE_PADk) && (paddata & SCE_PADLright)) 	{ target[2] +=DVHEIGHT; }
                if(!(paddata & SCE_PADk) && (paddata & SCE_PADLleft)) 	{ target[2] -=DVHEIGHT; }
                if(!(paddata & SCE_PADk) && (paddata & SCE_PADn)) 	{ target[1] -=DVHEIGHT; }
                if(!(paddata & SCE_PADk) && (paddata & SCE_PADo)) 	{ target[1] +=DVHEIGHT; }
                if(!(paddata & SCE_PADk) && (paddata & SCE_PADLup)) 	{ target[0] -=DVHEIGHT; }
                if(!(paddata & SCE_PADk) && (paddata & SCE_PADLdown)) 	{ target[0] +=DVHEIGHT; }

                if(!(paddata & SCE_PADk) && (paddata & SCE_PADRright)) 	{ vphi +=0.01; }
                if(!(paddata & SCE_PADk) && (paddata & SCE_PADRleft)) 	{ vphi -=0.01; }
                if(!(paddata & SCE_PADk) && (paddata & SCE_PADl)) 	{ vleng +=DVHEIGHT; }
                if(!(paddata & SCE_PADk) && (paddata & SCE_PADm)) 	{ vleng -=DVHEIGHT; }
                if(!(paddata & SCE_PADk) && (paddata & SCE_PADRup)) 	{ vtheta -=0.01; }
                if(!(paddata & SCE_PADk) && (paddata & SCE_PADRdown)) 	{ vtheta +=0.01; }

		if(vleng<=100.0) vleng=100.0;				//limiter
		if(vtheta<=0.01) vtheta=0.01;
		if(vtheta>=PIf-0.01) vtheta=PIf-0.01;

		view[0]= target[0]+vleng*sinf(vtheta)*cosf(vphi);	//view rotation
		view[1]= target[1]-vleng*cosf(vtheta);
		view[2]= target[2]+vleng*sinf(vtheta)*sinf(vphi);
					
                if((paddata & SCE_PADk) && (paddata & SCE_PADn))     	{ triangle=1; }	//triangle ON
                if((paddata & SCE_PADk) && (paddata & SCE_PADl))	{ triangle=0; }	//triangle OFF
                if((paddata & SCE_PADk) && (paddata & SCE_PADm)) 	{ aalias=1; }	//aalias ON
                if((paddata & SCE_PADk) && (paddata & SCE_PADo)) 	{ aalias=0; }	//aalias OFF


	    /****frame buffer clear by gray color****/
		dma_kick_schain((u_int)&My_dma_start);			//parameter DMA kick
		dma_kick_schain((u_int)&My_frame_clear_cloud);		//frame buffer clear

	    /****initial parameter****/
		dma_kick_schain((u_int)&My_baseoffset_400_170);		//set BASE&OFFSET for double buffer input data

		get_SWmatrix_from_view(view,target,SWmtx);		//get SCREEN/WORLD from viewp and target
		MulMatrix(PSmtx,SWmtx,PWmtx);				//get PRIM/WORLD for land & road
		MulMatrix(PWmtx,wlmat,plmat);				//get PRIM/LOCAL matrix for land & road

		set_VUmatrix(&My_PRIM_LOCAL, plmat);			//set PRIM/LOCAL matrix to DMA packet
		set_VUmatrix(&My_LIGHT_LOCAL, llmat);
		set_VUmatrix(&My_LIGHT_COLOR, lcmat);
		set_VUvector(&My_MAX_COLOR, maxcol);
		set_VUvector(&My_RGB_FIX, rgbfix);

		dma_kick_schain((u_int)&My_prim_local);			//send PRIM/LOCAL matrix to VUMEM
		dma_kick_schain((u_int)&My_light_local);		//send lighting parameter etc. to VUMEM
		dma_kick_schain((u_int)&My_light_color);
		dma_kick_schain((u_int)&My_max_color);
		dma_kick_schain((u_int)&My_rgb_fix);
		dma_kick_schain((u_int)&My_fogab);
		dma_kick_schain((u_int)&My_fog_color);
		dma_kick_schain((u_int)&My_adcbit);



	    /****FBP & OFY set according to "frame" & "oddeven"****/
		if((frame&1)==0){
			(&My_frame_buf)[0] = 0x000a0046;		//FRAME_1 FBP=70 FBW=10 PSM=0 FBMSK=0 
			(&My_frame_buf)[5] = 0x00007900+ (oddeven?8:0);	//XYOFFSET_1 OFX=27648 OFY=30976or30984
		}else{
			(&My_frame_buf)[0] = 0x000a0000;		//FRAME_1 FBP=0 FBW=10 PSM=0 FBMSK=0
			(&My_frame_buf)[5] = 0x00007900 + (oddeven?8:0);//XYOFFSET_1 OFX=27648 OFY=30976or30984
		}

		/*parameter DMA kick*/
		dma_kick_schain((u_int)&My_frame_buffer_set);


		//Mesh dimension 8-4/8-4/8-0
		A1=8;
		A2=4;
		A2S= A1/2-A2/2;
		A2E= A1/2+A2/2;

		B1=A2*2;
		B2=4;
		B2S= B1/2-B2/2;
		B2E= B1/2+B2/2;

		C1=B2*2;
		C2=0;

	    	// initialize terrain
		bigterr.mount_max= mount_max;
		bigterr.mount_sharp= mount_sharp;
		bigterr.tedge= TEDGE/4;
		bigterr.nmesh= A1;

		terrain_by_view(&bigterr, view, TEDGE/4);	//define XZ pos of bigterr from view

		init_terrain(&bigterr);				//define Y pos of bigterr 



		dma_kick_schain((u_int)&My_texture_31);		//TEX mode set for Snow.h,Rock.h,Forest.h,Field.h
								//NEAREST_MIPMAP_LINEAR

		My_TRIANGLE =triangle;				//triangle ON/OFF
		dma_kick_schain((u_int)&My_triangle);


		/*outest A1 x A1 terrain*/

		dma_kick_schain((u_int)Rpacket16_2);		//set random in VUMEM

		*(float *)&My_TEDGE = bigterr.tedge/2.0 * bigterr.mount_sharp;	//tedge*sharp
		dma_kick_schain((u_int)&My_tedge);

		My_SDIV = 4;				//subdivide start # to fsub.vuasm
		dma_kick_schain((u_int)&My_sdiv);

		My_NMESH = A1;				//# of mesh to fsub.vuasm
		dma_kick_schain((u_int)&My_nmesh);

		generate_STQ_packet((u_int*)STQpacket, 4.0);	//set STQ in VUMEM
		dma_kick_schain((u_int)STQpacket);

		terr.sdiv=4;				//subdivide start No. to generate_POS_packet

		for(i=0;i<A1;i++){
		  for(j=0;j<A1;j++){
		    if( !( (i>=A2S&&i<A2E) && (j>=A2S&&j<A2E) ) ){
		      if(view_clip(&bigterr,plmat,NEARZ,FARZ,i*4,j*4, 4)==0){
			copy_terrain(&bigterr, &terr, i*4, j*4, 2);		//subdivide bigterr to terr
			generate_POS_packet((u_int*)POSpacket, &terr, i, j, aalias);	//generate POS & SISJ & MSCAL packet
			dma_kick_schain((u_int)POSpacket );			//dma kick 
		      }
		    }
		  }
		}

		WaitVU1();

		dma_kick_schain((u_int)&My_texture_3);		//TEX mode set for Snow.h,Rock.h,Forest.h,Field.h
								//LINEAR_MIPMAP_LINEAR

		/* outer B1 x B1 terrain */

		dma_kick_schain((u_int)Rpacket16_1);		//set random in VUMEM

		*(float *)&My_TEDGE = bigterr.tedge/4.0 * bigterr.mount_sharp;	//tedge*sharp
		dma_kick_schain((u_int)&My_tedge);

		My_SDIV = 2;				//subdivide start # to fsub.vuasm
		dma_kick_schain((u_int)&My_sdiv);

		My_NMESH = B1;				//# of mesh to fsub.vuasm
		dma_kick_schain((u_int)&My_nmesh);

		generate_STQ_packet((u_int*)STQpacket, 2.0);	//set STQ in VUMEM
		dma_kick_schain((u_int)STQpacket);

		terr.sdiv=2;				//subdivide start No. to generate_POS_packet

		for(i=0;i<B1;i++){
		  for(j=0;j<B1;j++){
		    if( !( (i>=B2S&&i<B2E) && (j>=B2S&&j<B2E) ) ){
		      if(view_clip(&bigterr,plmat,NEARZ,FARZ,A2S*4+i*2,A2S*4+j*2, 2)==0){
			copy_terrain(&bigterr, &terr, A2S*4+i*2, A2S*4+j*2, 4);	//subdivide bigterr to terr
			generate_POS_packet((u_int*)POSpacket, &terr, i, j, aalias);	//generate POS & SISJ & MSCAL packet
			dma_kick_schain((u_int)POSpacket );			//dma kick 
		      }
		    }
		  }
		}

		WaitVU1();


		/* inner C1 x C1 terrain */

		dma_kick_schain((u_int)Rpacket16);	//set random in VUMEM

		*(float *)&My_TEDGE = bigterr.tedge/8.0 * bigterr.mount_sharp;	//tedge*sharp
		dma_kick_schain((u_int)&My_tedge);

		My_SDIV = 1;				//subdivide start # to fsub.vuasm
		dma_kick_schain((u_int)&My_sdiv);

		My_NMESH = C1;				//# of mesh to fsub.vuasm
		dma_kick_schain((u_int)&My_nmesh);

		generate_STQ_packet((u_int*)STQpacket, 1.0);	//set STQ in VUMEM
		dma_kick_schain((u_int)STQpacket);

		terr.sdiv=1;				//subdivide start No. to generate_POS_packet

		for(i=0;i<C1;i++){
		  for(j=0;j<C1;j++){
		    if(view_clip(&bigterr,plmat,NEARZ,FARZ,A2S*4+B2S*2+i,A2S*4+B2S*2+j, 1)==0){
		    	copy_terrain(&bigterr, &terr, A2S*4+B2S*2+i, A2S*4+B2S*2+j, 8);	//copy bigterr to terr
			generate_POS_packet((u_int*)POSpacket, &terr, i, j, aalias);		//generate POS & SISJ & MSCAL packet
			dma_kick_schain((u_int)POSpacket );				//dma kick 
		    }
		  }
		} 



		frame++;

		//WaitDMA();
		while(*D1_CHCR & 0x0100);
		while(*VIF1_STAT & 0x1f000003);
		WaitVU1();
		while(*GIF_STAT & 0x0c00);

		oddeven = !sceGsSyncV(0);
	}

}

void terrain_by_view(mesh *bigterr, float view[3], float tedge)
{
	int 	i,j;
	float   fview[4];
	int     iview[4];
	float	bcent[100][100][4];
	int	N;

	N= bigterr->nmesh;

        //find TEDGE-lattice point which is center of terrain
	fview[0]= view[0]+TEDGE/4;		//move mesh by (TEDGE/4,TEDGE/4) = outer mesh size
        fview[2]= view[2]+TEDGE/4;

        //iview=center of terrain		//move mesh by (TEDGE/4,TEDGE/4) = outer mesh size
        if(fview[0]>=0) iview[0]= (int)fview[0] - ((int)fview[0]%((int)(TEDGE/2)));
        else            iview[0]= (int)fview[0] -( ((int)(TEDGE/2))- ((int)(-fview[0])%((int)(TEDGE/2))) );
        if(fview[2]>=0) iview[2]= (int)fview[2] - ((int)fview[2]%((int)(TEDGE/2)));
        else            iview[2]= (int)fview[2] -( ((int)(TEDGE/2))- ((int)(-fview[2])%((int)(TEDGE/2))) );

	bcent[0][0][0] = iview[0] - tedge*((float)(N*4/2)-0.5);
	bcent[0][0][2] = iview[2] - tedge*((float)(N*4/2)-0.5);

	for(i=0;i<N*4;i++){
	  for(j=0;j<N*4;j++){
            bcent[i][j][0] = bcent[0][0][0] + tedge*j;
            bcent[i][j][2] = bcent[0][0][2] + tedge*i;
	  }
	}

	for(i=0;i<N*4;i++){
	  for(j=0;j<N*4;j++){
	    bigterr->pos[i][j][0]= bcent[i][j][0] - tedge/2;
	    bigterr->pos[i][j][2]= bcent[i][j][2] - tedge/2;
	    bigterr->pos[i][j][3]= 1.0;
	  }
	  bigterr->pos[i][N*4][0]= bcent[i][N*4-1][0] + tedge/2;
	  bigterr->pos[i][N*4][2]= bcent[i][N*4-1][2] - tedge/2;
	  bigterr->pos[i][N*4][3]= 1.0;
	}
	for(j=0;j<N*4;j++){
	  bigterr->pos[N*4][j][0]= bcent[N*4-1][j][0] - tedge/2;
	  bigterr->pos[N*4][j][2]= bcent[N*4-1][j][2] + tedge/2;
	  bigterr->pos[N*4][j][3]= 1.0;
	}
	bigterr->pos[N*4][N*4][0]= bcent[N*4-1][N*4-1][0] + tedge/2;
	bigterr->pos[N*4][N*4][2]= bcent[N*4-1][N*4-1][2] + tedge/2;
	bigterr->pos[N*4][N*4][3]= 1.0;
}

#define TEX_HEADER_SIZE 6

#define TEX_SIZE_SNOW   (128*128*3)
#define TEX_SIZE_ROCK   (64*64*3)
#define TEX_SIZE_FOREST (32*32*3)
#define TEX_SIZE_FIELD  (16*16*3)

#define DMA_QWORD_SNOW   (((TEX_SIZE_SNOW+15)/16)+TEX_HEADER_SIZE)
#define DMA_QWORD_ROCK   (((TEX_SIZE_ROCK+15)/16)+TEX_HEADER_SIZE)
#define DMA_QWORD_FOREST (((TEX_SIZE_FOREST+15)/16)+TEX_HEADER_SIZE)
#define DMA_QWORD_FIELD  (((TEX_SIZE_FIELD+15)/16)+TEX_HEADER_SIZE)

static u_long128 Snow[DMA_QWORD_SNOW];
static u_long128 Rock[DMA_QWORD_ROCK];
static u_long128 Forest[DMA_QWORD_FOREST];
static u_long128 Field[DMA_QWORD_FIELD];

// read texture from file and load to GS
void textureLoad(void)
{
    	u_long *p;

    	p=setTextureHeader((u_long *)Snow, 10432, 2, 1, 128, 128, 3); // adr, dvp, dbw, dpsm, rrw, rrh, col
    	readtex("host:snow.raw", p, TEX_SIZE_SNOW); // filename, buf, size

    	p=setTextureHeader((u_long *)Rock, 10688, 1, 1, 64, 64, 3); // adr, dvp, dbw, dpsm, rrw, rrh, col
    	readtex("host:rock.raw", p, TEX_SIZE_ROCK);

    	p=setTextureHeader((u_long *)Forest, 10752, 1, 1, 32, 32, 3); // adr, dvp, dbw, dpsm, rrw, rrh, col
    	readtex("host:forest.raw", p, TEX_SIZE_FOREST);

    	p=setTextureHeader((u_long *)Field, 10768, 1, 1, 16, 16, 3); // adr, dvp, dbw, dpsm, rrw, rrh, col
    	readtex("host:field.raw", p, TEX_SIZE_FIELD);

	FlushDCache();
    	//SyncDCache(Snow, Field+DMA_QWORD_FIELD);

        while((*D2_CHCR & 0x0100) || (*GIF_STAT & 0x0c00));     //wait DMA end

        //Snow
        *D2_QWC = DMA_QWORD_SNOW;
        *D2_MADR = (u_int)Snow & 0x0fffffff;
        *D2_CHCR = 1 | (1<<8);                          //DMA kick

        while((*D2_CHCR & 0x0100) || (*GIF_STAT & 0x0c00));     //wait DMA end

        //Rock
        *D2_QWC = DMA_QWORD_ROCK;
        *D2_MADR = (u_int)Rock & 0x0fffffff;
        *D2_CHCR = 1 | (1<<8);                          //DMA kick

        while((*D2_CHCR & 0x0100) || (*GIF_STAT & 0x0c00));     //wait DMA end

        //Forest
        *D2_QWC = DMA_QWORD_FOREST;
        *D2_MADR = (u_int)Forest & 0x0fffffff;
        *D2_CHCR = 1 | (1<<8);                          //DMA kick

        while((*D2_CHCR & 0x0100) || (*GIF_STAT & 0x0c00));     //wait DMA end

        //Field
        *D2_QWC = DMA_QWORD_FIELD;
        *D2_MADR = (u_int)Field & 0x0fffffff;
        *D2_CHCR = 1 | (1<<8);                          //DMA kick
        while((*D2_CHCR & 0x0100) || (*GIF_STAT & 0x0c00));     //wait DMA end
}
