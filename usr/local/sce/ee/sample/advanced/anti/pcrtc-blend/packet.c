/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
/*
*
*	VU & GS test data generator
*
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
#include "base.h"
// fractalobj.c

u_int *make_mpg_packet(u_int *p, u_int *mpg);
u_int *make_image_packet(u_int *p, u_int *imp, int txp, int txw, int txmd, int ofx, int ofy, int tw,int th);
u_int *make_direct_packet(u_int *p, u_int *packet, u_int num);
u_int *make_matrix_packet(u_int *p, float *plmx, float *lcmx, float *llmx, float *clp);
u_int *make_stripmesh_mtx_packet(u_int *p, int prim, PVECTOR *meshupper, PVECTOR *meshbottom, int meshn);
u_int *make_stripmesh_inline_packet(u_int *p, int prim, PVECTOR *mesh, int meshn);

u_int *make_mpg_pack(u_int *p, u_int *mpg);
u_int *make_image_pack(u_int *p, u_int *imp, int txp, int txw, int txmd, int ofx, int ofy, int tw,int th);
u_int *make_direct_pack(u_int *p, u_int *packet, u_int num);
u_int *make_matrix_pack(u_int *p, float *plmx, float *lcmx, float *llmx, float *clpmx);
//u_int *make_object_pack(u_int *p, int k);

u_int *make_pack_ret(u_int *p, u_int vifcode0, u_int vifcode1);
u_int *make_vubuf_preset_pack(u_int *p, int base, int offset, int outbuf0, int outbuf1);

u_int *make_pack_ret(u_int *p, u_int vifcode0, u_int vifcode1)
{
        *(p++)=DMAret | 0;			// DMAtag RET
        *(p++)=0;				// Null
        *(p++)=vifcode0;			// VIFcode
        *(p++)=vifcode1;			// VIFcode
        return(p);
}

u_int *make_mpg_pack(u_int *p, u_int *mpg)
{
        *(p++)=DMAcall | 0;			// DMAtag RET
        *(p++)=(u_int)mpg;			// Null
        *(p++)=0;				// 
        *(p++)=0;				// 
        return(p);
}

u_int *make_vubuf_preset_pack(u_int *p, int base, int offset, int outbuf0, int outbuf1)
{
	*(p++)=(DMAcnt| 1);   //  DMAtag CNT QWC=0 
	*(p++)=0;
	*(p++)=SCE_VIF1_SET_BASE(base,0);     				// Input dual buffer base
	*(p++)=SCE_VIF1_SET_OFFSET(offset,0);				// Input dual buffer offset
	// Clip code & Output dual buffer set Using 16bit Unpack mode
	*(p++)=SCE_VIF1_SET_STCYCL(4,4,0);
	*(p++)=SCE_VIF1_SET_UNPACK((20 | 1<<14), 1, V4_16, 0);  	// USN=1 Unsigned 16bit  @20 
	*(p++)= 0x8000 <<16 | 0x0000;					// ADCBIT,0 for GS Clip
	*(p++)= outbuf1 <<16 | outbuf0;  				// outbuf1,outbuf0 Pointer for VU1
        return(p);	
}

u_int *make_mpg_packet(u_int *p, u_int *mpg)
{
	p=make_mpg_pack(p,mpg);
	// BASE(120), OFFSET(240), OUTBUF0(0x25a), OUTBUF1(0x320)
	p=make_vubuf_preset_pack(p, 120, 240, 0x25a ,0x0320);  // VU MEM Dual I/O BUFFER Pointor
	p=make_pack_ret(p, VIFNOP, VIFNOP);
	return(p);
}

u_int *make_image_pack(u_int *p, u_int *imp, int txp, int txw, int txmd, int ofx, int ofy, int tw,int th)
{
int nw,pw,md;
	
	md= txmd & 0x7;
	pw=8;
	// Check Texture mode 
	if(md==0) pw=8;
	if(md==1) pw=6;
	if(md==2) pw=4;
	if(md==3) pw=2;
	if(md==4) pw=1;
	// Ajust length
	nw = (tw*th*pw)/32;
	if((tw*th*pw)%32 != 0) nw+=1;
			
        *(p++)=DMAcnt | (7);			// DMAtag CNT QWC=7
        *(p++)=0;				// Null
        *(p++)=0;				// VIFnop
        *(p++)=SCE_VIF1_SET_DIRECTHL(7, 0);  	// DIRECT

	// nloop=5, eop=0, pre=0, prim=0, flg=0:PACKET, nreg=1: NREG(0x0e:A_D)
	// TEXFLUSH 
	// BITBLTBUF SBP SBW SPSM DBP DBW DPSM 
	// TRXPOS SSAX SSAY DSAX DSAY DIR:(LtoR,UtoD) 
	// TRXREG RRW RRH 
	// TRXDIR XDIR=0:(HtoL) 
	// nloop=1024, eop=1, pre=0, prim=0, flg=2:IMAGE, nreg=0: 

 	*(((u_long *)p)++)=SCE_GIF_SET_TAG(5,  0, 0, 0, SCE_GIF_PACKED, 1);	*(((u_long *)p)++)=0x0000000e;
 	*(((u_long *)p)++)=0;							*(((u_long *)p)++)=SCE_GS_TEXFLUSH;
 	*(((u_long *)p)++)=SCE_GS_SET_BITBLTBUF(0, 0, 0, txp, txw, txmd);	*(((u_long *)p)++)=SCE_GS_BITBLTBUF;
 	*(((u_long *)p)++)=SCE_GS_SET_TRXPOS(0, 0, ofx, ofy, 0);		*(((u_long *)p)++)=SCE_GS_TRXPOS;
 	*(((u_long *)p)++)=SCE_GS_SET_TRXREG(tw,th);				*(((u_long *)p)++)=SCE_GS_TRXREG;
 	*(((u_long *)p)++)=SCE_GS_SET_TRXDIR(0);				*(((u_long *)p)++)=SCE_GS_TRXDIR;
 	*(((u_long *)p)++)=SCE_GIF_SET_TAG(nw,1,0,0,SCE_GIF_IMAGE,0);		*(((u_long *)p)++)=0x00000000;

        *(p++)=DMAref | (nw);			// DMAtag REF QWC=nw
        *(p++)=(u_int)imp;			// DMAtag ADDR=imp
        *(p++)=0;
        *(p++)=SCE_VIF1_SET_DIRECTHL(nw, 0);	// DIRECT(nw)
        return(p);
}

u_int *make_image_packet(u_int *p, u_int *imp, int txp, int txw, int txmd, int ofx, int ofy, int tw,int th)
{
	p=make_image_pack(p, imp, txp, txw, txmd, ofx, ofy, tw, th);
	p=make_pack_ret(p, VIFNOP, VIFNOP);
	return(p);
}


u_int *make_direct_pack(u_int *p, u_int *packet, u_int num)
{
        *(p++)=DMAref | (num+1);              		// DMAtag REF QWC=num+1
        *(p++)=(u_int)packet;    			// DMAtag ADDR=packet
        *(p++)=0;
        *(p++)=SCE_VIF1_SET_DIRECT(num+1, 0);		// DIRECT num+1
	return(p);
}

u_int *make_direct_packet(u_int *p, u_int *packet, u_int num)
{
	p=make_direct_pack(p, packet, num);
	p=make_pack_ret(p, SCE_VIF1_SET_STMASK(0), 0);	// STMASK Clear
	return(p);
}

u_int *make_single_matrix_pack(u_int *p, float *mtx, int vumemad)
{
        *(p++)=DMAref | 4;				// DMAtag REF QWC=4
        *(p++)=(u_int)mtx;				// DMAtag ADDR=mtx
        *(p++)=SCE_VIF1_SET_STCYCL(4,4,0);
	*(p++)=SCE_VIF1_SET_UNPACK(vumemad, 4, V4_32, 0);	// 
	return(p);
}

u_int *make_matrix_pack(u_int *p, float *plmx, float *lcmx, float *llmx, float *clp)
{
	*(p++)=(DMAcnt| 0);   				//  DMAtag CNT QWC=18 
	*(p++)=0;              				//	
	*(p++)=SCE_VIF1_SET_STCYCL(4,4,0);
	*(p++)=SCE_VIF1_SET_STMOD(0,0);	
	p=make_single_matrix_pack(p, plmx, 4);
	p=make_single_matrix_pack(p, lcmx, 8);
	p=make_single_matrix_pack(p, llmx, 12);
	p=make_single_matrix_pack(p, clp, 16);
	return(p);
	
}
u_int *make_matrix_pack_all(u_int *p, float *plmx, float *lcmx, float *llmx, float *clp)
{
	*(p++)=(DMAcnt| 0);   				//  DMAtag CNT QWC=18 
	*(p++)=0;              				//	
	*(p++)=SCE_VIF1_SET_STCYCL(4,4,0);
	*(p++)=SCE_VIF1_SET_STMOD(0,0);	
        *(p++)=DMAref | 4;				// DMAtag REF QWC=4
        *(p++)=(u_int)plmx;				// DMAtag ADDR=plmtx
        *(p++)=0;
	*(p++)=SCE_VIF1_SET_UNPACK(4, 4, V4_32, 0);	// Prim/Local
        *(p++)=DMAref | 4; 				// DMAtag REF QWC=4
        *(p++)=(u_int)lcmx;				// DMAtag ADDR=mtxlc
        *(p++)=0;
	*(p++)=SCE_VIF1_SET_UNPACK(8, 4, V4_32, 0);	// Light/Color
        *(p++)=DMAref | 4;                  		// DMAtag REF QWC=4
        *(p++)=(u_int)llmx;    				// DMAtag ADDR=mtxll
        *(p++)=0;
	*(p++)=SCE_VIF1_SET_UNPACK(12, 4, V4_32, 0);	// Light/Local	
        *(p++)=DMAref | 4;                  		// DMAtag REF QWC=4
        *(p++)=(u_int)clp;    				// DMAtag ADDR=maxc
        *(p++)=0;
	*(p++)=SCE_VIF1_SET_UNPACK(16, 4, V4_32, 0);  	// Clip Area
	return(p);
}	

u_int *make_matrix_packet(u_int *p, float *plmx, float *lcmx, float *llmx, float *clp)
{
	p=make_matrix_pack(p, plmx, lcmx, llmx, clp);
	p=make_pack_ret(p, SCE_VIF1_SET_STMASK(0), 0);
        return(p);
}	

u_int *make_stripmesh_mtx_packet(u_int *p, int prim, PVECTOR *meshupper, PVECTOR *meshbottom, int meshn)
{
	int	MN,DN;
	MN=meshn;
	DN=meshn*4;
	*(p++)=(DMAcnt| 1 );   				// DMAtag CNT QWC=1
	*(p++)=0;
	*(p++)=SCE_VIF1_SET_STCYCL(4,4,0);
	*(p++)=SCE_VIF1_SET_UNPACKR(0, 1, V4_32, 0);		// GIFtag 
	// nloop=MN*2, eop=1, pre=1, prim , flg=0:PACKET, nreg=3:
	*(((u_long *)p)++) = SCE_GIF_SET_TAG(MN*2, 1,1, prim ,0 , 3);
	*(((u_long *)p)++) = (u_long)0x412;   		// NREG: XYZF2(4), RGBAQ(1), ST(2)
	// StripUpper(ALL) REF 
        *(p++)=DMAref | DN;                  		// DMAtag REF QWC=DN
        *(p++)=(u_int)(meshupper);    			// DMAtag ADDR=&mesh[i][j]
        *(p++)=SCE_VIF1_SET_STCYCL(4,8,0);		//
	*(p++)=SCE_VIF1_SET_UNPACKR(1, DN, V4_32, 0);  	// 
	// StripBottom(ALL) REF
        *(p++)=DMAref | DN;                  		// DMAtag REF QWC=DN
        *(p++)=(u_int)(meshbottom);    			// DMAtag ADDR=&mesh[i+1][j]
        *(p++)=SCE_VIF1_SET_STCYCL(4,8,0);		//
	*(p++)=SCE_VIF1_SET_UNPACKR(5, DN, V4_32, 0);  	// 

	*(p++)=(DMAcnt| 0 );   				// DMAtag CNT QWC=0
	*(p++)=0;
	*(p++)=SCE_VIF1_SET_STCYCL(4,4,0);
	*(p++)=SCE_VIF1_SET_MSCAL(0, 0);
	return(p);
}

u_int *make_stripmesh_inline_packet(u_int *p, int prim, PVECTOR *mesh, int meshn)
{
	int	MN,DN;
	MN=meshn;
	DN=meshn*4;
	*(p++)=(DMAcnt| 1 );   				// DMAtag CNT QWC=1
	*(p++)=0;
	*(p++)=SCE_VIF1_SET_STCYCL(4,4,0);
	*(p++)=SCE_VIF1_SET_UNPACKR(0, 1, V4_32, 0);		// GIFtag 
	// nloop=MN, eop=1, pre=1, prim , flg=0:PACKET, nreg=3:
	*(((u_long *)p)++) = SCE_GIF_SET_TAG(MN, 1,1, prim ,0 , 3);
	*(((u_long *)p)++) = (u_long)0x412;   		// NREG: XYZF2(4), RGBAQ(1), ST(2)
	// Strip(ALL) REF 
        *(p++)=DMAref | DN;                  		// DMAtag REF QWC=DN
        *(p++)=(u_int)(mesh);    			// DMAtag ADDR=&mesh[i][j]
        *(p++)=SCE_VIF1_SET_STCYCL(4,4,0);		//
	*(p++)=SCE_VIF1_SET_UNPACKR(1, DN, V4_32, 0);  	// 

	*(p++)=(DMAcnt| 0 );   				// DMAtag CNT QWC=0
	*(p++)=0;
	*(p++)=SCE_VIF1_SET_STCYCL(4,4,0);
	*(p++)=SCE_VIF1_SET_MSCAL(0, 0);
	return(p);
}
