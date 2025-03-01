/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 */
/************************************
*
*	misc packet
*
*************************************/
#include <eekernel.h>
#include <eeregs.h>
#include <stdlib.h>
#include <math.h>
#include <libdev.h>
#include <libgraph.h>

union {
        u_long128       l128;
        u_long          l64[2];
        u_int           i32[4];
} Giftag2;


#define V4_32   0x6c


void make_mscal_packet(u_int *dp,int start)
{
        *dp++= SCE_VIF1_SET_FLUSHA(0);
        *dp++= SCE_VIF1_SET_MSCAL(start, 0);
        *dp++=0;
        *dp++=0;
}

