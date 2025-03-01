/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - clip_vu1 -
 *
 *                         Version 0.3.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : main.h
 *                     Description : main procedures
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.3.0         
 */

#define PI 3.14159265358979323846f
#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 224.0f
#define SCREEN_OFFSET_X 1728.0f
#define SCREEN_OFFSET_Y 1936.0f

//#define IMAGE_SIZE 256
//#define IMAGE_SIZE 64
#define IMAGE_SIZE 128

sceVu0FVECTOR camera_p = { 0, 0, -33, 0 };
sceVu0FVECTOR camera_zd = { 0, 0, 1, 1 };
sceVu0FVECTOR camera_yd = { 0, 1, 0, 1 };
sceVu0FVECTOR camera_rot = { 0, 0, 0, 0 };

sceVu0FVECTOR light0 = { -0.5, 0.5, 0.5, 0 };
sceVu0FVECTOR light1 = { -0.5, -1.5, -1.0, 0 };
sceVu0FVECTOR light2 = { 1.0, -0.5, 0.5, 0 };
//sceVu0FVECTOR light0 = { 0.0, 1.5, 0.5, 0 };
//sceVu0FVECTOR light1 = { 1.5, -0.5, 0.5, 0 };
//sceVu0FVECTOR light2 = { -1.5, -0.5, 0.5, 0 };

sceVu0FVECTOR color0  = { 0.77, 0.75, 0.75, 1 };
sceVu0FVECTOR color1  = { 0.15, 0.23, 0.15, 1 };
sceVu0FVECTOR color2  = { 0.1, 0.1, 0.23, 1 };
sceVu0FVECTOR ambient = { 0.15, 0.15, 0.15, 0 };

//sceVu0FVECTOR clip_volume = { 2047, 2047, 16.0, 16777215.0};
sceVu0FVECTOR clip_volume = { 250, 200, 16.0, 16777215.0};

sceVu0FVECTOR obj_trans = { 0, 0, 0, 0 };
sceVu0FVECTOR obj_rot = { 0.5*PI, PI, 0, 0 };

sceVu0FMATRIX world_view;
sceVu0FMATRIX view_screen;
sceVu0FMATRIX view_clip;
sceVu0FMATRIX world_screen;
sceVu0FMATRIX local_screen;
sceVu0FMATRIX local_world;
sceVu0FMATRIX normal_light;
sceVu0FMATRIX light_color;
sceVu0FMATRIX local_light;
sceVu0FMATRIX local_color;

sceVu0FMATRIX work;
sceVu0FMATRIX unit_mat;

u_int paddata;
u_char rdata[32];
u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

sceGsDBuff      db;
sceGsLoadImage  gs_limage;
TexEnv          texenv;

extern void InitVu1DmaPacket(Vu1DmaPacket *dma, Vu1ObjPacket *head0,
			     Vu1ObjPacket *head1);
extern Vu1ObjPacket* GetVu1ObjHead(Vu1DmaPacket *dma);
extern Vu1ObjPacket* AddVu1ObjPacket(Vu1DmaPacket *head, u_int *obj,
				     sceVu0FVECTOR obj_rot,
				     sceVu0FVECTOR obj_trans);
extern Vu1ObjPacket* CloseVu1ObjPacket(Vu1DmaPacket *dma);
extern Vu1EnvPacket* GetVu1EnvHead(Vu1DmaPacket *dma);
extern Vu1EnvPacket* CloseVu1EnvPacket(Vu1DmaPacket *dma);

extern u_int My_clip_code __attribute__((section(".vudata")));
extern u_int My_basic_code __attribute__((section(".vudata")));
extern u_int My_cube_start __attribute__((section(".vudata")));
extern u_int My_torus_start __attribute__((section(".vudata")));
extern u_int My_clip64  __attribute__((section(".vudata")));
extern u_int My_clip128  __attribute__((section(".vudata")));

void sceVu0RotCameraMatrix(sceVu0FMATRIX m, sceVu0FVECTOR p, sceVu0FVECTOR zd,
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

// make view_screen & view_clip matrix
void SetViewScreenClipMatrix(sceVu0FMATRIX m, sceVu0FMATRIX mc,
			     float scrz, float ax, float ay, 
			     float cx, float cy, 
			     float zmin, float zmax, float nearz, float farz)
{
	float	az, cz;
	float   gsx, gsy;
	sceVu0FMATRIX	mt;		

	gsx = nearz*clip_volume[0]/scrz;
	gsy = nearz*clip_volume[1]/scrz;

	cz = (-zmax * nearz + zmin * farz) / (-nearz + farz);
	az  = farz * nearz * (-zmin + zmax) / (-nearz + farz);

	//     | scrz    0  0 0 |
	// m = |    0 scrz  0 0 | 
	//     |    0    0  0 1 |
	//     |    0    0  1 0 |
	sceVu0UnitMatrix(m);
	m[0][0] = scrz;	m[1][1] = scrz;
	m[2][2] = 0;	m[3][3] = 0;
	m[3][2] = 1.0;	m[2][3] = 1.0;

	//     | ax  0  0 cx |
	// m = |  0 ay  0 cy | 
	//     |  0  0 az cz |
	//     |  0  0  0  1 |
	sceVu0UnitMatrix(mt);
	mt[0][0] = ax;	mt[1][1] = ay;	mt[2][2] = az;
	mt[3][0] = cx;	mt[3][1] = cy;	mt[3][2] = cz;

	sceVu0MulMatrix(m, mt, m);
	
	//     |2n/2gsx    0        0           0      |
	// m = |   0    2n/2gsy     0           0      |
	//     |   0       0   (f+n)/(f-n) -2f*n/(f-n) |
	//     |   0       0        1           0      |
	sceVu0UnitMatrix(mc);
	mc[0][0] = 2 * nearz / (gsx-(-gsx));
	mc[1][1] = 2 * nearz / (gsy-(-gsy));
	mc[2][2] = (farz + nearz) / (farz-nearz);
	mc[3][2] = -2*(farz * nearz) / (farz-nearz);
	mc[2][3] = 1;	mc[3][3] = 0;

	return;
}

void SetGifClipLine(GifDmaPacket *dmagif)
{
    sceVu0IVECTOR vertex, color;
    sceVu0FVECTOR ul, ur, dl, dr;
    GifDmaPacket *pack = dmagif;

    ul[0] = -clip_volume[0];  ul[1] = -clip_volume[1];
    ul[2] = 512.0f*camera_zd[2];  ul[3] = 1.0f;  
    ur[0] = clip_volume[0];  ur[1] = -clip_volume[1];  
    ur[2] = 512.0f*camera_zd[2];  ur[3] = 1.0f;
    dl[0] = -clip_volume[0];  dl[1] = clip_volume[1];
    dl[2] = 512.0f*camera_zd[2];  dl[3] = 1.0f;  
    dr[0] = clip_volume[0];  dr[1] = clip_volume[1];  
    dr[2] = 512.0f*camera_zd[2];  dr[3] = 1.0f;
    color[0] = 200;  color[1] = 200;  
    color[2] = 200;  color[3] = 0;

    sceVu0MulMatrix(world_screen, view_screen, unit_mat);
    sceVu0MulMatrix(local_screen, world_screen, unit_mat);

    //  make packet(PACKED mode)
    pack->size = 0;
    pack->buf[pack->size].ul128 = (u_long128)0;
    pack->buf[pack->size++].ui32[0] = 0x70000000|(5*2+1);// DMA end
							     
    //  add vertex info(GIFtag,STQ&RGBA&XYZ)
    pack->buf[pack->size].ul64[0] 
      = SCE_GIF_SET_TAG(5, 1, 1, 0x02,SCE_GIF_PACKED,2); // LINEstrip*4
    pack->buf[pack->size++].ul64[1] = 0x041L;
    sceVu0RotTransPers(vertex, local_screen, ul, 1);
    pack->buf[pack->size++].ul128 = *((u_long128*)color);
    pack->buf[pack->size++].ul128 = *((u_long128*)vertex);
    sceVu0RotTransPers(vertex, local_screen, ur, 1);
    pack->buf[pack->size++].ul128 = *((u_long128*)color);
    pack->buf[pack->size++].ul128 = *((u_long128*)vertex);
    sceVu0RotTransPers(vertex, local_screen, dr, 1);
    pack->buf[pack->size++].ul128 = *((u_long128*)color);
    pack->buf[pack->size++].ul128 = *((u_long128*)vertex);
    sceVu0RotTransPers(vertex, local_screen, dl, 1);
    pack->buf[pack->size++].ul128 = *((u_long128*)color);
    pack->buf[pack->size++].ul128 = *((u_long128*)vertex);
    sceVu0RotTransPers(vertex, local_screen, ul, 1);
    pack->buf[pack->size++].ul128 = *((u_long128*)color);
    pack->buf[pack->size++].ul128 = *((u_long128*)vertex);
   
}
