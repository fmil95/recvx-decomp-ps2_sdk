/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - blow -
 *
 *                         Version 0.3.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *          Name : physics.c
 *          Description : particle physics calc functions(for gas & blow).
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.3.0          Jun, 2,1999     munekis     first version
 */

#include <eekernel.h>
#include <eeregs.h>
#include <libgraph.h>
#include <libdma.h>
#include <libdev.h>
#include <math.h>
#include <stdlib.h>
#include <sifdev.h>
#include <libvu0.h>

extern unsigned int My_dma_start __attribute__((section(".vudata")));
extern unsigned int My_dma_start_part __attribute__((section(".vudata")));
extern unsigned int My_dma_start_shadow_part __attribute__((section(".vudata")));

extern unsigned int My_matrix __attribute__((section(".vudata")));
extern unsigned int My_shadow_matrix __attribute__((section(".vudata")));
extern unsigned int My_ground_matrix __attribute__((section(".vudata")));
extern unsigned int My_offset __attribute__((section(".vudata")));
extern unsigned int My_shadow_offset __attribute__((section(".vudata")));

extern unsigned int My_texture1 __attribute__((section(".vudata")));
extern unsigned int My_texture2 __attribute__((section(".vudata")));
extern unsigned int My_texture3 __attribute__((section(".vudata")));

extern unsigned int My_part __attribute__((section(".vudata")));
extern unsigned int My_part1 __attribute__((section(".vudata")));
extern unsigned int My_shadow_part __attribute__((section(".vudata")));
extern unsigned int My_shadow_part1 __attribute__((section(".vudata")));
extern unsigned int My_part_src __attribute__((section(".vudata")));

extern unsigned int My_part_second_pole  __attribute__((section(".vudata")));
extern unsigned int My_part1_second_pole  __attribute__((section(".vudata")));
extern unsigned int My_part_third_pole  __attribute__((section(".vudata")));
extern unsigned int My_part1_third_pole __attribute__((section(".vudata")));

extern unsigned int My_shadow_part_pole2 __attribute__((section(".vudata")));
extern unsigned int My_shadow_part1_pole2 __attribute__((section(".vudata")));
extern unsigned int My_shadow_part_pole3 __attribute__((section(".vudata")));
extern unsigned int My_shadow_part1_pole3 __attribute__((section(".vudata")));


#ifdef _GAS_PARTICLE

#include "gas.h"

// ---- global variables for wind ----
float f_wpower;
float f_wtheta;


void SetParticlePosition(u_int frame) {

	float *prev_pos1, *pos1;
        float *src_vel1, *src_vel2, *src_vel3;
	float *vel1, *vel2, *vel3;
	float *prev_pos2, *pos2, *prev_pos3, *pos3;

	float *spos1, *prev_spos1;
	float *prev_spos2, *spos2, *prev_spos3, *spos3;
	float *sscale1, *sscale2, *sscale3;

	u_int *rgba1, *rgba2, *rgba3;
	u_int *srgba1, *srgba2, *srgba3;
	float accl[3], r1, r2, cosine1, sine1;
	u_int i, j, offset, shadow_offset, part_id, i1;
	int n;
	int iframe;



	// ---- pointer for particle ----
	pos1 = (float*) &My_part;
	prev_pos1 = pos1;
	vel1  = pos1 + 4;
	rgba1 = (u_int*) (vel1 + 4);
	offset = &My_part1 - &My_part;

	src_vel1 = (float*) &My_part_src;
	src_vel2 = src_vel1 + 4;
	src_vel3 = src_vel1 + 8;

	// set pointer for 2nd, 3rd emitter.
	pos2 = (float*) &My_part_second_pole;
	prev_pos2 = pos2;
	vel2  = pos2 + 4;
	rgba2 = (u_int*) (vel2 + 4);

	pos3 = (float*) &My_part_third_pole;
	prev_pos3 = pos3;
	vel3  = pos3 + 4;
	rgba3 = (u_int*) (vel3 + 4);

	// ---- pointer for particle shadow ----
	shadow_offset = &My_shadow_part1 - &My_shadow_part;
	spos1 = (float*) &My_shadow_part;
	prev_spos1 = spos1;
	sscale1 = spos1 + 4;
	srgba1 = (u_int*) (sscale1 + 4);

	spos2 = (float*) &My_shadow_part_pole2;
	prev_spos2 = spos2;
	sscale2 = spos2 + 4;
	srgba2 = (u_int*) (sscale2 + 4);

	spos3 = (float*) &My_shadow_part_pole3;
	prev_spos3 = spos3;
	sscale3 = spos3 + 4;
	srgba3 = (u_int*) (sscale3 + 4);


	
	if(frame == 0) {

                // --- init wind params ---
                f_wpower = 0.0001f;
                f_wtheta = 0.0f;

		for(j = 0; j < NUM_BLOCK; j++) {

			for(i = 0; i < NUM_PART; i++) {

				for(n = 0; n < NUM_EXPLODE ; n++) {	
				        
					src_vel1[0] = 0.0f;
					src_vel1[1] = 0.0f;
					src_vel1[2] = 0.0f;
					src_vel2[0] = 0.0f;
					src_vel2[1] = 0.0f;
					src_vel2[2] = 0.0f;
					src_vel3[0] = 0.0f;
					src_vel3[1] = 0.0f;
					src_vel3[2] = 0.0f;

					spos1[0] = pos1[0] = -5.0f;
			 		spos1[1] = pos1[1] = 0.0f;
			 		spos1[2] = pos1[2] = 0.0f;

					spos2[0] = pos2[0] = 10.0f;
			 		spos2[1] = pos2[1] = 0.0f;
			 		spos2[2] = pos2[2] = 15.0f;

					spos3[0] = pos3[0] = 10.0f;
			 		spos3[1] = pos3[1] = 0.0f;
			 		spos3[2] = pos3[2] = -15.0f;

					vel1[0] = src_vel1[0] * BLUR_LENGTH;
			 		vel1[1] = src_vel1[1] * BLUR_LENGTH;
			 		vel1[2] = src_vel1[2] * BLUR_LENGTH;

					vel2[0] = src_vel2[0] * BLUR_LENGTH;
			 		vel2[1] = src_vel2[1] * BLUR_LENGTH;
			 		vel2[2] = src_vel2[2] * BLUR_LENGTH;

					vel3[0] = src_vel3[0] * BLUR_LENGTH;
			 		vel3[1] = src_vel3[1] * BLUR_LENGTH;
			 		vel3[2] = src_vel3[2] * BLUR_LENGTH;

					sscale1[0] = 2.0f;
					sscale2[0] = 2.0f;
					sscale3[0] = 2.0f;

					rgba1[0] = PARTICLE_INTENSITY;
					rgba1[1] = PARTICLE_INTENSITY;
					rgba1[2] = PARTICLE_INTENSITY;
					rgba1[3] = INIT_ALPHA;

					rgba2[0] = PARTICLE_INTENSITY;
					rgba2[1] = PARTICLE_INTENSITY;
					rgba2[2] = PARTICLE_INTENSITY;
					rgba2[3] = INIT_ALPHA;

					rgba3[0] = PARTICLE_INTENSITY;
					rgba3[1] = PARTICLE_INTENSITY;
					rgba3[2] = PARTICLE_INTENSITY;
					rgba3[3] = INIT_ALPHA;

					srgba1[0] = SHADOW_INTENSITY;
					srgba1[1] = SHADOW_INTENSITY;
					srgba1[2] = SHADOW_INTENSITY;
					srgba1[3] = INIT_ALPHA;

					srgba2[0] = SHADOW_INTENSITY;
					srgba2[1] = SHADOW_INTENSITY;
					srgba2[2] = SHADOW_INTENSITY;
					srgba2[3] = INIT_ALPHA;

					srgba3[0] = SHADOW_INTENSITY;
					srgba3[1] = SHADOW_INTENSITY;
					srgba3[2] = SHADOW_INTENSITY;
					srgba3[3] = INIT_ALPHA;


					pos1 += 12;
					pos2 += 12;
					pos3 += 12;

					spos1 += 12;
					spos2 += 12;
					spos3 += 12;

					sscale1 += 12;
					sscale2 += 12;
					sscale3 += 12;

					vel1 += 12;
					vel2 += 12;
					vel3 += 12;

					rgba1 += 12;
					rgba2 += 12;
					rgba3 += 12;

					srgba1 += 12;
					srgba2 += 12;
					srgba3 += 12;

					src_vel1  += 12;
					src_vel2 += 12;
					src_vel3  += 12;
				}
			}

			pos1  = prev_pos1 + offset;
			vel1  = pos1 + 4;
			rgba1 = (u_int*) (vel1 + 4);
			prev_pos1 = pos1;

			pos2 = prev_pos2 + offset;
			vel2 = pos2 + 4;
			rgba2 = (u_int*) (vel2 + 4);
			prev_pos2 = pos2;

			pos3 = prev_pos3 + offset;
			vel3 = pos3 + 4;
			rgba3 = (u_int*) (vel3 + 4);
			prev_pos3 = pos3;

			spos1 = prev_spos1 + shadow_offset;
			sscale1 = spos1 + 4;
			srgba1 = (u_int*) (sscale1 + 4);
			prev_spos1 = spos1;
			
			spos2 = prev_spos2 + shadow_offset;
			sscale2 = spos2 + 4;
			srgba2 = (u_int*) (sscale2 + 4);
			prev_spos2 = spos2;

			spos3 = prev_spos3 + shadow_offset;
			sscale3 = spos3 + 4;
			srgba3 = (u_int*) (sscale3 + 4);
			prev_spos3 = spos3;

		}
		return;
	
	}


	iframe = frame % 180;
	r1 = PI * (180.0f + 120.0f * sinf((float) iframe *  2.0f * PI / 180.0f)) / 180.0f;
	f_wpower = 0.0005f;
        cosine1 = f_wpower * cosf(r1);
        sine1 = f_wpower * sinf(r1);
        

	// ===== PARTICLE SETTING ======
	for(j = 0; j < NUM_BLOCK; j++) {
		for(i = 0; i < NUM_PART; i++) {

			// start to emit or not.
			part_id = j * NUM_PART + i;
			if(part_id > frame * PART_BY_FRAME) 
				return;
		
			// --- calc particle position --- 
			// write blur partice shadow
			for(n = 0; n < NUM_EXPLODE; n++) {


				// --- set position of each blur particle ---
			        r2 = (float) rand() / (float) RAND_MAX * 0.005f;
			        r1 = ((float) rand() / (float) RAND_MAX - 0.5f);
			        accl[0] = r1 * r2;

			        r1 = ((float) rand() / (float) RAND_MAX);
			        accl[1] = r1 * 0.001f;

			        r2 = (float) rand() / (float) RAND_MAX * 0.005f + 0.001f;
			        r1 = ((float) rand() / (float) RAND_MAX - 0.5f);
			        accl[2] = r1 * r2;
				if(pos1[1] > 2.0f) {
					accl[0] += cosine1;
					accl[2] += sine1;
				}

				src_vel1[0] += accl[0];
				src_vel1[1] += accl[1];
				src_vel1[2] += accl[2];

				pos1[0] += src_vel1[0];
				pos1[1] += src_vel1[1];
				pos1[2] += src_vel1[2];
				spos1[0] = pos1[0];
				spos1[2] = pos1[2];

				// ============================ for 2nd pole ========================
			        r2 = (float) rand() / (float) RAND_MAX * 0.005f;
			        r1 = ((float) rand() / (float) RAND_MAX - 0.5f);
			        accl[0] = r1 * r2;
			        r1 = ((float) rand() / (float) RAND_MAX);
			        accl[1] = r1 * 0.001f;
			        r2 = (float) rand() / (float) RAND_MAX * 0.005f + 0.001f;
			        r1 = ((float) rand() / (float) RAND_MAX - 0.5f);
			        accl[2] = r1 * r2;

				if(pos2[1] > 2.0f) {
					accl[0] += cosine1;
					accl[2] += sine1;
				}

				src_vel2[0] += accl[0];
				src_vel2[1] += accl[1];
				src_vel2[2] += accl[2];

				pos2[0] += src_vel2[0];
				pos2[1] += src_vel2[1];
				pos2[2] += src_vel2[2];
				spos2[0] = pos2[0];
				spos2[2] = pos2[2];

				// --- for 3rd pole ---
				// ============================ for 2nd pole ========================
			        r2 = (float) rand() / (float) RAND_MAX * 0.005f;
			        r1 = ((float) rand() / (float) RAND_MAX - 0.5f);
			        accl[0] = r1 * r2;
			        r1 = ((float) rand() / (float) RAND_MAX);
			        accl[1] = r1 * 0.001f;
			        r2 = (float) rand() / (float) RAND_MAX * 0.005f + 0.001f;
			        r1 = ((float) rand() / (float) RAND_MAX - 0.5f);
			        accl[2] = r1 * r2;

				if(pos3[1] > 2.0f) {
					accl[0] += cosine1;
					accl[2] += sine1;
				}

				src_vel3[0] += accl[0];
				src_vel3[1] += accl[1];
				src_vel3[2] += accl[2];

				pos3[0] += src_vel3[0];
				pos3[1] += src_vel3[1];
				pos3[2] += src_vel3[2];

				spos3[0] = pos3[0];
				spos3[2] = pos3[2];


				// --- velocity ---
				vel1[0] = src_vel1[0] * BLUR_LENGTH;
				vel2[0] = src_vel2[0] * BLUR_LENGTH;
				vel3[0] = src_vel3[0] * BLUR_LENGTH;
				vel1[1] = src_vel1[1] * BLUR_LENGTH;
				vel2[1] = src_vel2[1] * BLUR_LENGTH;
				vel3[1] = src_vel3[1] * BLUR_LENGTH;
				vel1[2] = src_vel1[2] * BLUR_LENGTH;
				vel2[2] = src_vel2[2] * BLUR_LENGTH;
				vel3[2] = src_vel3[2] * BLUR_LENGTH;
					
				// --- set scale of shadow ---
				sscale1[0] = 7.0f;
				sscale2[0] = 7.0f;
				sscale3[0] = 7.0f;




				// --- alpha decay ---
			        i1 = frame % ALPHA_DEC_FRAME;
				if(i1 == 0 && rgba1[3] != 0) {
				    rgba1[3]--;
				    rgba2[3]--;
                                    rgba3[3]--;
				}

				if(i1 == 0 && srgba1[3] != 0) {
				    srgba1[3]--;
				    srgba2[3]--;
                                    srgba3[3]--;
				}

                                
				// =========== CLIP FOR NO.1 ============
				if(pos1[0] > 40.0f  || pos1[0] < -40.0f ||
                                   pos1[1] > 40.0f ||
                                   pos1[2] > 40.0f  || pos1[2] < -40.0f ) {
					spos1[0] = pos1[0] = -5.0f;
					spos1[1] = pos1[1] = 0.0f;
					spos1[2] = pos1[2] = 0.0f;
					src_vel1[0] = 0.0f;
					src_vel1[1] = 0.0f;
					src_vel1[2] = 0.0f;
					rgba1[3] = srgba1[3] = INIT_ALPHA;
				}

				// =========== CLIP FOR NO.2 ============
				if(pos2[0] > 40.0f  || pos2[0] < -40.0f ||
                                   pos2[1] > 40.0f ||
                                   pos2[2] > 40.0f  || pos2[2] < -40.0f ) {
					spos2[0] = pos2[0] = 10.0f;
					spos2[1] = pos2[1] = 0.0f;
					spos2[2] = pos2[2] = 15.0f;
					src_vel2[0] = 0.0f;
					src_vel2[1] = 0.0f;
					src_vel2[2] = 0.0f;
					rgba2[3] = srgba2[3] = INIT_ALPHA;
				}


				// =========== CLIP FOR NO.3 ============
				// for x clip
				if(pos3[0] > 40.0f  || pos3[0] < -40.0f ||
                                   pos3[1] > 40.0f ||
                                   pos3[2] > 40.0f  || pos3[2] < -40.0f ) {
					spos3[0] = pos3[0] = 10.0f;
					spos3[1] = pos3[1] = 0.0f;
					spos3[2] = pos3[2] = -15.0f;
					src_vel3[0] = 0.0f;
					src_vel3[1] = 0.0f;
					src_vel3[2] = 0.0f;
					rgba3[3] = srgba3[3] = INIT_ALPHA;
				}



				src_vel1 += 12;
				src_vel2 += 12;
				src_vel3 += 12;
				pos1 += 12;
				pos2 += 12;
				pos3 += 12;
				vel1 += 12;
				vel2 += 12;
				vel3 += 12;
				rgba1 += 12;
				rgba2 += 12;
				rgba3 += 12;

				spos1 += 12;
				spos2 += 12;
				spos3 += 12;
				sscale1 += 12;
				sscale2 += 12;
				sscale3 += 12;
				srgba1 += 12;
				srgba2 += 12;
				srgba3 += 12;
			}

		} 
	
		pos1 = prev_pos1 + offset; 
		vel1 = pos1 + 4; 
		rgba1 = (u_int*) (vel1 + 4); 
		prev_pos1 = pos1; 

		pos2 = prev_pos2 + offset;
		vel2 = pos2 + 4;
		rgba2 = (u_int*) (vel2 + 4);
		prev_pos2 = pos2;

		pos3 = prev_pos3 + offset;
		vel3 = pos3 + 4;
		rgba3 = (u_int*) (vel3 + 4);
		prev_pos3 = pos3;

		// --- particle shadow ---
		spos1 = prev_spos1 + shadow_offset; 
		sscale1 = spos1 + 4; 
		srgba1 = (u_int*) (sscale1 + 4); 
		prev_spos1 = spos1; 

		spos2 = prev_spos2 + shadow_offset; 
		sscale2 = spos2 + 4; 
		srgba2 = (u_int*) (sscale2 + 4); 
		prev_spos2 = spos2; 

		spos3 = prev_spos3 + shadow_offset; 
		sscale3 = spos3 + 4; 
		srgba3 = (u_int*) (sscale3 + 4); 
		prev_spos3 = spos3; 



	}

	

}

#else


#include "blow.h"

void SetParticlePosition(u_int frame) {

	float *prev_pos1, *pos1, *src_vel, *src_vel2, *src_vel_ori;
	float *vel1, *vel2, *vel3;
	float *prev_pos2, *pos2, *prev_pos3, *pos3;

	float *spos1, *prev_spos1;
	float *prev_spos2, *spos2, *prev_spos3, *spos3;
	float *sscale1, *sscale2, *sscale3;

	u_int *rgba1, *rgba2, *rgba3;
	u_int *srgba1, *srgba2, *srgba3;
	float accl[3], r, r1, r2, v1;
	u_int i, j, offset, shadow_offset, part_id, index;
	int n;



	// ---- pointer for particle ----
	pos1 = (float*) &My_part;
	prev_pos1 = pos1;
	vel1  = pos1 + 4;
	rgba1 = (u_int*) (vel1 + 4);
	offset = &My_part1 - &My_part;

	src_vel = (float*) &My_part_src;
	src_vel2 = src_vel + 4;
	src_vel_ori = src_vel + 8;

	// set pointer for 2nd, 3rd emitter.
	pos2 = (float*) &My_part_second_pole;
	prev_pos2 = pos2;
	vel2  = pos2 + 4;
	rgba2 = (u_int*) (vel2 + 4);

	pos3 = (float*) &My_part_third_pole;
	prev_pos3 = pos3;
	vel3  = pos3 + 4;
	rgba3 = (u_int*) (vel3 + 4);

	// ---- pointer for particle shadow ----
	shadow_offset = &My_shadow_part1 - &My_shadow_part;
	spos1 = (float*) &My_shadow_part;
	prev_spos1 = spos1;
	sscale1 = spos1 + 4;
	srgba1 = (u_int*) (sscale1 + 4);

	spos2 = (float*) &My_shadow_part_pole2;
	prev_spos2 = spos2;
	sscale2 = spos2 + 4;
	srgba2 = (u_int*) (sscale2 + 4);

	spos3 = (float*) &My_shadow_part_pole3;
	prev_spos3 = spos3;
	sscale3 = spos3 + 4;
	srgba3 = (u_int*) (sscale3 + 4);


	if(frame == 0) 
		for(i = 0; i < NUM_BLOCK * NUM_PART * NUM_EXPLODE; i++) bound[i] = 0;
	

	// -- set accl --
	accl[1] = GRAVITY;


	if(frame == 0) {
		for(j = 0; j < NUM_BLOCK; j++) {

			for(i = 0; i < NUM_PART; i++) {

				r = (float) rand() / 0x7fffffff * 2.0f * PI;
				r1 = (float) rand() / 0x7fffffff;

				for(n = 0; n < NUM_EXPLODE ; n++) {	
					// set default velocity
					v1 = (float) rand() / 0x7fffffff;
					src_vel[0] = 0.25f * r1 * cosf(r);
					src_vel[1] = LAUNCH_VELOCITY - v1 * 0.3f;
					src_vel[2] = 0.25f * r1 * sinf(r);

					src_vel_ori[0] = 0.25f * r1 * cosf(r);
					src_vel_ori[1] = LAUNCH_VELOCITY;
					src_vel_ori[2] = 0.25f * r1 * sinf(r);
					r2 = r + SPREAD_ANGLE * PI * ((float) rand() / 0x7fffffff - 0.5f);
					src_vel2[0] = SPREAD_SPEED * cosf(r2);
					src_vel2[1] = LAUNCH_VELOCITY;
					src_vel2[2] = SPREAD_SPEED * sinf(r2);

					spos1[0] = pos1[0] = -5.0f;
			 		spos1[1] = pos1[1] = 0.0f;
			 		spos1[2] = pos1[2] = 0.0f;

					spos2[0] = pos2[0] = 10.0f;
			 		spos2[1] = pos2[1] = 0.0f;
			 		spos2[2] = pos2[2] = 15.0f;

					spos3[0] = pos3[0] = 10.0f;
			 		spos3[1] = pos3[1] = 0.0f;
			 		spos3[2] = pos3[2] = -15.0f;

					vel1[0] = src_vel[0] * BLUR_LENGTH;
			 		vel1[1] = src_vel[1] * BLUR_LENGTH;
			 		vel1[2] = src_vel[2] * BLUR_LENGTH;

					vel2[0] = src_vel[0] * BLUR_LENGTH;
			 		vel2[1] = src_vel[1] * BLUR_LENGTH;
			 		vel2[2] = src_vel[2] * BLUR_LENGTH;

					vel3[0] = src_vel[0] * BLUR_LENGTH;
			 		vel3[1] = src_vel[1] * BLUR_LENGTH;
			 		vel3[2] = src_vel[2] * BLUR_LENGTH;

					sscale1[0] = 2.0f;
					sscale2[0] = 2.0f;
					sscale3[0] = 2.0f;

					rgba1[0] = PARTICLE_INTENSITY;
					rgba1[1] = PARTICLE_INTENSITY;
					rgba1[2] = PARTICLE_INTENSITY;
					rgba1[3] = INIT_ALPHA;

					rgba2[0] = PARTICLE_INTENSITY;
					rgba2[1] = PARTICLE_INTENSITY;
					rgba2[2] = PARTICLE_INTENSITY;
					rgba2[3] = INIT_ALPHA;

					rgba3[0] = PARTICLE_INTENSITY;
					rgba3[1] = PARTICLE_INTENSITY;
					rgba3[2] = PARTICLE_INTENSITY;
					rgba3[3] = INIT_ALPHA;

					srgba1[0] = SHADOW_INTENSITY;
					srgba1[1] = SHADOW_INTENSITY;
					srgba1[2] = SHADOW_INTENSITY;
					srgba1[3] = INIT_ALPHA;

					srgba2[0] = SHADOW_INTENSITY;
					srgba2[1] = SHADOW_INTENSITY;
					srgba2[2] = SHADOW_INTENSITY;
					srgba2[3] = INIT_ALPHA;

					srgba3[0] = SHADOW_INTENSITY;
					srgba3[1] = SHADOW_INTENSITY;
					srgba3[2] = SHADOW_INTENSITY;
					srgba3[3] = INIT_ALPHA;


					pos1 += 12;
					pos2 += 12;
					pos3 += 12;

					spos1 += 12;
					spos2 += 12;
					spos3 += 12;

					sscale1 += 12;
					sscale2 += 12;
					sscale3 += 12;

					vel1 += 12;
					vel2 += 12;
					vel3 += 12;

					rgba1 += 12;
					rgba2 += 12;
					rgba3 += 12;

					srgba1 += 12;
					srgba2 += 12;
					srgba3 += 12;

					src_vel  += 12;
					src_vel2 += 12;
					src_vel_ori  += 12;
				}
			}

			pos1  = prev_pos1 + offset;
			vel1  = pos1 + 4;
			rgba1 = (u_int*) (vel1 + 4);
			prev_pos1 = pos1;

			pos2 = prev_pos2 + offset;
			vel2 = pos2 + 4;
			rgba2 = (u_int*) (vel2 + 4);
			prev_pos2 = pos2;

			pos3 = prev_pos3 + offset;
			vel3 = pos3 + 4;
			rgba3 = (u_int*) (vel3 + 4);
			prev_pos3 = pos3;

			spos1 = prev_spos1 + shadow_offset;
			sscale1 = spos1 + 4;
			srgba1 = (u_int*) (sscale1 + 4);
			prev_spos1 = spos1;
			
			spos2 = prev_spos2 + shadow_offset;
			sscale2 = spos2 + 4;
			srgba2 = (u_int*) (sscale2 + 4);
			prev_spos2 = spos2;

			spos3 = prev_spos3 + shadow_offset;
			sscale3 = spos3 + 4;
			srgba3 = (u_int*) (sscale3 + 4);
			prev_spos3 = spos3;

		}
		return;
	
	}



	// ===== PARTICLE SETTING ======
	for(j = 0; j < NUM_BLOCK; j++) {
		for(i = 0; i < NUM_PART; i++) {

			// start to emit or not.
			part_id = j * NUM_PART + i;
			if(part_id > frame * PART_BY_FRAME) 
				return;
		
			// --- calc particle position --- 
			// write blur partice shadow
			for(n = 0; n < NUM_EXPLODE; n++) {


				// --- set position of each blur particle ---
				src_vel[1] += accl[1];

				pos1[0] += src_vel[0];
				pos1[1] += src_vel[1];
				pos1[2] += src_vel[2];
				spos1[0] = pos1[0];
				spos1[2] = pos1[2];

				// --- for 2nd pole ---
				pos2[0] = pos1[0] + 15.0f;
				pos2[1] = pos1[1];
				pos2[2] = pos1[2] + 15.0f;
				spos2[0] = pos2[0];
				spos2[2] = pos2[2];

				// --- for 3rd pole ---
				pos3[0] = pos1[0] + 15.0f;
				pos3[1] = pos1[1];
				pos3[2] = pos1[2] - 15.0f;
				spos3[0] = pos3[0];
				spos3[2] = pos3[2];

				// --- velocity ---
				vel1[1] = src_vel[1] * BLUR_LENGTH;
				vel2[1] = src_vel[1] * BLUR_LENGTH;
				vel3[1] = src_vel[1] * BLUR_LENGTH;
					
				// --- set scale of shadow ---
				sscale1[0] = 6.0f - pos1[1];
				if(sscale1[0] < 0.0f) sscale1[0] = 0.0f;
				sscale2[0] = 6.0f - pos2[1];
				if(sscale2[0] < 0.0f) sscale2[0] = 0.0f;
				sscale3[0] = 6.0f - pos3[1];
				if(sscale3[0] < 0.0f) sscale3[0] = 0.0f;


				// for x clip
				if(pos1[0] > 40.0f || pos1[0] < -40.0f) {
					pos1[0] = -5.0f;
					pos1[1] = 0.0f;
					pos1[2] = 0.0f;
				}

				// for y clip
				if(pos1[1] > 100.0f || pos1[1] < 0.0f) {
					index = j * NUM_PART * NUM_EXPLODE + i * NUM_EXPLODE + n;
					if(bound[index] == 0) {
						bound[index] = 1; 
						src_vel[0] = src_vel2[0];
						src_vel[2] = src_vel2[2];

						src_vel[1] = - BOUNCE_COEFF * src_vel[1];
						pos1[1] = 0.0f;
					} else {
						bound[index] = 0; 
						pos1[0] = -5.0f;
						pos1[1] = 0.0f;
						pos1[2] = 0.0f;
						src_vel[0] = src_vel_ori[0];
						src_vel[1] = src_vel_ori[1];
						src_vel[2] = src_vel_ori[2];
					}
				}

				// for z clip
				if(pos1[2] > 40.0f || pos1[2] < -40.0f) {

					pos1[0] = -5.0f;
					pos1[1] = 0.0f;
					pos1[2] = 0.0f;
				}


				src_vel  += 12;
				src_vel2 += 12;
				src_vel_ori += 12;
				pos1 += 12;
				pos2 += 12;
				pos3 += 12;
				vel1 += 12;
				vel2 += 12;
				vel3 += 12;
				rgba1 += 12;
				rgba2 += 12;
				rgba3 += 12;

				spos1 += 12;
				spos2 += 12;
				spos3 += 12;
				sscale1 += 12;
				sscale2 += 12;
				sscale3 += 12;
				srgba1 += 12;
				srgba2 += 12;
				srgba3 += 12;
			}

		} 
	
		pos1 = prev_pos1 + offset; 
		vel1 = pos1 + 4; 
		rgba1 = (u_int*) (vel1 + 4); 
		prev_pos1 = pos1; 

		pos2 = prev_pos2 + offset;
		vel2 = pos2 + 4;
		rgba2 = (u_int*) (vel2 + 4);
		prev_pos2 = pos2;

		pos3 = prev_pos3 + offset;
		vel3 = pos3 + 4;
		rgba3 = (u_int*) (vel3 + 4);
		prev_pos3 = pos3;

		// --- particle shadow ---
		spos1 = prev_spos1 + shadow_offset; 
		sscale1 = spos1 + 4; 
		srgba1 = (u_int*) (sscale1 + 4); 
		prev_spos1 = spos1; 

		spos2 = prev_spos2 + shadow_offset; 
		sscale2 = spos2 + 4; 
		srgba2 = (u_int*) (sscale2 + 4); 
		prev_spos2 = spos2; 

		spos3 = prev_spos3 + shadow_offset; 
		sscale3 = spos3 + 4; 
		srgba3 = (u_int*) (sscale3 + 4); 
		prev_spos3 = spos3; 



	}

	

}

#endif
