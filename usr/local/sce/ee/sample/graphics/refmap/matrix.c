/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - mug -
 *
 *                        Version 0.4.0
 *
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : matrix.c
 *                     Description : main procedures
 *
 *       Version   Date           Design              Log
 *  --------------------------------------------------------------------
 *       0.4.0     Aug, 3rd ,1999 munekis            first version
 */


#include <stdlib.h>
#include <math.h>
#include <sifdev.h>
#include "mathfunc.h"

extern float My_rot_trans_matrix[] __attribute__((section(".vudata")));
extern float My_light_matrix[] __attribute__((section(".vudata")));

extern sceSamp0FVECTOR light0;
extern sceSamp0FVECTOR light1;
extern sceSamp0FVECTOR light2;
extern unsigned int paddata;

sceSamp0FVECTOR rot = { 0.0f, 0.0f, 0.0f, 0.0f };
sceSamp0FVECTOR trans = { 0.0f, 2.0f, 0.0f, 0.0f };


void set_object_matrix() {

        sceSamp0FMATRIX local_world, unit, normal_light, local_light;

        sceSamp0UnitMatrix(unit);

        
       

        rot[0] += 0.0241f;
        rot[1] += 0.0373f;
        rot[2] += 0.0217f;

        if(rot[0] > 2.0f * PI) rot[0] -= 2.0f * PI;
        if(rot[1] > 2.0f * PI) rot[1] -= 2.0f * PI;
        if(rot[2] > 2.0f * PI) rot[2] -= 2.0f * PI;



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


        return;
}
