/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - multi movie -
 *
 *                         Version 1.20
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            ezcube.c
 *                    library file for ezcube3
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.00           Aug. 13.1999    ywashizu    the first version
 *       1.20           Oct, 15,1999    ywashizu    new IPU movie format
 */


#include "ezcube.h"
#define ERR_STOP while(1)

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

// function to calculate ViewWorlMatrix
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
			sceVu0IVECTOR v3, IDEC_MOVIE movie){
    float s_0 = 0.0f;
    float t_0 = 0.0f;
    float s_1 = (float)(movie.width)/(1<<(movie.tw));
    float t_1 = (float)(movie.height)/(1<<(movie.th));
    float s[4], t[4], q[4];

    q[0] = 1.0f / (float)v0[3];
    q[1] = 1.0f / (float)v1[3];
    q[2] = 1.0f / (float)v2[3];
    q[3] = 1.0f / (float)v3[3];
    s[0] = s_0 * q[0];
    s[1] = s_1 * q[1];
    s[2] = s_0 * q[2];
    s[3] = s_1 * q[3];
    t[0] = t_0 * q[0];
    t[1] = t_0 * q[1];
    t[2] = t_1 * q[2];
    t[3] = t_1 * q[3];

    sceVif1PkAddGsData(packet,
    		       SCE_GS_SET_TEX0(movie.texbp, movie.texbw, 0, movie.tw,
				       movie.th, 0, 0, 0, 0, 0, 0, 0));
    sceVif1PkAddGsData(packet, SCE_GS_SET_PRIM(0x04,0,1,0,0,CUBE_AA1,0,0,0));
    sceVif1PkAddGsData(packet, SCE_GS_SET_ST(*(u_int*)&s[0], *(u_int*)&t[0]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_RGBAQ(c[0], c[1], c[2], 0x00,
						*(u_int*)&q[0]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_XYZ(v0[0], v0[1], v0[2]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_ST(*(u_int*)&s[1], *(u_int*)&t[1]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_RGBAQ(c[0], c[1], c[2], 0x00,
						*(u_int*)&q[1]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_XYZ(v1[0], v1[1], v1[2]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_ST(*(u_int*)&s[2], *(u_int*)&t[2]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_RGBAQ(c[0], c[1], c[2], 0x00,
						*(u_int*)&q[2]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_XYZ(v2[0], v2[1], v2[2]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_ST(*(u_int*)&s[3], *(u_int*)&t[3]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_RGBAQ(c[0], c[1], c[2],0x00,
						*(u_int*)&q[3]));
    sceVif1PkAddGsData(packet, SCE_GS_SET_XYZ(v3[0], v3[1], v3[2]));
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

// function to initialize GS registers
void SetGsRegs(int alpha_on, int tex_on)
{
    sceGifPacket packetGS;	
    const u_long giftag[2] = { SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1), 
			       0x000000000000000eL };
    sceDmaChan *dmaGif;
    u_long128* packetAddr=(u_long128*)malloc(PACKETSIZE*sizeof(u_long128));
    dmaGif = sceDmaGetChan(SCE_DMA_GIF);
    
    sceGifPkInit(&packetGS, packetAddr);
    //     (u_long128*)malloc(PACKETSIZE*sizeof(u_long128)));
    sceGifPkReset(&packetGS);
    
    // add CNT Tag
    sceGifPkCnt(&packetGS, 0, 0, 0);
    
    // add GIF Tag 
    sceGifPkOpenGifTag(&packetGS, *(u_long128*)&giftag);
    
    if(alpha_on){
	// packet to set Alpha-Blending
	sceGifPkAddGsAD(&packetGS,
			SCE_GS_ALPHA_1, SCE_GS_SET_ALPHA(0,1,0,1,0));
    }
    
    if(tex_on){
	// packet to initialize texture buffer
	sceGifPkAddGsAD(&packetGS, SCE_GS_TEXFLUSH, 0);
    }
    
    // close GIF Tag
    sceGifPkCloseGifTag(&packetGS);
    
    // add End packet
    sceGifPkEnd(&packetGS, 0, 0, 0);
    
    // finish making packet
    sceGifPkTerminate(&packetGS);
    
    FlushCache(0);
    sceGsSyncPath(0, 0);
    // DMA Start
    sceDmaSend(dmaGif, 
	       (u_long128*)((u_int)packetGS.pBase));
    sceGsSyncPath(0, 0);
    free(packetAddr);
}

void ReadIPUHeader(IDEC_MOVIE* movie, u_char* fileData, u_int texbp)
{
    movie->width =  (int)*(u_short*)(fileData + 8);
    movie->height = (int)*(u_short*)(fileData + 10);
    movie->nframes = (int)*(u_int*)(fileData + 12);    // initialization    
    movie->frame_cnt = movie->nframes;
    movie->mbx = movie->width/16;
    movie->mby = movie->height/16;
    movie->origin_x = (DISP_WIDTH - movie->width)/2;
    movie->origin_y = (DISP_HEIGHT - movie->height)/2;
    movie->texbp = texbp;
    movie->texbw = movie->width/64;

    if(movie->width > MAX_WIDTH){
	printf("too big width\n");
	ERR_STOP;
    }
    while(1){
	static int half = 0x0200;
	static int tw = 10;
	if(movie->width > half){
	    movie->tw = tw;
	    break;
	}
	half >>= 1;
	tw -= 1;
    }

    if(movie->height > MAX_HEIGHT){
	printf("too big HEIGHT\n");
	ERR_STOP;
    }
    while(1){
	static int half = 0x0100;
	static int th = 9;
	if(movie->height > half){
	    movie->th = th;
	    break;
	}
	half >>= 1;
	th -= 1;
    }
}

u_int getFlags()
{
    u_int ret;

    sceIpuFDEC(0);
    sceIpuSync(0,0);
    ret = sceIpuGetFVdecResult();
    ret = (ret >> 24) & 0xff;   // get flags;
    sceIpuSync(0,0);
    sceIpuFDEC(8);              // proceed 7 bit
    sceIpuSync(0,0);
    return ret;
}
