/* SCEI CONFIDENTIAL
   "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
   */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - cube -
 *
 *                         Version 1.10
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            main.c
 *                     main function of cube
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      1.10            July,21,1999     ywashizu    
 */


#include <eekernel.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <libdev.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <libpkt.h>
#include <libvu0.h>
#include <sifdev.h>
#include <sifrpc.h>
#include <libpad.h>

#define PKTSIZE 0x10000

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 224
#define PI (3.141592f)

#define SETVECTOR(v,x,y,z,w) ((v)[0]=x,(v)[1]=y,(v)[2]=z,(v)[3]=w)

#define CUBE_HEIGHT_H 80.0f
#define CUBE_WIDTH_H  80.0f
#define CUBE_DEPTH_H  80.0f

#define WALL_HEIGHT_H 150.0f
#define WALL_WIDTH_H  200.0f
#define WALL_DEPTH_H  250.0f

#define CUBE_AA1 0                        // Anti-Aliasing for cube OFF 
#define WALL_AA1 1                        // Anti-Aliasing for frame ON 

void SetRGB(sceVu0IVECTOR* c0, sceVu0FMATRIX LocalLightMatrix, 
	    sceVu0FMATRIX LightColorMatrix, sceVu0FVECTOR* v1,
	    sceVu0FVECTOR* c1);
void SetCubeVertex(sceVu0FVECTOR* v1);
void SetWallVertex(sceVu0FVECTOR* wallF);
void SetObjectColor(sceVu0FVECTOR* c1);
void create_normal(sceVu0FVECTOR normal, sceVu0FVECTOR v0, 
		   sceVu0FVECTOR v1, sceVu0FVECTOR v2);
void light_rgb(sceVu0IVECTOR c0, sceVu0FMATRIX LocalLightMatrix, 
	       sceVu0FMATRIX LightColorMatrix, sceVu0FVECTOR v0, 
	       sceVu0FVECTOR v1, sceVu0FVECTOR v2, sceVu0FVECTOR c1);
void create_viewworld(sceVu0FMATRIX ViewWorldMatrix, sceVu0FVECTOR camera_rot,
		      sceVu0FVECTOR camera_p, sceVu0FVECTOR camera_zd, 
		      sceVu0FVECTOR camera_yd);
void set_square_packet(sceVif1Packet* packet, sceVu0IVECTOR c, 
		       sceVu0IVECTOR v0, sceVu0IVECTOR v1, sceVu0IVECTOR v2,
		       sceVu0IVECTOR v3);
void set_line_packet(sceVif1Packet* packet, sceVu0IVECTOR c,
		     sceVu0IVECTOR v0, sceVu0IVECTOR v1);

u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

int main(){
    u_int fr;
    float rot_x = 0.0f, rot_y = 0.0f, rot_z = 0.0f, rot_w = 0.0f;
    float trans_x = 0.0f, trans_y = 0.0f, trans_z = 0.0f, trans_w = 0.0f;
    float delta = 1.0f * PI / 180.0f;

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

    sceVu0FVECTOR camera_p = { 0.0f, 0.0f, -800.0f, 1.0f };
    sceVu0FVECTOR camera_zd = { 0.0f, 0.0f, 1.0f, 1.0f };
    sceVu0FVECTOR camera_yd = { 0.0f, 1.0f, 0.0f, 1.0f };
    sceVu0FVECTOR camera_rot = { 0.0f, 0.0f, 0.0f, 1.0f };

    sceVu0FVECTOR light0 = { 0.0f, 1.0f, 0.0f, 1.0f };
    sceVu0FVECTOR light1 = { 0.0f, 0.0f, 1.0f, 1.0f };
    sceVu0FVECTOR light2 = { 1.0f, 0.0f, 0.0f, 1.0f };

    sceVu0FVECTOR color0  = { 0.0f, 0.0f, 1.0f, 1.0f };
    sceVu0FVECTOR color1  = { 0.0f, 1.0f, 0.0f, 1.0f };
    sceVu0FVECTOR color2  = { 1.0f, 0.0f, 0.0f, 1.0f };

    sceVu0FVECTOR ambient = { 0.3f, 0.3f, 0.3f, 0.0f };

    sceVu0FMATRIX RotMatrix;
    sceVu0FMATRIX WorldLocalMatrix;
    sceVu0FMATRIX ViewWorldMatrix;
    sceVu0FMATRIX ScreenViewMatrix;
    sceVu0FMATRIX ScreenWorldMatrix;
    sceVu0FMATRIX ScreenLocalMatrix;

    sceVu0FVECTOR trans = { trans_x, trans_y, trans_z, trans_w };
    sceVu0FVECTOR rot = { rot_x, rot_y, rot_z, rot_w };

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

    // initialization
    sceGsResetPath();
    sceDmaReset(1);

    // get DMA channel number
    dmaVif = sceDmaGetChan(SCE_DMA_VIF1);
    dmaVif->chcr.TTE = 1;

    // initialize packet
    sceVif1PkInit(&packet, (u_long128*)0x70000000);

    // initialize GS
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

    // set up double buffer
    sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT,
		     SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);

    // update to memory for the value of sceGsSetDefDBuff 
    FlushCache(0);

    // setting vertex and color of cube
    SetCubeVertex(v1);
    SetObjectColor(c1);

    // setting vertex and color of wire frame
    SetWallVertex(wallF);
    SETVECTOR(gray,0x38, 0x38, 0x38, 0x80);

    {
      const u_long giftag0[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1),
				  0x000000000000000eL };
      sceGifPacket packetGS;
      sceDmaChan *dmaGif;
      u_long128* work = (u_long128*)malloc(PKTSIZE * sizeof(u_long128));

      sceGifPkInit(&packetGS, work);

      // get DMA channel number
      dmaGif = sceDmaGetChan(SCE_DMA_GIF);

      sceGifPkReset(&packetGS);                  // reset packet
      sceGifPkCnt(&packetGS, 0, 0, 0);           // add CNT Tag
      sceGifPkOpenGifTag(&packetGS, *(u_long128*)&giftag0);//add GIF Tag
      sceGifPkAddGsAD(&packetGS, SCE_GS_ALPHA_1, SCE_GS_SET_ALPHA(0,1,0,1,0));
      sceGifPkCloseGifTag(&packetGS);
      sceGifPkEnd(&packetGS, 0, 0, 0);	        // add END Tag
      sceGifPkTerminate(&packetGS);	        // finish making packet

      FlushCache(0);
      sceDmaSend(dmaGif, (u_long128*)((u_int)packetGS.pBase));   // DMA Start
      sceGsSyncPath(0, 0);
      
      free(work);
    }

    // making matrix for lighting calculation
    sceVu0NormalLightMatrix( NormalLightMatrix, light0, light1, light2);
    sceVu0LightColorMatrix( LightColorMatrix, color0, color1, color2, ambient);

    // wait for odd field
    while(sceGsSyncV(0) == 0);

    // open pad device
    scePadInit(0);
    scePadPortOpen(0, 0, pad_dma_buf);

    fr = 0;
    do{
	int oddeven;
	const u_long giftag_cube[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 1, 9),
					0x0000000515151510L };
	const u_long giftag_wall[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 1,  4),
					0x0000000000005510L };
	u_int j;
	
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
	    rot[0]=rot_x;
	    rot[1]=rot_y;
	    rot[2]=rot_z;
	    trans[2]=trans_z;
	    camera_rot[0] = 0.0f;
	    camera_rot[1] = 0.0f;
	}
	
	sceVu0UnitMatrix(RotMatrix);
	sceVu0RotMatrix(RotMatrix, RotMatrix, rot);
	
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
	
	// make matrix to transform from View to Screen
	sceVu0ViewScreenMatrix(ScreenViewMatrix, 512.0f, 1.0f, 0.47f,
			       2048.0f, 2048.0f, 1.0f, 16777215.0f,
			       64.0f,65536.0f);
	sceVu0MulMatrix(ScreenWorldMatrix, ScreenViewMatrix,
			ViewWorldMatrix);
	sceVu0MulMatrix(ScreenLocalMatrix, ScreenWorldMatrix,
			WorldLocalMatrix);
	
	/* ’¸“_‚ð“§Ž‹•ÏŠ· */
	for(j = 0 ; j < 8 ; j++){
	    sceVu0RotTransPers(v0[j], ScreenLocalMatrix, v1[j], 1);
	}
	
	for(j = 0 ; j < 8 ; j++){
	    sceVu0RotTransPers(wall[j], ScreenWorldMatrix, wallF[j], 1);
	}
	
	SetRGB( c0, LocalLightMatrix, LightColorMatrix, v1, c1);
	
	// add packets of cube by using REGLIST mode
	sceVif1PkOpenGifTag(&packet, *(u_long128*)&giftag_cube);
	set_square_packet(&packet, c0[0], v0[5], v0[3], v0[7],
			  v0[1]);
	set_square_packet(&packet, c0[1], v0[0], v0[6], v0[1],
			  v0[7]);
	set_square_packet(&packet, c0[2], v0[2], v0[4], v0[0],
			  v0[6]);
	set_square_packet(&packet, c0[3], v0[3], v0[2], v0[1],
			  v0[0]);
	set_square_packet(&packet, c0[4], v0[3], v0[5], v0[2],
			   v0[4]);
	set_square_packet(&packet, c0[5], v0[4], v0[5], v0[6],
			   v0[7]);
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
	
	sceGsSyncPath(0, 0);
	// DMA start
	sceDmaSend(dmaVif,
		   (u_long128*)(((u_int)packet.pBase&0x3ff0)|0x80000000));
    }while(1);

    printf("finish!\n");
    return 0;
}

// function to make normal
void create_normal(sceVu0FVECTOR normal, sceVu0FVECTOR v0,
		   sceVu0FVECTOR v1, sceVu0FVECTOR v2){
    sceVu0FVECTOR work1, work2;
    sceVu0SubVector(work1, v1, v0);
    sceVu0SubVector(work2, v2, v0);
    sceVu0OuterProduct(normal, work1, work2);
}

// set the value of RGBA after lighting calculation
void light_rgb(sceVu0IVECTOR c0, sceVu0FMATRIX LocalLightMatrix,
	       sceVu0FMATRIX LightColorMatrix, sceVu0FVECTOR v0, 
	       sceVu0FVECTOR v1, sceVu0FVECTOR v2, sceVu0FVECTOR c1){
    sceVu0FVECTOR vector;
    
    create_normal(vector, v0, v2, v1);
    sceVu0Normalize(vector, vector);

    sceVu0ApplyMatrix(vector, LocalLightMatrix, vector);
    sceVu0ClampVector(vector, vector, 0.0f, 1.0f);
    vector[3] = 1.0f;
    sceVu0ApplyMatrix(vector, LightColorMatrix, vector);
    sceVu0ClampVector(vector, vector, 0.0f, 1.0f);
    sceVu0MulVector(vector, c1, vector);

    sceVu0FTOI0Vector(c0, vector);
}

//function to calculate ViewWorlMatrix
void create_viewworld(sceVu0FMATRIX ViewWorldMatrix,
		      sceVu0FVECTOR camera_rot, sceVu0FVECTOR camera_p, 
		      sceVu0FVECTOR camera_zd, sceVu0FVECTOR camera_yd){
    sceVu0FMATRIX work;
    sceVu0FVECTOR camera_p2, camera_zd2, camera_yd2;
    sceVu0UnitMatrix(work);
    sceVu0RotMatrixX(work,work,camera_rot[0]);
    sceVu0RotMatrixY(work,work,camera_rot[1]);
    sceVu0ApplyMatrix(camera_zd2, work, camera_zd);
    sceVu0ApplyMatrix(camera_yd2, work, camera_yd);
    sceVu0ApplyMatrix(camera_p2, work, camera_p);
    sceVu0CameraMatrix(ViewWorldMatrix, camera_p2, camera_zd2, camera_yd2);
}

// make cube packet and set texture routine
void set_square_packet(sceVif1Packet* packet,  sceVu0IVECTOR c,
			sceVu0IVECTOR v0, sceVu0IVECTOR v1, sceVu0IVECTOR v2,
			sceVu0IVECTOR v3){
	float q[4];
    q[0] = 1.0f / (float)v0[3];
    q[1] = 1.0f / (float)v1[3];
    q[2] = 1.0f / (float)v2[3];
    q[3] = 1.0f / (float)v3[3];
    sceVif1PkAddGsData(packet, SCE_GS_SET_PRIM(0x04,0,0,0,0,CUBE_AA1,0,0,0));
    sceVif1PkAddGsData(packet, SCE_GS_SET_RGBAQ(c[0], c[1],
						c[2], 0x00,
						*(u_int*)&q[0]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_XYZ(v0[0], v0[1],
					      v0[2]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_RGBAQ(c[0], c[1],
						c[2], 0x00,
						*(u_int*)&q[1]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_XYZ(v1[0], v1[1],
					      v1[2]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_RGBAQ(c[0], c[1],
						c[2], 0x00,
						*(u_int*)&q[2]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_XYZ(v2[0], v2[1],
					      v2[2]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_RGBAQ(c[0], c[1],
						c[2],0x00,
						*(u_int*)&q[3]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_XYZ(v3[0], v3[1],
					      v3[2]));
}

// make frame packet routine
void set_line_packet(sceVif1Packet* packet, sceVu0IVECTOR c,
		     sceVu0IVECTOR v0,sceVu0IVECTOR v1 )
{
    float q = 1.0f ;
    sceVif1PkAddGsData(packet, SCE_GS_SET_PRIM(0x01,0,0,0,0,WALL_AA1,0,0,0));
    sceVif1PkAddGsData(packet, SCE_GS_SET_RGBAQ(c[0], c[1], c[2],
						c[3], *(u_int *)&q));
    sceVif1PkAddGsData(packet, SCE_GS_SET_XYZ(v0[0],v0[1],v0[2]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_XYZ(v1[0],v1[1],v1[2]));
}

void SetCubeVertex( sceVu0FVECTOR* v1)
{
    SETVECTOR(v1[0],-CUBE_WIDTH_H, CUBE_HEIGHT_H,-CUBE_DEPTH_H,1.0f);
    SETVECTOR(v1[1],-CUBE_WIDTH_H, CUBE_HEIGHT_H, CUBE_DEPTH_H,1.0f);
    SETVECTOR(v1[2],-CUBE_WIDTH_H,-CUBE_HEIGHT_H,-CUBE_DEPTH_H,1.0f);
    SETVECTOR(v1[3],-CUBE_WIDTH_H,-CUBE_HEIGHT_H, CUBE_DEPTH_H,1.0f);
    SETVECTOR(v1[4], CUBE_WIDTH_H,-CUBE_HEIGHT_H,-CUBE_DEPTH_H,1.0f);
    SETVECTOR(v1[5], CUBE_WIDTH_H,-CUBE_HEIGHT_H, CUBE_DEPTH_H,1.0f);
    SETVECTOR(v1[6], CUBE_WIDTH_H, CUBE_HEIGHT_H,-CUBE_DEPTH_H,1.0f);
    SETVECTOR(v1[7], CUBE_WIDTH_H, CUBE_HEIGHT_H, CUBE_DEPTH_H,1.0f);
}

void SetWallVertex(sceVu0FVECTOR* wallF)
{
    SETVECTOR(wallF[0],-WALL_WIDTH_H, WALL_HEIGHT_H,-WALL_DEPTH_H,1.0f);
    SETVECTOR(wallF[1],-WALL_WIDTH_H, WALL_HEIGHT_H, WALL_DEPTH_H,1.0f);
    SETVECTOR(wallF[2],-WALL_WIDTH_H,-WALL_HEIGHT_H,-WALL_DEPTH_H,1.0f);
    SETVECTOR(wallF[3],-WALL_WIDTH_H,-WALL_HEIGHT_H, WALL_DEPTH_H,1.0f);
    SETVECTOR(wallF[4], WALL_WIDTH_H,-WALL_HEIGHT_H,-WALL_DEPTH_H,1.0f);
    SETVECTOR(wallF[5], WALL_WIDTH_H,-WALL_HEIGHT_H, WALL_DEPTH_H,1.0f);
    SETVECTOR(wallF[6], WALL_WIDTH_H, WALL_HEIGHT_H,-WALL_DEPTH_H,1.0f);
    SETVECTOR(wallF[7], WALL_WIDTH_H, WALL_HEIGHT_H, WALL_DEPTH_H,1.0f);
}

void SetObjectColor(sceVu0FVECTOR* c1)
{
    SETVECTOR(c1[0],255.0f, 255.0f, 255.0f, 0.0f);
    SETVECTOR(c1[1],255.0f, 255.0f, 255.0f, 0.0f);
    SETVECTOR(c1[2],255.0f, 255.0f, 255.0f, 0.0f);
    SETVECTOR(c1[3],255.0f, 255.0f, 255.0f, 0.0f);
    SETVECTOR(c1[4],255.0f, 255.0f, 255.0f, 0.0f);
    SETVECTOR(c1[5],255.0f, 255.0f, 255.0f, 0.0f);
}

void SetRGB(sceVu0IVECTOR* c0, sceVu0FMATRIX LocalLightMatrix, 
	    sceVu0FMATRIX LightColorMatrix, sceVu0FVECTOR* v1,
	    sceVu0FVECTOR* c1)
{
    light_rgb(c0[0], LocalLightMatrix, LightColorMatrix,
	      v1[1], v1[7], v1[3], c1[0]);
    light_rgb(c0[1], LocalLightMatrix, LightColorMatrix,
	      v1[0], v1[6], v1[1], c1[1]);
    light_rgb(c0[2], LocalLightMatrix, LightColorMatrix,
	      v1[2], v1[4], v1[0], c1[2]);
    light_rgb(c0[3], LocalLightMatrix, LightColorMatrix,
	      v1[0], v1[1], v1[2], c1[3]);
    light_rgb(c0[4], LocalLightMatrix, LightColorMatrix,
	      v1[2], v1[3], v1[4], c1[4]);
    light_rgb(c0[5], LocalLightMatrix, LightColorMatrix,
	      v1[4], v1[5], v1[6], c1[5]);
}

