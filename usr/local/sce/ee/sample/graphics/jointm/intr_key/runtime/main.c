/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *
 *                         Version 0.5.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : joint matrix interpolation runtime
 *                     Description : main procedures
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.5.0          Aug,16,1999     aoki
 */

#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <libdev.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <libvu0.h>
#include <libpad.h>
#include "matintr.h"
#include "aseq.h"

typedef struct {
    sceVu0FVECTOR rot;
    sceVu0FVECTOR trans;
    sceVu0FMATRIX local_world;
    sceVu0FMATRIX *parent_matrix;
    sceVu0FMATRIX local_world_light;
    sceVu0FMATRIX *parent_light;
    u_int *dmapacket;
    MatIntr matintr;
} OBJECT;


void KickDma1Sc(void *addr);
void set_matrix(OBJECT *o);
void init_object();
void RotCameraMatrix(sceVu0FMATRIX m, sceVu0FVECTOR p, sceVu0FVECTOR zd, sceVu0FVECTOR yd, sceVu0FVECTOR rot);
void set_local_matrix(OBJECT *o);
void set_world_matrix();
void LoadMatrix(OBJECT *o);

extern u_int My_dma_start __attribute__((section(".vudata")));
extern u_int My_dma_next  __attribute__((section(".vudata")));
extern u_int My_matrix __attribute__((section(".vudata")));
extern u_int My_rot_trans_matrix __attribute__((section(".vudata")));
extern u_int My_light_matrix __attribute__((section(".vudata")));
extern u_int My_light_color_matrix __attribute__((section(".vudata")));
extern u_int torso_packet __attribute__((section(".vudata")));
extern u_int head_packet __attribute__((section(".vudata")));
extern u_int arm_packet __attribute__((section(".vudata")));
extern u_int leg_packet __attribute__((section(".vudata")));


// object handler
OBJECT obj[10]=
{
    {
	{0,0,0,0},
	{0,0,0,0},
	{}, NULL, {}, NULL,
	&torso_packet,
	{},
    },
    {
	{0,0,0,0,},
	{0.0f,-3.5f,0.0f,0.0f,},
	{}, NULL, {}, NULL,
	&head_packet,
	{},
    },
    {
	{0,0,-0.4,0,},
	{1.5f,-3.0f,0.0f,0.0f,},
	{}, NULL, {}, NULL,
	&arm_packet,
	{},
    },
    {
	{0,0,0,0,},
	{0.0f,2.0f,0.0f,0.0f,},
	{}, NULL, {}, NULL,
	&arm_packet,
	{},
    },
    {
	{0,0,0.4,0,},
	{-1.5f,-3.0f,0.0f,0.0f,},
	{}, NULL, {}, NULL,
	&arm_packet,
	{},
    },
    {
	{0,0,0,0,},
	{0.0f,2.0f,0.0f,0.0f,},
	{}, NULL, {}, NULL,
	&arm_packet,
	{},
    },
    {
	{0,0,0.2,0,},
	{0.6f,0.5f,0.0f,0.0f,},
	{}, NULL, {}, NULL,
	&leg_packet,
	{},
    },
    {
	{0,0,0,0,},
	{0.0f,2.8f,0.0f,0.0f,},
	{}, NULL, {}, NULL,
	&leg_packet,
	{},
    },
    {
	{0,0,-0.2,0,},
	{-0.6f,0.5f,0.0f,0.0f,},
	{}, NULL, {}, NULL,
	&leg_packet,
	{},
    },
    {
	{0,0,0,0,},
	{0.0f,2.8f,0.0f,0.0f,},
	{}, NULL, {}, NULL,
	&leg_packet,
	{},
    },
};


#define PI 3.14159265358979323846f
#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 224.0f
#define SCREEN_OFFSET_X 1728.0f
#define SCREEN_OFFSET_Y 1936.0f


sceVu0FVECTOR camera_p = { 0, 0, -25, 0 };
sceVu0FVECTOR camera_zd = { 0, 0, 1, 1 };
sceVu0FVECTOR camera_yd = { 0, 1, 0, 1 };
sceVu0FVECTOR camera_rot = { 0, 0, 0, 0 };


sceVu0FVECTOR light0 = { 0.0, 1.5, 0.5, 0 };
sceVu0FVECTOR light1 = { 1.5, -0.5, 0.5, 0 };
sceVu0FVECTOR light2 = { -1.5, -0.5, 0.5, 0 };

sceVu0FVECTOR color0  = { 0.8, 0.8, 0.8, 0 };
sceVu0FVECTOR color1  = { 0.8, 0.8, 0.8, 0 };
sceVu0FVECTOR color2  = { 0.8, 0.8, 0.8, 0 };

sceVu0FVECTOR ambient = { 0.2, 0.2, 0.2, 0 };

sceVu0FVECTOR obj_trans = { 0, 0, 0, 0 };
sceVu0FVECTOR obj_rot = { 0, 0, 0, 0 };
sceVu0FMATRIX unitmat;
sceVu0FVECTOR zerovec = {0.0f, 0.0f, 0.0f, 1.0f};
sceVu0FVECTOR vec1= {1.0f, 1.0f, 1.0f, 1.0f};

sceVu0FMATRIX normal_light;


u_int paddata;
u_char rdata[32];
u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

sceGsDBuff      db;

u_int  frame;

int main()
{
    int    odev;
    float  delta = 1.0f / 180.0f * PI;
    int    toggle = 0;
    int i;
    OBJECT *o;
    sceVu0FVECTOR prmax, prmin, plmax, plmin;
    sceVu0FVECTOR *pr, *pl;

    sceSifInitRpc(0);
    while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
			0, NULL) < 0){
	printf("Can't load module sio2man\n");
    }
    while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
			0, NULL) < 0){
	printf("Can't load module padman\n");
    }

    sceGsResetPath();
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

    // --- set double buffer ---
    sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224,
		     SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
    *(u_long *)&db.clear0.rgbaq =
	SCE_GS_SET_RGBAQ(0x10, 0x10, 0x20, 0x80, 0x3f800000);
    *(u_long *)&db.clear1.rgbaq =
	SCE_GS_SET_RGBAQ(0x10, 0x10, 0x20, 0x80, 0x3f800000);

    FlushCache(0);

    init_object();
    // --- open pad ---
    scePadInit(0);
    scePadPortOpen(0, 0, pad_dma_buf);

    frame = 0;
    odev = !sceGsSyncV(0);


    while(1){
	pl= &obj[0].rot;
	sceVu0ScaleVector(plmax, vec1, PI);
	sceVu0ScaleVector(plmin, vec1, -PI);

	pr= &camera_rot;
	prmax[0]=PI*0.4; prmin[0]= -PI*0.4;
	prmax[1]=PI; prmin[1]= -PI;
	prmax[2]= -10; prmin[2]= -100;

	// --- read pad ---
	if(scePadRead(0, 0, rdata) > 0){
	    paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
	}
	else{
	    paddata = 0;
	}
	
	// --- object rotate & change view point ---
	if(paddata & SCE_PADLdown){
	    (*pl)[0] += delta; if((*pl)[0]>plmax[0]) (*pl)[0] = plmax[0]; }
	if(paddata & SCE_PADLup){
	    (*pl)[0] -= delta; if((*pl)[0]<plmin[0]) (*pl)[0] = plmin[0]; }
	if(paddata & SCE_PADLright){
	    (*pl)[1] += delta; if((*pl)[1]>plmax[1]) (*pl)[1] = plmax[1]; }
	if(paddata & SCE_PADLleft){
	    (*pl)[1] -= delta; if((*pl)[1]<plmin[1]) (*pl)[1] = plmin[1]; }
	if(paddata & SCE_PADL1){
	    (*pl)[2] += delta; if((*pl)[2]>plmax[2]) (*pl)[2] = plmax[2]; }
	if(paddata & SCE_PADL2){
	    (*pl)[2] -= delta; if((*pl)[2]<plmin[2]) (*pl)[2] = plmin[2]; }
	if(paddata & SCE_PADRdown){
	    (*pr)[0] += delta; if((*pr)[0]>prmax[0]) (*pr)[0] = prmax[0]; }
	if(paddata & SCE_PADRup){
	    (*pr)[0] -= delta; if((*pr)[0]<plmin[0]) (*pr)[0] = plmin[0]; }
	if(paddata & SCE_PADRright){
	    (*pr)[1] += delta; if((*pr)[1]>prmax[1]) (*pr)[1] = prmax[1]; }
	if(paddata & SCE_PADRleft){
	    (*pr)[1] -= delta; if((*pr)[1]<plmin[1]) (*pr)[1] = plmin[1]; }
	if(paddata & SCE_PADR1){
	    camera_p[2] -= delta*5; if(camera_p[2]<-100) camera_p[2] = -100; }
	if(paddata & SCE_PADR2){
	    camera_p[2] += delta*5; if(camera_p[2]>-10) camera_p[2] = -10; }
	if(!toggle && (paddata & SCE_PADstart)){
	    init_object();  // replay start
	    toggle = 1;
	}else if(!(paddata & SCE_PADstart)){toggle = 0;}

	FlushCache(0);
	sceGsSwapDBuff(&db, frame);
	sceGsSyncPath(0, 0);

	set_world_matrix();

	o= &obj[0];
	for (i=0; i<10; i++){
	    // get next local matrix
	    MatIntrLoadMatrix(o->local_world, &o->matintr);
	    // calc global/light matrix
	    set_local_matrix(o);
	    // set model data to dma chain
	    *((&My_dma_next)+1) = (u_int)o->dmapacket;
	    // start dma
	    KickDma1Sc(&My_dma_start);

	    o++;
	}

	sceGsSyncPath(0, 0);
	sceGsSetHalfOffset((frame&1) ? &db.draw1 : &db.draw0, 2048,2048,odev);

	frame++;
	odev = !sceGsSyncV(0);
    }

    return 0;

}


void KickDma1Sc(void *addr)
{
// --- kick dma to draw object ---
    sceGsSyncPath(0, 0);
    *D1_QWC  = 0x00;
    *D1_TADR = (u_int)addr & 0x0fffffff;
    *D_STAT = 2;
    FlushCache(0);
    *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
}

void CopyMatrix(u_int *addr, sceVu0FMATRIX m)
{
    int i,j,k;

    k=0;
    for (i=0; i<4; i++){
	for (j=0; j<4; j++){
  	    (addr)[k++]  = *(u_int *)&(m[i][j]);
	}
    }
}


void set_local_matrix(OBJECT *o)
{
    sceVu0FMATRIX light;

    // cancel translation for light
    SetTransMatrix(o->local_world_light, o->local_world, zerovec);

    // local-world rotation&translation
    sceVu0MulMatrix(o->local_world, *o->parent_matrix, o->local_world);
    // set local-world to DMA packet
    CopyMatrix(&My_rot_trans_matrix, o->local_world);

    // local-world rotation
    sceVu0MulMatrix(o->local_world_light, *o->parent_light, o->local_world_light);

    // light(normal) matrix
    sceVu0MulMatrix(light, normal_light, o->local_world_light);
    // set light(normal) matrix to DMA packet
    CopyMatrix(&My_light_matrix, light);

}

void set_world_matrix()
{
    sceVu0FMATRIX world_view;
    sceVu0FMATRIX view_screen;
    sceVu0FMATRIX light_color;
    sceVu0FMATRIX world_screen;

// color&normal matrix setting
    sceVu0NormalLightMatrix(normal_light, light0, light1, light2);
    sceVu0LightColorMatrix(light_color, color0, color1, color2, ambient);

    CopyMatrix(&My_light_color_matrix,light_color);

    RotCameraMatrix(world_view, camera_p, camera_zd, camera_yd, camera_rot);

// view -> screen matrix
    sceVu0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
			     2048.0f, 2048.0f, 1.0f, 16777215.0f,
			     1.0f,65536.0f);

// world -> screen matrix
    sceVu0MulMatrix(world_screen, view_screen, world_view);
    CopyMatrix(&My_matrix, world_screen);
}

void init_object()
{
    int i;

    sceVu0UnitMatrix(unitmat);

    obj[0].parent_matrix= &unitmat;
    obj[0].parent_light= &unitmat;

    for (i=1; i<10; i++){
	obj[i].parent_matrix= &obj[i-1].local_world;
	obj[i].parent_light= &obj[i-1].local_world_light;
    }
    obj[2].parent_matrix= &obj[0].local_world;
    obj[2].parent_light= &obj[0].local_world_light;
    obj[4].parent_matrix= &obj[0].local_world;
    obj[4].parent_light= &obj[0].local_world_light;
    obj[6].parent_matrix= &obj[0].local_world;
    obj[6].parent_light= &obj[0].local_world_light;
    obj[8].parent_matrix= &obj[0].local_world;
    obj[8].parent_light= &obj[0].local_world_light;

    MatIntrInit(&obj[0].matintr, aseqstart0,  aseqdiff0, keynum);
    MatIntrInit(&obj[1].matintr, aseqstart1,  aseqdiff1, keynum);
    MatIntrInit(&obj[2].matintr, aseqstart2,  aseqdiff2, keynum);
    MatIntrInit(&obj[3].matintr, aseqstart3,  aseqdiff3, keynum);
    MatIntrInit(&obj[4].matintr, aseqstart4,  aseqdiff4, keynum);
    MatIntrInit(&obj[5].matintr, aseqstart5,  aseqdiff5, keynum);
    MatIntrInit(&obj[6].matintr, aseqstart6,  aseqdiff6, keynum);
    MatIntrInit(&obj[7].matintr, aseqstart7,  aseqdiff7, keynum);
    MatIntrInit(&obj[8].matintr, aseqstart8,  aseqdiff8, keynum);
    MatIntrInit(&obj[9].matintr, aseqstart9,  aseqdiff9, keynum);
}



void RotCameraMatrix(sceVu0FMATRIX m, sceVu0FVECTOR p, sceVu0FVECTOR zd,
		     sceVu0FVECTOR yd, sceVu0FVECTOR rot)
{
    sceVu0FMATRIX work;
    sceVu0FVECTOR direction, vertical, position;

    sceVu0UnitMatrix(work);
    sceVu0RotMatrixX(work,work,rot[0]);
    sceVu0RotMatrixY(work,work,rot[1]);
    sceVu0RotMatrixZ(work,work,rot[2]);
    sceVu0ApplyMatrix(direction, work, zd);
    sceVu0ApplyMatrix(vertical, work, yd);
    sceVu0ApplyMatrix(position, work, p);
    sceVu0CameraMatrix(m, position, direction, vertical);
}
