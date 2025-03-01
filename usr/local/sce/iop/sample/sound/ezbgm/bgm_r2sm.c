/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *                  I/O Proseccor sample program
 *                          Version 1.20
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                      ezbgm.irx - bgm_r2sm.c
 *                  raw to spu pcm (mono version)
 *
 *   Version   Date            Design    Log
 *  --------------------------------------------------------------------
 *   1.20      Nov.23.1999     morita    modify for EzBGM
 */


#include <kernel.h>

#define SPU_BLOCK 512

//SPÇégÇ¶ÇŒÇ‡Ç¡Ç∆ë¨Ç≠Ç»ÇÈÇ™Åc

void _BgmRaw2SpuMono( unsigned int *src, unsigned int *dst, unsigned int block )
{
	int i;

	for ( i = 0; i < block; i++ )
	{
		memcpy( (void*)((int)dst+i*SPU_BLOCK*2), (void*)((int)src+i*SPU_BLOCK), SPU_BLOCK );
		memcpy( (void*)((int)dst+i*SPU_BLOCK*2+SPU_BLOCK), (void*)((int)src+i*SPU_BLOCK) , SPU_BLOCK );
	}

	return;
}


