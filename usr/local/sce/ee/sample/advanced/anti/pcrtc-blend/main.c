/*  "PlayStation 2" Technical Sample */
/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
/*
 *       Version   Date           Design              Log
 *  --------------------------------------------------------------------
 *       1.0.0     june,15,2000    Akio Ohba          first version
 */



#include <stdlib.h>
#include <stdio.h>
#include <eekernel.h>
#include <eeregs.h>
#include <math.h>
#include <libgraph.h>
#include <libdma.h>
#include <libpkt.h>
#include <libvu0.h>
#include <libdev.h>
#include <sifdev.h>
#include <eestruct.h>
#include <sifrpc.h>
#include <libpad.h>

#include "base.h"

// gscntrl.c: Original GS code include External Input/Sync
extern void GS_runtime_preset(u_int frm, int flg);

// work.dvpasm
extern unsigned int  My_dma_mpg_start __attribute__((section(".vudata")));
extern unsigned int  My_dma_mpg_end __attribute__((section(".vudata")));
// packet.c
extern u_int	*make_mpg_packet(u_int *p, u_int *mpg);

extern u_int	*make_pack_ret(u_int *p, u_int vifcode0, u_int vifcode1);
extern u_int	*make_vubuf_preset_pack(u_int *p, int base, int offset, int outbuf0, int outbuf1);

extern u_int	*make_image_packet(u_int *p, u_int *imp, int txp, int txw, int txmd, int ofx, int ofy, int tw,int th);
extern u_int	*make_direct_packet(u_int *p, u_int *packet, u_int num);
extern u_int	*make_matrix_packet(u_int *p, float *plmx, float *lcmx, float *llmx, float *clp);
extern u_int	*make_stripmesh_mtx_packet(u_int *p, int prim, PVECTOR *meshupper, PVECTOR *meshbottom, int meshn);
extern u_int	*make_stripmesh_inline_packet(u_int *p, int prim, PVECTOR *mesh, int meshn);

// matrix.c

extern void	view2swmtx(float view[3],float target[3],float swmx[4][4]);
extern void	set_llmtx(float lv[4], float wlm[4][4], float llm[4][4]);
extern void	psmtx2plmtx(float psm[4][4], float swm[4][4], float pwm[4][4], float wlm[4][4], float plm[4][4]);
extern inline void Mul_Matrix_Matrix(sceVu0FMATRIX a,sceVu0FMATRIX b, sceVu0FMATRIX m);

// dma.c
extern void	dma_packet_VIF1_Tag(u_int *packet);
extern u_int	GetCOUNT(void);

// fractalobj.c
extern u_int	cltbf[256] __attribute__((aligned(16)));
extern PVECTORM vgeo[6];
extern TEX8M	tgeo[6];


extern void 	maketexture(void);			// make unified texture buffer
extern void 	makeclut(void);				// 8bit CLUT Making: cltbf[256
extern void 	fractalgeo(int seed);			// fractal texture
extern void 	fractalvp(void);			// Sphere data
extern void 	MakeObjectMesh(void);			// 

// DMAtag Area
unsigned int	staticlinktagarea[100000*4] __attribute__((aligned(16)));	// Call tag realtime
unsigned int	dynamiclinktagarea[1000*4] __attribute__((aligned(16)));	// Call tag realtime

// Matrix
float	psmtx[4][4] __attribute__((aligned(16)));	// Prim(GS)/Screen 	Matrix
float	swmtx[4][4] __attribute__((aligned(16)));	// Screen/World 	Matrix
float	pwmtx[4][4] __attribute__((aligned(16)));	// Prim(GS)/World  	Matrix
float	wlmtx[4][4] __attribute__((aligned(16)));	// World/Local 		Matrix
// Matrix for MPG(VU1)
float	plmtx[4][4] __attribute__((aligned(16)));	// Prim(GS)/Local 	Matrix (to VU1)
float	llmtx[4][4] __attribute__((aligned(16)));	// Local/Light 		Matrix (to VU1)
float	lcmtx[4][4] __attribute__((aligned(16)));	// Light/Color 		Matrix (to VU1)
float	clpm[16]  __attribute__((aligned(16))) ;	// Clip Aria (to VU1)

void	pad_read_dualshock2(void);

sceGsDBuff db;

u_int paddata; //NS
u_char rdata[32]; //NS
u_long128 pad_dma_buf[scePadDmaBufferMax] __attribute__((aligned(64))); //NS
int padRu,padRd,padRl,padRr,padRfu,padRfd,padLu,padLd,padLl,padLr,padLfu,padLfd,padsel,padstr,padRtop,padLtop;
int padRtopB,padLtopB;
int padARud,padARlr,padALud,padALlr;
int padARu,padARd,padARl,padARr,padARfu,padARfd,padALu,padALd,padALl,padALr,padALfu,padALfd;

u_int frame;
int oddeven;

float	view[4]	__attribute__((aligned(16)));
float	target[4]__attribute__((aligned(16)));
float	ldir[4]	__attribute__((aligned(16)));


void matrix_init(void)
{
float *mf;
/* Matrix Init. */
/****************************************************************************
	{SCRZ,        0,  0,   0}, 		{SCRZ,        0, OFX,  0},
	{0,    SCRZ*0.5,  0,   0}, 	->	{0,    SCRZ*0.5, OFY,  0},
	{OFX,       OFY, ZA, 1.0}, 		{0,           0,  ZA, ZB},
	{0,           0, ZB,   0} 		{0,           0, 1.0,  0} 
	   Real memory Image	-> Transepose ->    VU&System Matrix
*****************************************************************************/
	// prim(GS)/Screen
	psmtx[0][0]=SCRZ;	psmtx[1][0]=0.0f;	psmtx[2][0]=OFX;	psmtx[3][0]=0.0f;
	psmtx[0][1]=0.0f;	psmtx[1][1]=SCRZ*1.0f;	psmtx[2][1]=OFY;	psmtx[3][1]=0.0f; 
	psmtx[0][2]=0.0f;	psmtx[1][2]=0.0f;	psmtx[2][2]= ZA;	psmtx[3][2]= ZB; 
	psmtx[0][3]=0.0f;	psmtx[1][3]=0.0f;	psmtx[2][3]=1.0f;	psmtx[3][3]=0.0f; 
	// Light/Color & BGcolor
	// L1 Color             L2 Color(N/A)		L3 Color(N/A)		BGcolor(Environment Light)
	lcmtx[0][0]=0.5f;	lcmtx[1][0]=0.0f;	lcmtx[2][0]=0.0f;	lcmtx[3][0]= 0.5f; 
	lcmtx[0][1]=0.5f;	lcmtx[1][1]=0.0f;	lcmtx[2][1]=0.0f;	lcmtx[3][1]= 0.5f; 
	lcmtx[0][2]=0.5f;	lcmtx[1][2]=0.0f;	lcmtx[2][2]=0.0f;	lcmtx[3][2]= 0.5f; 
	lcmtx[0][3]=1.0f;	lcmtx[1][3]=0.0f;	lcmtx[2][3]=0.0f;	lcmtx[3][3]= 0.5f; 
	// Set Clip Area
	mf= clpm;
	(mf)[0]=2048.0f-2048.0f; (mf)[1]=2048.0f-2048.0f; (mf)[2]=0.0f;	(mf)[3]=1.0f;		// left top near  for Clip
	(mf)[4]=2048.0f+2047.0f; (mf)[5]=2048.0f+2047.0f; (mf)[6]=0.0f;	(mf)[7]=160000.0f;	// right bottom far for Clip
}
/* GS PRESET Packet */
#define ISN    8
unsigned long initcode[ISN+1][2] __attribute__((aligned(16))) = {
// nloop=ISN, eop=1, pre=0, prim=0, flg=0:PACKET, nreg=1: NREG(0x0e:A_D) 
// COLCLAMP CLAMP=1
// PABE PABE=0
// ALPHA_1 A=0 B=1 C=0 D=1 FIX=0 
// FOGCOL FCR=100 FCG=100 FCB=100
// PRMODECONT AC=1
// TEXA TA0=128 AEM=0 TA1=0
// TEST_1 ATE=0 ATST=0 AREF=0 AFAIL=0 DATE=0 DATM=0 ZTE=1 ZTST=2
// CLAMP_1 WMS=0 WMT=0 MINU=0 MAXU=0 MINV=0 MAXV=0
  {SCE_GIF_SET_TAG(ISN,   1, 0, 0, SCE_GIF_PACKED, 1),  0x0000000e},  
  {SCE_GS_SET_COLCLAMP(1),                      	SCE_GS_COLCLAMP},      // 1
  {SCE_GS_SET_PABE(0),                          	SCE_GS_PABE},          // 2
  {SCE_GS_SET_ALPHA(0, 1, 0, 1, 0),             	SCE_GS_ALPHA_1},       // 3
  {SCE_GS_SET_FOGCOL(100,100,100),              	SCE_GS_FOGCOL},        // 4
  {SCE_GS_SET_PRMODECONT(1),                    	SCE_GS_PRMODECONT},    // 5
  {SCE_GS_SET_TEXA(128, 0, 0),                  	SCE_GS_TEXA},          // 6
  {SCE_GS_SET_TEST(0,0,0,0,0,0,1,2),            	SCE_GS_TEST_1},        // 7
  {SCE_GS_SET_CLAMP(0,0,0,0,0,0),               	SCE_GS_CLAMP_1},       // 8
};

/* GS Frame Packet */
#define FSN    12
unsigned long framecode[FSN+1][2] __attribute__((aligned(16))) = {
// nloop=FSN, eop=1, pre=0, prim=0, flg=0:PACKET, nreg=1: NREG(0x0e:A_D)   GIFtag
// TEX1_1 LCM=1 MXL=0 MMAG=1 MMIN=1 MTBA=0 L=0 K=0
// TEX0_1 TBP0=9600 TBW=1 PSM=1 TW=6 TH=6 TCC=1 TFX=0 CBP=0 CPSM=0 CSM=0 CSA=0 CLD=0
// FRAME_1 FBP=0 FBW=10 PSM=2 FBMSK=0
// XYOFFSET_1 OFX=27648:(2048-320)*16  OFY=30976(2048-112)*16
// ZBUF_1 ZBP=150 PSM=1 ZMSK=0
// SCISSOR_1 SCAX0=0 SCAX1=639 SCAY0=0 SCAY1=223
// TEST_1 ATE=0 ATST=0 AREF=0 AFAIL=0 DATE=0 DATM=0 ZTE=1 ZTST=1
// PRIM PRIM=6 IIP=0 TME=0 FGE=0 ABE=0 AA1=0 FST=0 CTXT=0 FIX=0
// RGBAQ 200 200 200 0 0.000000
// XYZF2 1728.000000 1936.000000 0 0
// XYZF2 2368.000000 2160.000000 0 0
// TEST_1 ATE=0 ATST=0 AREF=0 AFAIL=0 DATE=0  DATM=0 ZTE=1 ZTST=2 
  {SCE_GIF_SET_TAG(FSN, 1, 0, 0, SCE_GIF_PACKED, 1),    			0x0000000e    }, // GIFtag A_D
  {SCE_GS_SET_FRAME(DBUF0, DISPWIDTH/64, 0, 0),                   		SCE_GS_FRAME_1},
  {SCE_GS_SET_XYOFFSET((2048-(DISPWIDTH/2))*16,(2048-(DISPHEIGHT/2))*16), 	SCE_GS_XYOFFSET_1},
  {SCE_GS_SET_TEX1(1,0,1,1,0,0,0),                      			SCE_GS_TEX1_1 },        
  //{SCE_GS_SET_TEX0(INBUF,TXW/64,1,N,N,1,0,0,0,0,0,0),				SCE_GS_TEX0_1 }, // 32bit Direct 
  {SCE_GS_SET_TEX0(TXDP,TXWW/64,19,M,M,1,0,CLTDP,0,0,0,1),			SCE_GS_TEX0_1 }, // 8bit CLUT
  {SCE_GS_SET_ZBUF(ZBUF,1,0),                            			SCE_GS_ZBUF_1},
  {SCE_GS_SET_SCISSOR_1(0 ,DISPWIDTH-1,0 ,DISPHEIGHT-1),			SCE_GS_SCISSOR_1},
  {SCE_GS_SET_TEST(0,0,0,0,0,0,1,1),                    			SCE_GS_TEST_1},
  {SCE_GS_SET_PRIM(6,0,0,0,0,0,0,0,0),                  			SCE_GS_PRIM  }, // SPRITE( Frame Clear)
  {SCE_GS_SET_RGBAQ(0,0,20,0,0),                   				SCE_GS_RGBAQ }, // RGBA (BG color)
  {SCE_GS_SET_XYZF((2048-(DISPWIDTH/2))*16,(2048-(DISPHEIGHT/2))*16,0,0),  	SCE_GS_XYZF2 }, // UL point
  {SCE_GS_SET_XYZF((2048+(DISPWIDTH/2))*16,(2048+(DISPHEIGHT/2))*16,0,0),  	SCE_GS_XYZF2 }, // DR point
  {SCE_GS_SET_TEST(0,0,0,0,0,0,1,2),                    			SCE_GS_TEST_1},
};

int main()
{
	u_int *pp;
	u_int *gsinitp, *vumpgp, *teximagepa[6],*texclutp, *gsframep, *vumtxp, *vuobjdtpa[6], *vuobjdtp;
	int i, j, k,tgl0,tgl1;
	u_int timer_start, timer_end, time;
	int rx,ry,rr,rs,rl,ru,rd,mx,my,mz;
        float  tx,ty,tz,fx,fy,fz,c1f,s1f,c2f,s2f,c3f,s3f,c4f,s4f,sc;

	u_short	*fbp,*offy; 
	// Set the address of fbp&offy in framecode
	fbp=(u_short *)(&(framecode[1][0]));
	offy=(u_short *)&(((u_int *)(&(framecode[2][0])))[1]);
 	/*   DevInit();  */
        {
		sceDmaReset(1);
		sceGsResetPath();
        }
	/* GS_ES4, NTSC, INTERLACE */
	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FIELD);

        /* Pad Set */
        sceSifInitRpc(0);
        while(sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
                            0, NULL) < 0){
            printf("Can't load module sio2man\n");
        }
        while(sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
                            0, NULL) < 0){
            printf("Can't load module padman\n");
        }
	/*  Vsync & Ckeck ODD/EVEN:  oddeven = !sceGsSyncV(0); */
        {
          VSync();
	  oddeven = !((DGET_GS_CSR() >> 13) & 0x1); // FIELD(EVEN:0 ODD:1)
	}
	// open PAD Device
	scePadInit(0);
	scePadPortOpen(0, 0, pad_dma_buf);
	while(scePadSetMainMode(0, 0, 1, 3) !=1)	{ VSync(); }	// Analog Mode 
	while(scePadEnterPressMode(0, 0) !=1)		{ VSync(); }	// Analog Switch Mode 


	/* Make geo Data */
 	fractalvp();		// Make fractal vp(sphere)
	fractalgeo(7962);	// Make fractal geo
 	makeclut();		// Make clut
	maketexture();		// make unified texture buffer
	MakeObjectMesh();    	// Object data maiking (Geo -> Object) 
	
	matrix_init();		// matrix initialize

	printf("%08x\n", (u_int)*GS_CSR); /* Gs version */
	// Static DMA Tag Link
	pp=staticlinktagarea;
	// gsinitp, teximagep, texclutp, gsframep, vumtxp, vuobjdtp : DMA Packet Link pointer
	gsinitp=pp;
	pp=make_direct_packet(pp, (u_int *)initcode, ISN); 	/* Set DMA Packet Link for GS:init */
	/* DMA: ch=VIF1 mode=SourceChain VIFmode=DIRECT (GS_Initial_code) */
	dma_packet_VIF1_Tag(gsinitp);				/* DMA start */

	/* Make Static DMA Packet Link for dynamiclink*/
	{
		/* Set DMA Packet Link for GS:frame code	(GS:Path2) */
		gsframep=pp;
		pp=make_direct_packet(pp , (u_int *)framecode,  FSN); 
		/* Set DMA Packet Link for MPG 			(VU1) */
		vumpgp=pp;
		{
			*(pp++)=DMAcall | 0;			// DMAtag RET
        		*(pp++)=(u_int)&My_dma_mpg_start;	// Null
    			*(pp++)=0;				// 
        		*(pp++)=0;				// 
 			// BASE(120), OFFSET(240), OUTBUF0(0x25a), OUTBUF1(0x320)
			pp=make_vubuf_preset_pack(pp, 120, 240, 0x25a ,0x0320);  // VU MEM Dual I/O BUFFER Pointor
			pp=make_pack_ret(pp, VIFNOP, VIFNOP);
		}
		/* Set DMA Packet Link for teximage 		(GS:Path2) */
		for(i=0;i<6;i++){
			teximagepa[i]=pp;
			pp=make_image_packet(pp, (u_int *)(&(tgeo[i].tex8[0][0])), TX8DP, TXWW/64, 19, 0, 0, HTW,HTH);
		}
		
		/* Set DMA Packet Link for clut  		(GS:Path2) */
		texclutp=pp;
		pp=make_image_packet(pp, (u_int *)cltbf, CLTDP, 1, 0, 0, 0, CLTWDTH,CLTWDTH);
		/* Set DMA Packet Link for Matrix 		(VU1) */
		vumtxp=pp;
		pp=make_matrix_packet(pp, (float *)plmtx, (float *)lcmtx, (float *)llmtx, (float *)clpm);

		//printf("Packet %x, %x, %d,  %d, \n",(u_int)staticlinktagarea,(u_int)pp,(u_int)(pp-staticlinktagarea),(u_int)(pp-staticlinktagarea)/4);
		/* Set DMA Packet Link for object	(VU1 )*/
		/* Mesh Data */
		for(k=0;k<6;k++){
			vuobjdtpa[k]=pp;
			{
				int	prim,max;
				prim= 4 | (1<<3) | (1<<4); 	// prim = (TRISTRIP:4|(IIP:1<<3)|(TME:1<<4) 
				for(i=0;i<MESH/2;i++){
	    		   		for(j=0;j<(MESH/2);j+=16){
						if(j+16>(MESH/2)) max=(MESH/2)-j+1; else max=17;
						pp=make_stripmesh_mtx_packet(pp, prim, &(vgeo[k].vct[i][j]), &(vgeo[k].vct[i+1][j]), max);
	    				}
				}
				pp=make_pack_ret(pp, VIFNOP, SCE_VIF1_SET_FLUSH(0));
			}
		}
		vuobjdtp=pp;
		{
			for(k=0;k<6;k++){
				int	prim,max;
				prim= 1 | (1<<3) | (0<<4) | (1<<7); 	// prim = (TRISTRIP:4|(IIP:1<<3)|(TME:1<<4) 
				for(i=0;i<MESH/2;i++){
	    		   		for(j=0;j<(MESH/2);j+=16){
						if(j+16>(MESH/2)) max=(MESH/2)-j+1; else max=17;
						pp=make_stripmesh_mtx_packet(pp, prim, &(vgeo[k].vct[i][j]), &(vgeo[k].vct[i+1][j]), max);
	    				}
				}
				prim= 2 | (1<<3) | (0<<4) | (1<<7); 	// prim = (TRISTRIP:4|(IIP:1<<3)|(TME:1<<4) 
				for(i=0;i<MESH/2+1;i++){
	    		   		for(j=0;j<(MESH/2);j+=16){
						if(j+16>(MESH/2)) max=(MESH/2)-j+1; else max=17;
						pp=make_stripmesh_inline_packet(pp, prim, &(vgeo[k].vct[i][j]), max);
	    				}
				}
			}
			pp=make_pack_ret(pp, VIFNOP, SCE_VIF1_SET_FLUSH(0));
		}
        	//printf("Packet %x, %x, %d,  %d, \n",(u_int)staticlinktagarea,(u_int)pp,(u_int)(pp-staticlinktagarea),(u_int)(pp-staticlinktagarea)/4);
	}
	// parameter reset

	oddeven = !sceGsSyncV(0);
	frame = 0;
	i =j = 0;
	fx=fy=fz=1.0f;
	tx=ty=tz=0.0f;
	tgl0=tgl1=mx=my=mz=rr=rs=rd=rl=ru=rx=ry=0;

	while(1){
		timer_start = GetCOUNT();
		//  GS RUNTIME MODE 
		GS_runtime_preset(oddeven,tgl1);
		// read PAD
		pad_read_dualshock2();
		if(padRtop-padRtopB==1) tgl0 ^=1;
		if(padLtop-padLtopB==1) tgl1 ^=1;
		padRtopB=padRtop;
		padLtopB=padLtop;
		// Parameter reset
		{
			if(padRtop==1)	{rx=0; ry=0; }
			if(padLtop==1)	{rr=0; rs=0;}
			if(padsel==1)	{fx=fy=fz=0.0f; }
			if(padstr==1)	{tx=ty=tz=0.0f; }
			if(frame>=4000) frame -=4000;
		}
		// Parameter set
		{
			// Object
			rx+=((padARud+4)>>3);
			ry+=((padARlr+4)>>3);
			// Light
			rs-=((padALud+4)>>5);
			rr-=((padALlr+4)>>5);
			// View
			fx+=2.0f*(padALr-padALl);
			fy+=2.0f*(padALd-padALu);
			fz+=2.0f*(padALfu-padALfd);
			// object
			tx+=2.0f*(padARr-padARl);
			ty+=2.0f*(padARd-padARu);
			tz+=2.0f*(padARfu-padARfd);
		}
		/* set World Local Matrix */
		c1f=cosf((float)(rx)*PIf*1.0f/4000.0f);
		s1f=sinf((float)(rx)*PIf*1.0f/4000.0f);
		c2f=cosf((float)(ry)*PIf*1.0f/4000.0f);
		s2f=sinf((float)(ry)*PIf*1.0f/4000.0f);
		wlmtx[0][0]=c2f;	wlmtx[1][0]=-s1f*s2f;	wlmtx[2][0]=-c1f*s2f;	wlmtx[3][0]= tx;
		wlmtx[0][1]=0.0f;	wlmtx[1][1]=c1f;	wlmtx[2][1]=-s1f;	wlmtx[3][1]= ty; 
		wlmtx[0][2]=s2f;	wlmtx[1][2]=c2f*s1f;	wlmtx[2][2]=c2f*c1f;	wlmtx[3][2]= tz; 
		wlmtx[0][3]=0.0f;	wlmtx[1][3]=0.0f;	wlmtx[2][3]=0.0f;	wlmtx[3][3]= 1.0f; 
		// Set Local/Light(L1)
		c3f=cosf((float)(rr)*PIf*1.0f/4000.0f);
		s3f=sinf((float)(rr)*PIf*1.0f/4000.0f);
		c4f=cosf((float)(rs)*PIf*1.0f/4000.0f);
		s4f=sinf((float)(rs)*PIf*1.0f/4000.0f);
		ldir[0]= -s3f*c4f;
		ldir[1]= -s4f;
		ldir[2]= -c3f*c4f;
		ldir[3]= 0.0f;
		set_llmtx(ldir, wlmtx, llmtx);
		// Set Screen/world Matrix from view&target
		view[0]= fx;
		view[1]= fy;
		view[2]= fz;
		target[0]= 0.0f;
		target[1]= 0.0f;
		target[2]= 10000.0f;
		view2swmtx(view,target,swmtx);
		// Set Prim(GS)/WorldMatrix->Prim(GS)/LocalMatrix
		psmtx2plmtx(psmtx, swmtx, pwmtx, wlmtx, plmtx);

		/* Set Dual Buffer & Interlace Offset (framecode) */
		if((oddeven)==0){
			*fbp=DBUF1;
			*offy=(2048-DISPHEIGHT/2)*16+0;
		}else{
			*fbp=DBUF0;
			*offy=(2048-DISPHEIGHT/2)*16+0;
		}

                {  // Dynamic DMA link
			pp=dynamiclinktagarea;
			*(pp++)=DMAcall | 0;    *(pp++)=(u_int)gsframep;		// DMAtag Call Gsframe
			*(pp++)=0; *(pp++)=0;						// 
			*(pp++)=DMAcall | 0;	*(pp++)=(u_int)vumpgp;			// DMAtag Call MPG
			*(pp++)=0; *(pp++)=0;						// 
			*(pp++)=DMAcall | 0;	*(pp++)=(u_int)texclutp;		// DMAtag Call Clut
			*(pp++)=0; *(pp++)=0;						// 
			*(pp++)=DMAcall | 0; 	*(pp++)=(u_int)vumtxp;			// DMAtag Call Matrix
			*(pp++)=0; *(pp++)=0;
						
			for(i=0; i<6;i++)
			{								//
				*(pp++)=DMAcall | 0;	*(pp++)=(u_int)teximagepa[i];	// DMAtag Call Tex8
				*(pp++)=0; *(pp++)=0;					// 
				*(pp++)=DMAcall | 0;	*(pp++)=(u_int)vuobjdtpa[i];	// DMAtag Call DATA
				*(pp++)=0; *(pp++)=0;					//
			}

	       		*(pp++)=DMAend  |  0;	*(pp++)=0;				// DMAtag End QWC=0
			*(pp++)=0; *(pp++)=0;						//
 			FlushCache(0);  
		
	        	sceGsSyncPath(0, 0);
       			dma_packet_VIF1_Tag(dynamiclinktagarea); // DMA start

	        	sceGsSyncPath(0, 0);
	        	
			if(tgl0==0){
				sc=2.0f;
				pp=dynamiclinktagarea;
				wlmtx[0][0]*=sc;	wlmtx[1][0]*=sc;	wlmtx[2][0]*=sc;
				wlmtx[0][1]*=sc;	wlmtx[1][1]*=sc;	wlmtx[2][1]*=sc;
				wlmtx[0][2]*=sc;	wlmtx[1][2]*=sc;	wlmtx[2][2]*=sc;
				psmtx2plmtx(psmtx, swmtx, pwmtx, wlmtx, plmtx);	
					
				*(pp++)=DMAcall | 0; 	*(pp++)=(u_int)vumtxp;		// DMAtag Call Matrix
				*(pp++)=0; *(pp++)=0;

				*(pp++)=DMAcall | 0;	*(pp++)=(u_int)vuobjdtp;	// DMAtag Call DATA
				*(pp++)=0; *(pp++)=0;					//

	       			*(pp++)=DMAend  |  0;	*(pp++)=0;			// DMAtag End QWC=0
				*(pp++)=0; *(pp++)=0;					//
 				FlushCache(0);  		
	        		sceGsSyncPath(0, 0);
       				dma_packet_VIF1_Tag(dynamiclinktagarea); 		// DMA start
       			}
						
	        }
		frame++;
		sceGsSyncPath(0, 0);
		timer_end = GetCOUNT();
                time=timer_end - timer_start;
		// printf("%d\n",time);

	        /*  Vsync & Ckeck ODD/EVEN:  oddeven = !sceGsSyncV(0); */
                {
                  //VSync();
		  //oddeven = ! ((DGET_GS_CSR() >> 13) & 0x1);   // FIELD(EVEN:0 ODD:1)
		  oddeven = !sceGsSyncV(0);
		}

	}
}

void pad_read_dualshock2(void)
{
	        if(scePadRead(0, 0, rdata) > 0){
			  paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
			  padARlr=rdata[4]-128; padARud=rdata[5]-128;
			  padALlr=rdata[6]-128; padALud=rdata[7]-128;
			  padALr=rdata[8]; 	padALl=rdata[9];
			  padALu=rdata[10]; 	padALd=rdata[11];
			  padARr=rdata[13]; 	padARl=rdata[15];
			  padARu=rdata[12]; 	padARd=rdata[14];
			  padALfu=rdata[16]; 	padALfd=rdata[18];
			  padARfu=rdata[17]; 	padARfd=rdata[19];
	        }
		else{
			  paddata=0;
			  padARlr=0; 	padARud=0;
			  padALl=0; 	padALr=0;
			  padALu=0; 	padALd=0;
			  padARl=0; 	padARr=0;
			  padARu=0; 	padARd=0;
			  padALfu=0; 	padALfd=0;
			  padARfu=0; 	padARfd=0;
		}
		if(paddata & SCE_PADRdown) { padRd=1;  } else{ padRd=0;  }
		if(paddata & SCE_PADRup)   { padRu=1;  } else{ padRu=0;  }
		if(paddata & SCE_PADRright){ padRr=1;  } else{ padRr=0;  }
		if(paddata & SCE_PADRleft) { padRl=1;  } else{ padRl=0;  }
		if(paddata & SCE_PADLdown) { padLd=1;  } else{ padLd=0;  }
		if(paddata & SCE_PADLup)   { padLu=1;  } else{ padLu=0;  }
		if(paddata & SCE_PADLright){ padLr=1;  } else{ padLr=0;  }
		if(paddata & SCE_PADLleft) { padLl=1;  } else{ padLl=0;  }
		if(paddata & SCE_PADL1)    { padLfu=1; } else{ padLfu=0; }
		if(paddata & SCE_PADL2)    { padLfd=1; } else{ padLfd=0; }
		if(paddata & SCE_PADR1)    { padRfu=1; } else{ padRfu=0; }
		if(paddata & SCE_PADR2)    { padRfd=1; } else{ padRfd=0; }
		if(paddata & SCE_PADstart) { padstr=1; } else{ padstr=0; }
		if(paddata & SCE_PADselect){ padsel=1; } else{ padsel=0; }
		if(paddata & SCE_PADj)     { padRtop=1;} else{ padRtop=0;}
		if(paddata & SCE_PADi)     { padLtop=1;} else{ padLtop=0;}
}


