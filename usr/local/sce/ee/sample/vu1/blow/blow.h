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
 *          Name : blow.h
 *          Description : parameter header file(for blow)
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.3.0          Jun, 2,1999     munekis     first version
 */

// --- params for particle ---
#define PART_SIZE 1.0f
#define PART_SIZE_SHADOW 1.0f
#define LAUNCH_VELOCITY 1.8f
#define BOUNCE_COEFF 0.35f
#define GRAVITY -0.04f
#define BLUR_LENGTH 0.10f
#define SPREAD_SPEED 0.15f
#define SPREAD_ANGLE 1.5f
#define SHADOW_INTENSITY 40
#define PARTICLE_INTENSITY 60
#define INIT_ALPHA 70.0f

#define NUM_EXPLODE 4
#define NUM_PART 1
#define NUM_BLOCK 120

#define PART_BY_FRAME 1

// --- params for screen, image, ... ---
#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 224.0f
#define SCREEN_OFFSET_X 1728.0f
#define SCREEN_OFFSET_Y 1936.0f

#define FOV 42.0f
#define ASPECT 1.42f
#define PI 3.14159265358979323846f

#define IMAGE_SIZE 64

extern u_int bound[NUM_BLOCK * NUM_PART * NUM_EXPLODE];
