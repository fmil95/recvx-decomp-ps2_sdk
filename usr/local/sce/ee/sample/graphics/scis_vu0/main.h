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
 *                     Name : main.h
 *
 *       Version      Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.5.0        Sep.22.1999     thatake     1st Version
 */

#define PI 3.14159265358979323846f
#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 224.0f

#define IMAGE_SIZE 128

sceVu0FVECTOR camera_p = { 0, 0, -15, 0 };
sceVu0FVECTOR camera_zd = { 0, 0, 1, 1 };
sceVu0FVECTOR camera_yd = { 0, 1, 0, 1 };
sceVu0FVECTOR camera_rot = { -0.15*PI, -0.25*PI, 0, 0 };

sceVu0FVECTOR light0 = { -0.5, 0.5, 0.5, 0 };
sceVu0FVECTOR light1 = { -0.5, -1.5, -1.0, 0 };
sceVu0FVECTOR light2 = { 1.0, -0.5, 0.5, 0 };

sceVu0FVECTOR color0  = { 0.77, 0.75, 0.75, 1 };
sceVu0FVECTOR color1  = { 0.15, 0.23, 0.15, 1 };
sceVu0FVECTOR color2  = { 0.1, 0.1, 0.23, 1 };
sceVu0FVECTOR ambient = { 0.15, 0.15, 0.15, 0 };

sceVu0FVECTOR clip_volume = { 250, 200, 16.0, 16777000.0};

sceVu0FVECTOR obj_trans = { 0, 0, 0, 0 };
sceVu0FVECTOR obj_rot = { PI, 0, 0, 0 };

sceVu0FMATRIX world_view;
sceVu0FMATRIX view_screen;
sceVu0FMATRIX view_clip;
sceVu0FMATRIX world_clip;
sceVu0FMATRIX clip_screen;
sceVu0FMATRIX world_screen;
sceVu0FMATRIX local_screen;
sceVu0FMATRIX local_world;
sceVu0FMATRIX normal_light;
sceVu0FMATRIX light_color;
sceVu0FMATRIX local_light;
sceVu0FMATRIX local_color;
sceVu0FMATRIX local_clip;

sceVu0FMATRIX work;
sceVu0FMATRIX unit_mat;

u_int paddata;
u_char rdata[32];
u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

sceGsDBuff      db;
sceGsLoadImage  gs_limage;
TexEnv          texenv;

extern u_int My_cube_start __attribute__((section(".vudata")));
extern u_int My_torus_start __attribute__((section(".vudata")));
extern u_int My_clip64  __attribute__((section(".vudata")));
extern u_int My_clip128  __attribute__((section(".vudata")));

extern u_int SampleCubeDataHead  __attribute__((section(".vudata")));
extern u_int SampleCubeDataBlock0  __attribute__((section(".vudata")));
extern u_int SampleTorusDataHead  __attribute__((section(".vudata")));
extern u_int SampleTorusDataBlock0  __attribute__((section(".vudata")));
extern u_int SampleTorus1DataHead  __attribute__((section(".vudata")));
extern u_int SampleTorus1DataBlock0  __attribute__((section(".vudata")));

extern void InitNodeArraySet(NodeArraySet *scissorflip);
extern void InitScissorPlane(ScissorPlaneArray *planeset);
extern void ResetNodeArraySet(NodeArraySet *scissorflip);
extern void FlipNodeArray(NodeArraySet *scissorflip);
extern void PushTriangleNodeArray(NodeArraySet *scissorflip, 
                                   Node *nod, int mode);
extern void PushFifoNodeArray(NodeArraySet *scissorflip, 
                                   Node *nod, int mode);
extern void ScissorTriangle(NodeArraySet *scissorflip, 
                     ScissorPlaneArray *planeset);
extern void DrawScissorPolygon(NodeArraySet *scissorflip, ObjData *obj, 
                               int num);

extern void LoadObj(ObjData *o, u_int *Head);
extern void ReleaseObj(ObjData *o);
extern void MakePacket(ObjData *obj, int num, int flag,
                NodeArraySet *scissorflip, ScissorPlaneArray *planeset);

// calculate lighting_color
void sceVu0NormalColorVector(sceVu0IVECTOR c0, 
                sceVu0FMATRIX local_light, sceVu0FMATRIX light_color, 
                               sceVu0FVECTOR v0, sceVu0FVECTOR c1)
{
    sceVu0FVECTOR vect;
    local_light[3][3] = 1.0;
    sceVu0ApplyMatrix(vect, local_light, v0); 
    sceVu0ClampVector(vect, vect, 0.0, 1.0);
    sceVu0ApplyMatrix(vect, light_color, vect); 
    sceVu0MulVector(vect, c1, vect);
    sceVu0ClampVector(vect, vect, 0.0, 255.0);
    sceVu0FTOI0Vector(c0, vect);
}

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
                             sceVu0FMATRIX mcs,
                             float scrz, float ax, float ay, 
                             float cx, float cy, 
                             float zmin, float zmax, float nearz, float farz)
{
        float   az, cz;
        float   gsx, gsy;
        sceVu0FMATRIX   mt;             

        gsx = nearz*clip_volume[0]/scrz;
        gsy = nearz*clip_volume[1]/scrz;

        cz = (-zmax * nearz + zmin * farz) / (-nearz + farz);
        az  = farz * nearz * (-zmin + zmax) / (-nearz + farz);

        //     | scrz    0  0 0 |
        // m = |    0 scrz  0 0 | 
        //     |    0    0  0 1 |
        //     |    0    0  1 0 |
        sceVu0UnitMatrix(m);
        m[0][0] = scrz; m[1][1] = scrz;
        m[2][2] = 0;    m[3][3] = 0;
        m[3][2] = 1.0;  m[2][3] = 1.0;

        //     | ax  0  0 cx |
        // m = |  0 ay  0 cy | 
        //     |  0  0 az cz |
        //     |  0  0  0  1 |
        sceVu0UnitMatrix(mt);
        mt[0][0] = ax;  mt[1][1] = ay;  mt[2][2] = az;
        mt[3][0] = cx;  mt[3][1] = cy;  mt[3][2] = cz;

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
        mc[2][3] = 1;   mc[3][3] = 0;

        //     |scrz*ax*gsx/n    0            0               cx      |
        // m = |   0        scrz*ay*gsy/n     0               cy      |
        //     |   0             0      (-zmax+zmin)/2  (zmin+zmax)/2 |
        //     |   0             0            0               1       |
        sceVu0UnitMatrix(mcs);
        mcs[0][0] = scrz*ax*gsx/nearz;
        mcs[1][1] = scrz*ay*gsy/nearz;
        mcs[2][2] = (-zmax+zmin)/2;
        mcs[3][2] = (zmax+zmin)/2;
        mcs[3][0] = cx; mcs[3][1] = cy; 
        //mcs[3][2] = cz;
        mcs[3][3] = 1;  

        return;
}

int sceVu0ViewVolumeClip(sceVu0FVECTOR clip, 
			 sceVu0FMATRIX local_clip, 
			 sceVu0FVECTOR vertex)
{
    register int ret;

    asm __volatile__("
    lqc2    vf4,0x0(%2)
    lqc2    vf5,0x10(%2)
    lqc2    vf6,0x20(%2)
    lqc2    vf7,0x30(%2)
    lqc2    vf8,0x0(%3)
    vmulax.xyzw     ACC, vf4,vf8
    vmadday.xyzw    ACC, vf5,vf8
    vmaddaz.xyzw    ACC, vf6,vf8
    vmaddw.xyzw      vf12,vf7,vf8
    vclipw.xyz   vf12, vf12
    vnop
    vnop
    vnop
    sqc2    vf12, 0x0(%1)
    vnop
    cfc2    %0, $vi18
    ":"=&r"(ret) : "r" (clip), "r" (local_clip), "r" (vertex));

     return ret;

};

void SetGifClipLine(GifDmaPacket *dmagif)
{
    sceVu0IVECTOR vertex, color;
    sceVu0FVECTOR ul, ur, dl, dr;
    GifDmaPacket *pack = dmagif;

    ul[0] = -(clip_volume[0]+3.0);  ul[1] = -(clip_volume[1]+3.0);
    ul[2] = 512.0f*camera_zd[2];  ul[3] = 1.0f;  
    ur[0] = clip_volume[0]+3.0;  ur[1] = -(clip_volume[1]+3.0);  
    ur[2] = 512.0f*camera_zd[2];  ur[3] = 1.0f;
    dl[0] = -(clip_volume[0]+3.0);  dl[1] = clip_volume[1]+3.0;
    dl[2] = 512.0f*camera_zd[2];  dl[3] = 1.0f;  
    dr[0] = clip_volume[0]+3.0;  dr[1] = clip_volume[1]+3.0;  
    dr[2] = 512.0f*camera_zd[2];  dr[3] = 1.0f;
    color[0] = 250;  color[1] = 220;  
    color[2] = 0;  color[3] = 0;

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
    
    FlushCache(0);
    sceGsSyncPath(0, 0);
    *D2_QWC  = 0x00;
    *D2_TADR = (u_int)dmagif->buf;
    *D_STAT = 2;
    *D2_CHCR = 1 | (1<<2) | (0<<4) | (0<<5) | (1<<6) | (0<<7) | (1<<8);

}
