/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - spot light -
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
 *       1.3.0        Dec,8,1999     thatake      1st version
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
#include <libpad.h>
#include <sifdev.h>
#include <sifrpc.h>

#include "defines.h"
#include "main.h"

#define OBJNUM  12
#define PACKNUM 50    // 80 is max

void DrawLightPoint(Vu1DmaPacket *dmavu1);

int main(){
        int    i;
    	int    odev;
    	u_int  frame;
	int    obj_switch = 0;
	int    obj_toggle = 0;
	int    count = 0;
        float  delta = 1.0f / 180.0f * PI;
	u_int  *pObj;
	int objNum = OBJNUM;
	int packNum = PACKNUM;
	Vu1DmaPacket dmavu1;
	Vu1EnvPacket *vu1env;

	sceVu0UnitMatrix(unit_mat);

	for(i=0;i<14;i++){
          ((float*)&My_cube_vertex)[4*i+0]*=0.05;
          ((float*)&My_cube_vertex)[4*i+1]*=0.05;
          ((float*)&My_cube_vertex)[4*i+2]*=0.05;
        }

	sceSifInitRpc(0);
        while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
                            0, NULL) < 0){
            printf("Can't load module sio2man\n");
        }
        while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
                         0, NULL) < 0){
            printf("Can't load module padman\n");
        }

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

	// --- load texture to GS local memory ---
	FlushCache(0);
	sceGsExecLoadImage(&gs_limage, (u_long128*) &My_texture256);
	sceGsSyncPath(0, 0);

	// --- set texture env 1 ---
	sceGsSetDefTexEnv(&texenv.gs_tex, 0, 6720, 
			  IMAGE_SIZE / 64, SCE_GS_PSMCT32,
			  8, 8, // 2**8=256,2**7=128
			  0,0,0,0,1);
	SCE_GIF_CLEAR_TAG(&texenv.giftag); 
	texenv.giftag.NLOOP = 9;
	texenv.giftag.EOP = 1;
	texenv.giftag.NREG = 1;
	texenv.giftag.REGS0 = 0xe;
	sceGsSetDefAlphaEnv(&texenv.gs_alpha, 0);
	*(u_long *) &texenv.gs_alpha.alpha1 = SCE_GS_SET_ALPHA(0,1,0,1,0);
	*(u_long *) &texenv.gs_test =  SCE_GS_SET_TEST(0,0,0,0,0,0,3,0x3);
	texenv.gs_test1addr = (long)SCE_GS_TEST_1;
	*(u_long*)&texenv.gs_tex.clamp1 = SCE_GS_SET_CLAMP(0,0,0,0,0,0); 
	*(u_long*)&texenv.gs_tex.tex11 = SCE_GS_SET_TEX1(1,0,1,1,0,0,0);
	FlushCache(0);
	sceGsPutDrawEnv(&texenv.giftag);
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
         }else{ paddata = 0; }

	 // --- object rotate & change view point ---
         if(paddata & SCE_PADRdown){ plight0[2]-=0.3;}
         if(paddata & SCE_PADRup){ plight0[2]+=0.3;}
         if(paddata & SCE_PADRright){ plight0[0]+=0.3;}
         if(paddata & SCE_PADRleft){ plight0[0]-=0.3;}
         if(paddata & SCE_PADR1){ 
	   plight0[1]+=0.15;
	   if(plight0[1]>-12)plight0[1]-=0.15; }
         if(paddata & SCE_PADR2){ plight0[1]-=0.15;}
         if(paddata & SCE_PADLdown){ 
	   light0_rot[0]-=delta;
	   if(light0_rot[0]<-PI*0.48) light0_rot[0]+=delta; 
	 }
         if(paddata & SCE_PADLup){ 
	   light0_rot[0]+=delta;
	   if(light0_rot[0]>PI*0.48) light0_rot[0]-=delta; 
	 }
         if(paddata & SCE_PADLright){ 
	   light0_rot[2]-=delta;
	   if(light0_rot[2]<-PI*0.48) light0_rot[2]+=delta; }
         if(paddata & SCE_PADLleft){ 
	   light0_rot[2]+=delta;
	   if(light0_rot[2]>PI*0.48) light0_rot[2]-=delta; }
         if(paddata & SCE_PADL1){ 
	   light1_rot[0] += delta; light2_rot[0] -= delta;
	   if(light1_rot[0]>PI)light1_rot[0]-=2.0*PI;
	   if(light2_rot[0]<-PI)light2_rot[0]+=2.0*PI; }
         if(paddata & SCE_PADL2){ 
	   light1_rot[0] -= delta; light2_rot[0] += delta;
	   if(light1_rot[0]<-PI)light1_rot[0]+=2.0*PI;
	   if(light2_rot[0]>PI)light2_rot[0]-=2.0*PI; }
	 if(!obj_toggle && (paddata & SCE_PADselect)){
	   obj_switch = !obj_switch; obj_toggle = 1;
	 }else if(!(paddata & SCE_PADselect)){obj_toggle = 0;}

         sceVu0RotMatrix(work, unit_mat, light0_rot);
	 sceVu0ApplyMatrix(light0_dir, work, light0);
         sceVu0RotMatrix(work, unit_mat, light1_rot);
	 sceVu0ApplyMatrix(light1_dir, work, light1);
         sceVu0RotMatrix(work, unit_mat, light2_rot);
	 sceVu0ApplyMatrix(light2_dir, work, light2);

	 FlushCache(0);
	 sceGsSwapDBuff(&db, frame);
	 sceGsSyncPath(0, 0);

	 // --- kick dma to send VU1 micro-code ---
         *D1_QWC  = 0x00;
	 *D1_TADR = (u_int)&My_spot_code & 0x0fffffff;
	 *D_STAT = 2;
	 FlushCache(0);
	 *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
	 sceGsSyncPath(0, 0);

         // get pre-calculation data structure
	 vu1env = GetVu1EnvHead(&dmavu1);

         // view_screen&view_clip matrix
	 SetViewScreenClipMatrix(view_screen, view_clip, 
				 clip_screen,
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
	 pObj = &My_grid100_start;
	 if(obj_switch==1) pObj = &My_grid_start;

	 for(i=0; i<objNum; i++){
	   count = i%packNum;

	   obj_trans[0] = -10.0f + (i%3)*10;
	   obj_trans[1] = 3.0f;
	   obj_trans[2] = -10.0f + 10*(i/3);

	   // --- Add object to packet ---
	   AddVu1ObjPacket(&dmavu1, pObj, obj_rot, obj_trans);
	 }

	 // --- Add torus to packet ---
	 AddVu1ObjPacket(&dmavu1, &My_torus_start, torus_rot, torus_trans);

	 sceGsSyncPath(0, 0);
	 // --- kick dma to draw object ---
	 *D1_QWC  = 0x00;
	 *D1_TADR = (u_int)CloseVu1ObjPacket(&dmavu1)|0x80000000;
	 *D_STAT = 2;
	 FlushCache(0);
	 *D1_CHCR = 1 | (1<<2) | (0<<4) | (0<<5) 
	   | (1<<6) | (0<<7) | (1<<8);


	 DrawLightPoint(&dmavu1);
	 
	 sceGsSyncPath(0, 0);
	 sceGsSetHalfOffset((frame&1)? &db.draw1:&db.draw0, 2048,2048,odev);
	 
         frame++;
	 odev = !sceGsSyncV(0);
	 
       }

       printf("finish!\n");
       return 0;

}


void DrawLightPoint(Vu1DmaPacket *dmavu1)
{
    u_int  *pObj;

    sceGsSyncPath(0, 0);
    // --- kick dma to send VU1 basic micro-code ---
    *D1_QWC  = 0x00;
    *D1_TADR = (u_int)&My_basic_code & 0x0fffffff;
    *D_STAT = 2;
    FlushCache(0);
    *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
    
    pObj = &My_cube_start;
    AddVu1ObjPacket(dmavu1, pObj, light0_rot, plight0);
    AddVu1ObjPacket(dmavu1, pObj, light1_rot, plight1);
    AddVu1ObjPacket(dmavu1, pObj, light2_rot, plight2);
    
    sceGsSyncPath(0, 0);          
    // --- kick dma to draw object ---
    *D1_QWC  = 0x00;
    *D1_TADR = (u_int)CloseVu1ObjPacket(dmavu1)|0x80000000;
    *D_STAT = 2;
    FlushCache(0);
    *D1_CHCR = 1 | (1<<2) | (0<<4) | (0<<5) 
      | (1<<6) | (0<<7) | (1<<8);

}













