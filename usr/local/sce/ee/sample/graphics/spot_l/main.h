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
 *                     Name : main.h
 *
 *       Version      Date            Design      Log
 *  --------------------------------------------------------------------
 *       1.3.0        Dec,8,1999     thatake      1st version
 */

#define PI 3.14159265358979323846f
#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 224.0f

#define IMAGE_SIZE 256

sceVu0FVECTOR camera_p = { 0, 0, -35, 0 };
sceVu0FVECTOR camera_zd = { 0, 0, 1, 1 };
sceVu0FVECTOR camera_yd = { 0, 1, 0, 1 };
sceVu0FVECTOR camera_rot = { -0.2*PI, 0, 0, 0 };

sceVu0FVECTOR light0 = { 0.0, 1.0, 0.0, 1.0 };
sceVu0FVECTOR light1 = { 0.0, 1.0, 0.0, 1.0 };
sceVu0FVECTOR light2 = { 0.0, 1.0, 0.0, 1.0 };

sceVu0FVECTOR plight0 = { 0.0, -14.0, 2.0, 1.0 };
sceVu0FVECTOR plight1 = { -11.0, -13.0, -11.0, 1.0 };
sceVu0FVECTOR plight2 = { 11.0, -13.0, -11.0, 1.0 };

sceVu0FVECTOR light0_rot = { -PI*0.05, 0, 0, 0 };
sceVu0FVECTOR light1_rot = { PI*0.2, 0, -PI*0.15, 0 };
sceVu0FVECTOR light2_rot = { PI*0.2, 0, PI*0.15, 0 };

sceVu0FVECTOR light0_dir;
sceVu0FVECTOR light1_dir;
sceVu0FVECTOR light2_dir;

sceVu0FVECTOR color0  = { 0.8, 0.15, 0.05, 0 };
sceVu0FVECTOR color1  = { 0.05, 0.8, 0.15, 0 };
sceVu0FVECTOR color2  = { 0.15, 0.05, 0.8, 0 };

sceVu0FVECTOR ambient = { 0.1, 0.1, 0.1, 0 };

sceVu0FVECTOR clip_volume = { 2047, 2047, 16.0, 16777215.0};

sceVu0FVECTOR obj_trans = { 0, 1.0, 0, 0 };
sceVu0FVECTOR obj_rot = {  -PI, 0, 0, 0 };
sceVu0FVECTOR obj_scale = {  1.0, 1.0, 1.0, 0.0 };

sceVu0FVECTOR torus_trans = { 0, 0, 0, 0 };
sceVu0FVECTOR torus_rot = {  0, PI*0.3, PI*0.2, 0 };

sceVu0FVECTOR zero_vector = {0, 0, 0, 1.0}; 

sceVu0FMATRIX world_view;
sceVu0FMATRIX view_screen;
sceVu0FMATRIX view_clip;
sceVu0FMATRIX clip_screen;
sceVu0FMATRIX world_screen;
sceVu0FMATRIX local_screen;
sceVu0FMATRIX local_world;
sceVu0FMATRIX normal_light;
sceVu0FMATRIX light_color;
sceVu0FMATRIX local_light;
sceVu0FMATRIX local_color;

sceVu0FMATRIX work;
sceVu0FMATRIX unit_mat;

sceGsDBuff      db;
sceGsLoadImage  gs_limage;
TexEnv          texenv;

u_int paddata;
u_char rdata[32];
u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

extern void InitVu1DmaPacket(Vu1DmaPacket *dma, Vu1ObjPacket *head0,
			     Vu1ObjPacket *head1);
extern Vu1ObjPacket* GetVu1ObjHead(Vu1DmaPacket *dma);
extern Vu1ObjPacket* AddVu1ObjPacket(Vu1DmaPacket *head, u_int *obj,
				     sceVu0FVECTOR obj_rot,
				     sceVu0FVECTOR obj_trans);
extern Vu1ObjPacket* CloseVu1ObjPacket(Vu1DmaPacket *dma);
extern Vu1EnvPacket* GetVu1EnvHead(Vu1DmaPacket *dma);
extern Vu1EnvPacket* CloseVu1EnvPacket(Vu1DmaPacket *dma);

extern u_int My_basic_code __attribute__((section(".vudata")));
extern u_int My_spot_code __attribute__((section(".vudata")));
extern u_int My_dma_setting  __attribute__((section(".vudata")));

extern u_int My_cube_start __attribute__((section(".vudata")));
extern u_int My_cube_vertex __attribute__((section(".vudata")));
extern u_int My_dma_start __attribute__((section(".vudata")));
extern u_int My_torus_start __attribute__((section(".vudata")));
extern u_int My_grid_start __attribute__((section(".vudata")));
extern u_int My_grid100_start __attribute__((section(".vudata")));
extern u_int My_texture256  __attribute__((section(".vudata")));


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

// make view_screen & view_clip & clip_screen matrix
void SetViewScreenClipMatrix(sceVu0FMATRIX m, sceVu0FMATRIX mc,
			     sceVu0FMATRIX mcs,
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

	//     |scrz*ax*gsx/n    0            0               cx      |
	// m = |   0        scrz*ay*gsy/n     0               cy      |
	//     |   0             0      (-zmax+zmin)/2  (zmin+zmax)/2 |
	//     |   0             0            0               1       |
	sceVu0UnitMatrix(mcs);
	mcs[0][0] = scrz*ax*gsx/nearz;
	mcs[1][1] = scrz*ay*gsy/nearz;
	mcs[2][2] = (-zmax+zmin)/2;
	mcs[3][2] = (zmax+zmin)/2;
	mcs[3][0] = cx;	mcs[3][1] = cy;	
	//mcs[3][2] = cz;
	mcs[3][3] = 1;	

	return;
}
