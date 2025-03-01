/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - ezcube -
 *
 *                         Version 0.30
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            main.c
 *                     main function of cube
 *
 *       Version        Date             Design      Log
 *  --------------------------------------------------------------------
 *      0.10            July,14,1999     ywashizu    
 *      0.20            Aug, 12,1999     ywashizu    new IPU movie format
 *      0.30            Oct, 15,1999     ywashizu    new IPU movie format
 */


#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <libdev.h>
#include <libipu.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <libpkt.h>
#include <libvu0.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <libpad.h>

#include "ezmovie.h"
#include "ezcube.h"

// data of uncache access
static u_int path3tag[bound(PK_SIZE*4, 64)]             bss_align(64);
static u_int bsTags[(MAX_BsDataSize/(BSDMASIZE) + 1)*4] bss_align(64);
static sceIpuRGB32 cscBuff[MAX_MBX * MAX_MBY]           bss_align(64);
static u_char bsData[MAX_BsDataSize]                    bss_align(64);

static void setToIPUtags(u_int *tag, u_char *p, int size);
static void setDMAtoIPU(u_int *tag);
static void setDMAfromIPU(sceIpuRGB32 *rgb32, int count);

void setLoadImageTags(u_int *tags, void *image, IDEC_MOVIE movie);
void setLoadImageTagsTile(u_int *tags, void *image, IDEC_MOVIE movie);
void loadImage(u_int *tags);

extern inline void scTag( u_int *p, void *addr, u_int id, u_int qwc)
{
    *(u_long*)p = (u_long)(u_int)addr << 32 
	| (u_long)id << 28 
	| (u_long)qwc << 0;
}

static int readFile(char *filename, unsigned char *data, int* filesize);

static u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

int main(){
    static sceVu0FVECTOR camera_p = { 0.0f, 0.0f, -800.0f, 1.0f };
    static sceVu0FVECTOR camera_zd = { 0.0f, 0.0f, 1.0f, 1.0f };
    static sceVu0FVECTOR camera_yd = { 0.0f, 1.0f, 0.0f, 1.0f };
    static sceVu0FVECTOR camera_rot = { 0.0f, 0.0f, 0.0f, 1.0f };

    static sceVu0FVECTOR light0 = { 0.0f, 1.0f, 0.0f, 1.0f };
    static sceVu0FVECTOR light1 = { 0.0f, 0.0f, 1.0f, 1.0f };
    static sceVu0FVECTOR light2 = { 1.0f, 0.0f, 0.0f, 1.0f };

    static sceVu0FVECTOR color0  = { 1.0f, 1.0f, 1.0f, 1.0f };
    static sceVu0FVECTOR color1  = { 0.0f, 0.0f, 1.0f, 1.0f };
    static sceVu0FVECTOR color2  = { 1.0f, 0.0f, 0.0f, 1.0f };

    static sceVu0FVECTOR ambient = { 0.3f, 0.3f, 0.3f, 0.0f };

    static sceVu0FVECTOR trans = { TRANS_X, TRANS_Y, TRANS_Z, TRANS_W };
    static sceVu0FVECTOR rot = { ROT_X, ROT_Y, ROT_Z, ROT_W };

    int i;
    u_int fr;
    int ret;
    float delta = 1.0f * PI / 180.0f;

    IDEC_MOVIE movie;

    u_int paddata;
    u_char rdata[32];

    sceGsDBuff db;
    sceVif1Packet packet;
    sceDmaChan *dmaVif;

    sceVu0FVECTOR v1[8];
    sceVu0IVECTOR v0[8];
    sceVu0IVECTOR c0[6];
    sceVu0FVECTOR c1[6];
    sceVu0IVECTOR gray; 
    sceVu0IVECTOR wall[8];
    sceVu0FVECTOR wallF[8];

    sceVu0FMATRIX RotMatrix;
    sceVu0FMATRIX WorldLocalMatrix;
    sceVu0FMATRIX ViewWorldMatrix;
    sceVu0FMATRIX ScreenViewMatrix;
    sceVu0FMATRIX ScreenWorldMatrix;
    sceVu0FMATRIX ScreenLocalMatrix;

    sceVu0FMATRIX NormalLightMatrix;
    sceVu0FMATRIX LightColorMatrix;
    sceVu0FMATRIX LocalLightMatrix;

    sceSifInitRpc(0);
    while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
			0, NULL) < 0){
	printf("Can't load module sio2man\n");
    }
    while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
			0, NULL) < 0){
	printf("Can't load module padman\n");
    }

    ret = readFile(BSFILE, bsData, &movie.datasize);
    if (!ret) {
	return 1;
    }

    //  Read IPU movie Header and Set movie data
    ReadIPUHeader(&movie, bsData, TEX1);

    sceGsResetPath();
    sceDmaReset(1);

    // get DMA channel number
    dmaVif = sceDmaGetChan(SCE_DMA_VIF1);
    dmaVif->chcr.TTE = 1;

    // packet initialization
    sceVif1PkInit(&packet, (u_long128*)0x70000000);

    // GS initialization
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

    // setting double buffer
    sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT,\
		     SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);

    // update to memory for the value of sceGsSetDefDBuff 
    FlushCache(0);

    // setting vertex and color of cube
    SetCubeVertex(v1);
    SetObjectColor(c1);

    // setting vertex and color of wire frame
    SetWallVertex(wallF);
    SETVECTOR(gray,0x38, 0x38, 0x38, 0x80);

    // initialize GS registers
    SetGsRegs(1, 1);

    // waiting for odd field 
    while(sceGsSyncV(0) == 0);

    // open pad device
    scePadInit(0);
    scePadPortOpen(0, 0, pad_dma_buf);
    
    // making matrix for lighting calculation
    sceVu0NormalLightMatrix( NormalLightMatrix, light0, light1, light2);
    sceVu0LightColorMatrix( LightColorMatrix, color0, color1, color2, ambient);

    // initialize IPU and IPU library
    sceIpuInit();

    // set DMA tags for path3 (uncached access)
    setLoadImageTags(path3tag, cscBuff, movie);
 
    // set DMA tags for IPU (uncached access)
    setToIPUtags(bsTags, bsData + 16, movie.datasize);

    fr= 0;

    for (i = 0; ; i++) {
	int oddeven;
	
	sceIpuReset();	                        // reset IPU
	sceIpuSync(0, 0);
	
	sceIpuBCLR(0);	                        // reset IPU_in_FIFO
	sceIpuSync(0, 0);
	
	setDMAtoIPU(bsTags);                 	// set DMA to IPU
	
		
	// play one sequence ... main loop
	for(movie.frame_cnt=0;movie.frame_cnt<movie.nframes;movie.frame_cnt++){
	    const u_long giftag_cube[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 1, 14),
					    0x0051251251251206L };
	    const u_long giftag_wall[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 1,  4),
					    0x0000000000005510L };
	    u_int j;
	    u_int flag;

	    // read pad information
	    if(scePadRead(0, 0, rdata) > 0){
		paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
	    }
	    else{
		paddata = 0;
	    }

	    if(paddata & SCE_PADLdown){ 
		rot[0] += delta; if(rot[0] >  PI){ rot[0] -= 2.0f * PI; } 
	    }
	    if(paddata & SCE_PADLup){
		rot[0] -= delta; if(rot[0] < -PI){ rot[0] += 2.0f * PI; }
	    }
	    if(paddata & SCE_PADLleft){
		rot[1] += delta; if(rot[1] >  PI){ rot[1] -= 2.0f * PI; } 
	    }
	    if(paddata & SCE_PADLright){
		rot[1] -= delta; if(rot[1] < -PI){ rot[1] += 2.0f * PI; }
	    }
	    if(paddata & SCE_PADL1){
		rot[2] += delta; if(rot[2] > PI){ rot[2] -= 2.0f * PI; }
	    }
	    if(paddata & SCE_PADL2){
		rot[2] -= delta; if(rot[2] < -PI){ rot[2] += 2.0f * PI; }
	    }

	    if(paddata & SCE_PADR1){
		if(trans[2] > -500){ trans[2] -= 10.0f;}
	    }
	    if(paddata & SCE_PADR2){
		if(trans[2] < 120){ trans[2] += 10.0f;}
	    }
					
	    if(paddata & SCE_PADRleft){
		camera_rot[1]+=delta;if(camera_rot[1]>=PI){camera_rot[1]=-PI;}
	    }
	    if(paddata & SCE_PADRright){
		camera_rot[1]-=delta;if(camera_rot[1]<=-PI){camera_rot[1]=PI;}
	    }
	    if(paddata & SCE_PADRdown){
		camera_rot[0]+=delta;if(camera_rot[0]>=PI){camera_rot[0]=-PI;}
	    }
	    if(paddata & SCE_PADRup){
		camera_rot[0]-=delta;if(camera_rot[0]<=-PI){camera_rot[0]=PI;}
	    }

	    if(paddata & SCE_PADstart){
		rot[0]=ROT_X;
		rot[1]=ROT_Y;
		rot[2]=ROT_Z;
		trans[2]=TRANS_Z;
		camera_rot[0] = 0.0f;
		camera_rot[1] = 0.0f;
	    }

	    sceVu0UnitMatrix(RotMatrix);
	    sceVu0RotMatrix(RotMatrix, RotMatrix, rot);

	    // set DMA from IPU 
	    setDMAfromIPU(cscBuff, movie.mbx*movie.mby);

	    // get flag data
	    flag = getFlags();

	    DPUT_IPU_CTRL(IPU_CTRL_VAL(flag));         	// Set IPU_CTRL

	    // IDEC command
	    sceIpuIDEC( 0, 0, 0, DTD(flag), 1, 0);
	    // RGB32: no dither : no offset : no dt decode : qsc : fb

	    FlushCache(0);
	    sceGsSyncPath(0, 0);

	    // packet initialization
	    sceVif1PkReset(&packet);
	    sceVif1PkCnt(&packet, 0);
	    sceVif1PkOpenDirectCode(&packet, 0);

	    // make matrix transforming from Local to World
	    sceVu0TransMatrix(WorldLocalMatrix, RotMatrix, trans);
	    // make lighting matrix
	    sceVu0MulMatrix(LocalLightMatrix, NormalLightMatrix,
			    WorldLocalMatrix);

	    // make matrix transforming from World to View
	    create_viewworld(ViewWorldMatrix, camera_rot, camera_p,
			     camera_zd, camera_yd);

	    /* 透視変換マトリクスを生成 */
	    sceVu0ViewScreenMatrix(ScreenViewMatrix, 512.0f, 1.0f, 0.47f,
				   2048.0f, 2048.0f, 1.0f, 16777215.0f,
				   64.0f,65536.0f);
	    sceVu0MulMatrix(ScreenWorldMatrix, ScreenViewMatrix,
			    ViewWorldMatrix);
	    sceVu0MulMatrix(ScreenLocalMatrix, ScreenWorldMatrix,
			    WorldLocalMatrix);

	    /* 頂点を透視変換 */
	    for(j = 0 ; j < 8 ; j++){
		sceVu0RotTransPers(v0[j], ScreenLocalMatrix, v1[j], 1);
	    }

	    for(j = 0 ; j < 8 ; j++){
		sceVu0RotTransPers(wall[j], ScreenWorldMatrix, wallF[j], 1);
	    }

	    SetRGB( c0, LocalLightMatrix, LightColorMatrix, v1, c1);

	    // add packets of cube by using REGLIST mode
	    sceVif1PkOpenGifTag(&packet, *(u_long128*)&giftag_cube);
	    set_square_packet(&packet,
			      c0[0], v0[5], v0[3], v0[7], v0[1], movie );
	    set_square_packet(&packet,
			      c0[1], v0[0], v0[6], v0[1], v0[7], movie );
	    set_square_packet(&packet,
			      c0[2], v0[2], v0[4], v0[0], v0[6], movie );
	    set_square_packet(&packet,
			      c0[3], v0[3], v0[2], v0[1], v0[0], movie );
	    set_square_packet(&packet,
			      c0[4], v0[3], v0[5], v0[2], v0[4], movie );
	    set_square_packet(&packet,
			      c0[5], v0[4], v0[5], v0[6], v0[7], movie );

	    sceVif1PkCloseGifTag(&packet);         // close GIF Tag

	    // add packets of frame
	    sceVif1PkOpenGifTag(&packet, *(u_long128*)&giftag_wall);
	    set_line_packet(&packet, gray, wall[0], wall[6] );
	    set_line_packet(&packet, gray, wall[0], wall[2] );
	    set_line_packet(&packet, gray, wall[2], wall[4] );
	    set_line_packet(&packet, gray, wall[4], wall[6] );
	    set_line_packet(&packet, gray, wall[2], wall[3] );
	    set_line_packet(&packet, gray, wall[3], wall[1] );
	    set_line_packet(&packet, gray, wall[1], wall[0] );
	    set_line_packet(&packet, gray, wall[4], wall[5] );
	    set_line_packet(&packet, gray, wall[5], wall[7] );
	    set_line_packet(&packet, gray, wall[7], wall[6] );
	    set_line_packet(&packet, gray, wall[5], wall[3] );
	    set_line_packet(&packet, gray, wall[1], wall[7] );
	    sceVif1PkCloseGifTag(&packet);	   // close GIF Tag

	    sceVif1PkCloseDirectCode(&packet);
	    sceVif1PkEnd(&packet, 0);
	    sceVif1PkTerminate(&packet);
	    // sceVif1PkDump(&packet);       
	    // FlushCache(0);               

	    /* Waiting for the start of V-Blank */
	    oddeven = !sceGsSyncV(0);
	
	    /* Add half pixel to offset address for interlace */
	    sceGsSetHalfOffset((fr&1)?(&db.draw1):(&db.draw0),
			       2048, 2048, oddeven);

	    // change display buffer
	    FlushCache(0);
	    sceGsSyncPath(0, 0);
	    sceGsSwapDBuff(&db, fr);
	    fr++;

	    sceIpuSync(0, 0);               // wait IDEC command to complete

	    while((*D3_CHCR>>8) & 0x01){    // wait until DMA from IPU finishes
		;
	    }

	    // skip start code
	    sceIpuFDEC(32);	              // fdec 32 bit
	    sceIpuSync(0, 0);

	    sceGsSyncPath(0, 0);

	    // load decompressed image to the texture buffer
	    loadImage(path3tag);

	    sceGsSyncPath(0, 0);
	    // DMA start
	    sceDmaSend(dmaVif,
		       (u_long128*)(((u_int)packet.pBase&0x3ff0)|0x80000000));
	}
    }
  printf("finish!\n");
  return 0;
}

// function to read file
// 'data' buffer needs to be big enough to read whole of the file
int readFile(char *filename, unsigned char *data, int* filesize)
{
    int fd;
    int ret = 1;
    int readcount;

    // open a file to read and check its size
    fd = sceOpen(filename, SCE_RDONLY);
    if (fd < 0) {
	printf("Cannot open %s\n", filename);
	return 0;
    }
    *filesize = sceLseek(fd, 0, SCE_SEEK_END);
    if (*filesize < 0) {
	printf("sceLseek() fails (%s)\n", filename);
	sceClose(fd);
	return 0;
    }
    ret = sceLseek(fd, 0, SCE_SEEK_SET);
    if (ret < 0) {
	printf("sceLseek() fails (%s)\n", filename);
	sceClose(fd);
	return 0;
    }
    
    printf("read %s(%dbytes) ... wait ...\n", filename, *filesize);
    
    // read bs data
    readcount = sceRead(fd, data, *filesize);
    if (readcount != *filesize) {
	printf("Cannot read %s\n", filename);
	sceClose(fd);
	return 0;
    }
    
    printf("done\n");
    
    sceClose(fd);
    return 1;
}

static void setToIPUtags(u_int *tag, u_char *p, int size)
{
    int chunksize;
    u_int *unctag = (u_int*)UncachedAddr(tag);
    u_char *dmap = DmaAddr(p);
    
    while (size > 0) {
	chunksize = min(size, BSDMASIZE);
	size -= chunksize;
	scTag(
	      unctag,		        // u_int *p,
	      dmap,	            	// u_int addr,
	      (size)? 3: 0,       	// u_int id,(ref/refe)
	      bound(chunksize,16)/16	// qwc
	      );
	unctag += 4;
	dmap += chunksize;
    }
}

static void setDMAtoIPU(u_int *tag)
{
    DPUT_D4_TADR((u_int)DmaAddr(tag));
    DPUT_D4_QWC(0);
    DPUT_D4_CHCR(0x105);
}

static void setDMAfromIPU(sceIpuRGB32 *rgb32, int count)
{
    DPUT_D3_MADR((u_int)DmaAddr(rgb32));
    DPUT_D3_QWC(count*sizeof(sceIpuRGB32)/16);
    DPUT_D3_CHCR(0x100);	                     // STR: 1, DIR: 0
}


