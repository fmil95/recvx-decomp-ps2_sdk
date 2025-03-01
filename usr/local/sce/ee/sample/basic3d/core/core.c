/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *                        Version 0.30
 *                            EUC
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.30           May,12,1999     hatakeda    core version
 */

#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <libdev.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
// #include <libpkt.h>
// #include <libvu0.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <libpad.h>

#include "mathfunc.h"

extern  u_int SampleCubeDataHead[];
extern  u_int SampleTorusDataHead[];
extern  u_int SampleTorus1DataHead[];
extern  u_int My_texture1[]  __attribute__((aligned(16)));

typedef struct {
  int         size;
  sceGifTag   giftag;
  sceGsTexEnv gs_tex;
}TexEnv;

typedef union {
  u_long128       ul128;
  u_long          ul64[2];
  u_int           ui32[4];
  sceSamp0FVECTOR fvect;
} QWdata;

typedef struct{
//  QWdata   buf[2048];
  QWdata   *buf;
  u_int    size;
  u_int    *pBase;
}GifPacket;

typedef struct{
    u_int  *pDataHead;
    u_int  magic;
    u_int  blockNum;
    u_int  prim;
    u_int  dataNum;
    u_int  *vertexNum;
    float  **pData;
    GifPacket *pack;
}ObjData;

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 224
#define PI (3.141592f)
#define IMAGE_SIZE   256

sceSamp0FVECTOR camera_p = { 0, 0, -25, 0 };
sceSamp0FVECTOR camera_zd = { 0, 0, 1, 1 };
sceSamp0FVECTOR camera_yd = { 0, 1, 0, 1 };
sceSamp0FVECTOR camera_rot = { 0, 0, 0, 0 };

sceSamp0FVECTOR light0 = { 0.0, 1.5, 0.5, 0 };
sceSamp0FVECTOR light1 = { 1.5, -0.5, 0.5, 0 };
sceSamp0FVECTOR light2 = { -1.5, -0.5, 0.5, 0 };

sceSamp0FVECTOR color0  = { 0.8, 0.3, 0.3, 1 };
sceSamp0FVECTOR color1  = { 0.3, 0.8, 0.3, 1 };
sceSamp0FVECTOR color2  = { 0.3, 0.3, 0.8, 1 };

sceSamp0FVECTOR ambient = { 0.2, 0.2, 0.2, 0 };

sceSamp0FVECTOR obj_trans = { 0, 0, 0, 0 };
sceSamp0FVECTOR obj_rot = { 0, 0, 0, 0 };

sceSamp0FMATRIX local_world;
sceSamp0FMATRIX world_view;
sceSamp0FMATRIX view_screen;
sceSamp0FMATRIX local_screen;

sceSamp0FMATRIX normal_light;
sceSamp0FMATRIX light_color;
sceSamp0FMATRIX local_light;
sceSamp0FMATRIX local_color;

sceSamp0FMATRIX work;

u_int paddata;
u_char rdata[32];
u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

sceGsDBuff      db;
sceGsLoadImage  pTex;
sceDmaChan      *dmaVif;
TexEnv          texenv;

void LoadObj(ObjData *o, u_int *Head);
void ReleaseObj(ObjData *o);
void MakePacket(ObjData *obj, int num);


int main()
{
    int       i;
    u_int     fr;
    float     delta = 1.0f * PI / 180.0f;
    ObjData   obj[2];
    int       obj_switch = 0;
    int       toggle = 0;
    int       odev;

    sceSifInitRpc(0);
    while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
			0, NULL) < 0){
	printf("Can't load module sio2man\n");
    }
    while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
			0, NULL) < 0){
	printf("Can't load module padman\n");
    }

    LoadObj(&obj[0], SampleCubeDataHead);
    LoadObj(&obj[1], SampleTorus1DataHead);

    sceGsResetPath();
    sceDmaReset(1);
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

    // --- set double buffer ---
    sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT,
		     SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
    *(u_long *)&db.clear0.rgbaq
      = SCE_GS_SET_RGBAQ(0x10, 0x10, 0x18, 0x80, 0x3f800000);
    *(u_long *)&db.clear1.rgbaq
      = SCE_GS_SET_RGBAQ(0x10, 0x10, 0x18, 0x80, 0x3f800000);

    sceGsSetDefLoadImage(&pTex,
			 6720,
			 IMAGE_SIZE / 64,
			 SCE_GS_PSMCT32,
			 0, 0,
			 IMAGE_SIZE, IMAGE_SIZE);

    FlushCache(0);
    sceGsExecLoadImage(&pTex, (u_long128 *)My_texture1);
    sceGsSyncPath(0, 0);

    // set texture env
    texenv.size = sceGsSetDefTexEnv(&texenv.gs_tex, 0, 6720, IMAGE_SIZE / 64,
		      SCE_GS_PSMCT32, 8, 8, 0, 0, 0, 0, 1);
    SCE_GIF_CLEAR_TAG(&texenv.giftag);
    texenv.giftag.NLOOP = texenv.size;
    texenv.giftag.EOP = 1;
    texenv.giftag.NREG = 1;
    texenv.giftag.REGS0 = 0xe;
    *(u_long*)&texenv.gs_tex.clamp1
      = SCE_GS_SET_CLAMP(0, 0, 0, 0, 0, 0);
    dmaVif = sceDmaGetChan(SCE_DMA_GIF);
    dmaVif->chcr.TTE = 1;
    FlushCache(0);
    sceDmaSendN(dmaVif, &texenv.giftag, texenv.size+1);
    sceGsSyncPath(0, 0);

    // view -> screen matrix
    sceSamp0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
			     2048.0f, 2048.0f, 1.0f, 16777215.0f,
			     1.0f,65536.0f);

    // --- pad open ---
    scePadInit(0);
    scePadPortOpen(0, 0, pad_dma_buf);

    odev = !sceGsSyncV(0);
    fr = 0;

    while(1){
        // --- read pad ---
	if(scePadRead(0, 0, rdata) > 0){
	    paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
	}
	else{
	    paddata = 0;
	}
	
	// --- object rotate & change view point ---
        if(paddata & SCE_PADLdown){
             obj_rot[0] += delta; if(obj_rot[0]>PI) obj_rot[0] -= 2.0f*PI; }
	if(paddata & SCE_PADLup){
             obj_rot[0] -= delta; if(obj_rot[0]<-PI) obj_rot[0] += 2.0f*PI; }
	if(paddata & SCE_PADLright){
             obj_rot[1] += delta; if(obj_rot[1]>PI) obj_rot[1] -= 2.0f*PI; }
	if(paddata & SCE_PADLleft){
             obj_rot[1] -= delta; if(obj_rot[1]<-PI) obj_rot[1] += 2.0f*PI; }
	if(paddata & SCE_PADL1){
             obj_rot[2] += delta; if(obj_rot[2]>PI) obj_rot[2] -= 2.0f*PI; }
	if(paddata & SCE_PADL2){
             obj_rot[2] -= delta; if(obj_rot[2]<-PI) obj_rot[2] += 2.0f*PI; }
        if(paddata & SCE_PADRdown){
             camera_rot[0] += delta; if(camera_rot[0]>PI*0.4) camera_rot[0] = 0.4f*PI; }
	if(paddata & SCE_PADRup){
             camera_rot[0] -= delta; if(camera_rot[0]<-PI*0.4) camera_rot[0] = -0.4f*PI; }
	if(paddata & SCE_PADRright){
             camera_rot[1] += delta; if(camera_rot[1]>PI) camera_rot[1] -= 2.0f*PI; }
	if(paddata & SCE_PADRleft){
             camera_rot[1] -= delta; if(camera_rot[1]<-PI) camera_rot[1] += 2.0f*PI; }
	if(paddata & SCE_PADR1){
             camera_p[2] -= delta*5; if(camera_p[2]<-100) camera_p[2] = -100; }
	if(paddata & SCE_PADR2){
             camera_p[2] += delta*5; if(camera_p[2]>-10) camera_p[2] = -10; }
	if(!toggle && (paddata & SCE_PADselect)){
             obj_switch = (++obj_switch)&0x01; toggle = 1;
	}else if(!(paddata & SCE_PADselect)){toggle = 0;}

        FlushCache(0);
	sceGsSwapDBuff(&db, fr);
	sceGsSyncPath(0, 0);

        // local -> world (rotate)matrix
        sceSamp0UnitMatrix(work);
        sceSamp0RotMatrix(local_world, work, obj_rot);

	// color&normal matrix setting
	sceSamp0NormalLightMatrix(normal_light, light0, light1, light2);
	sceSamp0LightColorMatrix(light_color, color0, color1, color2, ambient);

	// light(normal) -> local_light matrix
	sceSamp0MulMatrix(local_light, normal_light, local_world);

        // local -> world (rotate&translate)matrix
	sceSamp0TransMatrix(local_world, local_world, obj_trans);

	// world -> view matrix
	sceSamp0RotCameraMatrix(world_view,
				  camera_p, camera_zd, camera_yd, camera_rot);

	// local -> screen matrix
	sceSamp0MulMatrix(work, world_view, local_world);
	sceSamp0MulMatrix(local_screen, view_screen, work);

	for(i=0; i<obj[obj_switch].blockNum; i++){
	  MakePacket(&obj[obj_switch], i);
	  FlushCache(0);
	  sceDmaSend(dmaVif, (u_long128*)((u_int)obj[obj_switch].pack[i].buf|0x80000000));
	  sceGsSyncPath(0, 0);
	}

        sceGsSetHalfOffset((fr&1) ? &db.draw1 : &db.draw0, 2048,2048,odev);
        fr++;
        odev = !sceGsSyncV(0);
    };

    ReleaseObj(&obj[0]);
    ReleaseObj(&obj[1]);
    printf("finish!\n");
    return 0;
}

void LoadObj(ObjData *o, u_int *Head)
{
    int    i;
    o->pDataHead  = Head;
    o->magic      = *o->pDataHead;
    o->blockNum   = *(o->pDataHead+2)>>16;
    o->prim       = *(o->pDataHead+2)&0x0000ffff;
    o->dataNum    = *(o->pDataHead+3);

    o->pData      = malloc(sizeof(float*)*o->blockNum);
    o->vertexNum  = malloc(sizeof(u_int)*o->blockNum);
    o->pack       = malloc(sizeof(GifPacket)*o->blockNum);

    o->pData[0]     = (float *)(o->pDataHead+8);
    o->vertexNum[0] = *(o->pDataHead+4);
    for(i=0; i<o->blockNum-1; i++){
      o->pData[i+1] = (o->pData[i]+o->vertexNum[i]*4*4+4);
      o->vertexNum[i+1] = *(u_int *)(o->pData[i]+o->vertexNum[i]*4*4);
    }

}

void ReleaseObj(ObjData *o){
    free(o->pData);
    free(o->vertexNum);
    free(o->pack);
}

void MakePacket(ObjData *obj, int num)
{
    int j;
    GifPacket *pack = &obj->pack[num];
    sceSamp0IVECTOR v01,c01;
    sceSamp0FVECTOR tex;
    sceSamp0FVECTOR *vertex, *normal, *texUV, *color;
    vertex  = (sceSamp0FVECTOR *)obj->pData[num];
    normal  = (sceSamp0FVECTOR *)(obj->pData[num]+obj->vertexNum[num]*4);
    texUV   = (sceSamp0FVECTOR *)(obj->pData[num]+obj->vertexNum[num]*4*2);
    color   = (sceSamp0FVECTOR *)(obj->pData[num]+obj->vertexNum[num]*4*3);

//  make packet(PACKED mode)
    pack->size = 0;
    pack->buf  = (QWdata*)0x70000000;// on Scratchpad
    pack->buf[pack->size].ul128 = (u_long128)0;
    pack->buf[pack->size++].ui32[0] = 0x70000000|(obj->vertexNum[num]*3+1);

//  add vertex info(GIFtag,STQ&RGBA&XYZ)
    pack->buf[pack->size].ul64[0]
      = SCE_GIF_SET_TAG(obj->vertexNum[num], 1, 1,obj->prim,SCE_GIF_PACKED,3);
    pack->buf[pack->size++].ul64[1] = 0x0412L;
    for(j=0; j<obj->vertexNum[num]; j++){
      float Q = sceSamp0RotTransPers(v01, local_screen, vertex[j], 1);
      sceSamp0ScaleVector(tex, texUV[j], Q); // for Perspective correction
      sceSamp0NormalColorVector(c01, local_light, light_color,
				 normal[j], color[j]);
      pack->buf[pack->size++].ul128 = *((u_long128*)tex);
      pack->buf[pack->size++].ul128 = *((u_long128*)c01);
      pack->buf[pack->size++].ul128 = *((u_long128*)v01);
    }

}

