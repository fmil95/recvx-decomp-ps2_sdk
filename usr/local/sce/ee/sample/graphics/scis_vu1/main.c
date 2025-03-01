/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                       - scissoring VU1 -
 *
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : main.c
 *
 *       Version      Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.5.0        Sep,22,1999     thatake     1st Version
 */

#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <libdev.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <libvu0.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <libpad.h>

#include "defines.h"
#include "main.h"

#define OBJNUM  1
#define PACKNUM 50    // max 80

int main(){
        int    i;
    	int    odev;
    	u_int  frame;
	int    obj_switch = 0;
	int    obj_toggle = 0;
	int    micro_switch = 0;
	int    micro_toggle = 0;
	int    count = 0;
        float  delta = 0.7f / 180.0f * PI;
	u_int  *pObj;
	int objNum = OBJNUM;
	int packNum = PACKNUM;
	Vu1DmaPacket dmavu1;
	Vu1EnvPacket *vu1env;
	GifDmaPacket dmagif;

	sceSifInitRpc(0);
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
			    0, NULL) < 0){
	    printf("Can't load module sio2man\n");
	}
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
			    0, NULL) < 0){
	    printf("Can't load module padman\n");
	}

	sceVu0UnitMatrix(unit_mat);

	// --- set packetting buffer address (double buffer) ---
	InitVu1DmaPacket(&dmavu1, (Vu1ObjPacket*)0x70000000,
			          (Vu1ObjPacket*)0x70002000);

	sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
	sceGsResetPath();

	// --- set double buffer (frame buffer) ---
	sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224, 
                SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
        *(u_long *)&db.clear0.rgbaq = 
                SCE_GS_SET_RGBAQ(0x10, 0x10, 0x20, 0x80, 0x3f800000);
        *(u_long *)&db.clear1.rgbaq =
                SCE_GS_SET_RGBAQ(0x10, 0x10, 0x20, 0x80, 0x3f800000);

	// --- set load image 1---
	sceGsSetDefLoadImage(&gs_limage, 
			     6720,
			     IMAGE_SIZE / 64,
			     0,
			     0, 0,
			     IMAGE_SIZE, IMAGE_SIZE);

	FlushCache(0);

	// --- load texture to GS local memory ---
	sceGsExecLoadImage(&gs_limage, (u_long128*) &My_clip128);
	sceGsSyncPath(0, 0);

	FlushCache(0);

	// --- set texture env 1 ---
	sceGsSetDefTexEnv(&texenv.gs_tex, 0, 6720, 
			  IMAGE_SIZE / 64, SCE_GS_PSMCT32,
			  7, 7, // 2**8=256,2**7=128
			  0,0,0,0,1);
	SCE_GIF_CLEAR_TAG(&texenv.giftag); 
	texenv.giftag.NLOOP = 9;
	texenv.giftag.EOP = 1;
	texenv.giftag.NREG = 1;
	texenv.giftag.REGS0 = 0xe;
	sceGsSetDefAlphaEnv(&texenv.gs_alpha, 0);
	*(u_long *) &texenv.gs_alpha.alpha1 = SCE_GS_SET_ALPHA(0,2,0,1,0);
	*(u_long *) &texenv.gs_test =  SCE_GS_SET_TEST(0,0,0,0,0,0,3,0x3);
	texenv.gs_test1addr = (long)SCE_GS_TEST_1;
	*(u_long*)&texenv.gs_tex.clamp1 = SCE_GS_SET_CLAMP(0,0,0,0,0,0); 
	// *(u_long*)&texenv.gs_tex.tex11 = SCE_GS_SET_TEX1(1,0,1,1,0,0,0);
	FlushCache(0);
	sceGsPutDrawEnv(&texenv.giftag);
	sceGsSyncPath(0, 0);

	// --- kick dma to send VU1 micro-code ---
        *D1_QWC  = 0x00;
	*D1_TADR = (u_int)&My_scissor_code & 0x0fffffff;
	*D_STAT = 2;
	FlushCache(0);
	*D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
	sceGsSyncPath(0, 0);

	// --- open pad ---
	scePadInit(0);
	scePadPortOpen(0, 0, pad_dma_buf);

       frame = 0;
       odev = !sceGsSyncV(0);

       while(1){
	 // --- read pad ---
	   if(scePadRead(0, 0, rdata) > 0){
	       paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
	   }
	   else{
	       paddata = 0;
	   }

	 // --- object rotate & change view point ---
         if(paddata & SCE_PADRdown){ 
	   clip_volume[1] += 5; if(clip_volume[1]>2047) clip_volume[1]=2047;}
	 if(paddata & SCE_PADRup){ 
	   clip_volume[1] -= 5; if(clip_volume[1]<50) clip_volume[1]=50;}
	 if(paddata & SCE_PADRright){ 
	   clip_volume[0] += 5; if(clip_volume[0]>2047) clip_volume[0]=2047;}
	 if(paddata & SCE_PADRleft){ 
	   clip_volume[0] -= 5; if(clip_volume[0]<40) clip_volume[0]=40;}
	 if(paddata & SCE_PADR1){
	   clip_volume[0]-=5; clip_volume[1]-=4; 
	   if(clip_volume[0]<50 || clip_volume[1]<40)
	     {clip_volume[0]=50;clip_volume[1]=40;} }
	 if(paddata & SCE_PADR2){
	   clip_volume[0]+=5; clip_volume[1]+=4;
	   if(clip_volume[0]>2047 || clip_volume[1]>2047)
	     {clip_volume[0]=2047;clip_volume[1]=2047;} }

	 if(paddata & SCE_PADLdown){
	   camera_rot[0] += delta; 
	   if(camera_rot[0]>PI*0.48) camera_rot[0] = 0.48f*PI; }
	 if(paddata & SCE_PADLup){
	   camera_rot[0] -= delta; 
	   if(camera_rot[0]<-PI*0.48) camera_rot[0] = -0.48f*PI; }
	 if(paddata & SCE_PADLright){
	   camera_rot[1] -= delta; 
	   if(camera_rot[1]<-PI) camera_rot[1] += 2.0f*PI; }
	 if(paddata & SCE_PADLleft){
	   camera_rot[1] += delta; 
	   if(camera_rot[1]>PI) camera_rot[1] -= 2.0f*PI; }
	 if(paddata & SCE_PADL1){
	   camera_p[2] -= 0.1; if(camera_p[2]<-150) camera_p[2] = -150; }
	 if(paddata & SCE_PADL2){
	   camera_p[2] += 0.1; if(camera_p[2]>0) camera_p[2] = 0; }
	 if(!obj_toggle && (paddata & SCE_PADselect)){
	   obj_switch = !obj_switch; obj_toggle = 1;
	 }else if(!(paddata & SCE_PADselect)){obj_toggle = 0;}
	 if(!micro_toggle && (paddata & SCE_PADstart)){
	   micro_switch = !micro_switch; micro_toggle = 1;
	 }else if(!(paddata & SCE_PADstart)){micro_toggle = 0;}

	 FlushCache(0);
	 sceGsSwapDBuff(&db, frame);
	 sceGsSyncPath(0, 0);

         // get pre-calculation data structure
	 vu1env = GetVu1EnvHead(&dmavu1);

         // view_screen&view_clip matrix
	 SetViewScreenClipMatrix(view_screen, view_clip, 
				 vu1env->Clip_screen,
				 512.0f, 1.0f, 0.47f, 
				 2048.0f, 2048.0f, 1.0f, 16777000.0f, 
				 5.0f, 65536.0f);

	 // color&normal matrix setting
	 sceVu0NormalLightMatrix(vu1env->Normal_light, light0, light1, light2);
	 sceVu0LightColorMatrix(vu1env->Light_color, color0, color1, 
				color2, ambient);
	 // world -> view matrix
	 sceVu0RotCameraMatrix(world_view,
			       camera_p, camera_zd, camera_yd, camera_rot);
	 // world -> screen matrix
	 sceVu0MulMatrix(vu1env->World_screen, view_screen, world_view);

	 // world -> clip matrix
	 sceVu0MulMatrix(vu1env->World_clip, view_clip, world_view);

	 // --- kick dma to pre-calculation on VU1 ---
	 *D1_QWC  = 0x00;
	 *D1_TADR = (u_int)CloseVu1EnvPacket(&dmavu1);
	 *D_STAT = 2;
	 *D1_CHCR = 1 |(1<<2) |(0<<4) |(0<<5) |(1<<6) |(0<<7) |(1<<8);

	 // --- select draw object ---
	 pObj = &My_torus_start;
	 if(obj_switch==1) pObj = &My_grid_start;

         if(micro_switch==0){
	   (&My_cube_scissor_tag)[1] = 0x300ec000;
	   (&My_torus_scissor_tag)[1] = 0x300ec000;
	   (&My_grid_scissor_tag)[1] = 0x300ec000;
	 }else{
	   (&My_cube_scissor_tag)[1] = 0x30054000;
	   (&My_torus_scissor_tag)[1] = 0x30054000;
	   (&My_grid_scissor_tag)[1] = 0x30054000;
	 }

	 for(i=0; i<objNum; i++){
	   count = i%packNum;
	   /*
	   obj_trans[0] = -15.0 + (i%3)*15;
	   obj_trans[1] = -15.0 + (i/3)*15 - 45*(i/9);
	   obj_trans[2] = -15.0 + 15*(i/9);
	   */
	   // --- Add object to packet ---
	   AddVu1ObjPacket(&dmavu1, pObj, obj_rot, obj_trans);

	   if(i==objNum-1 || count==packNum-1){
	     FlushCache(0);
	     sceGsSyncPath(0, 0);

	     // --- kick dma to draw object ---
	     *D1_QWC  = 0x00;
	     *D1_TADR = (u_int)CloseVu1ObjPacket(&dmavu1)|0x80000000;
	     *D_STAT = 2;
	     *D1_CHCR = 1 | (1<<2) | (0<<4) | (0<<5) 
	       | (1<<6) | (0<<7) | (1<<8);
	   }
	 }     

	 // --- make clipping boundary line and kick dma ---
	 SetGifClipLine(&dmagif);
         FlushCache(0);
	 sceGsSyncPath(0, 0);
	 *D2_QWC  = 0x00;
	 *D2_TADR = (u_int)dmagif.buf;
	 *D_STAT = 2;
	 *D2_CHCR = 1 | (1<<2) | (0<<4) | (0<<5) | (1<<6) | (0<<7) | (1<<8);
         sceGsSyncPath(0, 0);

	 sceGsSetHalfOffset((frame&1)? &db.draw1:&db.draw0, 2048,2048,odev);

         frame++;
	 odev = !sceGsSyncV(0);

       }

       printf("finish!\n");
       return 0;

}














