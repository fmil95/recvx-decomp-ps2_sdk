/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *               Graphics Framework Sample Program
 *
 *              ---- Scene Viewer Version 0.40 ----
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                          Name : gmain.c
 *
 */

#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <libvu0.h>
#include <libdev.h>
#include <sifdev.h>
#include <libpad.h>
#include <string.h>
#include <malloc.h>

#include "defines.h"
#include "packbuf.h"
#include "vu1frame.h"
#include "camera.h"
#include "hier.h"
#include "anim.h"
#include "micro.h"
#include "gmain.h"

#define BG_GRAY 0x40

// use default value at sceGsSetDefDBuff
#define OFFX 2048
#define OFFY 2048

// buff size for DrawStatus()
#define STRING_SIZE (SCREEN_WIDTH/4)


static CameraPos camerapos_init={
    { 0.0f, 0.0f, -5000.0f, 0.0f },		// pos
    { 0.0f, 0.0f, 1.0f, 1.0f },			// zdir
    { 0.0f, 1.0f, 0.0f, 1.0f },			// ydir
    { 0.0f, 0.0f, 0.0f, 0.0f },			// rot
};

static sceVu0FVECTOR clip_volume = { 1000.0f, 1000.0f, 1.0f, 65535.0f};

static sceVu0FVECTOR light0 = { 0.0f, 1.5f, 0.5f, 0.0f };
static sceVu0FVECTOR light1 = { 1.5f, -0.5f, -1.0f, 0.0f };
static sceVu0FVECTOR light2 = { -1.5f, -0.5f, -0.5f, 0.0f };

static sceVu0FVECTOR color0  = { 0.7f, 0.7f, 0.7f, 0.0f };
static sceVu0FVECTOR color1  = { 0.5f, 0.5f, 0.5f, 0.0f };
static sceVu0FVECTOR color2  = { 0.4f, 0.4f, 0.4f, 0.0f };

static sceVu0FVECTOR ambient = { 0.3f, 0.3f, 0.3f, 0.0f };



/* for path1 packet */
static PACKET_BUF kick_pack;			/* order table */
static PACKET_BUF obj_pack;			// model data flow list
static PACKET_BUF shape_pack;			// sub model data flow list

static MICRO_FRAME base;			// micro-program #0
static MICRO_FRAME cull;			// micro-program #1
static int micro_switch = 0;

static SHAPE_FRAME *shape_frame=NULL;
static TEXTURE_FRAME *texture_frame=NULL;
static HIERARCHY_FRAME *hierarchy_frame=NULL;

static CameraPos camerapos;			// for controller 
static CAMERA cam0;				// for draw

static sceGsDBuff db;			// disp&draw environment

static u_int *shape_list=NULL;
static u_int *texture_list=NULL;

static int vu1work = 0;			// VUMEM1 workarea size (for micro program)

static u_long128 *print_buf[2];			// packet area for DrawStrings()

/* for display status (DrawStatus()) */
typedef struct {
    int micro_switch;
    int frame;
    int vertNum;
    float transTex;
    int cpu_count;
    int dma_count;
} Status;

static Status status; 



static void g_pad_ctl(u_int paddata);
static void g_pre_calc(u_int frame);
static u_int *MicroInit(u_int *pCurr);
static u_int *ObjectInit(u_int *data, u_int *pCurr);
static void DrawStatus(Status *st);
static int analyze_data(u_int *data);
static void register_ordertable(PACKET_BUF *ot, Status *st);
static void obj_finish();




/* main loop for graphics */
void g_main(u_int  frame, u_int paddata)
{
    // read controller data
    g_pad_ctl(paddata);
		
    // reset counter
    *T0_COUNT = 0;

    // pre-calculation for matrix, animation, and shared-objects w/CPU+VU0 
    g_pre_calc(frame);
    status.vertNum = 0;


    // register data-flow lists to the order_table (kick_pack)
    register_ordertable(&kick_pack, &status);

    FlushCache(0);
    status.cpu_count = *T0_COUNT;
    sceGsSyncPath(0, 0);

    /* kick order_table to dma  */
    status.dma_count = *T0_COUNT;
    Vu1PacketKickDma(kick_pack.pHead);

    sceGsSyncPath(0, 0);
    status.dma_count = *T0_COUNT - status.dma_count;

    /* status output to console */
    status.micro_switch=micro_switch;
    status.frame=frame;
    if (texture_frame) status.transTex=texture_frame->TransSizeTotal;
    else status.transTex=0.0f;
    DrawStatus(&status);
}

/* graphics operation during VSync */
void g_sync(u_int frame, int odev)
{
    static int dbp=0;

    sceGsSetHalfOffset((dbp&1) ? &db.draw1 : &db.draw0, 
    		       OFFX,OFFY,odev);
    sceGsSwapDBuff(&db, dbp++);

}


/* graphics operation at the end of this program */
void g_finish(void)
{
    int i;
    obj_finish();
    free((u_int *)Paddr(shape_pack.pHead));
    free((u_int *)Paddr(obj_pack.pHead));
    free((u_int *)Paddr(kick_pack.pHead));
    for (i=0; i<2; i++){
	free((u_int *)Paddr(print_buf[i]));
    }
}


/* clear model data */
void g_clear_model()
{
    obj_finish();
    obj_pack.pCurr=MicroInit(obj_pack.pCurr);

}

/* reset camera position */
void g_reset_camera(void)
{
    camerapos=camerapos_init;
}


/* read and initialize model data */
void g_read_model(u_int *data)
{
    // clear the present model
    if (shape_list) g_clear_model();		

    // initalize model data
    obj_pack.pCurr=ObjectInit(data, obj_pack.pCurr);

    // check packet buffer size
    if (Vu1PacketBufOverflow(&obj_pack) || Vu1PacketBufOverflow(&shape_pack)){
	ErrPrintf("buffer overflow: PACK_SIZE=%d, SHAPE_PACK_SIZE=%d",
		  ((int)obj_pack.pCurr-(int)obj_pack.pHead)/sizeof(u_long128),
		  ((int)shape_pack.pCurr-(int)shape_pack.pHead)/sizeof(u_long128));
    }
}


/* texenv for ginit() */
typedef struct {
    sceGifTag giftag;
    sceGsTest gs_test;
    long gs_test1addr;
    sceGsAlphaEnv gs_alpha;
    sceGsTexEnv gs_tex;
}TexEnv;
TexEnv          texenv;
/* graphics initialize */
void g_init()
{
    int i;

    sceGsResetPath();
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    
    /* --- set double buffer ---*/
    sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT,
		     SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
    *(u_long *)&db.clear0.rgbaq =
      SCE_GS_SET_RGBAQ(BG_GRAY, BG_GRAY, BG_GRAY, 0x80, 0x3f800000);
    *(u_long *)&db.clear1.rgbaq =
      SCE_GS_SET_RGBAQ(BG_GRAY, BG_GRAY, BG_GRAY, 0x80, 0x3f800000);
    FlushCache(0);

    /* --- set texture env --- */
    sceGsSetDefTexEnv(&texenv.gs_tex, 0, 6720, 
		      256/64, SCE_GS_PSMCT32,
		      8, 8, 0, 0, 0, 0, 1); /* tex01 here is not valid (overwrite it later) */
    SCE_GIF_CLEAR_TAG(&texenv.giftag);
    texenv.giftag.NLOOP = 9;
    texenv.giftag.EOP = 1;
    texenv.giftag.NREG = 1;
    texenv.giftag.REGS0 = 0xe;
    sceGsSetDefAlphaEnv(&texenv.gs_alpha, 0);
    *(u_long *) &texenv.gs_alpha.alpha1 = SCE_GS_SET_ALPHA(0, 1, 0, 1, 0);
    *(u_long *) &texenv.gs_test =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 0x3);
    texenv.gs_test1addr = (long)SCE_GS_TEST_1;
    FlushCache(0);
    sceGsPutDrawEnv(&texenv.giftag);
    sceGsSyncPath(0, 0);

    /* Init camera parameter & matrix */
    g_reset_camera();
    Camera_SetCamera_fromCameraPos(&cam0, &camerapos);
    Camera_SetClipVolume(&cam0, clip_volume);
    Camera_SetClipScreen(&cam0, 512.0f, 1.0f, 0.47f, (float)OFFX, (float)OFFY, 
			 5.0f, 16777000.0f, 1.0f, 65536.0f);
    Camera_SetMatrix(&cam0);

    /* Init matrix data for world-position */
    sceVu0UnitMatrix(hierarchy_root.local_world);
    sceVu0UnitMatrix(hierarchy_root.light_rot);

    /* Init packetting buffer */
    Vu1PacketBufInit(&kick_pack, KICK_SIZE, CACHED);	// for order_table
    Vu1PacketBufInit(&obj_pack, PACK_SIZE, CACHED);	// for model_dataflow_list
    Vu1PacketBufInit(&shape_pack, SHAPE_PACK_SIZE, CACHED); // for sub model_data_flow_list

    obj_pack.pCurr=MicroInit(obj_pack.pCurr);


    // start TIMER T0
    *T0_MODE = 2  | (0<<6) | (1<<7) | (0<<8);	


    for (i=0; i<2; i++){
	print_buf[i]=(u_long128 *)
	    memalign(64, bound(16*(16/2)*sizeof(u_long128)*STRING_SIZE, 64));
	if (print_buf[i] == NULL){
	    ErrPrintf("cannot malloc print_buf\n");
	    ERR_STOP;
	}
	print_buf[i] = (u_long128 *)Uncached(print_buf[i]);
    }

    FlushCache(0);
}

/* print messages on console */
void DrawStrings(int x, int y, char *fmt, ...)
{
    va_list ap;
    char string_buf[STRING_SIZE];
    static int dbp=0;

    va_start(ap, fmt);
    vsprintf(string_buf, fmt, ap);
    va_end(ap);


    if (string_buf[0]){				// not a null string
	sceDevFont(print_buf[dbp&1],
		   (OFFX-(SCREEN_WIDTH/2)+x)<<4,
		   (OFFY-(SCREEN_HEIGHT/2)+y)<<4,
		   0xffffffff, string_buf, 0);
	sceGsSyncPath(0, 0);
	*D1_QWC  = 0x00;
	*D1_TADR = Paddr(print_buf[dbp&1]);
	*D_STAT = 2;
	// print_buf is uncached
	*D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
	dbp++;
    }
}



/* analyze model data and set/allocate global handlers */
#define TYPE_PACKED 	0
#define TYPE_TEXTURE 	1
#define TYPE_SHAPE	2
#define TYPE_HIERARCHY	3
#define TYPE_SRCVN	4
#define TYPE_VERTEXINDEX 5
#define TYPE_NORMALINDEX 6
#define TYPE_SHAREV	7
#define TYPE_SHAREN	8
#define TYPE_KEYFRAME	9
#define TYPE_KEYVAL	10
#define TYPE_ANIM	11
static int analyze_data(u_int *data)
{
    int num;
    int i,j;
    u_int *p=data;
    int size;
    int totalsize;
    u_int  type;
    u_int *tmpp;

    if (p[0]!=TYPE_PACKED){
	ErrPrintf("illegal data type %d\n", p[0]);
	return -1;
    }
    num=p[3];
    totalsize=p[2];

    p+=4; totalsize-=4;

    for (i=0; i<num; i++){
	size=p[2];
	type=p[0];
	p+=4; totalsize-=4;

	switch (type){
	case TYPE_TEXTURE:
  	    texture_list=(u_int *)p;
	    break;
	case TYPE_SHAPE:
	    shape_list=(u_int *)p;
	    break;
	case TYPE_HIERARCHY:
  	    nhierarchy=p[3];
  	    hierarchy=(Hierarchy *)(p+4);
	    break;
	case TYPE_SRCVN:
	    nsrcvertex=p[0];
	    ndstvertex=p[1];
	    nsrcnormal=p[2];
	    ndstnormal=p[3];
	    srcvertex=(sceVu0FVECTOR *)(p+4);
	    srcnormal=srcvertex+nsrcvertex;

	    if (ndstvertex>0){
		dstvertex=(sceVu0FVECTOR *)memalign(64, bound(sizeof(float)*4*ndstvertex, 64));
		if(dstvertex == NULL){
		    ErrPrintf("cannot malloc dstvertex\n");
		    return -1;
		}
	    }
	    if (ndstnormal>0){
		dstnormal=(sceVu0FVECTOR *)memalign(64, bound(sizeof(float)*4*ndstnormal, 64));
		if(dstnormal == NULL){
		    ErrPrintf("cannot malloc dstnormal\n");
		    return -1;
		}
	    }

	    break;
	case TYPE_SHAREV:
	    shareshapeidvertex=p[2];
	    nsharevertex=p[3];
	    sharevertex=(Share *)(p+4);
	    break;
	case TYPE_SHAREN:
	    shareshapeidnormal=p[2];
	    nsharenormal=p[3];
	    sharenormal=(Share *)(p+4);
	    break;
	case TYPE_VERTEXINDEX:
	    indexvertex=(int *)(p+4);
	    break;
	case TYPE_NORMALINDEX:
	    indexnormal=(int *)(p+4);
	    break;
	case TYPE_KEYFRAME:
	    nkeyframe=p[3];
	    if (nkeyframe>0){
		keyframe=(int **)memalign(16, sizeof(int *)*nkeyframe);
		if(keyframe == NULL){
		    ErrPrintf("cannot malloc keyframe\n");
		    return -1;
		}
	    }
	    tmpp=p+4;
	    for (j=0; j<nkeyframe; j++){
		keyframe[j]=(int *)(tmpp+4);
		tmpp+=tmpp[2]+4;
	    }
	    break;
	case TYPE_KEYVAL:
	    nkeyvalue=p[3];
	    if (nkeyvalue>0){
		keyvalue=(float **)memalign(16, sizeof(float *)*nkeyvalue);
		if(keyvalue == NULL){
		    ErrPrintf("cannot malloc keyvalue\n");
		    return -1;
		}
	    }
	    tmpp=p+4;
	    for (j=0; j<nkeyvalue; j++){
		keyvalue[j]=(float *)(tmpp+4);
		tmpp+=tmpp[2]+4;
	    }

	    break;
	case TYPE_ANIM:
	    nanimation=p[3];
	    animation=(Animation *)(p+4);
	    break;
	}
	p+=size; totalsize-=size;
    }
    if (totalsize!=0){
	ErrPrintf("Illegal data size\n");
	return -1;
    }
    return 0;
}



/* analyze model data and set/allocate handlers */
static u_int *ObjectInit(u_int *data, u_int *pCurr)
{
    /* analyze model data and set/allocate global variables */
    if (analyze_data(data)<0) return pCurr;

    /* set/allocate local variables */

    /* Init Texture_swap environment */
    texture_frame=NULL;
    if (texture_list && texture_list[3]>0){
	texture_frame = (TEXTURE_FRAME *)memalign(16, sizeof(TEXTURE_FRAME)*1);
	if(texture_frame == NULL){
	    ErrPrintf("cannot malloc texture_frame\n");
	    return pCurr;
	}
	texture_frame->image=NULL;

	if (Vu1TexInit(texture_frame, texture_list)<0) return pCurr; // return on error
    }

    /* Init shape data & packetting environment */

    if (shape_list && shape_list[3]>0){
	shape_frame = (SHAPE_FRAME *)memalign(16, sizeof(SHAPE_FRAME)*1);
	if(shape_frame == NULL){
	    ErrPrintf("cannot malloc shape_frame\n");
	    return pCurr;
	}
	shape_frame->shape=NULL;

	if (Vu1ShapeInit(shape_frame, shape_list)<0) return pCurr; // return on error

	if (hierarchy && nhierarchy>0){
	    hierarchy_frame = (HIERARCHY_FRAME *)memalign(16, sizeof(HIERARCHY_FRAME)*1);
	    if(hierarchy_frame == NULL){
		ErrPrintf("cannot malloc hierarchy_frame\n");
		return pCurr;
	    }
	    hierarchy_frame->hier=NULL;
	    HierarchyInit(hierarchy_frame, hierarchy, nhierarchy, shape_frame);
	}
    }

    if (texture_frame){
/* make resident texture transfer packet */
	pCurr = Vu1ResidentTextureMakePacket(texture_frame,  pCurr);
    }

    if (shape_frame && hierarchy_frame){
	/* make shape data transfer packet */
	pCurr = Vu1ShapeFrameMakePacket(shape_frame, texture_frame, pCurr, vu1work, hierarchy_frame, &shape_pack);
    }

    return pCurr;
}

/* make data flow list for microprogram */
static u_int *MicroInit(u_int *pCurr)
{
    /* Init Vu1-microcode environment & matrix */
    Vu1MicroInit(&cull);
    Vu1MicroSetLightVect(&cull, 0, light0, light1, light2);
    Vu1MicroSetLightColor(&cull, 0, color0, color1, color2, ambient);

    Vu1MicroInit(&base);
    Vu1MicroSetLightVect(&base, 0, light0, light1, light2);
    Vu1MicroSetLightColor(&base, 0, color0, color1, color2, ambient);

    /* make micro program transfer packet (DMAtag & VIFcode) */
    pCurr = Vu1MicroMakePacket(&base, pCurr, My_micro_base, vu1work);
    pCurr = Vu1MicroMakePacket(&cull, pCurr, My_micro_cull, vu1work);

    return pCurr;
}


/* free local and global variables depending model data */
static void obj_finish()
{
    int i;

    sceGsSyncPath(0,0);

    Vu1PacketBufReset(&shape_pack);
    Vu1PacketBufReset(&obj_pack);

    if (texture_frame){
	if (texture_frame->image) free((u_int *)Paddr(texture_frame->image));
	free((u_int *)Paddr(texture_frame));
	texture_frame=NULL;
    }

    if (shape_frame){
	if (shape_frame->shape){
	    for (i=0; i<shape_frame->num; i++){
		free((u_int *)Paddr(shape_frame->shape[i].hier));
		free((u_int *)Paddr(shape_frame->shape[i].pGeomVertex));
		free((u_int *)Paddr(shape_frame->shape[i].pGeomNormal));
	    }
	    free((u_int *)Paddr(shape_frame->shape));
	}
	free((u_int *)Paddr(shape_frame));
	shape_frame=NULL;
    }

    if (hierarchy_frame){
	if (hierarchy_frame->hier) free((u_int *)Paddr(hierarchy_frame->hier));
	free((u_int *)Paddr(hierarchy_frame));
	hierarchy_frame=NULL;
    }

    shape_list=NULL;
    texture_list=NULL;
    nhierarchy=0;
    hierarchy=NULL;
    nanimation=0;
    animation=NULL;
    if (keyframe){
	free((u_int *)Paddr(keyframe));
	keyframe=NULL;
    }
    nkeyframe=0;
    if (keyvalue){
	free((u_int *)Paddr(keyvalue));
	keyvalue=NULL;
    }
    nkeyvalue=0;
    sharevertex=NULL;
    nsharevertex=0;
    shareshapeidvertex= -1;
    indexvertex=NULL;
    sharenormal=NULL;
    nsharenormal=0;
    shareshapeidnormal= -1;
    indexnormal=NULL;
    srcvertex=NULL;
    srcnormal=NULL;
    if (dstvertex){
	free((u_int *)Paddr(dstvertex));
	dstvertex=NULL;
    }
    if (dstnormal){
	free((u_int *)Paddr(dstnormal));
	dstnormal=NULL;
    }

    nsrcvertex=0;
    nsrcnormal=0;
    ndstvertex=0;
    ndstnormal=0;

}


/* procede controller input */
static void g_pad_ctl(u_int paddata)
{
    float  dr = 1.0f / 180.0f * PI;
    float  dt = 30.0f;
    static u_int oldpaddata=0;

/* --- object rotate & change view point ---*/
    if(paddata & SCE_PADLdown){
	camerapos.rot[0]+=dr; if(camerapos.rot[0]>PI) camerapos.rot[0] -= 2.0f*PI; 
    }
    if(paddata & SCE_PADLup){
	camerapos.rot[0]-=dr; if(camerapos.rot[0]<-PI) camerapos.rot[0] += 2.0f*PI; 
    }
    if(paddata & SCE_PADLright){
	camerapos.rot[1]-=dr; if(camerapos.rot[1]<-PI) camerapos.rot[1] += 2.0f*PI; 
    }
    if(paddata & SCE_PADLleft){
	camerapos.rot[1]+=dr; if(camerapos.rot[1]>PI) camerapos.rot[1] -= 2.0f*PI; 
    }
    if(paddata & SCE_PADL1){
	camerapos.rot[2]-=dr; if(camerapos.rot[2]<-PI) camerapos.rot[2] += 2.0f*PI; 
    }
    if(paddata & SCE_PADL2){
	camerapos.rot[2]+=dr; if(camerapos.rot[2]>PI) camerapos.rot[2] -= 2.0f*PI; 
    }
    if(paddata & SCE_PADRdown)  camerapos.pos[1]-=dt;
    if(paddata & SCE_PADRup)  camerapos.pos[1] += dt;
    if(paddata & SCE_PADRright)  camerapos.pos[0] -= dt;
    if(paddata & SCE_PADRleft)  camerapos.pos[0] += dt;
    if(paddata & SCE_PADR1) camerapos.pos[2] += dt;
    if(paddata & SCE_PADR2) camerapos.pos[2] -= dt;

    if (SCE_PADstart &paddata&(~oldpaddata)){
	micro_switch++; micro_switch&=0x01;
    }
    if (SCE_PADselect &paddata&(~oldpaddata)){
	g_reset_camera();
    }

    oldpaddata=paddata;
}

 
	// pre-calculation for matrix, animation, and shared-objects w/CPU+VU0 
static void g_pre_calc(u_int frame)
{
    /* set camera parameter & matrix */
    Camera_SetCamera_fromCameraPos(&cam0, &camerapos);
    Camera_SetMatrix(&cam0);

    /* set camera matrix to vu1 env */
    Vu1MicroSetWorldScreen((MICRO_FRAME *)Uncached(&cull), cam0.world_screen);
    Vu1MicroSetWorldClip((MICRO_FRAME *)Uncached(&cull), cam0.world_clip);
    Vu1MicroSetClipScreen((MICRO_FRAME *)Uncached(&cull), cam0.clip_screen);

    Vu1MicroSetWorldScreen((MICRO_FRAME *)Uncached(&base), cam0.world_screen);
    Vu1MicroSetWorldClip((MICRO_FRAME *)Uncached(&base), cam0.world_clip);
    Vu1MicroSetClipScreen((MICRO_FRAME *)Uncached(&base), cam0.clip_screen);
	

    if (hierarchy_frame){
	SetAnimation(frame, hierarchy_frame);
	SetHierarchy(hierarchy_frame);
    }
    if (shape_frame) SetShare(shape_frame);
}

/* register model data flow lists to the order table */
static void register_ordertable(PACKET_BUF *ot, Status *st)
{
    int i;
    /* reset buffer status */
    Vu1PacketBufReset(ot);


    /* add vu1 micro-code to vu1 rendering flow */
    switch(micro_switch){
    case 0: ot->pCurr = Vu1PacketAddMic(ot->pCurr, base.pPackData); break;
    case 1: ot->pCurr = Vu1PacketAddMic(ot->pCurr, cull.pPackData); break;
    }

    /* add resident texture to vu1 rendering flow */
    if(texture_frame != NULL){
	for (i=0; i<texture_frame->num; i++){
	    if (texture_frame->image[i].pPackData) {
		ot->pCurr= Vu1PacketAddObj(ot->pCurr, texture_frame->image[i].pPackData);
	    }
	}
    }

    /* add shape-data and non-resident texture to vu1 rendering flow */
    if (shape_frame){

	ot->pCurr = Vu1PacketAddObj(ot->pCurr, shape_frame->pPackData);
	st->vertNum += shape_frame->VertexTotal;
    }



}


/* status infomation on Console */
static void DrawStatus(Status *st)
{
    float  cpu_pow;
    float  dma_pow;

    int xpos=0;
    int ypos=8;

    cpu_pow = ((float)st->cpu_count * 100.0f) / 9765.625f;
    dma_pow = ((float)st->dma_count * 100.0f) / 9765.625f;


    switch(st->micro_switch){
    case 0:
	DrawStrings(xpos, ypos, "RENDERING: 3 dir_light"); ypos+=8; 
	break; 
    case 1:
	DrawStrings(xpos, ypos, "RENDERING: 3 dir_light with culling");     ypos+=8;
	break;
    }

    DrawStrings(xpos, ypos, "VERTICES %d, TEXTURE %.2fMb", st->vertNum, st->transTex); ypos+=8;

    DrawStrings(xpos, ypos, "USAGE: CPU %3.1f%%, DMA %3.1f%%", cpu_pow, dma_pow); ypos+=8;
    DrawStrings(xpos, ypos, "FRAME: %6u", st->frame); ypos+=8;
}

