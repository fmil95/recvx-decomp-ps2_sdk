/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - tex_swap -
 *
 *                        Version 0.4.0
 *
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : tex_swap.h
 *                     Description : param/prototypes for this sample.
 *
 *       Version   Date           Design              Log
 *  --------------------------------------------------------------------
 *       0.4.0     Jul,12,1999    munekis            first version
 */


#ifndef __TEX_SWAP_H__
#define __TEX_SWAP_H__


#include "mathfunc.h"

#define GROUND_LEVEL -1.0f
#define WALL 50.0f
#define REBOUND_COEFF 0.70f
#define FRICTION_COEFF 0.02f
#define N_SPHERES 24


// --- init values ---
#define INIT_POS_X  0.0f
#define INIT_POS_Y  -30.0f
#define INIT_POS_Z  0.0f

#define GRAVITY 9.8f / 60.0f

typedef struct {
    sceSamp0FVECTOR position;
    sceSamp0FVECTOR velocity;
    sceSamp0FVECTOR accl;
    sceSamp0FVECTOR rotation;
} Object_t;



void init_sphere(void);
void init_position(void);
void calc_position(void);
void set_sphere_matrix(void);


#endif // __TEX_SWAP_H__




