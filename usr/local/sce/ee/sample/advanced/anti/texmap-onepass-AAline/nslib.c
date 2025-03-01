/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *
 *                       - feather aa1 sample  -
 *
 *                         Version 0.1.0
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                     Name : nslib.c
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.1.0           Oct,07,1999     sasaki    modified from feather
 */

#include <stdio.h>
#include <eeregs.h>
#include <eekernel.h>           // kernel definitions
#include <libvu0.h>             // libvu0 definitions

typedef sceVu0FVECTOR           VECTOR;

static inline void _WatchVu1Reg(float i0, float r0, float i1, VECTOR r1)
{
        asm __volatile__ ("
#       WatchVu1Reg
#       ---------------------------------------------------------------
        ctc2            %0, $vi05
        ctc2            %1, $vi06
        vilwr.x         vi07, (vi05)
        vlqi.xyzw       vf07xyzw, (vi06++)
        nop
        nop
        nop
        nop
        cfc2            %2, $vi07
        sqc2            vf07, 0x00(%3)

        ": : "r" (i0) , "r" (r0), "r" (i1), "r" (r1));
}

void DumpVu1Reg(void)
{
	int i,ii;
        int i_addr, v_addr;
        float fi_addr, fv_addr;
        int i_data;
        float fi_data;
        VECTOR v_data;

        printf ("VIF0_STAT: %08x\n",*VIF0_STAT);
        printf ("VIF1_STAT: %08x\n",*VIF1_STAT);

        /* print register of vu1 */

	printf("VU1 REGISTER DUMP\n");
	for (i=0; i<32; i++) {

		if (i < 16) {
			ii = i;
		} else {
			ii = 0;
		}
        	i_addr = 0x420 + ii;
        	v_addr = 0x400 + i;
        	i_data = 0;
        	fi_addr = *(float*)&i_addr;
        	fv_addr = *(float*)&v_addr;
        	fi_data = *(float*)&i_data;

        	_WatchVu1Reg(fi_addr, fv_addr, fi_data, v_data);

        	FlushCache(WRITEBACK_DCACHE);
        	printf("VI%02d %10d  VF%02d %15f %15f %15f %15f \n",ii,*(int*)&fi_data,i,
			v_data[0], v_data[1], v_data[2], v_data[3]);

	}

}
