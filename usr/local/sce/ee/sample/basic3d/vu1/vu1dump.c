/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                         - vu1 -
 *
 *                         Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.10
 */

#include <libvu0.h>
disp_vu1regs()
{
	sceVu0IVECTOR vi;
	int adr,i;

	adr = 0x430;
        asm("ctc2 %0, $vi1"::"r"(adr));
        for(i=0; i<16; i++)
        {
                asm volatile ("vlqi.xyzw $vf2,($vi1++)");
                asm volatile ("sqc2 $vf2,0x0(%0)"::"r"(vi));
		printf("vi%02d:%08x %08x %08x %08x\n",i+16,vi[3],vi[2],vi[1],vi[0]);
        }
}

disp_vu1regi()
{
	sceVu0IVECTOR vi;
	int adr,i;

	adr = 0x420;
        asm("ctc2 %0, $vi1"::"r"(adr));
        for(i=0; i<16; i++)
        {
                asm volatile ("vlqi.xyzw $vf2,($vi1++)");
                asm volatile ("sqc2 $vf2,0x0(%0)"::"r"(vi));
		printf("vi%02d:%08x %08x %08x %08x\n",i,vi[3],vi[2],vi[1],vi[0]);
        }
}
disp_vu1regf()
{
	sceVu0FVECTOR vf;
	int adr,i;

	adr = 0x400;
        asm("ctc2 %0, $vi1"::"r"(adr));
        for(i=0; i<32; i++)
        {
                asm volatile ("vlqi.xyzw $vf2,($vi1++)");
                asm volatile ("sqc2 $vf2,0x0(%0)"::"r"(vf));
		printf("vf%02d:%f %f %f %f\n",i,vf[3],vf[2],vf[1],vf[0]);
        }
}
