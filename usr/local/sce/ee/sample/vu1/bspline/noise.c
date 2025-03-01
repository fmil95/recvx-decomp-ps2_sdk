/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                         - bspline -
 *
 *                         Version 0.3.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *          Name : noise.c
 *          Description : noise function (to deform).
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.3.0          Jun,1,1999     munekis     first version
 */

#include "noise.h"
#include "stdlib.h"

float linear_noise(float, float, float);

#define SIZE 100
#define SCALE 50.0f

float noise[SIZE][SIZE][SIZE];
float scale, pixel_size;


int init_noise() {

    int i, j, k;

    for(k = 0; k < SIZE; k++)
        for(i = 0; i < SIZE; i++) 
            for(j = 0; j < SIZE; j++)
                noise[i][j][k] = (float) rand() / (float) 0x70000000;

    scale = SCALE;
    pixel_size = 1.0f;


    return 0;
}


float turbulence(float u, float v, float k) {

    float t = 0;

    while(scale > pixel_size) {
        t += linear_noise(u / scale, v / scale, k / scale)* scale;
        scale /= 2.0f;
    }

    scale = SCALE;

    return t;
}


float linear_noise(float u, float v, float k) {

    int iu, iv, ik, ip, iq, ir;
    float du, dv, dk, bottom1, top1, bottom, top;

    iu = u; iv = v; ik = k;
    du = u - iu;
    dv = v - iv;
    dk = k - ik;

    iu = iu % SIZE;
    iv = iv % SIZE;
    ik = ik % SIZE;
    ip = (iu + 1) % SIZE;
    iq = (iv + 1) % SIZE;
    ir = (ik + 1) % SIZE;

    bottom1 = noise[iu][iv][ik] + du*(noise[ip][iv][ik] - noise[iu][iv][ik]);
    top1    = noise[iu][iq][ik] + du*(noise[ip][iq][ik] - noise[iu][iq][ik]);
    bottom = bottom1 + dv*(top1 - bottom1);

    bottom1 = noise[iu][iv][ir] + du*(noise[ip][iv][ir] - noise[iu][iv][ir]);
    top1    = noise[iu][iq][ir] + du*(noise[ip][iq][ir] - noise[iu][iq][ir]);
    top = bottom1 + dv*(top1 - bottom1);

    return (bottom + dk*(top-bottom));

}





