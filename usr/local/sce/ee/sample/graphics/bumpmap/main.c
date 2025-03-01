/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - bumpmap -
 *
 *                        Version 1.1.0
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
 *       1.1.0     Sep,10,1999    munekis            first version
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


extern u_int My_dma_start __attribute__((section(".vudata")));
extern u_int My_dma_setting __attribute__((section(".vudata")));
extern float My_matrix[] __attribute__((section(".vudata")));
extern float My_light_color_matrix[] __attribute__((section(".vudata")));
extern u_int buff1_tex[]  __attribute__((section(".vudata")));
extern u_int buff2_tex[]  __attribute__((section(".vudata")));
extern float My_light_matrix[] __attribute__((section(".vudata")));
extern float My_offset[] __attribute__((section(".vudata")));


void SetVu1PacketMatrix(void);
void set_light_matrix(void);

// --- defines ---
#define PI 3.14159265358979323846f
#define RAD_TO_DEG(x) (x * 180.0f / PI)
#define DEG_TO_RAD(x) (x * PI / 180.0f)

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 224.0f
#define SCREEN_OFFSET_X 1728.0f
#define SCREEN_OFFSET_Y 1936.0f

#define IMAGE_SIZE 256


#define VIEW_LENGTH 60.0f
#define INIT_THETA 45.0f
#define INIT_PHI -30.0f
#define ANGLE_DELTA 1.0f

typedef struct {
        sceGifTag giftag;
        sceGsTest gs_test;
        long gs_test1addr;
        sceGsAlphaEnv gs_alpha;
        sceGsTexEnv gs_tex;
}TexEnv;

sceVu0FVECTOR camera_p =   { 50.0f, -31.0f, 50.0f, 0.0f };
sceVu0FVECTOR camera_zd =  { -50.0f, 31.0f, -50.0f, 1.0f };
sceVu0FVECTOR camera_yd =  { 0.0f, 1.0f, 0.0f, 1.0f };
sceVu0FVECTOR camera_rot = { 0.0f, 0.0f,   0.0f, 0.0f };
sceVu0FVECTOR camera_int = { 0.0f, -3.0f, 0.0f, 0.0f };

sceVu0FVECTOR light0 = {  0.2f,  1.5f, 0.5f, 0.0f };
sceVu0FVECTOR light1 = {  1.5f, -0.5f, 0.5f, 0.0f };
sceVu0FVECTOR light2 = { -1.5f, -0.5f, 0.5f, 0.0f };

sceVu0FVECTOR color0  = { 0.9f, 0.9f, 0.9f, 1.0f };
sceVu0FVECTOR color1  = { 0.3f, 0.3f, 0.3f, 1.0f };
sceVu0FVECTOR color2  = { 0.2f, 0.2f, 0.2f, 1.0f };

sceVu0FVECTOR ambient = { 0.1f, 0.1f, 0.1f, 0.0f };


sceVu0FMATRIX local_world;
sceVu0FMATRIX world_view;
sceVu0FMATRIX view_screen;
sceVu0FMATRIX local_screen;

sceVu0FMATRIX normal_light;
sceVu0FMATRIX light_color;
sceVu0FMATRIX local_light;
sceVu0FMATRIX local_color;

u_int paddata;
u_char rdata[32];
u_long128 pad_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

sceGsDBuff      db;
sceGsLoadImage  gs_limage;
TexEnv          texenv;


#define OFFSET1  0.008f
#define OFFSET2  0.014f

int main()
{
        int    odev;
        u_int  frame, toggle_start = 0, toggle_select = 0, bump_tex = 0;
        float  delta, theta, phi, radius = 0.0f, light_angle = 0.0f;
        float  offset, offset_max;

	sceSifInitRpc(0);
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
			    0, NULL) < 0){
	    printf("Can't load module sio2man\n");
	}
	while (sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
			    0, NULL) < 0){
	    printf("Can't load module padman\n");
	}

        delta = DEG_TO_RAD(ANGLE_DELTA);
        theta = DEG_TO_RAD(INIT_THETA);
        phi = DEG_TO_RAD(INIT_PHI);

        sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

        // --- set double buffer ---
        sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224,
                SCE_GS_ZGEQUAL, SCE_GS_PSMZ32, SCE_GS_CLEAR);
        *(u_long *)&db.clear0.rgbaq =
                SCE_GS_SET_RGBAQ(0x00, 0x00, 0x00, 0x80, 0x3f800000);
        *(u_long *)&db.clear1.rgbaq =
                SCE_GS_SET_RGBAQ(0x00, 0x00, 0x00, 0x80, 0x3f800000);


        // --- set texture environment ---
        sceGsSetDefTexEnv(&texenv.gs_tex, 0, 8768, IMAGE_SIZE / 64, SCE_GS_PSMCT32,
                          8, 8, 0, 0, 0, 0, 1);
                SCE_GIF_CLEAR_TAG(&texenv.giftag);
                texenv.giftag.NLOOP = 9;
                texenv.giftag.EOP = 1;
                texenv.giftag.NREG = 1;
                texenv.giftag.REGS0 = 0xe;
                sceGsSetDefAlphaEnv(&texenv.gs_alpha, 0);
        *(u_long *) &texenv.gs_alpha.alpha1 = SCE_GS_SET_ALPHA(0, 1, 0, 1, 0);
        *(u_long *) &texenv.gs_test =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 0x3);
        texenv.gs_test1addr = (long)SCE_GS_TEST_1;
        *(u_long*)&texenv.gs_tex.clamp1 = SCE_GS_SET_CLAMP(0, 0, 0, 0, 0, 0);
	*(u_long*)&texenv.gs_tex.tex11 = SCE_GS_SET_TEX1(0, 0, 1, 1, 0, 0, 0);

        FlushCache(0);
        sceGsPutDrawEnv(&texenv.giftag);
        sceGsSyncPath(0, 0);


        // --- load microcode, textures ---
        *D1_QWC  = 0x00;
        *D1_TADR = (u_int)&My_dma_setting & 0x0fffffff;
        *D_STAT = 2;
        FlushCache(0);
        *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);


        frame = 0;
        odev = !sceGsSyncV(0);

        // --- pad ---
	scePadInit(0);
	scePadPortOpen(0, 0, pad_dma_buf);

        offset = 0.0f;
        offset_max = OFFSET1;

        while(1){


                sceGsSetHalfOffset((frame&1) ? &db.draw1 : &db.draw0, 2048, 2048, odev);
                sceGsSwapDBuff(&db, frame);
                sceGsSyncPath(0, 0);

                set_light_matrix();

		// --- READ PAD ---
		if(scePadRead(0, 0, rdata) > 0){
		    paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
		}
		else{
		    paddata = 0;
		}


		// --- CAMERA ROTATION ---
		if(paddata & SCE_PADLup) {
			phi -= delta;
                        if(phi < -0.4999f * PI) phi = -0.4999f * PI;
		} else if(paddata & SCE_PADLdown) {
			phi += delta;
                        if(phi > 0.0f) phi = 0.0f;
		}

		if(paddata & SCE_PADLright) {
			theta += delta;
		} else if(paddata & SCE_PADLleft) {
			theta -= delta;
		}
		
		camera_p[0] = VIEW_LENGTH * cosf(phi) * cosf(theta);
		camera_p[1] = VIEW_LENGTH * sinf(phi);
		camera_p[2] = VIEW_LENGTH * cosf(phi) * sinf(theta);

		camera_zd[0] = camera_int[0] - camera_p[0]; 
		camera_zd[1] = camera_int[1] - camera_p[1]; 
		camera_zd[2] = camera_int[2] - camera_p[2]; 


                sceVu0LightColorMatrix(light_color, color0, color1, color2, ambient);

                // WORLD --> VIEW MATRIX
                sceVu0CameraMatrix(world_view,
                        camera_p, camera_zd, camera_yd);

                // VIEW --> SCREEN MATRIX
                sceVu0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
                                 2048.0f, 2048.0f, 1.0f, 16777215.0f,
                                 1.0f,65536.0f);

                // local -> screen matrix
                sceVu0MulMatrix(local_screen, view_screen, world_view);

                SetVu1PacketMatrix();


                // --- ROTATE LIGHT ---
                if(paddata & SCE_PADRright) {
                        radius = radius + 2.0f;
                }
                if(paddata & SCE_PADRleft) {
                        radius = radius - 2.0f;
                }


                // --- SELECT BUMP TEXTURE ---
                if((paddata & SCE_PADselect) && !toggle_select) {
                        toggle_select = 1;
                        bump_tex = !bump_tex;
                        if(bump_tex) {
                                buff1_tex[0] = 0x20012240;
                                buff2_tex[0] = 0x20012240;
                                offset_max = OFFSET2;
                        } else {
                                buff1_tex[0] = 0x20011e40;
                                buff2_tex[0] = 0x20011e40;
                                offset_max = OFFSET1;
                        }
                        offset = 0.0f;
                        toggle_start = 0;

                } else if(!(paddata & SCE_PADselect)) {
                        toggle_select = 0;
                }


                // ---- BUMP HEIGHT UP & DOWN ---
                if((paddata & SCE_PADstart) && !toggle_start) {
                        if(offset == 0.0f) toggle_start = 1;
                        if(offset == offset_max) toggle_start = 2;
                }

                if(toggle_start == 1) {
                       offset = offset + 0.0001f;
                       if(offset > offset_max) {
                              toggle_start = 0;
                              offset = offset_max;
                       }
                }

                if(toggle_start == 2) {
                       offset = offset - 0.0001f;
                       if(offset < 0.0f) {
                               toggle_start = 0;
                               offset = 0.0f;
                       }
                }

                My_offset[0] = offset * cosf(radius * PI / 180.0f);
                My_offset[1] = offset * sinf(radius * PI / 180.0f);


                // --- LIGHT ROTATION ---
                if(paddata & SCE_PADRleft) {
                        light_angle = light_angle + 2.0f;
                }
                if(paddata & SCE_PADRright) {
                        light_angle = light_angle - 2.0f;
                }

#define AMP 1.4f
                light0[0] = AMP * cosf(light_angle * PI / 180.0f);
                light0[2] = AMP * sinf(light_angle * PI / 180.0f);



                // --- KICK DMA TO DRAW ---
                *D1_QWC  = 0x00;
                *D1_TADR = (u_int)&My_dma_start & 0x0fffffff;
                *D_STAT = 2;
                FlushCache(0);
                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

                sceGsSyncPath(0, 0);

                frame++;
                odev = !sceGsSyncV(0);
        }

        return 0;

}

void SetVu1PacketMatrix(void){


        // My_matrix <- local_screen
        My_matrix[0] = local_screen[0][0];
        My_matrix[1] = local_screen[0][1];
        My_matrix[2] = local_screen[0][2];
        My_matrix[3] = local_screen[0][3];

        My_matrix[4] = local_screen[1][0];
        My_matrix[5] = local_screen[1][1];
        My_matrix[6] = local_screen[1][2];
        My_matrix[7] = local_screen[1][3];

        My_matrix[8] = local_screen[2][0];
        My_matrix[9] = local_screen[2][1];
        My_matrix[10] = local_screen[2][2];
        My_matrix[11] = local_screen[2][3];

        My_matrix[12] = local_screen[3][0];
        My_matrix[13] = local_screen[3][1];
        My_matrix[14] = local_screen[3][2];
        My_matrix[15] = local_screen[3][3];

        // My_light_color
        My_light_color_matrix[0] = light_color[0][0];
        My_light_color_matrix[1] = light_color[0][1];
        My_light_color_matrix[2] = light_color[0][2];
        My_light_color_matrix[3] = light_color[0][3];

        My_light_color_matrix[4] = light_color[1][0];
        My_light_color_matrix[5] = light_color[1][1];
        My_light_color_matrix[6] = light_color[1][2];
        My_light_color_matrix[7] = light_color[1][3];

        My_light_color_matrix[8] = light_color[2][0];
        My_light_color_matrix[9] = light_color[2][1];
        My_light_color_matrix[10] = light_color[2][2];
        My_light_color_matrix[11] = light_color[2][3];

        My_light_color_matrix[12] = light_color[3][0];
        My_light_color_matrix[13] = light_color[3][1];
        My_light_color_matrix[14] = light_color[3][2];
        My_light_color_matrix[15] = light_color[3][3];

}


void set_light_matrix() {

        sceVu0FVECTOR rot = { 0.0f, 0.0f, 0.0f, 0.0f };
        sceVu0FVECTOR trans = { 0.0f, 2.0f, 0.0f, 0.0f };
        sceVu0FMATRIX local_world, unit, normal_light, local_light;

        sceVu0UnitMatrix(unit);
 

        sceVu0RotMatrix(local_world, unit, rot);
        sceVu0NormalLightMatrix(normal_light, light0, light1, light2);
        sceVu0MulMatrix(local_light, normal_light, local_world);

        // local -> world (rotate&translate)matrix
        sceVu0TransMatrix(local_world, local_world, trans);

        // My_light_matrix <- local_light
        My_light_matrix[0] = local_light[0][0];
        My_light_matrix[1] = local_light[0][1];
        My_light_matrix[2] = local_light[0][2];
        My_light_matrix[3] = local_light[0][3];

        My_light_matrix[4] = local_light[1][0];
        My_light_matrix[5] = local_light[1][1];
        My_light_matrix[6] = local_light[1][2];
        My_light_matrix[7] = local_light[1][3];

        My_light_matrix[8] = local_light[2][0];
        My_light_matrix[9] = local_light[2][1];
        My_light_matrix[10] = local_light[2][2];
        My_light_matrix[11] = local_light[2][3];

        My_light_matrix[12] = local_light[3][0];
        My_light_matrix[13] = local_light[3][1];
        My_light_matrix[14] = local_light[3][2];
        My_light_matrix[15] = local_light[3][3];

        return;
}

