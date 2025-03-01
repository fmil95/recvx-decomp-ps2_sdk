/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 1.6
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                       - mpeg streaming -
 *
 *                         Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            audidec.c
 *                   functions for audio decoder
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.10           12.17.1999      umemura     the first version
 */
#include <stdio.h>
#include <sif.h>
#include <sifdev.h>
#include <librspu2.h>
#include <eekernel.h>
#include "defs.h"
#include "audiodec.h"

#define AU_HEADER_SIZE 40
#define UNIT_SIZE 512
#define PRESET_VALUE(count)	(count)

static void iopGetArea(int *pd0, int *d0, int *pd1, int *d1,
	AudioDec *ad, int pos);
static int sendToIOP2area(int pd0, int d0, int pd1, int d1,
	u_char *ps0, int s0, u_char *ps1, int s1);
static int sendToIOP(int dst, u_char *src, int size);

// ////////////////////////////////////////////////////////////////
//
// Create audio decoder
//
int audioDecCreate(
    AudioDec *ad,
    u_char *buff,
    int buffSize,
    int iopBuffSize
)
{
    ad->state = AU_STATE_INIT;
    ad->hdrCount = 0;
    ad->data = buff;
    ad->put = 0;
    ad->count = 0;
    ad->size = buffSize;
    ad->totalBytes = 0;
    ad->totalBytesSent = 0;

    ad->iopBuffSize = iopBuffSize;
    ad->iopLastPos = 0;

    sceSifInitIopHeap();
    ad->iopBuff = (int)sceSifAllocIopHeap(iopBuffSize);
    if(ad->iopBuff < 0) {
	printf( "Cannot allocate IOP memory\n");
	return 0;
    }
    printf("Allocate IOP memory 0x%08x\n", ad->iopBuff);

    return 1;
}

// ////////////////////////////////////////////////////////////////
//
// Re-initialize audio decoder
//
int audioDecReset(AudioDec *ad)
{
    int i;
    SpuCommonAttr c_attr;

    for( i = 0; i < 2; i++ )
    {
	//--- set commonn attribute
	sceSpu2Remote(1, rSpuSetCore, i );
	c_attr.mask = (SPU_COMMON_MVOLL |  SPU_COMMON_MVOLR);
	c_attr.mvol.left  = 0x0000;	/* Master volume (left) */
	c_attr.mvol.right = 0x0000;	/* Master volume (right) */
	sceSpu2Remote(1, rSpuSetCommonAttr, &c_attr );
    }
    // stop DMA
    sceSpu2Remote(1, rSpuAutoDMAStop );

    ad->state = AU_STATE_INIT;
    ad->hdrCount = 0;
    ad->put = 0;
    ad->count = 0;
    ad->totalBytes = 0;
    ad->totalBytesSent = 0;
    ad->iopLastPos = 0;

}

// ////////////////////////////////////////////////////////////////
//
// Delete audio decoder
//
int audioDecDelete(AudioDec *ad)
{
    sceSifFreeIopHeap((void *)ad->iopBuff);
    return 1;
}

// ////////////////////////////////////////////////////////////////
//
// Get empty areas
//
void audioDecBeginPut(AudioDec *ad,
	u_char **ptr0, int *len0, u_char **ptr1, int *len1)
{
    int len;

    // ///////////////////////////////////////
    //
    // return ADS header area when (state == AU_STATE_INIT)
    //
    if (ad->state == AU_STATE_INIT) {
    	*ptr0 = (u_char*)&ad->sshd + ad->hdrCount;
	*len0 = AU_HDR_SIZE - ad->hdrCount;
	*ptr1 = (u_char*)ad->data;
	*len1 = ad->size;

	return;
    }

    // ///////////////////////////////////////
    //
    // Return the empty areas
    //
    len = ad->size - ad->count;

    if (ad->size -  ad->put >= len) { // area0
    	*ptr0 = ad->data + ad->put;
	*len0 = len;
	*ptr1 = NULL;
	*len1 = 0;
    } else {			    // area0 + area1
    	*ptr0 = ad->data + ad->put;
	*len0 = ad->size - ad->put;
	*ptr1 = ad->data;
	*len1 = len - (ad->size - ad->put);
    }
}

// ////////////////////////////////////////////////////////////////
//
// Update pointer
//
void audioDecEndPut(AudioDec *ad, int size)
{
    if (ad->state == AU_STATE_INIT) {
    	int hdr_add = min(size, AU_HDR_SIZE - ad->hdrCount);
    	ad->hdrCount += hdr_add;

	if (ad->hdrCount >= AU_HDR_SIZE) {
	    ad->state = AU_STATE_PRESET;

	    printf("-------- audio information --------------------\n");
	    printf("[%c%c%c%c]\n"
	       "header size:                            %d\n"
	       "type(0:PCM big, 1:PCM little, 2:ADPCM): %d\n"
	       "sampling rate:                          %dHz\n"
	       "channels:                               %d\n"
	       "interleave size:                        %d\n"
	       "interleave start block address:         %d\n"
	       "interleave end block address:           %d\n",
		    ad->sshd.id[0],
		    ad->sshd.id[1],
		    ad->sshd.id[2],
		    ad->sshd.id[3],
		    ad->sshd.size,
		    ad->sshd.type,
		    ad->sshd.rate,
		    ad->sshd.ch,
		    ad->sshd.interSize,
		    ad->sshd.loopStart,
		    ad->sshd.loopEnd
	    );

	    printf("[%c%c%c%c]\n"
	       "data size:                              %d\n",
		    ad->ssbd.id[0],
		    ad->ssbd.id[1],
		    ad->ssbd.id[2],
		    ad->ssbd.id[3],
		    ad->ssbd.size
	    );

	}
	size -= hdr_add;
    }
    ad->put = (ad->put + size) % ad->size;
    ad->count += size;
    ad->totalBytes += size;
}

// ////////////////////////////////////////////////////////////////
//
// Check to see if enough data is already sent to IOP or not
//
int audioDecIsPreset(AudioDec *ad)
{
    return ad->totalBytesSent >= PRESET_VALUE(ad->iopBuffSize);
}

// ////////////////////////////////////////////////////////////////
//
// Start to play audio data
//
int audioDecStart(AudioDec *ad)
{
    int i;
    SpuCommonAttr c_attr;

    for(i = 0; i < 2; i++) {
	//--- set commonn attribute
	sceSpu2Remote(1, rSpuSetCore, i);
	c_attr.mask = (SPU_COMMON_MVOLL |  SPU_COMMON_MVOLR);
	c_attr.mvol.left  = 0x3fff;	/* Master volume (left) */
	c_attr.mvol.right = 0x3fff;	/* Master volume (right) */
	sceSpu2Remote(1, rSpuSetCommonAttr, &c_attr );
    }

    sceSpu2Remote(1, rSpuAutoDMAWrite, ad->iopBuff,
    	(ad->iopBuffSize/UNIT_SIZE)*UNIT_SIZE, SPU_AUTODMA_LOOP);

    ad->state = AU_STATE_PLAY;
}

// ////////////////////////////////////////////////////////////////
//
// Send data to IOP
//
int audioDecSendToIOP(AudioDec *ad)
{
    int pd0, pd1, d0, d1;
    u_char *ps0, *ps1;
    int s0, s1;
    int count_sent = 0;
    int countAdj;
    int pos;

    switch (ad->state) {
        case AU_STATE_INIT:
	    return 0;
	    break;

        case AU_STATE_PRESET:
	    pd0 = ad->iopBuff + (ad->totalBytesSent) % ad->iopBuffSize;
	    d0 = ad->iopBuffSize - ad->totalBytesSent;
	    pd1 = 0;
	    d1 = 0;
	    break;

        case AU_STATE_PLAY:
	    pos = 
		(sceSpu2Remote(1, rSpuAutoDMAGetStatus ) & 0x00FFFFFF)
			- ad->iopBuff;
	    iopGetArea(&pd0, &d0, &pd1, &d1, ad, pos);
	    break;
    }

    ps0 = ad->data + (ad->put - ad->count + ad->size) % ad->size;
    ps1 = ad->data;

    // adjust to UNIT_SIZE boundary
    countAdj = (ad->count / UNIT_SIZE) * UNIT_SIZE;

    s0 = min(ad->data + ad->size - ps0, countAdj);
    s1 = countAdj - s0;

    if (d0 + d1 >= UNIT_SIZE && s0 + s1 >= UNIT_SIZE) {
    	count_sent = sendToIOP2area(pd0, d0, pd1, d1, ps0, s0, ps1, s1);
    }

    ad->count -= count_sent;

    ad->totalBytesSent += count_sent;
    ad->iopLastPos = (ad->iopLastPos + count_sent) % ad->iopBuffSize;

    return count_sent;
}

// ////////////////////////////////////////////////////////////////
//
// Get empty area of IOP audio buffer
//
static void iopGetArea(int *pd0, int *d0, int *pd1, int *d1,
					AudioDec *ad, int pos)
{
    int len = (pos + ad->iopBuffSize - ad->iopLastPos - UNIT_SIZE)
    			% ad->iopBuffSize;

    // adjust to UNIT_SIZE boundary
    len = (len / UNIT_SIZE) * UNIT_SIZE;

    if (ad->iopBuffSize -  ad->iopLastPos >= len) { // area0
    	*pd0 = ad->iopBuff + ad->iopLastPos;
	*d0 = len;
	*pd1 = 0;
	*d1 = 0;
    } else {			    // area0 + area1
    	*pd0 = ad->iopBuff + ad->iopLastPos;
	*d0 = ad->iopBuffSize - ad->iopLastPos;
	*pd1 = ad->iopBuff;
	*d1 = len - (ad->iopBuffSize - ad->iopLastPos);
    }
}

// ////////////////////////////////////////////////////////////////
//
// Send data to IOP
//
static int sendToIOP2area(int pd0, int d0, int pd1, int d1,
		u_char *ps0, int s0, u_char *ps1, int s1)
{
    if (d0 + d1 < s0 + s1) {
    	int diff = (s0 + s1) - (d0 + d1);
	if (diff >= s1) {
	    s0 -= (diff - s1);
	    s1 = 0;
	} else {
	    s1 -= diff;
	}
    }

    //
    // (d0 + d1 >= s0 + s1)
    //
    if (s0 >= d0) {
    	sendToIOP(pd0,			ps0,		d0);
    	sendToIOP(pd1,			ps0 + d0,	s0 - d0);
    	sendToIOP(pd1 + s0 - d0,	ps1,		s1);
    } else { // s0 < d0
    	if (s1 >= d0 - s0) {
	    sendToIOP(pd0,		ps0,		s0);
	    sendToIOP(pd0 + s0,		ps1,		d0 - s0);
	    sendToIOP(pd1,		ps1 + d0 - s0,	s1 - (d0 - s0));
	} else { // s1 < d0 - s0
	    sendToIOP(pd0,		ps0,		s0);
	    sendToIOP(pd0 + s0,		ps1,		s1);
	}
    }
    return s0 + s1;
}

// ////////////////////////////////////////////////////////////////
//
// Send data to IOP
//
static int sendToIOP(int dst, u_char *src, int size)
{
    sceSifDmaData transData;
    int did;

    if (size <= 0) {
        return 0;
    }

    transData.data = (u_int)src;
    transData.addr = (u_int)dst;
    transData.size = size;
    transData.mode = 0; // caution
    FlushCache(0);

    did = sceSifSetDma( &transData, 1 );

    while (sceSifDmaStat(did) >= 0)
    	;

    return size;
}

