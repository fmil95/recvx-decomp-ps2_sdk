/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - mipmap -
 *
 *                        Version 0.3.0
 *
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : main.c
 *                     Description : main procedures
 *
 *       Version   Date           Design              Log
 *  --------------------------------------------------------------------
 *       0.3.0      Jun, 4,1999    munekis            first version
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
#include <libpad.h>

#include "mathfunc.h"

extern u_int My_dma_start[] __attribute__((section(".vudata")));
extern float My_matrix[] __attribute__((section(".vudata")));
extern float My_rot_trans_matrix[] __attribute__((section(".vudata")));
extern float My_light_matrix[] __attribute__((section(".vudata")));
extern u_int My_texture0[] __attribute__((section(".vudata")));
extern u_int My_texture1[] __attribute__((section(".vudata")));
extern u_int My_texture2[] __attribute__((section(".vudata")));
extern u_int My_texture3[] __attribute__((section(".vudata")));
extern u_int My_texture4[] __attribute__((section(".vudata")));
extern u_int My_texture5[] __attribute__((section(".vudata")));
extern u_int My_texture6[] __attribute__((section(".vudata")));


void SetVu1PacketMatrix(void);

#define PI 3.14159265358979323846f
#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 224.0f
#define SCREEN_OFFSET_X 1728.0f
#define SCREEN_OFFSET_Y 1936.0f

#define IMAGE_SIZE 256

typedef struct {
        sceGifTag giftag;
        sceGsTest gs_test;
        long gs_test1addr;
        sceGsMiptbp1 gs_miptbp1;
        long gs_miptbp1addr;
        sceGsMiptbp2 gs_miptbp2;
        long gs_miptbp2addr;
        sceGsAlphaEnv gs_alpha;
        sceGsTexEnv gs_tex;
}TexEnv;

sceSamp0FVECTOR camera_p =   { 0.0f, 0.0f, -25.0f, 0.0f };
sceSamp0FVECTOR camera_zd =  { 0.0f, 0.0f,   1.0f, 1.0f };
sceSamp0FVECTOR camera_yd =  { 0.0f, 1.0f,   0.0f, 1.0f };
sceSamp0FVECTOR camera_rot = { 0.0f, 0.0f,   0.0f, 0.0f };

sceSamp0FVECTOR light0 = {  0.0f,  1.5f, 0.5f, 0.0f };
sceSamp0FVECTOR light1 = {  1.5f, -0.5f, 0.5f, 0.0f };
sceSamp0FVECTOR light2 = { -1.5f, -0.5f, 0.5f, 0.0f };

sceSamp0FVECTOR color0  = { 0.3f, 0.3f, 0.8f, 1.0f };
sceSamp0FVECTOR color1  = { 0.8f, 0.3f, 0.3f, 1.0f };
sceSamp0FVECTOR color2  = { 0.3f, 0.8f, 0.3f, 1.0f };

sceSamp0FVECTOR ambient = { 0.2f, 0.2f, 0.2f, 0.0f };

sceSamp0FVECTOR obj_trans = { 0.0f, 0.0f, 0.0f, 0.0f };
sceSamp0FVECTOR obj_rot =   { 0.0f, 0.0f, 0.0f, 0.0f };

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
sceGsLoadImage  gs_limage;
TexEnv          texenv;


int main()
{
        int    odev;
        u_int  frame, frame2;
        float  f1 = 0.0f;
	sceSifInitRpc(0);
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
			    0, NULL) < 0){
	    printf("Can't load module sio2man\n");
	}
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
			    0, NULL) < 0){
	    printf("Can't load module padman\n");
	}

        sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
        sceGsResetPath();

        // --- set double buffer ---
        sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224,
                SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
        *(u_long *)&db.clear0.rgbaq =
                SCE_GS_SET_RGBAQ(0x10, 0x10, 0xa0, 0x80, 0x3f800000);
        *(u_long *)&db.clear1.rgbaq =
                SCE_GS_SET_RGBAQ(0x10, 0x10, 0xa0, 0x80, 0x3f800000);


        // --- set load image 0---
        sceGsSetDefLoadImage(&gs_limage,
                             6720,
                             IMAGE_SIZE / 64,
                              SCE_GS_PSMCT16,
                             0, 0,
                             IMAGE_SIZE, IMAGE_SIZE);

        FlushCache(0);

        sceGsExecLoadImage(&gs_limage, (u_long128*) My_texture0);
        sceGsSyncPath(0, 0);


        // --- set load image 1 ---
        sceGsSetDefLoadImage(&gs_limage,
                             7232,
                             128 / 64,
                             SCE_GS_PSMCT16,
                             0, 0,
                             128, 128);
        FlushCache(0);
        sceGsExecLoadImage(&gs_limage, (u_long128*) My_texture1);
        sceGsSyncPath(0, 0);

        // --- set load image 2 ---
        sceGsSetDefLoadImage(&gs_limage,
                             7360,
                             64 / 64,
                             SCE_GS_PSMCT16,
                             0, 0,
                             64, 64);
        FlushCache(0);
        sceGsExecLoadImage(&gs_limage, (u_long128*) My_texture2);
        sceGsSyncPath(0, 0);

        // --- set load image 3 ---
        sceGsSetDefLoadImage(&gs_limage,
                             7392,
                             1,
                             SCE_GS_PSMCT16,
                             0, 0,
                             32, 32);
        FlushCache(0);
        sceGsExecLoadImage(&gs_limage, (u_long128*) My_texture3);
        sceGsSyncPath(0, 0);

        // --- set load image 4 ---
        sceGsSetDefLoadImage(&gs_limage,
                             7400,
                             1,
                             SCE_GS_PSMCT16,
                             0, 0,
                             16, 16);
        FlushCache(0);
        sceGsExecLoadImage(&gs_limage, (u_long128*) My_texture4);
        sceGsSyncPath(0, 0);

        // --- set load image 5 ---
        sceGsSetDefLoadImage(&gs_limage,
                             7402,
                             1,
                             SCE_GS_PSMCT16,
                             0, 0,
                             8, 8);
        FlushCache(0);
        sceGsExecLoadImage(&gs_limage, (u_long128*) My_texture5);
        sceGsSyncPath(0, 0);

        // --- set load image 6 ---
        


        // --- set texture environment ---
                sceGsSetDefTexEnv(&texenv.gs_tex, 0, 6720, IMAGE_SIZE / 64, SCE_GS_PSMCT16,
                          8, 8, 1, 0, 0, 0, 1);
                SCE_GIF_CLEAR_TAG(&texenv.giftag);
                texenv.giftag.NLOOP = 11;
                texenv.giftag.EOP = 1;
                texenv.giftag.NREG = 1;
                texenv.giftag.REGS0 = 0xe;
                sceGsSetDefAlphaEnv(&texenv.gs_alpha, 0);
        *(u_long *) &texenv.gs_alpha.alpha1 = SCE_GS_SET_ALPHA(0, 1, 0, 1, 0);
        *(u_long *) &texenv.gs_test =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 3, 0x3);
        texenv.gs_test1addr = (long)SCE_GS_TEST_1;
        *(u_long*)&texenv.gs_tex.clamp1 = SCE_GS_SET_CLAMP(0, 0, 0, 0, 0, 0);

        *(u_long *) &texenv.gs_miptbp1 = SCE_GS_SET_MIPTBP1_1(7232, 2, 7360, 1, 7392, 1);
        texenv.gs_miptbp1addr = (long) SCE_GS_MIPTBP1_1;
        *(u_long *) &texenv.gs_miptbp2 = SCE_GS_SET_MIPTBP2_1(7400, 1, 7402, 1, 7403, 1);
        texenv.gs_miptbp2addr = (long) SCE_GS_MIPTBP2_1;


        // --- open pad ---
	scePadInit(0);
	scePadPortOpen(0, 0, pad_dma_buf);

        frame = 0;
        frame2 = 0;
        odev = !sceGsSyncV(0);

        obj_trans[0] = 0.0f;
        sceSamp0TransMatrix(local_world, local_world, obj_trans);


        while(1){

                // --- read pad ---
	    if(scePadRead(0, 0, rdata) > 0){
		paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
	    }
	    else{
		paddata = 0;
	    }

                if(!(paddata & SCE_PADRright)) {
                        frame2++;
                        f1 = (float) frame2 * 2.0f * PI / 1000.0f;
                        if(f1 > 2.0f * PI) f1 -= 2.0f * PI;
                        camera_p[2] = -5.0f + 200.0f * (sinf(f1) - 1.0f);
                }


                sceGsSwapDBuff(&db, frame);
                sceGsSyncPath(0, 0);


                // ===== draw right grid (mipmap) =====
                                *(u_long*)&texenv.gs_tex.tex11 = SCE_GS_SET_TEX1(0, 5, 1, 5, 0, 0, 0xffb8);
                FlushCache(0);
                sceGsPutDrawEnv(&texenv.giftag);



                // local -> world (rotate)matrix
                sceSamp0UnitMatrix(work);
                sceSamp0RotMatrix(local_world, work, obj_rot);

                // color&normal matrix setting
                sceSamp0NormalLightMatrix(normal_light, light0, light1, light2);
                sceSamp0LightColorMatrix(light_color, color0, color1, color2, ambient);

                // light(normal) -> local_light matrix
                sceSamp0MulMatrix(local_light, normal_light, local_world);

                // local -> world (rotate&translate)matrix
                obj_trans[0] = 6.0f;
                sceSamp0TransMatrix(local_world, local_world, obj_trans);

                // world -> view matrix
                sceSamp0RotCameraMatrix(world_view,
                        camera_p, camera_zd, camera_yd, camera_rot);

                // view -> screen matrix
                sceSamp0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
                                 2048.0f, 2048.0f, 1.0f, 16777215.0f,
                                 1.0f,65536.0f);

                // local -> screen matrix
                sceSamp0MulMatrix(local_screen, view_screen, world_view);

                SetVu1PacketMatrix();



                // --- kick dma to draw right object ---
                *D1_QWC  = 0x00;
                *D1_TADR = (u_int)My_dma_start & 0x0fffffff;
                *D_STAT = 2;
                FlushCache(0);
                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
                sceGsSyncPath(0, 0);


                // ===== draw left grid (no mipmap) =====
                *(u_long*)&texenv.gs_tex.tex11 = SCE_GS_SET_TEX1(0, 0, 1, 1, 0, 0, 0);
                FlushCache(0);
                sceGsPutDrawEnv(&texenv.giftag);

                // local -> world (rotate&translate)matrix
                obj_trans[0] = -12.0f;
                sceSamp0TransMatrix(local_world, local_world, obj_trans);

                // local -> screen matrix
                SetVu1PacketMatrix();

                // --- kick dma to draw left object ---
                *D1_QWC  = 0x00;
                *D1_TADR = (u_int)My_dma_start & 0x0fffffff;
                *D_STAT = 2;
                FlushCache(0);
                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
                sceGsSyncPath(0, 0);

                sceGsSetHalfOffset((frame&1) ? &db.draw1 : &db.draw0, 2048,2048,odev);
                frame++;
                odev = !sceGsSyncV(0);
        }

        return 0;

}

void SetVu1PacketMatrix(void){

        // My_light_matrix <- local_light
        My_light_matrix[0] = local_light[0][0];
        My_light_matrix[1] = local_light[1][0];
        My_light_matrix[2] = local_light[2][0];
        My_light_matrix[3] = local_light[3][0];

        My_light_matrix[4] = local_light[0][1];
        My_light_matrix[5] = local_light[1][1];
        My_light_matrix[6] = local_light[2][1];
        My_light_matrix[7] = local_light[3][1];

        My_light_matrix[8] = local_light[0][2];
        My_light_matrix[9] = local_light[1][2];
        My_light_matrix[10] = local_light[2][2];
        My_light_matrix[11] = local_light[3][2];

        My_light_matrix[12] = local_light[0][3];
        My_light_matrix[13] = local_light[1][3];
        My_light_matrix[14] = local_light[2][3];
        My_light_matrix[15] = local_light[3][3];

        // My_rot_trans_matrix <- world_view
        My_rot_trans_matrix[0] = local_world[0][0];
        My_rot_trans_matrix[1] = local_world[1][0];
        My_rot_trans_matrix[2] = local_world[2][0];
        My_rot_trans_matrix[3] = local_world[3][0];

        My_rot_trans_matrix[4] = local_world[0][1];
        My_rot_trans_matrix[5] = local_world[1][1];
        My_rot_trans_matrix[6] = local_world[2][1];
        My_rot_trans_matrix[7] = local_world[3][1];

        My_rot_trans_matrix[8] = local_world[0][2];
        My_rot_trans_matrix[9] = local_world[1][2];
        My_rot_trans_matrix[10] = local_world[2][2];
        My_rot_trans_matrix[11] = local_world[3][2];

        My_rot_trans_matrix[12] = local_world[0][3];
        My_rot_trans_matrix[13] = local_world[1][3];
        My_rot_trans_matrix[14] = local_world[2][3];
        My_rot_trans_matrix[15] = local_world[3][3];

        // My_matrix <- local_screen
        My_matrix[0] = local_screen[0][0];
        My_matrix[1] = local_screen[1][0];
        My_matrix[2] = local_screen[2][0];
        My_matrix[3] = local_screen[3][0];

        My_matrix[4] = local_screen[0][1];
        My_matrix[5] = local_screen[1][1];
        My_matrix[6] = local_screen[2][1];
        My_matrix[7] = local_screen[3][1];

        My_matrix[8] = local_screen[0][2];
        My_matrix[9] = local_screen[1][2];
        My_matrix[10] = local_screen[2][2];
        My_matrix[11] = local_screen[3][2];

        My_matrix[12] = local_screen[0][3];
        My_matrix[13] = local_screen[1][3];
        My_matrix[14] = local_screen[2][3];
        My_matrix[15] = local_screen[3][3];
}


