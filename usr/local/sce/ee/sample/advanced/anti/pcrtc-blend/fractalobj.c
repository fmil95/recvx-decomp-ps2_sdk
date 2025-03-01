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

#define RAD 	(0.5773502f)

void	makeclut(void);
void	fractalgeo(int seed);
void	maketexture(void);
void	fractalvp(void);
void	MakeObjectMesh(void);

int 	clut[256][3];
u_int	cltbf[256] __attribute__((aligned(16)));	// DMA clut buffer

PVECTORM 	vgeo[6];
TEX8M		tgeo[6];

/************* Fractal Texture **********************************/
int	fpp;				/* geo parameter	*/
int	geo_top[HLM+1][HLM+1] __attribute__((aligned(16)));
int	geo_btm[HLM+1][HLM+1] __attribute__((aligned(16)));
int	geo_sph[HLM+1][HLM*4] __attribute__((aligned(16)));
int	gtmph[HLM+1][HLM+1] __attribute__((aligned(16)));
int	gtmpx[HLM+1][HLM+1] __attribute__((aligned(16)));
/************* RC OCTAHEDRON ************************************/
SVECTOR v_top[LVN+1][LVN+1] __attribute__((aligned(16)));
SVECTOR v_btm[LVN+1][LVN+1] __attribute__((aligned(16)));
SVECTOR sph_vn[LVN+1][LVN*4] __attribute__((aligned(16)));
SVECTOR tmph[LVN+1][LVN+1] __attribute__((aligned(16)));
SVECTOR tmpx[LVN+1][LVN+1] __attribute__((aligned(16)));

int sd,*SEED;

void VectorNormalSS(SVECTOR *vi, SVECTOR *vo)
{
float abs;
        abs= sqrtf(vi->vx*vi->vx+vi->vy*vi->vy+vi->vz*vi->vz);
        vo->vx = vi->vx/abs;
        vo->vy = vi->vy/abs;
        vo->vz = vi->vz/abs;
}

void hgendatavp(SVECTOR *out,SVECTOR *dt1,SVECTOR *dt2,SVECTOR *dt3,SVECTOR *dt4,int hstep,int i)
{
  SVECTOR tmp;
	tmp.vx=(dt1->vx+dt2->vx+dt3->vx+dt4->vx)/4;
	tmp.vy=(dt1->vy+dt2->vy+dt3->vy+dt4->vy)/4;
	tmp.vz=(dt1->vz+dt2->vz+dt3->vz+dt4->vz)/4;
	VectorNormalSS(&tmp,out); 
}
void vgendatavp(SVECTOR *out,SVECTOR *dt1,SVECTOR *dt2,SVECTOR *dt3,SVECTOR *dt4,int hstep,int i)
{
  SVECTOR tmp;
	tmp.vx=(dt1->vx+dt2->vx+dt3->vx+dt4->vx)/4;
	tmp.vy=(dt1->vy+dt2->vy+dt3->vy+dt4->vy)/4;
	tmp.vz=(dt1->vz+dt2->vz+dt3->vz+dt4->vz)/4;
	VectorNormalSS(&tmp,out);  
}
void cgendatavp(SVECTOR *out,SVECTOR *dt1,SVECTOR *dt2,SVECTOR *dt3,SVECTOR *dt4,int hstep,int i)
{
  SVECTOR tmp;
	tmp.vx=(dt1->vx+dt2->vx+dt3->vx+dt4->vx)/4;
	tmp.vy=(dt1->vy+dt2->vy+dt3->vy+dt4->vy)/4;
	tmp.vz=(dt1->vz+dt2->vz+dt3->vz+dt4->vz)/4;
	VectorNormalSS(&tmp,out);
}

void MakeObjectMesh(void)
{
	int	i,j,k,x,y;
	for(k=0;k<6;k++){
		if(k<4){
		  for(j=0;j<LVN+1;j++){
	    	  	for(i=0;i<LVN+1;i++){
				tmph[j][i]= sph_vn[j][(i+k*LVN)%(4*LVN)];
	    	  	}
		  }
		}
		if(k==4){
		  for(j=0;j<LVN+1;j++){
	    	  	for(i=0;i<LVN+1;i++){
				tmph[j][i]= v_top[j][i];
	    	  	}
		  }
		}
		if(k==5){
		  for(j=0;j<LVN+1;j++){
	    	  	for(i=0;i<LVN+1;i++){
				tmph[j][i]= v_btm[j][i];
	    	  	}
		  }
		}
		for(y=0;y<(LVN+1);y++){
	    	  for(x=0;x<(LVN+1);x++){
			vgeo[k].vct[y][x].vx=1000.f*tmph[y][x].vx;
			vgeo[k].vct[y][x].vy=1000.f*tmph[y][x].vy;
			vgeo[k].vct[y][x].vz=1000.f*tmph[y][x].vz;
			vgeo[k].vct[y][x].vw=1.0f;
			vgeo[k].vct[y][x].nx=tmph[y][x].vx;
			vgeo[k].vct[y][x].ny=tmph[y][x].vy;
			vgeo[k].vct[y][x].nz=tmph[y][x].vz;
			vgeo[k].vct[y][x].nw=1.0f;
			
			vgeo[k].vct[y][x].u=(0.5f+((float)x*LM/(MESH)))/(1<<M);
			vgeo[k].vct[y][x].v=(0.5f+((float)y*LM/(MESH)))/(1<<M);
			vgeo[k].vct[y][x].n0=1.0f;
			vgeo[k].vct[y][x].n1=1.0f;
			vgeo[k].vct[y][x].r=128.0f;
			vgeo[k].vct[y][x].g=128.0f;
			vgeo[k].vct[y][x].b=128.0f;
			vgeo[k].vct[y][x].a=128.0f;
		  }
		}
	}	
}
void maketexture(void)
{
int i,j,k,x,y;
	for(k=0;k<6;k++){
	   if(k<4){
		for(j=0;j<(HLM+1);j++){
	    	  for(i=0;i<(HLM+1);i++){
	    		gtmph[j][i]=geo_sph[j][(i+k*HLM)%(4*HLM)];
	    	  }
		}
	   }
	   if(k==4){
		for(j=0;j<(HLM+1);j++){
	    	  for(i=0;i<(HLM+1);i++){
	    		gtmph[j][i]=geo_top[j][i];
	    	  }
		}
	   }
	   if(k==5){
		for(j=0;j<(HLM+1);j++){
	    	  for(i=0;i<(HLM+1);i++){
	    		gtmph[j][i]=geo_btm[j][i];
	    	  }
		}
	   }
	   for(y=0;y<(HTH);y++){
	     for(x=0;x<(HTW);x++){
			tgeo[k].tex8[y][x]=gtmph[(y)%(HLM+1)][(x)%(HLM+1)]>>4;
	     }
	   }
	}
}

void haddvp(u_int i)
{
 u_int x,y,hstep,step,lx,rx,uy,dy,cx,cy,j,ccx;
 SVECTOR  *dt1,*dt2,*dt3,*dt4,out;
 step = LVN >> i;
 hstep = step >> 1;
 for(j=0;j<4;j++){
  for (x = hstep; x < LVN; x += step){
    for (y = step; y < (LVN); y += step){
	cx=x+j*LVN; cy=y;
	lx = (cx-hstep)%(4*LVN);
	rx = (cx+hstep)%(4*LVN);
	uy = (cy-hstep);
	dy = (cy+hstep);
	
	dt1 = &sph_vn[uy][cx];
	dt2 = &sph_vn[dy][cx];
	dt3 = &sph_vn[cy][lx];
	dt4 = &sph_vn[cy][rx];
	ccx=cx%LVN;
	if(uy==0){					// top connection
		if(j==0) dt1 = &v_top[LVN][ccx];	// bottom
		if(j==1) dt1 = &v_top[(LVN-ccx)][LVN];	// right
		if(j==2) dt1 = &v_top[0][(LVN-ccx)];	// top
		if(j==3) dt1 = &v_top[ccx][0];		// left
	}
	if(dy==LVN){					// bottom connection
		if(j==0) dt2 = &v_btm[0][ccx];		// top
		if(j==1) dt2 = &v_btm[ccx][LVN];	// right
		if(j==2) dt2 = &v_btm[LVN][LVN-ccx];	// bottom
		if(j==3) dt2 = &v_btm[LVN-ccx][0];	// left
	}
	hgendatavp(&out,dt1,dt2,dt3,dt4,hstep,i);
	sph_vn[cy][cx]= out;
    } 
  }
 } 
}

void vaddvp(u_int i)
{
 u_int x,y,hstep,step,lx,rx,uy,dy,cx,cy,j,ccx;
 SVECTOR  *dt1,*dt2,*dt3,*dt4,out;
 step = LVN >> i;
 hstep = step >> 1;
 for(j=0;j<4;j++){
  for (x = 0; x < LVN; x +=step){
    for (y = hstep; y < (LVN); y +=step){
	cx=x+j*LVN; cy=y;
	lx = (cx-hstep)%(4*LVN);
	rx = (cx+hstep)%(4*LVN);
	uy = (cy-hstep);
	dy = (cy+hstep);

	dt1 = &sph_vn[uy][cx];
	dt2 = &sph_vn[dy][cx];
	dt3 = &sph_vn[cy][lx];
	dt4 = &sph_vn[cy][rx];
	ccx=cx%LVN;	
	if(uy==0){
		if(j==0) dt1 = &v_top[LVN][ccx];
		if(j==1) dt1 = &v_top[(LVN-ccx)][LVN];
		if(j==2) dt1 = &v_top[0][(LVN-ccx)];
		if(j==3) dt1 = &v_top[ccx][0];
	}
	if(dy==LVN){
		if(j==0) dt2 = &v_btm[0][ccx];
		if(j==1) dt2 = &v_btm[ccx][LVN];
		if(j==2) dt2 = &v_btm[LVN][(LVN-ccx)];
		if(j==3) dt2 = &v_btm[(LVN-ccx)][0];
	}
	vgendatavp(&out,dt1,dt2,dt3,dt4,hstep,i);
	sph_vn[cy][cx]= out;
    } 
  }
 } 
}
void caddvp(u_int i)
{
 u_int x,y,hstep,step,lx,rx,uy,dy,cx,cy,j;
 SVECTOR  *dt1,*dt2,*dt3,*dt4,out;
 step = LVN >> i;
 hstep = step >> 1;
 for(j=0;j<4;j++){
  for (x = hstep; x < (LVN); x +=step){
    for (y = hstep; y < (LVN); y +=step){
	cx=x+j*LVN; cy=y;
	lx = (cx-hstep)%(4*LVN);
	rx = (cx+hstep)%(4*LVN);
	uy = (cy-hstep);
	dy = (cy+hstep);
	dt1 = &sph_vn[uy][lx];
	dt2 = &sph_vn[dy][lx];
	dt3 = &sph_vn[uy][rx];
	dt4 = &sph_vn[dy][rx];
	lx=lx%LVN;
	rx=rx%LVN; if(rx==0) rx=LVN;
	if(uy==0){
		if(j==0) dt1 = &v_top[LVN][lx];
		if(j==1) dt1 = &v_top[(LVN-lx)][LVN];
		if(j==2) dt1 = &v_top[0][(LVN-lx)];
		if(j==3) dt1 = &v_top[lx][0];
	}
	if(dy==LVN){
		if(j==0) dt2 = &v_btm[0][lx];
		if(j==1) dt2 = &v_btm[lx][LVN];
		if(j==2) dt2 = &v_btm[LVN][(LVN-lx)];
		if(j==3) dt2 = &v_btm[(LVN-lx)][0];
	}
	if(uy==0){
		if(j==0) dt3 = &v_top[LVN][rx];
		if(j==1) dt3 = &v_top[(LVN-rx)][LVN];
		if(j==2) dt3 = &v_top[0][(LVN-rx)];
		if(j==3) dt3 = &v_top[rx][0];
	}
	if(dy==LVN){
		if(j==0) dt4 = &v_btm[0][rx];
		if(j==1) dt4 = &v_btm[rx][LVN];
		if(j==2) dt4 = &v_btm[LVN][(LVN-rx)];
		if(j==3) dt4 = &v_btm[(LVN-rx)][0];
	}

	cgendatavp(&out,dt1,dt2,dt3,dt4,hstep,i);
	sph_vn[cy][cx]= out; 
    } 
  }
 }
}
void haddvptop(u_int i)
{
 u_int x,y,hstep,step,lx,rx,uy,dy,cx,cy;
 SVECTOR  *dt1,*dt2,*dt3,*dt4,out;
  step = LVN >> i;
  hstep = step >> 1;
  for (x = hstep; x < (LVN); x += step){
    for (y = 0; y < (LVN+1); y += step){
	cx=x; cy=y;
	lx = (cx-hstep);
	rx = (cx+hstep);
	uy = (cy-hstep);
	dy = (cy+hstep);
	if(y==0)   dt1 = &sph_vn[(LVN-uy)%LVN][(LVN-cx)%LVN+2*LVN];
	else       dt1 = &v_top[uy][cx];
	if(y==LVN) dt2 = &sph_vn[dy%LVN][cx];
	else       dt2 = &v_top[dy][cx];
	dt3 = &v_top[cy][lx];
	dt4 = &v_top[cy][rx];
	
	
	hgendatavp(&out,dt1,dt2,dt3,dt4,hstep,i);
	v_top[cy][cx]= out;
    } 
  } 
}
void vaddvptop(u_int i)
{
 u_int x,y,hstep,step,lx,rx,uy,dy,cx,cy;
 SVECTOR  *dt1,*dt2,*dt3,*dt4,out;
  step = LVN >> i;
  hstep = step >> 1;
  for (x = 0; x < (LVN+1); x +=step){
    for (y = hstep; y < (LVN); y +=step){
	cx=x; cy=y;
	lx = (cx-hstep);
	rx = (cx+hstep);
	uy = (cy-hstep);
	dy = (cy+hstep);
	dt1 = &v_top[uy][cx];
	dt2 = &v_top[dy][cx];
	if(x==0)   dt3 = &sph_vn[(LVN-lx)%LVN][cy+3*LVN];
	else       dt3 = &v_top[cy][lx];
	if(x==LVN) dt4 = &sph_vn[rx%LVN][(LVN-cy)%LVN+LVN];
	else       dt4 = &v_top[cy][rx];

	vgendatavp(&out,dt1,dt2,dt3,dt4,hstep,i);
	v_top[cy][cx]= out;
    } 
  } 
}
void caddvptop(u_int i)
{
 u_int x,y,hstep,step,lx,rx,uy,dy,cx,cy;
 SVECTOR  *dt1,*dt2,*dt3,*dt4,out;
 step = LVN >> i;
  hstep = step >> 1;
  for (x = hstep; x < (LVN); x +=step){
    for (y = hstep; y < (LVN); y +=step){
	cx=x; cy=y;
	lx = (cx-hstep);
	rx = (cx+hstep);
	uy = (cy-hstep);
	dy = (cy+hstep);
	
	dt1 = &v_top[uy][lx];
	dt2 = &v_top[dy][lx];
	dt3 = &v_top[uy][rx];
	dt4 = &v_top[dy][rx];

	cgendatavp(&out,dt1,dt2,dt3,dt4,hstep,i);
	v_top[cy][cx]= out; 
    } 
  } 
}

void haddvpbtm(u_int i)
{
 u_int x,y,hstep,step,lx,rx,uy,dy,cx,cy;
 SVECTOR  *dt1,*dt2,*dt3,*dt4,out;
  step = LVN >> i;
  hstep = step >> 1;
  for (x = hstep; x < (LVN); x += step){
    for (y = 0; y < (LVN+1); y += step){
	cx=x; cy=y;
	lx = (cx-hstep);
	rx = (cx+hstep);
	uy = (cy-hstep);
	dy = (cy+hstep);
	if(y==0)   dt1 = &sph_vn[uy%LVN][cx];
	else       dt1 = &v_btm[uy][cx];
	if(y==LVN) dt2 = &sph_vn[(LVN-dy)%LVN][(LVN-cx)%LVN+2*LVN];
	else       dt2 = &v_btm[dy][cx];
	dt3 = &v_btm[cy][lx];
	dt4 = &v_btm[cy][rx];
	
	hgendatavp(&out,dt1,dt2,dt3,dt4,hstep,i);
	v_btm[cy][cx]= out;
    } 
  } 
}

void vaddvpbtm(u_int i)
{
 u_int x,y,hstep,step,lx,rx,uy,dy,cx,cy;
 SVECTOR  *dt1,*dt2,*dt3,*dt4,out;
  step = LVN >> i;
  hstep = step >> 1;
  for (x = 0; x < (LVN+1); x +=step){
    for (y = hstep; y < (LVN); y +=step){
	cx=x; cy=y;
	lx = (cx-hstep);
	rx = (cx+hstep);
	uy = (cy-hstep);
	dy = (cy+hstep);
	dt1 = &v_btm[uy][cx];
	dt2 = &v_btm[dy][cx];
	if(x==0)   dt3 = &sph_vn[lx%LVN][(LVN-cy)+3*LVN];
	else       dt3 = &v_btm[cy][lx];
	if(x==LVN) dt4 = &sph_vn[(LVN-rx)%LVN][cy%LVN+LVN];
	else       dt4 = &v_btm[cy][rx];

	vgendatavp(&out,dt1,dt2,dt3,dt4,hstep,i);
	v_btm[cy][cx]= out;
    } 
  } 
}

void caddvpbtm(u_int i)
{
 u_int x,y,hstep,step,lx,rx,uy,dy,cx,cy;
 SVECTOR  *dt1,*dt2,*dt3,*dt4,out;
 step = LVN >> i;
 hstep = step >> 1;
  for (x = hstep; x < (LVN); x +=step){
    for (y = hstep; y < (LVN); y +=step){
	cx=x; cy=y;
	lx = (cx-hstep);
	rx = (cx+hstep);
	uy = (cy-hstep);
	dy = (cy+hstep);
	dt1 = &v_btm[uy][lx];
	dt2 = &v_btm[dy][lx];
	dt3 = &v_btm[uy][rx];
	dt4 = &v_btm[dy][rx];

	cgendatavp(&out,dt1,dt2,dt3,dt4,hstep,i);
	v_btm[cy][cx]= out; 
    } 
  } 
}

void haddsph(u_int i)
{
 u_int x,y,step,hstep,lx,rx,uy,dy,cx,cy,j,ccx;
 int	dt1,dt2,dt3,dt4,q,d;
 step = HLM >> i;
 hstep = step >> 1;
 for(j=0 ; j < 4 ;j++){
  for (x = hstep; x < HLM; x += step){
    for (y = step; y < (HLM); y += step){
	cx=x+j*HLM; cy=y;
	lx = (cx-hstep) % (4*HLM);
	rx = (cx+hstep) % (4*HLM);
	uy = (cy-hstep);
	dy = (cy+hstep);
	dt1 = geo_sph[uy][cx];
	dt2 = geo_sph[dy][cx];
	dt3 = geo_sph[cy][lx];
	dt4 = geo_sph[cy][rx];
	ccx=cx%HLM;
	if(uy == 0){
		if(j==0) dt1 = geo_top[HLM][ccx];
		if(j==1) dt1 = geo_top[(HLM-ccx)][HLM];
		if(j==2) dt1 = geo_top[0][(HLM-ccx)];
		if(j==3) dt1 = geo_top[ccx][0];
	}
	if(dy==HLM){
		if(j==0) dt2 = geo_btm[0][ccx];
		if(j==1) dt2 = geo_btm[ccx][HLM];
		if(j==2) dt2 = geo_btm[HLM][(HLM-ccx)];
		if(j==3) dt2 = geo_btm[(HLM-ccx)][0];
	}
	else		
	{  
 		d = hstep*32*(1+i*fpp);
		q = ((int)rand())%8192;
		q = 4096-q;
		geo_sph[cy][cx] = (dt1+dt2+dt3+dt4)/4+((q* d)>>12); 
	}
    } 
  } 
 }
}

void vaddsph(u_int i)
{
u_int x,y,step,hstep,lx,rx,uy,dy,cx,cy,j,ccx;
int	dt1,dt2,dt3,dt4,q,d;
 step = HLM >> i;
 hstep = step >> 1;
 for(j=0 ; j < 4 ;j++){	
  for (x = 0; x < HLM; x +=step){
    for (y = hstep; y < HLM; y +=step){
	cx=x+j*HLM; cy=y;
	lx = (cx-hstep) % (4*HLM);
	rx = (cx+hstep) % (4*HLM);
	uy = (cy-hstep);
	dy = (cy+hstep);
	dt1 = geo_sph[uy][cx];
	dt2 = geo_sph[dy][cx];
	dt3 = geo_sph[cy][lx];
	dt4 = geo_sph[cy][rx];
	ccx=cx%HLM;	
	if(uy==0){
		if(j==0) dt1 = geo_top[HLM][ccx];
		if(j==1) dt1 = geo_top[(HLM-ccx)][HLM];
		if(j==2) dt1 = geo_top[0][(HLM-ccx)];
		if(j==3) dt1 = geo_top[ccx][0];
	}
	if(dy==HLM){
		if(j==0) dt2 = geo_btm[0][ccx];
		if(j==1) dt2 = geo_btm[ccx][HLM];
		if(j==2) dt2 = geo_btm[HLM][(HLM-ccx)];
		if(j==3) dt2 = geo_btm[(HLM-ccx)][0];
	}
	{   
		d = hstep*32*(1+i*fpp); 
 		q = ((int)rand())%8192;
		q = 4096-q;
		geo_sph[cy][cx] = (dt1+dt2+dt3+dt4)/4+((q*d)>>12);
	}
    } 
  } 
 }
}
void caddsph(u_int i)
{
u_int x,y,step,hstep,lx,rx,uy,dy,cx,cy,j;
int	dt1,dt2,dt3,dt4,q,d;
 step = HLM >> i;
 hstep = step >> 1;
 for(j=0 ; j < 4 ;j++){	
  for (x = hstep; x < HLM; x +=step){
    for (y = hstep; y < HLM; y +=step){
	cx=x+j*HLM; cy=y;
	lx = (cx-hstep) % (4*HLM);
	rx = (cx+hstep) % (4*HLM);
	uy = (cy-hstep);
	dy = (cy+hstep);
	dt1 = geo_sph[uy][lx];
	dt2 = geo_sph[dy][lx];
	dt3 = geo_sph[uy][rx];
	dt4 = geo_sph[dy][rx];
	lx=lx%HLM;
	rx=rx%HLM; if(rx==0) rx=HLM;
	if(uy==0){
		if(j==0) dt1 = geo_top[HLM][lx];
		if(j==1) dt1 = geo_top[(HLM-lx)][HLM];
		if(j==2) dt1 = geo_top[0][(HLM-lx)];
		if(j==3) dt1 = geo_top[lx][0];
	}
	if(dy==HLM){
		if(j==0) dt2 = geo_btm[0][lx];
		if(j==1) dt2 = geo_btm[lx][HLM];
		if(j==2) dt2 = geo_btm[HLM][(HLM-lx)];
		if(j==3) dt2 = geo_btm[(HLM-lx)][0];
	}
	if(uy==0){
		if(j==0) dt3 = geo_top[HLM][rx];
		if(j==1) dt3 = geo_top[(HLM-rx)][HLM];
		if(j==2) dt3 = geo_top[0][(HLM-rx)];
		if(j==3) dt3 = geo_top[rx][0];
	}
	if(dy==HLM){
		if(j==0) dt4 = geo_btm[0][rx];
		if(j==1) dt4 = geo_btm[rx][HLM];
		if(j==2) dt4 = geo_btm[HLM][(HLM-rx)];
		if(j==3) dt4 = geo_btm[(HLM-rx)][0];
	}
	{   
 		d = hstep*32*(1+i*fpp);
		q = ((int)rand())%8192;
		q = 4096-q;
		geo_sph[cy][cx] = (dt1+dt2+dt3+dt4)/4+(( q * d )>>12);
	}
     } 
  } 
 }
}


void haddsphtop(u_int i)
{
 u_int x,y,step,hstep,lx,rx,uy,dy,cx,cy;
 int	dt1,dt2,dt3,dt4,q,d;
 step = HLM >> i;
 hstep = step >> 1;
  for (x = hstep; x < HLM; x += step){
    for (y = 0; y < (HLM+1); y += step){
	cx=x; cy=y;
	lx = (cx-hstep);
	rx = (cx+hstep);
	uy = (cy-hstep);
	dy = (cy+hstep);
	dt1 = geo_top[uy][cx];
	dt2 = geo_top[dy][cx];
	dt3 = geo_top[cy][lx];
	dt4 = geo_top[cy][rx];
	if(y==0)   dt1 = geo_sph[(HLM-uy)%HLM][(HLM-cx)%HLM+2*HLM];
	if(y==HLM) dt2 = geo_sph[dy%HLM][cx];
	{   
 		d = hstep*32*(1+i*fpp);
		q = ((int)rand())%8192;
		q = 4096-q;
		geo_top[cy][cx] = (dt1+dt2+dt3+dt4)/4+((q* d)>>12); 
	}
    } 
  } 
}
void vaddsphtop(u_int i)
{
u_int x,y,step,hstep,lx,rx,uy,dy,cx,cy;
int	dt1,dt2,dt3,dt4,q,d;
 step = HLM >> i;
 hstep = step >> 1;
  for (x = 0; x < (HLM+1); x +=step){
    for (y = hstep; y < (HLM+1); y +=step){
	cx=x; cy=y;
	lx = (cx-hstep);
	rx = (cx+hstep);
	uy = (cy-hstep);
	dy = (cy+hstep);
	dt1 = geo_top[uy][cx];
	dt2 = geo_top[dy][cx];
	dt3 = geo_top[cy][lx];
	dt4 = geo_top[cy][rx];
	if(x==0)   dt3 = geo_sph[(HLM-lx)%HLM][cy+3*HLM];
	if(x==HLM) dt4 = geo_sph[rx%HLM][(HLM-cy)%HLM+HLM];
	{   
 		d = hstep*32*(1+i*fpp); 
		q = ((int)rand())%8192;
		q = 4096-q;
		geo_top[cy][cx] = (dt1+dt2+dt3+dt4)/4+((q*d)>>12);
	}
    } 
  } 
}
void caddsphtop(u_int i)
{
u_int x,y,step,hstep,lx,rx,uy,dy,cx,cy;
int	dt1,dt2,dt3,dt4,q,d;
 step = HLM >> i;
 hstep = step >> 1;
  for (x = hstep; x < HLM+1; x +=step){
    for (y = hstep; y < HLM+1; y +=step){
	cx=x; cy=y;
	lx = (cx-hstep);
	rx = (cx+hstep);
	uy = (cy-hstep);
	dy = (cy+hstep);
	dt1 = geo_top[uy][lx];
	dt2 = geo_top[dy][lx];
	dt3 = geo_top[uy][rx];
	dt4 = geo_top[dy][rx];
	{   
 		d = hstep*32*(1+i*fpp); 
		q = ((int)rand())%8192;
		q = 4096-q;
		geo_top[cy][cx] = (dt1+dt2+dt3+dt4)/4+(( q * d )>>12);
	}
     } 
  } 
}

void haddsphbtm(u_int i)
{
 u_int x,y,step,hstep,lx,rx,uy,dy,cx,cy;
 int	dt1,dt2,dt3,dt4,q,d;
 step = HLM >> i;
 hstep = step >> 1;
  for (x = hstep; x < HLM; x += step){
    for (y = 0; y < (HLM+1); y += step){
	cx=x; cy=y;
	lx = (cx-hstep);
	rx = (cx+hstep);
	uy = (cy-hstep);
	dy = (cy+hstep);
	dt1 = geo_btm[uy][cx];
	dt2 = geo_btm[dy][cx];
	dt3 = geo_btm[ cy][lx];
	dt4 = geo_btm[ cy][rx];
	if(y==0)   dt1 = geo_sph[uy%HLM][cx];
	if(y==HLM) dt2 = geo_sph[(HLM-dy)%HLM][(HLM-cx)%HLM+2*HLM];

	{   
		d = hstep*32*(1+i*fpp);
		q = ((int)rand())%8192;
		q = 4096-q;
		geo_btm[cy][cx] = (dt1+dt2+dt3+dt4)/4+((q* d)>>12); 
	}
    } 
  } 
}

void vaddsphbtm(u_int i)
{
u_int x,y,step,hstep,lx,rx,uy,dy,cx,cy;
int	dt1,dt2,dt3,dt4,q,d;
 step = HLM >> i;
 hstep = step >> 1;
  for (x = 0; x < (HLM+1); x +=step){
    for (y = hstep; y < (HLM+1); y +=step){
	cx=x; cy=y;
	lx = (cx-hstep);
	rx = (cx+hstep);
	uy = (cy-hstep);
	dy = (cy+hstep);

	dt1 = geo_btm[ uy][cx];
	dt2 = geo_btm[ dy][cx];
	dt3 = geo_btm[ cy][lx];
	dt4 = geo_btm[ cy][rx];
	if(x==0)   dt3 = geo_sph[lx%HLM][(HLM-cy)+3*HLM];
	if(x==HLM) dt4 = geo_sph[(HLM-rx)%HLM][cy%HLM+HLM];
	{  
		d = hstep*32*(1+i*fpp);
		q = ((int)rand())%8192;
		q = 4096-q;
		geo_btm[cy][cx] = (dt1+dt2+dt3+dt4)/4+((q*d)>>12);
	}
    } 
  } 
}

void caddsphbtm(u_int i)
{
u_int x,y,step,hstep,lx,rx,uy,dy,cx,cy;
int	dt1,dt2,dt3,dt4,q,d;
 step = HLM >> i;
 hstep = step >> 1;
  for (x = hstep; x < HLM+1; x +=step){
    for (y = hstep; y < HLM+1; y +=step){
	cx=x; cy=y;
	lx = (cx-hstep);
	rx = (cx+hstep);
	uy = (cy-hstep);
	dy = (cy+hstep);
	dt1 = geo_btm[uy][lx];
	dt2 = geo_btm[dy][lx];
	dt3 = geo_btm[uy][rx];
	dt4 = geo_btm[dy][rx];
	{
 		d = hstep*32*(1+i*fpp); 
		q = ((int)rand())%8192;
		q = 4096-q;
		geo_btm[cy][cx] = (dt1+dt2+dt3+dt4)/4+(( q * d )>>12);
	}
     } 
  } 
}

void fractalgeo(int seed)
{
int i,j,k,max,min,p,r;
int v1,v2,v3,v4,v5,v6,v7,v8;
/* Fractal Geo-Data Generator */
 SEED = &sd;
 srand(seed);
 *SEED = seed<<16;
 v1=v2=v3=v4=v5=v6=v7=v8=2048;
 geo_sph[  0][0+0*HLM]=geo_top[HLM][  0] = v1;	/* 0.5	*/
 geo_sph[  0][0+1*HLM]=geo_top[HLM][HLM] = v2;	/* 0.5	*/
 geo_sph[  0][0+2*HLM]=geo_top[  0][HLM] = v3;	/* 0.5	*/
 geo_sph[  0][0+3*HLM]=geo_top[  0][  0] = v4;	/* 0.5	*/
 geo_sph[HLM][0+0*HLM]=geo_btm[  0][  0] = v5;	/* 0.5	*/
 geo_sph[HLM][0+1*HLM]=geo_btm[  0][HLM] = v6;	/* 0.5	*/
 geo_sph[HLM][0+2*HLM]=geo_btm[HLM][HLM] = v7;	/* 0.5	*/
 geo_sph[HLM][0+3*HLM]=geo_btm[HLM][  0] = v8;	/* 0.5	*/
 for(j=0 ; j < HN ;j++){
	caddsph(j); caddsphtop(j); caddsphbtm(j);
	haddsph(j); haddsphtop(j); haddsphbtm(j);
	vaddsph(j); vaddsphtop(j); vaddsphbtm(j);
 }
  for(i =0; i < HLM+1; i++){
	geo_sph[  0][i+0*HLM]		=geo_top[  HLM][i];	
	geo_sph[  0][i+1*HLM]		=geo_top[HLM-i][HLM];	
	geo_sph[  0][i+2*HLM]		=geo_top[    0][HLM-i];	
	geo_sph[  0][(i+3*HLM)%(4*HLM)]	=geo_top[    i][0];	
	geo_sph[HLM][i+0*HLM]		=geo_btm[    0][i];	
	geo_sph[HLM][i+1*HLM]		=geo_btm[    i][HLM];	
	geo_sph[HLM][i+2*HLM]		=geo_btm[  HLM][HLM-i];	
	geo_sph[HLM][(i+3*HLM)%(4*HLM)]	=geo_btm[HLM-i][0];	
  }

/* Normalize */
  min = 4096;
  max = 0;
  for (k = 0; k < 4; k++){
   for (i =0; i < HLM; i++){
    for (j =0; j < HLM; j++){
	p = geo_sph[i][j+k*HLM];
	if(p < min) min = p;
	if(p > max) max = p;
    }
   }
  }
   for (i =0; i < HLM+1; i++){
    for (j =0; j < HLM+1; j++){
	p = geo_top[i][j];
	if(p < min) min = p;
	if(p > max) max = p;
	p = geo_btm[i][j];
	if(p < min) min = p;
	if(p > max) max = p;
    }
   }
  min += 500;
  r = (1<<24)/(max-min);
  for (k = 0; k < 4; k++){
   for (i = 0; i < (HLM+1); i++){
    for (j =0; j < HLM; j++){
	p = geo_sph[i][j+k*HLM];
	if(p < min) p=min;
	p = (p-min)*r>>12;
	if(p < 0) p = 0;
	if(p > 4095) p= 4095;
	geo_sph[i][j+k*HLM]=p;
    }
   }
  }
   for (i =0; i < (HLM+1); i++){
    for (j =0; j < (HLM+1); j++){
	p = geo_top[i][j];
	if(p < min) p=min;
	p = (p-min)*r>>12;
	if(p < 0) p = 0;
	if(p > 4095) p= 4095;
	geo_top[i][j]=p;
	p = geo_btm[i][j];
	if(p < min) p=min;
	p = (p-min)*r>>12;
	if(p < 0) p = 0;
	if(p > 4095) p= 4095;
	geo_btm[i][j]=p;
    }
   }
 }

void fractalvp(void)
{
u_int i,j;
SVECTOR v0,v1,v2,v3,v4,v5,v6,v7,v8;
/* Fractal Geo-Data Generator */
	v0.vx= 0; v0.vy= 0; v0.vz= 0; v0.vw= 1.0f;
	v1.vx= RAD; v1.vy= RAD; v1.vz= RAD; v1.vw= 1.0f;
	v2.vx=-RAD; v2.vy= RAD; v2.vz= RAD; v2.vw= 1.0f;
	v3.vx=-RAD; v3.vy=-RAD; v3.vz= RAD; v3.vw= 1.0f;
	v4.vx= RAD; v4.vy=-RAD; v4.vz= RAD; v4.vw= 1.0f;
	v5.vx= RAD; v5.vy= RAD; v5.vz=-RAD; v5.vw= 1.0f;
	v6.vx=-RAD; v6.vy= RAD; v6.vz=-RAD; v6.vw= 1.0f;
	v7.vx=-RAD; v7.vy=-RAD; v7.vz=-RAD; v7.vw= 1.0f;
	v8.vx= RAD; v8.vy=-RAD; v8.vz=-RAD; v8.vw= 1.0f;

	sph_vn[0][0+LVN*0]=v_top[LVN][  0]=v1;
	sph_vn[0][0+LVN*1]=v_top[LVN][LVN]=v2;
	sph_vn[0][0+LVN*2]=v_top[  0][LVN]=v3;
	sph_vn[0][0+LVN*3]=v_top[  0][  0]=v4;
	sph_vn[LVN][0+LVN*0]=v_btm[  0][  0]=v5;
	sph_vn[LVN][0+LVN*1]=v_btm[  0][LVN]=v6;
	sph_vn[LVN][0+LVN*2]=v_btm[LVN][LVN]=v7;
	sph_vn[LVN][0+LVN*3]=v_btm[LVN][  0]=v8;

	for(j=0 ; j < VN ;j++){
		caddvptop(j); caddvpbtm(j); caddvp(j);
		haddvptop(j); haddvpbtm(j); haddvp(j); 
		vaddvptop(j); vaddvpbtm(j); vaddvp(j); 
 	}
 	
	for(i =0; i < LVN+1; i++){
		sph_vn[0][i+0*LVN]=v_top[LVN][i];	
		sph_vn[0][i+1*LVN]=v_top[LVN-i][LVN];	
		sph_vn[0][i+2*LVN]=v_top[0][LVN-i];	
		sph_vn[0][(i+3*LVN)%(4*LVN)]=v_top[i][0];	
		sph_vn[LVN][i+0*LVN]=v_btm[0][i];	
		sph_vn[LVN][i+1*LVN]=v_btm[i][LVN];	
		sph_vn[LVN][i+2*LVN]=v_btm[LVN][LVN-i];	
		sph_vn[LVN][(i+3*LVN)%(4*LVN)]=v_btm[LVN-i][0];	
  	}
  	
}


/**********************************************************************************************/
#define SEA	110
#define PLAIN	32
#define PtoD	48
#define StoP	6

void cluttoclutbuf(void){
	int	i,r,g,b,a;
	for(i = 0 ; i < CLTWDTH*CLTWDTH ; i++){
		a=0x80; b=clut[i][2]; g=clut[i][1]; r=clut[i][0]; 
		cltbf[(i/32)*32+((i/16)%2)*8+((i%16)/8)*16+(i%8)]= a*256*256*256+b*256*256+g*256+r;
	}
}

void makeclut(void)
{
int i,p,s,l,pd,sp,a;
	pd= PtoD;
	s = SEA;
	p = PLAIN;
	sp= StoP;
	// sea
	a=0;
	for(i=0;i<s;i++){
		clut[a+i][2]= (96*i/s +100);
		clut[a+i][1]= (96*i/s);
		clut[a+i][0]= (96*i/s);
	}
	// plain
	a+=s;
	//S to P
	for(i=0;i<sp;i++){
		clut[a+i][2]= 196-i*196/sp; 
		clut[a+i][1]= 96+i*24/sp;
		clut[a+i][0]= 96-i*96/sp;
	} 
	a+=sp;
	for(i=0;i<p;i++){
		clut[a+i][2]= (i*32/p ); 
		clut[a+i][1]= (i*24/p+120 );
		clut[a+i][0]= (i*96/p  );
	} 
	//P to D
	a+=p;
	for(i=0;i<pd;i++){
		clut[a+i][2]= (32-i*16/pd); 
		clut[a+i][1]= (144-i*96/pd);
		clut[a+i][0]= (96+i*12/pd);
	} 
	// desert
	a+=pd;
	l=256-a;
	if(l < 1) l= 1;
	for(i=0;i<l;i++){
		clut[a+i][2]= (16+i*64/l); 
		clut[a+i][1]= (48+i*64/l  );
		clut[a+i][0]= (108+i*16/l );
	} 
	cluttoclutbuf();
}
