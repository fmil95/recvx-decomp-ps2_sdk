/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                    - scissoring CORE&VU0  -
 *
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : main.c
 *                     Description : main procedures
 *
 *       Version      Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.5.0        Sep.22.1999     thatake     1st Version
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
#define PACKNUM 50    // 80 is max

int main(){
        int    i, j;
        int    odev;
        u_int  frame;
        int    obj_switch = 0;
        int    obj_toggle = 0;
        int    tag_switch = 0;
        int    tag_toggle = 0;
        int    count = 0;
        float  delta = 0.7f / 180.0f * PI;
        int    objNum = OBJNUM;
        int    packNum = PACKNUM;
        NodeArraySet   scissorflip;
        ScissorPlaneArray  planeset;
        ObjData obj[2];
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

        InitNodeArraySet(&scissorflip);
        InitScissorPlane(&planeset);

        LoadObj(&obj[0], &SampleCubeDataHead);
        LoadObj(&obj[1], &SampleTorus1DataHead);

        sceVu0UnitMatrix(unit_mat);

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

       // --- open pad ---
       FlushCache(0);
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
           camera_p[2] -= 0.2; if(camera_p[2]<-150) camera_p[2] = -150; }
         if(paddata & SCE_PADL2){
           camera_p[2] += 0.2; if(camera_p[2]>2) camera_p[2] = 2; }
         if(!obj_toggle && (paddata & SCE_PADselect)){
           obj_switch = !obj_switch; obj_toggle = 1;
         }else if(!(paddata & SCE_PADselect)){obj_toggle = 0;}
         if(!tag_toggle && (paddata & SCE_PADstart)){
           tag_switch = !tag_switch; tag_toggle = 1;
         }else if(!(paddata & SCE_PADstart)){tag_toggle = 0;}
         
         FlushCache(0);
         sceGsSwapDBuff(&db, frame);
         sceGsSyncPath(0, 0);

         // view_screen&view_clip&clip_screen matrix
         SetViewScreenClipMatrix(view_screen, view_clip, clip_screen,
                                512.0f, 1.0f, 0.47f, 
                                2048.0f, 2048.0f, 1.0f, 16777000.0f, 
                                8.0f, 65536.0f);

         // color&normal matrix setting
         sceVu0NormalLightMatrix(normal_light, light0, light1, light2);
         sceVu0LightColorMatrix(light_color, color0, color1, 
                                color2, ambient);
         sceVu0MulMatrix(local_light, normal_light, local_world);

         // world -> view matrix
         sceVu0RotCameraMatrix(world_view,
                               camera_p, camera_zd, camera_yd, camera_rot);
         // world -> screen matrix
         sceVu0MulMatrix(world_screen, view_screen, world_view);

         // world -> clip matrix
         sceVu0MulMatrix(world_clip, view_clip, world_view);

         for(i=0; i<objNum; i++){
           count = i%packNum;

           InitNodeArraySet(&scissorflip);

           // local -> world (rotate)matrix
           sceVu0RotMatrix(local_world, unit_mat, obj_rot);

           // local -> world (rotate&translate)matrix
           sceVu0TransMatrix(local_world, local_world, obj_trans);

           // local -> clip matrix
           sceVu0MulMatrix(local_clip, world_clip, local_world);
           sceVu0MulMatrix(local_screen, world_screen, local_world);

           for(j=0; j<obj[obj_switch].blockNum; j++){
               MakePacket(&obj[obj_switch], j, tag_switch,
                          &scissorflip, &planeset);
               FlushCache(0);
               sceGsSyncPath(0, 0);
               *D2_QWC  = 0x00;
               *D2_TADR = (u_int)&(obj[obj_switch].pack[j].buf)&0x7fffffff;
               *D_STAT = 4;
               *D2_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
           }
         }

         // --- make clipping boundary line and kick dma ---
         SetGifClipLine(&dmagif);

         sceGsSyncPath(0, 0);
         sceGsSetHalfOffset((frame&1)? &db.draw1:&db.draw0, 2048,2048,odev);
         frame++;
         odev = !sceGsSyncV(0);

       }
       printf("finish!\n");
       return 0;

};


void LoadObj(ObjData *o, u_int *Head)
{
    int    i;
    int    psum = 0;
    o->pDataHead  = Head; 
    o->magic      = *o->pDataHead;
    o->blockNum   = *(o->pDataHead+2)>>16;
    o->prim       = *(o->pDataHead+2)&0x0000ffff;
    o->dataNum    = *(o->pDataHead+3);

    o->pData      = malloc(sizeof(float*)*o->blockNum);
    o->vertexNum  = malloc(sizeof(u_int)*o->blockNum);
    o->polyNum    = malloc(sizeof(u_int)*o->blockNum);
    o->pack       = malloc(sizeof(GifDmaPacket)*o->blockNum);
    o->packbuf    = malloc(sizeof(GifDmaPacket)*1);
//    o->packbuf    = malloc(sizeof(GifDmaPacket)*o->blockNum);

    o->pData[0]     = (float *)(o->pDataHead+8);
    o->vertexNum[0] = *(o->pDataHead+4);
    if(o->prim&0x3) o->polyNum[0] = o->vertexNum[0]/3;
    else if(o->prim&0x4) o->polyNum[0] = o->vertexNum[0]-2;
    psum+=o->polyNum[0];
    for(i=0; i<o->blockNum-1; i++){ 
      o->pData[i+1] = (o->pData[i]+o->vertexNum[i]*4*4+4);
      o->vertexNum[i+1] = *(u_int *)(o->pData[i]+o->vertexNum[i]*4*4);
      if(o->prim&0x3) o->polyNum[i+1] = o->vertexNum[i+1]/3;
      else if(o->prim&0x4) o->polyNum[i+1] = o->vertexNum[i+1]-2;
      psum+=o->polyNum[i+1];
    }  
    o->psumNum = psum;
};

void ReleaseObj(ObjData *o)
{
    free(o->pData);
    free(o->vertexNum);
    free(o->pack);
    free(o->packbuf);
};

void MakePacket(ObjData *obj, int num, int flag,
                NodeArraySet *scissorflip, ScissorPlaneArray *planeset)
{
    int j;
    u_int clipflag = 0;
    float Q;
    Node  node;

    GifDmaPacket *pack = &obj->pack[num];
    sceVu0IVECTOR v01, c01;
    sceVu0FVECTOR tex, clip, GScolor;
    sceVu0FVECTOR *vertex, *normal, *texUV, *color;
    vertex  = (sceVu0FVECTOR *)obj->pData[num];
    normal  = (sceVu0FVECTOR *)(obj->pData[num]+obj->vertexNum[num]*4);
    texUV   = (sceVu0FVECTOR *)(obj->pData[num]+obj->vertexNum[num]*4*2);
    color   = (sceVu0FVECTOR *)(obj->pData[num]+obj->vertexNum[num]*4*3);

//  make packet(PACKED mode)
    pack->size = 0;
//    pack->buf  = (QWdata*)0x70000000;//on Scratchpad
    pack->buf[pack->size].ul128 = (u_long128)0;
    pack->buf[pack->size++].ui32[0] = 0x70000000|(obj->vertexNum[num]*3+1);
            
//  add vertex info(GIFtag,STQ&RGBA&XYZ)
    pack->buf[pack->size].ul64[0] 
      = SCE_GIF_SET_TAG(obj->vertexNum[num], 1, 1,obj->prim,SCE_GIF_PACKED,3);
    pack->buf[pack->size++].ul64[1] = 0x0412L;

    for(j=0; j<obj->vertexNum[num]; j++){

      clipflag |= 0x3f&sceVu0ViewVolumeClip(clip, local_clip, vertex[j]);

      sceVu0RotTransPers(v01, local_screen, vertex[j], 0);
      sceVu0NormalColorVector(c01, local_light, light_color,
                                 normal[j], color[j]);
      sceVu0ITOF0Vector(GScolor, c01);

      sceVu0CopyVector(node.vertex, vertex[j]);
      sceVu0CopyVector(node.normal, normal[j]);
      sceVu0CopyVector(node.color, GScolor);
      sceVu0CopyVector(node.texUV, texUV[j]);
      sceVu0CopyVector(node.clipV, clip);

      if((obj->prim&0x8)!=0)
        PushTriangleNodeArray(scissorflip, &node, 0);
      else PushTriangleNodeArray(scissorflip, &node, 1);

      if(clipflag&0x3ffff){
          if((obj->prim&0x4 && j>=2) 
             || (obj->prim&0x3 && (j+1)%3==0)){
              v01[3]|=0x8000;
              ScissorTriangle(scissorflip, planeset);
              DrawScissorPolygon(scissorflip, obj, flag);
              ResetNodeArraySet(scissorflip);
          }
      }
      clipflag = clipflag<<6; // keep flag bits (recent two points)

      sceVu0ApplyMatrix(tex, local_screen, vertex[j]);
      Q = 1.0/tex[3];
      sceVu0ScaleVector(tex, texUV[j], Q); // for Perspective correction
      pack->buf[pack->size++].ul128 = *((u_long128*)tex);
      pack->buf[pack->size++].ul128 = *((u_long128*)c01);
      pack->buf[pack->size++].ul128 = *((u_long128*)v01);
    }

};










