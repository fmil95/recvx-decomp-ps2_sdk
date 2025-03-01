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
 *                     Name : phys.c
 *                     Description : main procedures
 *
 *       Version   Date           Design              Log
 *  --------------------------------------------------------------------
 *       0.4.0     Jul,12,1999    munekis            first version
 */


#include <stdlib.h>
#include <math.h>
#include "tex_swap.h"

extern Object_t spheres[N_SPHERES];
extern float My_rot_trans_matrix[] __attribute__((section(".vudata")));
extern float My_light_matrix[] __attribute__((section(".vudata")));
extern float My_light_color_matrix[] __attribute__((section(".vudata")));
extern float matrix_sphere0[] __attribute__((section(".vudata")));
extern float matrix_sphere1[] __attribute__((section(".vudata")));



#define RAND_VALUE() ((float) rand() / ((float) RAND_MAX))

void init_sphere(void) {

        int i, j, k;
   
        for(i = 0; i < N_SPHERES; i++) {
                j = i / 6;
                k = i % 6;
                spheres[i].position[0] = 10.0f * RAND_VALUE() - 5.0f + j * 10.0f - 20.0f;
                spheres[i].position[1] = -10.0f * RAND_VALUE() - 40.0f;
                                spheres[i].position[2] = 10.0f * RAND_VALUE() - 5.0f + k * 10.0f - 20.0f;
                spheres[i].position[3] = 1.0f;

                spheres[i].velocity[0] = 2.0f * RAND_VALUE() - 1.0f;
                spheres[i].velocity[1] = 0.0f;
                spheres[i].velocity[2] = 2.0f * RAND_VALUE() - 1.0f;
                spheres[i].velocity[3] = 0.0f;

                spheres[i].accl[0] = 0.0f;
                spheres[i].accl[1] = 0.0f;
                spheres[i].accl[2] = 0.0f;
                spheres[i].accl[3] = 0.0f;

                spheres[i].rotation[0] = 0.0f;
                spheres[i].rotation[1] = 0.0f;
                spheres[i].rotation[2] = 0.0f;
                spheres[i].rotation[3] = 0.0f;
                
        }
        return;
}

void calc_position() {

        int i, j;
        float x, y, z, distance;
        
        for(i = 0; i < N_SPHERES; i++) {

                spheres[i].accl[0] = 0.0f;
                spheres[i].accl[1] = GRAVITY;
                spheres[i].accl[2] = 0.0f;
                if(spheres[i].position[1] == GROUND_LEVEL) {
                        spheres[i].accl[0] = -FRICTION_COEFF * spheres[i].velocity[0];
                        spheres[i].accl[2] = -FRICTION_COEFF * spheres[i].velocity[2];
                }

                spheres[i].velocity[0] += spheres[i].accl[0];
                spheres[i].velocity[1] += spheres[i].accl[1];
                spheres[i].velocity[2] += spheres[i].accl[2];
                
                spheres[i].position[0] += spheres[i].velocity[0];
                spheres[i].position[1] += spheres[i].velocity[1];
                spheres[i].position[2] += spheres[i].velocity[2];
        

                if(spheres[i].position[1] >= GROUND_LEVEL) {
                        spheres[i].position[1] = GROUND_LEVEL;
                        spheres[i].velocity[1] *= - REBOUND_COEFF;
                }

                // ---- x coordinates ----
                if(spheres[i].position[0] > WALL) {
                        spheres[i].position[0] = WALL;
                        spheres[i].velocity[0] *= - REBOUND_COEFF;
                }
                if(spheres[i].position[0] < -WALL) {
                        spheres[i].position[0] = -WALL;
                        spheres[i].velocity[0] *= - REBOUND_COEFF;
                }

                if(spheres[i].position[2] > WALL) {
                        spheres[i].position[2] = WALL;
                        spheres[i].velocity[2] *= - REBOUND_COEFF;
                }
                if(spheres[i].position[2] < -WALL) {
                        spheres[i].position[2] = -WALL;
                        spheres[i].velocity[2] *= - REBOUND_COEFF;
                }

                spheres[i].rotation[2] -= (RAD_TO_DEG(spheres[i].velocity[0]) * 0.005f);
                spheres[i].rotation[0] -= (RAD_TO_DEG(spheres[i].velocity[2]) * 0.005f); 


        }

#define RADIUS 5.0f

        for(i = 0; i < N_SPHERES - 1; i++) {
	        for(j = i+1; j < N_SPHERES; j++) {
                        x = spheres[i].position[0] - spheres[j].position[0];                   
                        y = spheres[i].position[1] - spheres[j].position[1];                   
                        z = spheres[i].position[2] - spheres[j].position[2];
                        distance = x*x + y*y + z*z;
                        if(distance < 2.0f * 2.0f * RADIUS * RADIUS) {
                                distance = sqrtf(distance);
                                spheres[i].position[0] 
                                    = spheres[j].position[0] + 10.0f * x / distance; 
                                spheres[i].position[1] 
                                    = spheres[j].position[1] + 10.0f * y / distance; 
                                spheres[i].position[2] 
                                    = spheres[j].position[2] + 10.0f * z / distance; 

                                x = spheres[i].velocity[0];
                                y = spheres[i].velocity[1];
                                z = spheres[i].velocity[2];

                                spheres[i].velocity[0] = spheres[j].velocity[0];
                                spheres[i].velocity[1] = spheres[j].velocity[1];
                                spheres[i].velocity[2] = spheres[j].velocity[2];
                                spheres[j].velocity[0] = x;
                                spheres[j].velocity[1] = y;
                                spheres[j].velocity[2] = z;

                        }                
	        }
        }

        return;
}

extern sceSamp0FVECTOR light0;
extern sceSamp0FVECTOR light1;
extern sceSamp0FVECTOR light2;

void set_sphere_matrix() {

        int i, offset;
        float *sphere_matrix;

        sceSamp0FVECTOR rot = { 0.0f, 0.0f, 0.0f, 0.0f };
        sceSamp0FVECTOR trans = { 0.0f, 2.0f, 0.0f, 0.0f };
        sceSamp0FMATRIX local_world, unit, normal_light, local_light;

        sceSamp0UnitMatrix(unit);
 

        sceSamp0RotMatrix(local_world, unit, rot);
        sceSamp0NormalLightMatrix(normal_light, light0, light1, light2);
        sceSamp0MulMatrix(local_light, normal_light, local_world);

        // local -> world (rotate&translate)matrix
        sceSamp0TransMatrix(local_world, local_world, trans);

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
       sphere_matrix = (float*) matrix_sphere0;
       offset = (float*) matrix_sphere1 - (float*) matrix_sphere0;


        for(i = 0; i < N_SPHERES; i++) {
                sceSamp0RotMatrix(local_world, unit, spheres[i].rotation);
                sceSamp0NormalLightMatrix(normal_light, light0, light1, light2);
                sceSamp0MulMatrix(local_light, normal_light, local_world);

                // local -> world (rotate&translate)matrix
                sceSamp0TransMatrix(local_world, local_world, spheres[i].position);

                // sphere_matrix <- world_view
                sphere_matrix[0] = local_world[0][0];
                sphere_matrix[1] = local_world[1][0];
                sphere_matrix[2] = local_world[2][0];
                sphere_matrix[3] = local_world[3][0];

                sphere_matrix[4] = local_world[0][1];
                sphere_matrix[5] = local_world[1][1];
                sphere_matrix[6] = local_world[2][1];
                sphere_matrix[7] = local_world[3][1];

                sphere_matrix[8] = local_world[0][2];
                sphere_matrix[9] = local_world[1][2];
                sphere_matrix[10] = local_world[2][2];
                sphere_matrix[11] = local_world[3][2];

                sphere_matrix[12] = local_world[0][3];
                sphere_matrix[13] = local_world[1][3];
                sphere_matrix[14] = local_world[2][3];
                sphere_matrix[15] = local_world[3][3];

                sphere_matrix += 16;

                sphere_matrix[0] = local_light[0][0];
                sphere_matrix[1] = local_light[1][0];
                sphere_matrix[2] = local_light[2][0];
                sphere_matrix[3] = local_light[3][0];

                sphere_matrix[4] = local_light[0][1];
                sphere_matrix[5] = local_light[1][1];
                sphere_matrix[6] = local_light[2][1];
                sphere_matrix[7] = local_light[3][1];

                sphere_matrix[8] = local_light[0][2];
                sphere_matrix[9] = local_light[1][2];
                sphere_matrix[10] = local_light[2][2];
                sphere_matrix[11] = local_light[3][2];

                sphere_matrix[12] = local_light[0][3];
                sphere_matrix[13] = local_light[1][3];
                sphere_matrix[14] = local_light[2][3];
                sphere_matrix[15] = local_light[3][3];

                sphere_matrix = sphere_matrix - 16 + offset;

        }

        return;
}
