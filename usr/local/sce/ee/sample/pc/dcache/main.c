/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
// Version 0.00
// ASCII
// 
// Copyright (C) 1998-1999 Sony Computer Entertainment Inc. All Rights Reserved.
// 
// Version        Date            Design      Log
// --------------------------------------------------------------------
// 0.00           06/21/99        koji        1st version

#include <stdio.h>
#include <eekernel.h>
#include <libpc.h>

#define BUFSZ	64*2
#define UNCACHED(p) ((char *)((unsigned int)p | 0x20000000))

static char buffer[BUFSZ] __attribute__((aligned (64)));

void CountDcacheMissForOneLq(u_long128 *ptr, int control, u_char *str) {
	int i;
	register u_long128 tmp;
	int counter0, counter1;
	int c0[BUFSZ/16], c1[BUFSZ/16];

	for (i=0; i<(BUFSZ/16); i++) {

		scePcStart(control, 0, 0);
		asm __volatile__ ("
			lq	%0, 0(%1)
			sync.l
			": "=r" (tmp) : "r"  (ptr)
		);
		counter0 = scePcGetCounter0();
		counter1 = scePcGetCounter1();

		c0[i] = counter0;
		c1[i] = counter1;
		ptr++;
	}

	for (i=0; i<(BUFSZ/16); i++)
		printf("%s: %2d,  D$ miss: %2d\n", str, c0[i], c1[i]);
}


int main(void) {
	int control;
	u_long128 *ptr;

	control  = (SCE_PC0_LOAD_COMP   | SCE_PC_U0);
	control |= (SCE_PC1_DCACHE_MISS | SCE_PC_U1);
	control |= SCE_PC_CTE;

	printf("Cached:\n");
	FlushCache(WRITEBACK_DCACHE);
	ptr = (u_long128 *)buffer;
	CountDcacheMissForOneLq(ptr, control, "Num of Load");

	printf("Uncached:\n");
	FlushCache(WRITEBACK_DCACHE);
	ptr = (u_long128 *)UNCACHED(buffer);
	CountDcacheMissForOneLq(ptr, control, "Num of Load");

	control  = (SCE_PC0_CPU_CYCLE   | SCE_PC_U0);
	control |= (SCE_PC1_DCACHE_MISS | SCE_PC_U1);
	control |= SCE_PC_CTE;

	printf("Cached:\n");
	FlushCache(WRITEBACK_DCACHE);
	ptr = (u_long128 *)buffer;
	CountDcacheMissForOneLq(ptr, control, "CPU cycle  ");

	printf("Uncached:\n");
	FlushCache(WRITEBACK_DCACHE);
	ptr = (u_long128 *)UNCACHED(buffer);
	CountDcacheMissForOneLq(ptr, control, "CPU cycle  ");

	return 0;
}

// Local variables:
// tab-width: 4
// End:
// vi:set tabstop=4:
