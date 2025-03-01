/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *                    Basic Sample Program
 *                        Version 0.30
 *                            EUC
 * 
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 * 
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.30           Jun,12,1999     shino       VU0 version
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

// vu0.c function
void sceVu0RotTransPersColN(u_long128 *v0, sceVu0FMATRIX m0, sceVu0FVECTOR *v1, sceVu0FVECTOR *nv,
                sceVu0FVECTOR *st,sceVu0FVECTOR *c,sceVu0FMATRIX ll,sceVu0FMATRIX lc, int n);
void sceVu0RotTransPersClipGsColN(u_long128 *v0, sceVu0FMATRIX m0, sceVu0FVECTOR *v1, sceVu0FVECTOR *nv,
	sceVu0FVECTOR *st,sceVu0FVECTOR *c,sceVu0FMATRIX ll,sceVu0FMATRIX lc, int n);
void sceVu0RotCameraMatrix(sceVu0FMATRIX m, sceVu0FVECTOR p, sceVu0FVECTOR zd,
	sceVu0FVECTOR yd, sceVu0FVECTOR rot);
void sceVu0ViewClipMatrix(sceVu0FMATRIX vc, float scrw, float scrh, float scrz, float zmin, float zmax);
int sceVu0ClipBBox(sceVu0FMATRIX mc, sceVu0FVECTOR *vm);

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 224
#define SCREEN_DEPTH 512.0f
#define PI (3.141592f)
#define IMAGE_SIZE   256
#if 1
#define CLIP_WIDTH 540.0f
#define CLIP_HEIGHT 180.0f
#else
#define CLIP_WIDTH (float)SCREEN_WIDTH
#define CLIP_HEIGHT (float)SCREEN_HEIGHT
#endif
#define NEAR_CLIP 40.0f
#define FAR_CLIP 100.0f

#define PIXCEL_ASPECT (((float)SCREEN_HEIGHT*4.0f)/((float)SCREEN_WIDTH*3.0f))

extern  u_int SampleCubeDataHead[];
extern  u_int SampleTorus1DataHead[];
extern  sceVu0FVECTOR SampleCubeBoundaryBox[];
extern  sceVu0FVECTOR SampleTorus1BoundaryBox[];
extern  u_int My_texture1[]  __attribute__((aligned(16)));

typedef struct {
  int         size;
  sceGifTag   giftag;
  sceGsTexEnv gs_tex;
}TexEnv;

typedef struct {
  int         size;
  sceGifTag   giftag;
  sceGsTest gs_test;
  long gs_test1addr;
  sceGsAlphaEnv gs_alpha;
}AlphaEnv;

typedef union {
  u_long128       ul128;
  u_long          ul64[2];
  u_int           ui32[4];
  sceVu0FVECTOR vect;
} QWdata;

typedef struct{
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
    sceVu0FVECTOR  *Bbox;
}ObjData;

sceVu0FVECTOR camera_p = { 0, 0, -80, 0 };
sceVu0FVECTOR camera_zd = { 0, 0, 1, 1 };
sceVu0FVECTOR camera_yd = { 0, 1, 0, 1 };
sceVu0FVECTOR camera_rot = { 0, 0, 0, 0 };

sceVu0FVECTOR light0 = { 0.0, 1.5, 0.5, 0 };
sceVu0FVECTOR light1 = { 1.5, -0.5, 0.5, 0 };
sceVu0FVECTOR light2 = { -1.5, -0.5, 0.5, 0 };

sceVu0FVECTOR color0  = { 0.8, 0.3, 0.3, 1 };
sceVu0FVECTOR color1  = { 0.3, 0.8, 0.3, 1 };
sceVu0FVECTOR color2  = { 0.3, 0.3, 0.8, 1 };

sceVu0FVECTOR ambient = { 0.2, 0.2, 0.2, 0 };

sceVu0FVECTOR obj_trans = { 0, 0, 0, 0 };
sceVu0FVECTOR obj_rot = { 0, 0, 0, 0 };

sceVu0FMATRIX local_rot;
sceVu0FMATRIX local_world;
sceVu0FMATRIX world_view;
sceVu0FMATRIX view_screen;
sceVu0FMATRIX local_screen;

sceVu0FMATRIX view_clip;
sceVu0FMATRIX local_clip;

sceVu0FMATRIX normal_light;
sceVu0FMATRIX light_color;
sceVu0FMATRIX local_light;
sceVu0FMATRIX local_color;

sceVu0FMATRIX work;

u_int paddata;
u_char rdata[32];
u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

sceGsDBuff      db;
sceGsLoadImage  pTex;
sceDmaChan      *dmaVif;
TexEnv          texenv;
AlphaEnv        alphaenv;

void LoadObj(ObjData *o, u_int *Head);
void ReleaseObj(ObjData *o);
void MakePacket(ObjData *obj, int num, int sw);
void MakeNearClipSurface(ObjData *obj, int num);
void MakeFarClipSurface(ObjData *obj, int num);
void MakeClipSurface(int sw);


void (*func[])() = {sceVu0RotTransPersColN,sceVu0RotTransPersClipGsColN};

int main()
{

    int     i,j,k,l;
    u_int   fr;
    float   delta = 1.0f * PI / 180.0f;
    ObjData         obj[2];
    int       obj_switch = 0;
    int       clip_switch = 0;
    int       sf_switch = 0;
    u_int	pre_button=0;
    int     odev;

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
    obj[0].Bbox=SampleCubeBoundaryBox;
    obj[1].Bbox=SampleTorus1BoundaryBox;

    sceGsResetPath();
    sceDmaReset(1);
    sceVpu0Reset();

    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT,
		     SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
    *(u_long *)&db.clear0.rgbaq
      = SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10, 0x80, 0x3f800000);
    *(u_long *)&db.clear1.rgbaq
      = SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10, 0x80, 0x3f800000);

    sceGsSetDefLoadImage(&pTex,
			 6720,
			 IMAGE_SIZE / 64,
			 SCE_GS_PSMCT32,
			 0, 0,
			 IMAGE_SIZE, IMAGE_SIZE);

    FlushCache(0);

    sceGsExecLoadImage(&pTex, (u_long128 *)My_texture1);
    sceGsSyncPath(0, 0);

    FlushCache(0);

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
    FlushCache(0);
    dmaVif = sceDmaGetChan(SCE_DMA_GIF);
    dmaVif->chcr.TTE = 1;
    sceDmaSendN(dmaVif, &texenv.giftag, texenv.size+1);

    sceGsSyncPath(0, 0);

    // set alpha env
    alphaenv.size = sceGsSetDefAlphaEnv(&alphaenv.gs_alpha, 0);
    SCE_GIF_CLEAR_TAG(&alphaenv.giftag);
    alphaenv.giftag.NLOOP = alphaenv.size;
    alphaenv.giftag.EOP = 1;
    alphaenv.giftag.NREG = 1;
    alphaenv.giftag.REGS0 = 0xe;
    *(u_long *) &alphaenv.gs_alpha.alpha1 = SCE_GS_SET_ALPHA(0, 2, 0, 1, 0);
    *(u_long *) &alphaenv.gs_test =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 3, 0x3);
    alphaenv.gs_test1addr = (long)SCE_GS_TEST_1;
    FlushCache(0);
    dmaVif = sceDmaGetChan(SCE_DMA_GIF);
    dmaVif->chcr.TTE = 1;
    sceDmaSendN(dmaVif, &alphaenv.giftag, texenv.size+1);

    sceGsSyncPath(0, 0);

    // view -> screen matrix

    sceVu0ViewScreenMatrix(view_screen, SCREEN_DEPTH, 1.0f, PIXCEL_ASPECT,
                             2048.0f, 2048.0f, 1.0f, 16777215.0f,
                             1.0f,65536.0f);

    sceVu0ViewClipMatrix(view_clip, CLIP_WIDTH, CLIP_HEIGHT/PIXCEL_ASPECT, 
				SCREEN_DEPTH, NEAR_CLIP, FAR_CLIP);


    scePadInit(0);
    scePadPortOpen(0, 0, pad_dma_buf);

    odev = !sceGsSyncV(0);
    fr = 0;

    do{
	pre_button = paddata;
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
             camera_p[2] -= delta*10; if(camera_p[2]<-150) camera_p[2] = -150; }
        if(paddata & SCE_PADR2){
             camera_p[2] += delta*10; if(camera_p[2]>-10) camera_p[2] = -10; }
        if(rdata[7] >0xc0){
             camera_p[1] -= delta*10.0f; if(camera_p[1]<-100) camera_p[1] = -100; }
        if(rdata[6] >0xc0){
             camera_p[0] -= delta*10.0f; if(camera_p[0]<-100) camera_p[0] = -100; }
        if(rdata[7] <0x40){
             camera_p[1] += delta*10.0f; if(camera_p[1]>100) camera_p[1] = 100; }
        if(rdata[6] <0x40){
             camera_p[0] += delta*10.0f; if(camera_p[0]>100) camera_p[0] = 100; }

        if((paddata & SCE_PADi & ~pre_button)){
             clip_switch = clip_switch^0x02; }
        if((paddata & SCE_PADj & ~pre_button)){
             sf_switch = sf_switch^0x01;  }

        if((paddata & SCE_PADselect & ~pre_button)){
             obj_switch = obj_switch^0x01;}
        if((paddata & SCE_PADstart & ~pre_button)){
             clip_switch = clip_switch^0x01; }

        FlushCache(0);
	sceGsSwapDBuff(&db, fr);
	sceGsSyncPath(0, 0);

        // local -> world (rotate)matrix
	sceVu0UnitMatrix(work);
	sceVu0RotMatrix(local_rot, work, obj_rot);

	// color&normal matrix setting
	sceVu0NormalLightMatrix(normal_light, light0, light1, light2);
	sceVu0LightColorMatrix(light_color, color0, color1, color2, ambient);
	
	for(j=0;j<3;j++)
	{
	    for(k=0;k<3;k++)
	    {
	        for(l=0;l<3;l++)
	        {
		    obj_trans[0]= 20.0f*(float)(j-1);
		    obj_trans[1]= 20.0f*(float)(k-1);
		    obj_trans[2]= 20.0f*(float)(l-1);

		    // light(normal) -> local_light matrix
	    	    sceVu0MulMatrix(local_light, normal_light, local_rot);

		    // local -> world (rotate&translate)matrix
		    sceVu0TransMatrix(local_world, local_rot, obj_trans);

		    // world -> view matrix
		    sceVu0RotCameraMatrix(world_view, camera_p, camera_zd, camera_yd,camera_rot);

		    // local -> screen matrix
		    sceVu0MulMatrix(work, world_view, local_world);
		    sceVu0MulMatrix(local_screen, view_screen, work);

		    sceVu0MulMatrix(local_clip, view_clip, work);

		    if(sceVu0ClipBBox(local_clip, obj[obj_switch].Bbox) & clip_switch) continue;

		    for(i=0; i<obj[obj_switch].blockNum; i++){

		        MakePacket(&obj[obj_switch], i, clip_switch >>1);
		        FlushCache(0);
		        sceDmaSend(dmaVif, (u_long128*)((u_int)obj[obj_switch].pack[i].buf|0x80000000));
		        sceGsSyncPath(0, 0);
		    }
	        }
	    }
	}
	if(clip_switch & 1)
		MakeClipSurface(sf_switch);

        sceGsSetHalfOffset((fr&1)? &db.draw1:&db.draw0,2048,2048,odev);
	fr++;
        odev = !sceGsSyncV(0);


    }while(1);

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

void MakePacket(ObjData *obj, int num,int sw)
{
    GifPacket *pack = &obj->pack[num];
    sceVu0FVECTOR *vertex, *normal, *texUV, *color;
    vertex  = (sceVu0FVECTOR *)obj->pData[num];
    normal  = (sceVu0FVECTOR *)(obj->pData[num]+obj->vertexNum[num]*4);
    texUV   = (sceVu0FVECTOR *)(obj->pData[num]+obj->vertexNum[num]*4*2);
    color   = (sceVu0FVECTOR *)(obj->pData[num]+obj->vertexNum[num]*4*3);

  // make packet(PACKED mode)
    pack->size = 0;
    pack->buf = (QWdata *)0x70000000;

  // add DMAtag
    pack->buf[pack->size].ul128 = (u_long128)0;
    pack->buf[pack->size++].ui32[0] = 0x70000000 | (obj->vertexNum[num]*3 + 1);

  // add vertex info(GIFtag,STQ&RGBA&XYZ)
    pack->buf[pack->size].ul64[0]
      = SCE_GIF_SET_TAG(obj->vertexNum[num], 1, 1,obj->prim,SCE_GIF_PACKED,3);
    pack->buf[pack->size++].ul64[1] = 0x0412L;

    switch(sw)
    {
	case 0:
	    func[0](&pack->buf[pack->size].ul128, local_screen, vertex,
		 normal, texUV,color, local_light,light_color, obj->vertexNum[num]);
	    break;
	case 1:
	    func[1](&pack->buf[pack->size].ul128, local_screen, vertex,
		 normal, texUV,color, local_light,light_color, obj->vertexNum[num]);
	    break;
	default :
	    break;
    }
}

void MakeClipSurface(int sw)
{
    GifPacket pack;
    
    QWdata buf[10];
    float surface[2]={NEAR_CLIP, FAR_CLIP};
	
    pack.buf = buf;

//   make packet(PACKED mode)
    pack.size = 0;
    pack.buf = (QWdata *)0x70000000;

  // add DMAtag
    pack.buf[pack.size].ul128 = (u_long128)0;
    pack.buf[pack.size++].ui32[0] = (0x70000000 | 4);

  // add vertex info(GIFtag,STQ&RGBA&XYZ)
    pack.buf[pack.size].ul64[0]
      = SCE_GIF_SET_TAG(1, 1, 1,0x46,SCE_GIF_PACKED,3);
    pack.buf[pack.size++].ul64[1] = 0x0441L;


  // RGBAQ
    pack.buf[pack.size].ui32[0] = 128;
    pack.buf[pack.size].ui32[1] = 128;
    pack.buf[pack.size].ui32[2] = 128;
    if(sw==0)
        pack.buf[pack.size++].ui32[3] = 96;
    else
        pack.buf[pack.size++].ui32[3] = 128;
	
  // (x0,y0,z0)
    pack.buf[pack.size].vect[0] = -CLIP_WIDTH/2.0f+2048.0f;
    pack.buf[pack.size].vect[1] = -CLIP_HEIGHT/2.0f+2048.0f;
    pack.buf[pack.size].vect[2] = view_screen[2][2]+view_screen[3][2]/surface[sw];
    pack.buf[pack.size].vect[3] = 0.0f;

    sceVu0FTOI4Vector(*(sceVu0IVECTOR *)&(pack.buf[pack.size].vect),pack.buf[pack.size].vect);
    pack.size++;
    
  // (x1,y1,z0)
    pack.buf[pack.size].vect[0] = CLIP_WIDTH/2.0f+2048.0f;
    pack.buf[pack.size].vect[1] = CLIP_HEIGHT/2.0f+2048.0f;
    pack.buf[pack.size].vect[2] = view_screen[2][2]+view_screen[3][2]/surface[sw];
    pack.buf[pack.size].vect[3] =0.0f; 

    sceVu0FTOI4Vector(*(sceVu0IVECTOR *)&(pack.buf[pack.size].vect),pack.buf[pack.size].vect);

    FlushCache(0);
    sceDmaSend(dmaVif, (u_long128*)((u_int)pack.buf|0x80000000));
    sceGsSyncPath(0, 0);
}
