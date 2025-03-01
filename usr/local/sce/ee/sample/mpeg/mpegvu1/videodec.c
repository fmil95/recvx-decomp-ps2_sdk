/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                       - mpeg csc on vu1 -
 *
 *                         Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 2000 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                         videodec.c
 *                 functions for video decoder
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.10           12.17.1999      umemura     the first version
 *       0.20           01.17.2000      umemura     vu1
 */
#include <stdio.h>
#include <eetypes.h>
#include "defs.h"
#include "vobuf.h"
#include "videodec.h"
#include "disp.h"

extern VideoDec videoDec;
extern Display display;

void videoDecMain(VideoDec *vd);
int decBs0(VideoDec *vd);
void endDisplay();
int mpegError(sceMpeg *mp, sceMpegCbDataError *cberror, void *anyData);
int mpegNodata(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData);
int mpegStopDMA(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData);
int mpegRestartDMA(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData);
int mpegTS(sceMpeg *mp, sceMpegCbDataTimeStamp *cbts, void *anyData);
static int cpy2area(u_char *pd0, int d0, u_char *pd1, int d1,
    u_char *ps0, int s0, u_char *ps1, int s1);

extern int vblankCount;

// ////////////////////////////////////////////////////////////////
//
//  Create video decoder
//
int videoDecCreate(VideoDec *vd,
    u_char *mpegWork, int mpegWorkSize,
    u_long128 *data, u_long128 *tag,
    int tagSize, TimeStamp *pts, int n_pts)
{
    // Create sceMpeg
    sceMpegCreate(&vd->mpeg, mpegWork, mpegWorkSize);

    // Add Callbacks
    sceMpegAddCallback(&vd->mpeg,
    	sceMpegCbError, (sceMpegCallback)mpegError, NULL);
    sceMpegAddCallback(&vd->mpeg,
    	sceMpegCbNodata, mpegNodata, NULL);
    sceMpegAddCallback(&vd->mpeg,
    	sceMpegCbStopDMA, mpegStopDMA, NULL);
    sceMpegAddCallback(&vd->mpeg,
    	sceMpegCbRestartDMA, mpegRestartDMA, NULL);
    sceMpegAddCallback(&vd->mpeg, sceMpegCbTimeStamp,
    	(sceMpegCallback)mpegTS, NULL);

    vd->state = VD_STATE_NORMAL;

    // Create input video buffer
    viBufCreate(&vd->vibuf, data, tag, tagSize, pts, n_pts);

    cscVu1Init(&vd->csc);

    return 1;
}

// ////////////////////////////////////////////////////////////////
//
//  Set decode mode
//
void videoDecSetDecodeMode(VideoDec *vd, int ni, int np, int nb)
{
    sceMpegSetDecodeMode(&vd->mpeg, ni, np, nb);
}

// ////////////////////////////////////////////////////////////////
//
//  Choose stream to be decoded
//
int videoDecSetStream(VideoDec *vd, int strType, int ch,
	sceMpegCallback cb, void *data)
{
    sceMpegAddStrCallback(&vd->mpeg, strType, ch, cb, data);
    return 1;
}

// ////////////////////////////////////////////////////////////////
//
//  Get areas to put data in video input buffer
//
void videoDecBeginPut(VideoDec *vd,
	u_char **ptr0, int *len0, u_char **ptr1, int *len1)
{
    viBufBeginPut(&vd->vibuf, ptr0, len0, ptr1, len1);
}

// ////////////////////////////////////////////////////////////////
//
//  Proceed pointers of video input buffer
//
void videoDecEndPut(VideoDec *vd, int size)
{
    viBufEndPut(&vd->vibuf, size);
}

// ////////////////////////////////////////////////////////////////
//
//  Delete video decoder
//
int videoDecDelete(VideoDec *vd)
{
    viBufDelete(&vd->vibuf);
    sceMpegDelete(&vd->mpeg);

    return 1;
}

// ////////////////////////////////////////////////////////////////
//
//  Abort decoding
//
void videoDecAbort(VideoDec *vd)
{
    vd->state = VD_STATE_ABORT;
}

// ////////////////////////////////////////////////////////////////
//
//  Get state of video decoder
//
u_int videoDecGetState(VideoDec *vd)
{
    return vd->state;
}

// ////////////////////////////////////////////////////////////////
//
//  Set state of video decoder
//
u_int videoDecSetState(VideoDec *vd, u_int state)
{
    u_int old = vd->state;

    vd->state = state;

    return old;
}

// ////////////////////////////////////////////////////////////////
//
//  Put time stamp to the video decoder
//
int videoDecPutTs(VideoDec *vd, long pts_val,
    long dts_val, u_char *start, int len)
{
    TimeStamp ts;

    // Set PTS
    ts.pts = pts_val;
    ts.dts = dts_val;
    ts.pos = start - (u_char*)vd->vibuf.data;
    ts.len = len;
    return viBufPutTs(&videoDec.vibuf, &ts);
}

// ////////////////////////////////////////////////////////////////
//
//  Data size in video input buffer
//
int videoDecInputCount(VideoDec *vd)
{
    return viBufCount(&vd->vibuf);
}

// ////////////////////////////////////////////////////////////////
//
//  Empty space size in video input buffer
//
int videoDecInputSpaceCount(VideoDec *vd)
{
    u_char *ptr0;
    u_char *ptr1;
    int len0, len1;

    videoDecBeginPut(vd, &ptr0, &len0, &ptr1, &len1);

    return len0 + len1;
}

// ////////////////////////////////////////////////////////////////
//
//  Flush video decoder
//
int videoDecFlush(VideoDec *vd)
{
    u_char *pd0;
    u_char *pd1;
    u_char *pd0Unc;
    u_char *pd1Unc;
    u_char seq_end_code[4] = {0x00, 0x00, 0x01, 0xb7};
    int d0, d1;
    int len;

    videoDecBeginPut(vd, &pd0, &d0, &pd1, &d1);

    if (d0 + d1 < 4) {
    	return 0;
    }

    pd0Unc = (u_char*)UncAddr(pd0);
    pd1Unc = (u_char*)UncAddr(pd1);

    len = cpy2area(pd0Unc, d0, pd1Unc, d1, seq_end_code, 4, NULL, 0);

    videoDecEndPut(&videoDec, len);

    viBufFlush(&vd->vibuf);

    if (vd->state == VD_STATE_NORMAL) {
	vd->state = VD_STATE_FLUSH;
    }

    return 1;
}

// ////////////////////////////////////////////////////////////////
//
//  Check to see if input buffer and reference image buffer are
//  flushed
//
int videoDecIsFlushed(VideoDec *vd)
{
    return (videoDecInputCount(vd) == 0)
    	&& sceMpegIsRefBuffEmpty(&vd->mpeg);
}

// ////////////////////////////////////////////////////////////////
//
//  Main function of decode thread
//
void videoDecMain(VideoDec *vd)
{
    viBufReset(&vd->vibuf);

    voBufReset(&voBuf);

    decBs0(vd);

    while (voBuf.count)
    	;

     videoDecSetState(vd, VD_STATE_END);
}

// ////////////////////////////////////////////////////////////////
//
// Decode bitstream using MPEG decoder
//
// return value
//     1: normal end
//     -1: aborted
int decBs0(VideoDec *vd)
{
    VoData *voData;
    sceIpuRAW8 *raw8;
    int ret;
    int status = 1;
    sceMpeg *mp = &vd->mpeg;
    int picture_structure;
    extern int csct;

    // ////////////////////////////
    // 
    //  Main loop to decode MPEG bitstream
    // 
    while (!sceMpegIsEnd(&vd->mpeg)) {

	if (videoDecGetState(vd) == VD_STATE_ABORT) {
	    status = -1;
	    printf("decode thread: aborted\n");
	    break;
	}

	// ////////////////////////////////////////////////
	// 
	//  Get next available ouput buffer from voBuf
	//  switch to another thread if voBuf is full
	//
	while (!(voData = voBufGetData(&voBuf))) {
	    switchThread();
	}
	raw8 = (sceIpuRAW8*)voData->v;

	viBufAddDMA(&vd->vibuf);

	// ////////////////////////////////////////////////
	// 
	//  Get decoded picture in sceIpuRGB32 format
	//
        ret = sceMpegGetPictureRAW8(mp, raw8, MAX_WIDTH/16 * MAX_HEIGHT/16);
	picture_structure = (int)((mp->flags >> 3) & 0x3);

	if (picture_structure == SCE_MPEG_FRAME) {
	    csct = (mp->flags & 0x180)?
	    	CSCVU1_PROG_FRAME:
	    	CSCVU1_INTER_FRAME;
	} else {
	    csct = CSCVU1_FIELD;
	}

        if (ret < 0) {
	    ErrMessage("sceMpegGetPicture() decode error");
	}

	if (mp->frameCount == 0) {
	    int i;

	    // ////////////////////////////
	    // 
	    //  Set tags
	    // 
	    for (i = 0; i < N_VOBUF; i++) {
		// for progressive frame picture
		cscVu1SetTag(voBuf.tagProg[i].v, CSCVU1_PROG_FRAME,
		    voBuf.data[i].v, mp->width, mp->height);

		// for interlace frame picture
		cscVu1SetTag(voBuf.tagInter[i].v, CSCVU1_INTER_FRAME,
		    voBuf.data[i].v, mp->width, mp->height);

		// for field picture (fl)
		cscVu1SetTag(voBuf.tagTop[i].v, CSCVU1_FIELD,
		    voBuf.data[i].v, mp->width, mp->height/2);
		cscVu1SetTag(voBuf.tagBot[i].v, CSCVU1_FIELD,
		    voBuf.data[i].v
		    + (mp->width * mp->height / (2 * 256)),
			    mp->width, mp->height/2);
	    }
	}

	// ////////////////////////////
	// 
	//  Increment video output buffer count
	// 
	voBufIncCount(&voBuf);

	switchThread();
    }

    sceMpegReset(mp);

    return status;
}

// ////////////////////////////////////////////////////////////////
//
//  Callback function for sceMpegCbError
//
int mpegError(sceMpeg *mp, sceMpegCbDataError *cberror, void *anyData)
{
    printf("%s\n", cberror->errMessage);
    return 1;
}

// ////////////////////////////////////////////////////////////////
//
//  Callback function for sceMpegCbNodata
//
int mpegNodata(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData)
{
    switchThread();
    viBufAddDMA(&videoDec.vibuf);
    return 1;
}

// ////////////////////////////////////////////////////////////////
//
//  Callback function for sceMpegCbStopDMA
//
int mpegStopDMA(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData)
{
    // Stop DMA
    viBufStopDMA(&videoDec.vibuf);
    return 1;
}

// ////////////////////////////////////////////////////////////////
//
//  Callback function for sceMpegCbRestartDMA
//
int mpegRestartDMA(sceMpeg *mp, sceMpegCbData *cbdata, void *anyData)
{
    // Restart DMA
    viBufRestartDMA(&videoDec.vibuf);
    return 1;
}

// ////////////////////////////////////////////////////////////////
//
//  Callback function for sceMpegCbTimeStamp
//	retruns PTS and DTS
//
int mpegTS(sceMpeg *mp, sceMpegCbDataTimeStamp *cbts, void *anyData)
{
    TimeStamp ts;

    viBufGetTs(&videoDec.vibuf, &ts);
    cbts->pts = ts.pts;
    cbts->dts = ts.dts;
    return 1;
}

// ////////////////////////////////////////////////////////////////
//
//  copy 2 areas
//
static int cpy2area(u_char *pd0, int d0, u_char *pd1, int d1,
    u_char *ps0, int s0, u_char *ps1, int s1)
{
    if (d0 + d1 < s0 + s1) {
        return 0;
    }

    if (s0 >= d0) {
    	memcpy(pd0,		ps0,		d0);
    	memcpy(pd1,		ps0 + d0,	s0 - d0);
    	memcpy(pd1 + s0 - d0,	ps1,		s1);
    } else { // s0 < d0
    	if (s1 >= d0 - s0) {
	    memcpy(pd0,		ps0,		s0);
	    memcpy(pd0 + s0,	ps1,		d0 - s0);
	    memcpy(pd1,		ps1 + d0 - s0,	s1 - (d0 - s0));
	} else { // s1 < d0 - s0
	    memcpy(pd0,		ps0,		s0);
	    memcpy(pd0 + s0,	ps1,		s1);
	}
    }
    return s0 + s1;
}

