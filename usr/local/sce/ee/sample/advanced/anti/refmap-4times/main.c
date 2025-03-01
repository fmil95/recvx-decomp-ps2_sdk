/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - mug -
 *
 *              -- modified for antialiasing demo --
 *                    -- subpixel shifts --
 *
 *                        Version 0.4.0
 *
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : main.c
 *                     Description : main procedures
 *
 *       Version   Date           Design              Log
 *  --------------------------------------------------------------------
 *       0.4.0     Aug, 3rd ,1999 munekis            first version
 *                 Apr, 28th,2000 NOBsasaki          for aa demo
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


extern u_int My_dma_setup __attribute__((section(".vudata")));
extern u_int My_tex_setup __attribute__((section(".vudata")));
extern u_int My_dma_start __attribute__((section(".vudata")));
extern u_int My_dma_line_start __attribute__((section(".vudata")));

extern float My_matrix[] __attribute__((section(".vudata")));
extern float My_light_color_matrix[] __attribute__((section(".vudata")));

extern u_int My_dma_next  __attribute__((section(".vudata")));
extern u_int My_texture0  __attribute__((section(".vudata")));
extern u_int My_position0  __attribute__((section(".vudata")));
extern u_int My_position1  __attribute__((section(".vudata")));

extern float My_rot_trans_matrix2[] __attribute__((section(".vudata")));
extern float My_rot_trans_matrix3[] __attribute__((section(".vudata")));
extern float My_rot_trans_matrix4[] __attribute__((section(".vudata")));

extern void set_dither_matrix(void);
extern void add_frame(void);

extern void snapshot(int frame);

extern void DumpVu1Reg(void);

void SetVu1PacketMatrix(void);
void MoveGrid(u_int);

#define ERR_STOP while(1)

// ---- structures and defines for physics ----




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
        sceGsAlphaEnv gs_alpha;
        sceGsTexEnv gs_tex;
} TexEnv1;

typedef struct {
        sceGifTag giftag;
        sceGsTest gs_test;
        long gs_test2addr;
        sceGsAlphaEnv2 gs_alpha;
        sceGsTexEnv2 gs_tex;
} TexEnv2;

typedef struct {
	sceGsDispEnv	disp[2];
	sceGifTag	giftag;
	sceGsDrawEnv2	draw;
	sceGsClear	clear;
} sceGsSBuff;


typedef struct {
        sceGifTag giftag;
        sceGsFrame frame2;
        long frame2addr;
        sceGsAlpha alpha2;
        long alpha2addr;
        sceGsTest gs_test2;
        long gs_test2addr;
	sceGsXyoffset xyoffset2;
	long xyoffset2addr;
} LineBuf2;

sceSamp0FVECTOR camera_p =   { 0.0f, 0.0f, 20.0f, 0.0f };
sceSamp0FVECTOR camera_zd =  { 0.0f, 0.0f, -1.0f, 0.0f };
sceSamp0FVECTOR camera_yd =  { 0.0f, 1.0f, 0.0f, 0.0f };
sceSamp0FVECTOR camera_rot = { 0.0f, 0.0f, 0.0f, 0.0f };
sceSamp0FVECTOR camera_int = { 0.0f, 0.0f, 0.0f, 1.0f };
sceSamp0FVECTOR tmp_vector = { 0.0f, 0.0f, 0.0f, 1.0f };
sceSamp0FVECTOR tmp2_vector = { 0.0f, 0.0f, 100.0f, 1.0f };
sceSamp0FVECTOR tmp3_vector = { 10.0f, 0.0f, 0.0f, 1.0f };
sceSamp0FVECTOR tmp4_vector = { 0.0f, 10.0f, 0.0f, 1.0f };
sceSamp0FVECTOR tmp5_vector = { 0.0f, 0.0f, 0.0f, 1.0f };

sceSamp0FVECTOR light0 = {  1.0f,  1.40710f, 1.0f, 0.0f };
sceSamp0FVECTOR light1 = {  -1.0f, 0.0f, 0.0f, 0.0f };
sceSamp0FVECTOR light2 = { -1.5f, -0.5f, 0.5f, 0.0f };

sceSamp0FVECTOR color0  = { 2.5f, 2.5f, 2.5f, 1.0f };
sceSamp0FVECTOR color1  = { 1.0f, 1.0f, 1.0f, 1.0f };
sceSamp0FVECTOR color2  = { 0.0f, 0.0f, 0.0f, 1.0f };

sceSamp0FVECTOR ambient = { 0.2f, 0.2f, 0.2f, 0.0f };

sceSamp0FVECTOR obj_trans = { 0.0f, 5.0f, 0.0f, 0.0f };
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

u_char act_direct[6], act_align[6];
u_long128 pad_dma_buf[scePadDmaBufferMax] __attribute__((aligned(64)));

sceGsDBuff      db;
sceGsLoadImage  gs_limage;
TexEnv1          texenv1;

sceGsSBuff	sb;

LineBuf2	lb1;

// --- prototype ---
void set_object_matrix();


int main()
{
        int    odev;
        u_int  frame;
        float  delta, theta, phi;
        float l_theta, l_phi;

        /* paddata */
        u_int paddata;
        u_char rdata[32];
        int state;
        int pad_ok, act_ok, pad_id, pad_exid;
        int term_id;
        static int set_exmode = 1;
        static int set_actuator = 1;
	int i, ret;
	int obj_flg = 0;	//0: shading  1: wire frame
	float VIEW_LENGTH = 20.0f;

#define INIT_THETA 0.0f
#define INIT_PHI 0.0f
#define ANGLE_DELTA 0.2f

	sceSifInitRpc(0);
	while(sceSifLoadModule("host0:/usr/local/sce/iop/modules/sio2man.irx",
			 0, NULL) < 0){
	    printf("Can't load module sio2man\n");
	}
	while(sceSifLoadModule("host0:/usr/local/sce/iop/modules/padman.irx",
			    0, NULL) < 0){
	    printf("Can't load module padman\n");
	}

	// --- print message ---
	printf("\nsubpixel shift: 640x224 PSMCT32 4 times\n");

        // --- view point params ---
        delta = DEG_TO_RAD(ANGLE_DELTA);
        theta = DEG_TO_RAD(INIT_THETA);
        phi = DEG_TO_RAD(INIT_PHI);

        // --- light direction params ---
        l_theta = DEG_TO_RAD(225.0f);
        l_phi = DEG_TO_RAD(45.0f);

        sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

        // --- set double buffer ---
        sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, 640, 224,
                SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);
        *(u_long *)&db.clear0.rgbaq =
                SCE_GS_SET_RGBAQ(0x00, 0x00, 0x00, 0x00, 0x3f800000);
        *(u_long *)&db.clear1.rgbaq =
                SCE_GS_SET_RGBAQ(0x00, 0x00, 0x00, 0x00, 0x3f800000);

	// --- set drawing buffer environment ---
	sceGsSetDefDrawEnv2(&sb.draw, SCE_GS_PSMCT32, 640, 224, SCE_GS_ZGEQUAL, SCE_GS_PSMZ24);

	*(u_long*)&sb.draw.frame2 = (u_long)0x00000000000a00d2; //FBP=210

	sceGsSetDefClear2(&sb.clear, SCE_GS_ZGEQUAL, 2048-320, 2048-112, 640, 224, 0xe0, 0xe0, 0xe0, 0, 0);

	SCE_GIF_CLEAR_TAG(&sb.giftag);
	sb.giftag.NLOOP = 14;
	sb.giftag.EOP = 1;
	sb.giftag.NREG = 1;
	sb.giftag.REGS0 = 0xe;

        // --- set texture environment ---
        sceGsSetDefTexEnv(&texenv1.gs_tex, 0, 9600, IMAGE_SIZE / 64, SCE_GS_PSMCT32,
                          8, 8, 0, 0, 0, 0, 1);
                SCE_GIF_CLEAR_TAG(&texenv1.giftag);
                texenv1.giftag.NLOOP = 9;
                texenv1.giftag.EOP = 1;
                texenv1.giftag.NREG = 1;
                texenv1.giftag.REGS0 = 0xe;
                sceGsSetDefAlphaEnv(&texenv1.gs_alpha, 0);
        *(u_long *) &texenv1.gs_alpha.alpha1 = SCE_GS_SET_ALPHA(0, 1, 0, 1, 0);
        *(u_long *) &texenv1.gs_test =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 2);
        texenv1.gs_test1addr = (long)SCE_GS_TEST_1;
        *(u_long*)&texenv1.gs_tex.clamp1 = SCE_GS_SET_CLAMP(0, 0, 0, 0, 0, 0);
	*(u_long*)&texenv1.gs_tex.tex11 = SCE_GS_SET_TEX1(0, 0, 1, 1, 0, 0, 0);

        FlushCache(0);
        sceGsPutDrawEnv(&texenv1.giftag);
        sceGsSyncPath(0, 0);

	// --- set alpha2 ---
	SCE_GIF_CLEAR_TAG(&lb1.giftag);
	lb1.giftag.NLOOP = 4;
	lb1.giftag.EOP = 1;
	lb1.giftag.NREG = 1;
	lb1.giftag.REGS0 = 0xe;

        lb1.frame2addr = (long)SCE_GS_FRAME_2;
	*(u_long*)&lb1.frame2 = (u_long)0x00000000000a00d2; //FBP=210
        lb1.alpha2addr = (long)SCE_GS_ALPHA_2;
	*(u_long*)&lb1.alpha2 = SCE_GS_SET_ALPHA(0, 1, 0, 1, 0);
        lb1.gs_test2addr = (long)SCE_GS_TEST_2;
        *(u_long *) &lb1.gs_test2 =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 1, 2);
        lb1.xyoffset2addr = (long)SCE_GS_XYOFFSET_2;

        // --- load texture ---
        *D1_QWC  = 0x00;
        *D1_TADR = (u_int)&My_dma_setup & 0x0fffffff;
        *D_STAT = 2;
        FlushCache(0);
        *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

        sceGsSyncPath(0, 0);

	set_dither_matrix();

        frame = 0;
        odev = !sceGsSyncV(0);

        // --- open pad ---
        scePadInit(0);
        ret = scePadPortOpen(0, 0, pad_dma_buf);
        if (ret == 0) {
            printf("Pad port open error.\n");
            ERR_STOP;
        }
        pad_id = 0;     pad_exid = 0;
        for (i=0; i<6; i++) act_direct[i] = 0;


        while(1){
		

                sceGsSetHalfOffset2((frame&1) ? &sb.draw : &sb.draw, 2048, 2048, odev);

                sceGsSyncPath(0, 0);

                sceGsSwapDBuff(&db, frame);
                sceGsSyncPath(0, 0);

                set_object_matrix();		//set local_world matrix

                // --- get pad status ---
                pad_ok = 0;
                state = scePadGetState(0, 0);
                if (state == scePadStateDiscon) pad_ok = 0;
                if ((state == scePadStateFindCTP1) || (state == scePadStateStable)) {
                        pad_ok = 1;
                } else {
                        pad_ok = 0;
                }

                // --- get pad type ---
                if ((pad_ok == 1) && (pad_exid != 7)) {
                        pad_id = scePadInfoMode(0, 0, InfoModeCurID, 0);
                        if ((set_exmode == 1) && (pad_id == 4)) {                       // STANDARD PAD
                                scePadSetMainMode(0, 0, 1, 3);
                                set_exmode = 0;
                        }
                        pad_exid = scePadInfoMode(0, 0, InfoModeCurExID, 0);
                        pad_ok = 0;
                }

                // --- actuator setting ---
                if ((pad_ok == 1) && (pad_exid == 7) && (set_actuator == 1)) {                  // DUAL SHOCK
                        act_align[0] = 0;
                        act_align[1] = 1;
                        act_align[2] = 0xff;
                        act_align[3] = 0xff;
                        act_align[4] = 0xff;
                        act_align[5] = 0xff;
                        act_ok = scePadSetActAlign(0, 0, act_align);
                        set_actuator = 0;
                        pad_ok = 0;
                }

                // read PAD
                if((pad_ok == 1) && (scePadRead(0, 0, rdata) > 0)) {
                    paddata = 0xffff ^ ((rdata[2] << 8) | rdata[3]);
                    term_id = (rdata[1]>>4);
                }
                else{
                    paddata = 0;
                }

		// ====================== calc camera position ========================

		if(paddata & SCE_PADLup) {
			phi -= delta;
                        if(phi < -0.4999f * PI) phi = -0.4999f * PI;
		} else if(paddata & SCE_PADLdown) {
			phi += delta;
                        if(phi > 0.4999f) phi = 0.4999f;
		}

		if(paddata & SCE_PADLright) {
			theta += delta;
		} else if(paddata & SCE_PADLleft) {
			theta -= delta;
		}

		if(paddata & SCE_PADR1) {
			VIEW_LENGTH -= VIEW_LENGTH/200.0f;
			if (VIEW_LENGTH < 13.0f) VIEW_LENGTH = 13.0f;
		} else if(paddata & SCE_PADR2) {
			VIEW_LENGTH += VIEW_LENGTH/200.0f;
			if (VIEW_LENGTH > 200.0f) VIEW_LENGTH = 200.0f;
		}
		

		camera_p[0] = VIEW_LENGTH * cosf(phi) * cosf(theta);
		camera_p[1] = VIEW_LENGTH * sinf(phi);
		camera_p[2] = VIEW_LENGTH * cosf(phi) * sinf(theta);

		camera_zd[0] = camera_int[0] - camera_p[0]; 
		camera_zd[1] = camera_int[1] - camera_p[1]; 
		camera_zd[2] = camera_int[2] - camera_p[2]; 

                sceSamp0LightColorMatrix(light_color, color0, color1, color2, ambient);


                // ==================== calc light rotation =======================
#define LIGHT_RADIUS 2.0f
                light0[0] = LIGHT_RADIUS * cosf(l_phi) * cosf(l_theta);
                light0[1] = LIGHT_RADIUS * sinf(l_phi);
                light0[2] = LIGHT_RADIUS * cosf(l_phi) * sinf(l_theta);

		if(paddata & SCE_PADRright) {
			l_theta += delta;
		} else if(paddata & SCE_PADRleft) {
			l_theta -= delta;
		}
                
		if(paddata & SCE_PADRup) {
			l_phi += delta;
		} else if(paddata & SCE_PADRdown) {
			l_phi -= delta;
		}

		if(paddata & SCE_PADL1) {
			obj_flg = 0;			//shaded triangle
		} else if(paddata & SCE_PADL2) {
			obj_flg = 1;			//line
		}


        	// --- setup texture mode ---
        	*D1_QWC  = 0x00;
        	*D1_TADR = (u_int)&My_tex_setup & 0x0fffffff;
        	*D_STAT = 2;
        	FlushCache(0);
        	*D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

        	sceGsSyncPath(0, 0);


                // world -> view matrix
                sceSamp0RotCameraMatrix(world_view,
                        camera_p, camera_zd, camera_yd, camera_rot);

                // view -> screen matrix
                sceSamp0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.5f,
                                 2048.0f, 2048.0f, 1.0f, 16777215.0f,
                                 1.0f,65536.0f);

                // local -> screen matrix
                sceSamp0MulMatrix(local_screen, view_screen, world_view);

                SetVu1PacketMatrix();

		// --- clear drawing buffer ---
     		FlushCache(0);
 		sceGsPutDrawEnv(&sb.giftag);
     		sceGsSyncPath(0, 0);

		//set offset
		lb1.xyoffset2 = sb.draw.xyoffset2;

		// --- set offset to drawing buffer ---
	        FlushCache(0);
       		sceGsPutDrawEnv(&lb1.giftag);
        	sceGsSyncPath(0, 0);

                // --- kick dma to draw object ---
		if (obj_flg == 0) {

	                // --- kick dma to draw right object ---
	                *D1_QWC  = 0x00;
	                *D1_TADR = (u_int)&My_dma_start & 0x0fffffff;
	                *D_STAT = 2;
	                FlushCache(0);
	                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

		} else if (obj_flg == 1) {

	                // --- kick dma to draw right object ---
	                *D1_QWC  = 0x00;
	                *D1_TADR = (u_int)&My_dma_line_start & 0x0fffffff;
	                *D_STAT = 2;
	                FlushCache(0);
	                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

		}

        	sceGsSyncPath(0, 0);

		//accumulate with 0.25 alpha
		add_frame();

		// --- clear drawing buffer ---
     		FlushCache(0);
 		sceGsPutDrawEnv(&sb.giftag);
     		sceGsSyncPath(0, 0);

		//set offset y+0.25
		*(u_long*)&lb1.xyoffset2 = *(u_long*)&sb.draw.xyoffset2 | (u_long)0x0000000400000000L;

		// --- set offset to drawing buffer ---
	        FlushCache(0);
       		sceGsPutDrawEnv(&lb1.giftag);
        	sceGsSyncPath(0, 0);

                // --- kick dma to draw object ---
		if (obj_flg == 0) {

	                // --- kick dma to draw right object ---
	                *D1_QWC  = 0x00;
	                *D1_TADR = (u_int)&My_dma_start & 0x0fffffff;
	                *D_STAT = 2;
	                FlushCache(0);
	                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

		} else if (obj_flg == 1) {

	                // --- kick dma to draw right object ---
	                *D1_QWC  = 0x00;
	                *D1_TADR = (u_int)&My_dma_line_start & 0x0fffffff;
	                *D_STAT = 2;
	                FlushCache(0);
	                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

		}

        	sceGsSyncPath(0, 0);

		//accumulate with 0.25 alpha
		add_frame();

		// --- clear drawing buffer ---
     		FlushCache(0);
 		sceGsPutDrawEnv(&sb.giftag);
     		sceGsSyncPath(0, 0);

		//set offset x+0.5
		*(u_long*)&lb1.xyoffset2 = *(u_long*)&sb.draw.xyoffset2 | (u_long)0x0000000000000008L;

		// --- set offset to drawing buffer ---
	        FlushCache(0);
       		sceGsPutDrawEnv(&lb1.giftag);
        	sceGsSyncPath(0, 0);

                // --- kick dma to draw object ---
		if (obj_flg == 0) {

	                // --- kick dma to draw right object ---
	                *D1_QWC  = 0x00;
	                *D1_TADR = (u_int)&My_dma_start & 0x0fffffff;
	                *D_STAT = 2;
	                FlushCache(0);
	                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

		} else if (obj_flg == 1) {

	                // --- kick dma to draw right object ---
	                *D1_QWC  = 0x00;
	                *D1_TADR = (u_int)&My_dma_line_start & 0x0fffffff;
	                *D_STAT = 2;
	                FlushCache(0);
	                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

		}

        	sceGsSyncPath(0, 0);

		//accumulate with 0.25 alpha
		add_frame();

		// --- clear drawing buffer ---
     		FlushCache(0);
 		sceGsPutDrawEnv(&sb.giftag);
     		sceGsSyncPath(0, 0);

		//set offset x+0.5 y+0.25
		*(u_long*)&lb1.xyoffset2 = *(u_long*)&sb.draw.xyoffset2 | (u_long)0x0000000400000008L;

		// --- set offset to drawing buffer ---
	        FlushCache(0);
       		sceGsPutDrawEnv(&lb1.giftag);
        	sceGsSyncPath(0, 0);

                // --- kick dma to draw object ---
		if (obj_flg == 0) {

	                // --- kick dma to draw right object ---
	                *D1_QWC  = 0x00;
	                *D1_TADR = (u_int)&My_dma_start & 0x0fffffff;
	                *D_STAT = 2;
	                FlushCache(0);
	                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

		} else if (obj_flg == 1) {

	                // --- kick dma to draw right object ---
	                *D1_QWC  = 0x00;
	                *D1_TADR = (u_int)&My_dma_line_start & 0x0fffffff;
	                *D_STAT = 2;
	                FlushCache(0);
	                *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);

		}

        	sceGsSyncPath(0, 0);

		//accumulate with 0.25 alpha
		add_frame();

                frame++;

                odev = !sceGsSyncV(0);

        }

        return 0;

}

void SetVu1PacketMatrix(void){


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

        // My_light_color
        My_light_color_matrix[0] = light_color[0][0];
        My_light_color_matrix[1] = light_color[1][0];
        My_light_color_matrix[2] = light_color[2][0];
        My_light_color_matrix[3] = light_color[3][0];

        My_light_color_matrix[4] = light_color[0][1];
        My_light_color_matrix[5] = light_color[1][1];
        My_light_color_matrix[6] = light_color[2][1];
        My_light_color_matrix[7] = light_color[3][1];

        My_light_color_matrix[8] = light_color[0][2];
        My_light_color_matrix[9] = light_color[1][2];
        My_light_color_matrix[10] = light_color[2][2];
        My_light_color_matrix[11] = light_color[3][2];

        My_light_color_matrix[12] = light_color[0][3];
        My_light_color_matrix[13] = light_color[1][3];
        My_light_color_matrix[14] = light_color[2][3];
        My_light_color_matrix[15] = light_color[3][3];

}

typedef struct {
        sceGifTag       giftag;
        u_int           dt[0x80];
} GsPacketData __attribute__((aligned(16)));

void set_dither_matrix(void)
{
	int i,nloop;
	GsPacketData gpd;

	nloop = 3;

       	SCE_GIF_CLEAR_TAG(&gpd.giftag);
       	gpd.giftag.NLOOP = nloop;
       	gpd.giftag.EOP = 1;
       	gpd.giftag.NREG = 1;
       	gpd.giftag.REGS0 = 0xe; // A_D

	i=0;
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x0000001a; gpd.dt[i+1] = (u_int)0x00000000; gpd.dt[i] = (u_int)0x00000001; i+=4;
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x00000044; gpd.dt[i+1] = (u_int)0x60712435; gpd.dt[i] = (u_int)0x71603524; i+=4;
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x00000046; gpd.dt[i+1] = (u_int)0x00000000; gpd.dt[i] = (u_int)0x00000001; i+=4;

	*D2_QWC = gpd.giftag.NLOOP+1;
	
       	*D2_MADR = (u_int)&gpd.giftag & 0x0fffffff;
	FlushCache(0);
       	*D2_CHCR = (1 << 8) | 1;
	
       	sceGsSyncPath(0, 0);

}

void add_frame(void)
{
	int i,nloop;
	GsPacketData gpd;

	nloop = 13;

       	SCE_GIF_CLEAR_TAG(&gpd.giftag);
       	gpd.giftag.NLOOP = nloop;
       	gpd.giftag.EOP = 1;
       	gpd.giftag.NREG = 1;
       	gpd.giftag.REGS0 = 0xe; // A_D

	i=0;
//ALPHA_1 A=0 B=2 C=2 D=1 FIX=33
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x00000042; gpd.dt[i+1] = (u_int)0x00000021; gpd.dt[i] = (u_int)0x00000068; i+=4;
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x00000014; gpd.dt[i+1] = (u_int)0x00000000; gpd.dt[i] = (u_int)0x00000000; i+=4;
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x00000018; gpd.dt[i+1] = (u_int)0x00000000; gpd.dt[i] = (u_int)0x00000000; i+=4;
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x0000004e; gpd.dt[i+1] = (u_int)0x00000001; gpd.dt[i] = (u_int)0x0100008c; i+=4;
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x00000047; gpd.dt[i+1] = (u_int)0x00000000; gpd.dt[i] = (u_int)0x00030000; i+=4;
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x00000006; gpd.dt[i+1] = (u_int)0x00000006; gpd.dt[i] = (u_int)0xa8029a40; i+=4;
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x00000001; gpd.dt[i+1] = (u_int)0x00000000; gpd.dt[i] = (u_int)0x80808080; i+=4;
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x00000000; gpd.dt[i+1] = (u_int)0x00000000; gpd.dt[i] = (u_int)0x0000015e; i+=4;
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x00000003; gpd.dt[i+1] = (u_int)0x00000000; gpd.dt[i] = (u_int)0x00080008; i+=4;
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x00000004; gpd.dt[i+1] = (u_int)0x00000000; gpd.dt[i] = (u_int)0x00000000; i+=4;
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x00000003; gpd.dt[i+1] = (u_int)0x00000000; gpd.dt[i] = (u_int)0x0e082808; i+=4;
gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x00000004; gpd.dt[i+1] = (u_int)0x00000000; gpd.dt[i] = (u_int)0x0e002800; i+=4;

gpd.dt[i+3] = (u_int)0x0; gpd.dt[i+2] = (u_int)0x0000003f; gpd.dt[i+1] = (u_int)0x00000000; gpd.dt[i] = (u_int)0x00000000; i+=4;

	*D2_QWC = gpd.giftag.NLOOP+1;
	
       	*D2_MADR = (u_int)&gpd.giftag & 0x0fffffff;
	FlushCache(0);
       	*D2_CHCR = (1 << 8) | 1;
	
       	sceGsSyncPath(0, 0);

}

