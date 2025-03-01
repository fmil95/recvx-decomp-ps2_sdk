/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/* 
 *              Emotion Engine Library Sample Program
 *                          Version 0.11
 *                           Shift-JIS
 *
 *      Copyright (C) 2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *	                  EzADPCM - rpc.c
 *		             RPC routine
 *
 *	Version		Date		Design	Log
 *  --------------------------------------------------------------------
 *	0.10		Feb. 3, 2000	kaol		
 *	0.11		Feb.27, 2000	kaol	same as 0.10
 */

#include <eekernel.h>
#include <sif.h>
#include <sifrpc.h>
#include "ezadpcm.h"

#define DATA_SIZE_STRING 64
#define DATA_SIZE_NORMAL 16

// 64 bytes: キャッシュライン不整合を避けるために
static unsigned int sbuf [16] __attribute__((aligned (64)));
static sceSifClientData cd;

// RPC の初期化
int
ezAdpcm_RPCinit (void)
{
    sceSifInitRpc (0);
    while (1) {
	if (sceSifBindRpc (&cd, EzADPCM_DEV, 0) < 0) {
	    printf("error: sceSifBindRpc \n");
	    return (-1);
	}
	if (cd.serve != 0) break;
    }
    return 0;
}

// IOP 側の関数を RPC する
int
ezAdpcm_RPC (int command, int data)
{
    int rsize = 64;
    if ((command & EzADPCM_COMMAND_MASK) == EzADPCM_OPEN) {
	sceSifCallRpc (&cd,  command, 0, (void *) data, DATA_SIZE_STRING,
		       (void *) sbuf, rsize, NULL, NULL);
    } else {
	sbuf[0] = data;
	sceSifCallRpc (&cd,  command, 0, (void *) sbuf, DATA_SIZE_NORMAL,
		       (void *) sbuf, rsize, NULL, NULL);
    }
    return sbuf [0];
}

/* ----------------------------------------------------------------
 *	End on File
 * ---------------------------------------------------------------- */
/* This file ends here, DON'T ADD STUFF AFTER THIS */
