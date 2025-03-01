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
 *                            main.c
 *             main function for mpeg streming program
 *
 *       Version        Date            Design   Log
 *  --------------------------------------------------------------------
 *       0.10           12.17.1999      umemura  the first version
 *       0.20           01.14.2000      umemura  stack size is modified
 */
#include <stdio.h>
#include <string.h>
#include <librspu2.h>
#include "defs.h"
#include "sifrpc.h"
#include "sifdev.h"
#include "videodec.h"
#include "disp.h"
#include "readbuf.h"
#include "libpad.h"
#include "vobuf.h"
#include "audiodec.h"
#include <libcdvd.h>    /* CD/DVD Streaming */

// ////////////////////////////////////////////////////////////////
//
// Definitions
//
#define STACK_SIZE    (16*1024)
#define DEF_STACK_SIZE    2048
#define MAX_MBX		(MAX_WIDTH/16)
#define MAX_MBY		(MAX_HEIGHT/16)
#define IOP_BUFF_SIZE (12288*2) // 512 * 48
#define MOVIE_ABORTED SCE_PADRdown
#define MODULEDIR	"host0:/usr/local/sce/iop/modules/"
#define DEF_PRIORITY       0
#define ERR_STOP while(1)

// ////////////////////////////////////////////////////////////////
//
// Variables
//
// These variables could be accessed from Uncached Area
static VoData voBufData[N_VOBUF] bss_align(64);
static VoTag voBufTag[N_VOBUF] bss_align(64);

static u_long128 viBufTag[VIBUF_SIZE + 1] bss_align(64);
// -------------- this needs to be 64 byte boudary -------------------

// These variables are NOT accessed from Uncached Area

static u_char mpegWork[SCE_MPEG_BUFFER_SIZE(MAX_WIDTH, MAX_HEIGHT)]
							bss_align(64);
static char defStack[DEF_STACK_SIZE] bss_align(64);
ReadBuf readBuf bss_align(64);
static u_char audioBuff[IOP_BUFF_SIZE*2] bss_align(64);
static u_long128 viBufData[VIBUF_SIZE * VIBUF_ELM_SIZE/16]
							bss_align(64);
static char videoDecStack[STACK_SIZE] bss_align(64);
static TimeStamp timeStamp[VIBUF_TS_SIZE];
static u_long128 controller_dma_buf[scePadDmaBufferMax]  __attribute__((aligned (64)));

Display display;
int videoDecTh;
int defaultTh;
int bs_fd;
int bs_filesize;
VideoDec videoDec;
AudioDec audioDec;
VoBuf voBuf;
int frd;
u_int controller_val;
int isWithAudio = 1;

char *commandname = NULL;
char *VERSION = "1.1";

u_int local_lsn;                /* CD/DVD Streaming */
sceCdRMode mode;                /* CD/DVD Streaming */
sceCdlFILE fp;                  /* CD/DVD Streaming */
u_char *ring_buf;               /* CD/DVD Streaming */

// ////////////////////////////////////////////////////////////////
//
// Function declarations
//
static void initAll(char *bsfile);
static void termAll();
static void defMain(void);
static u_int movie(char *name);
static int readMpeg(VideoDec *vd, ReadBuf *rb, int fd, int filesize);
static int isAudioOK();
static void usage();

int openFile(char *filename, int *filesize);
void dispMain(void);
void videoDecMain(VideoDec *vd);
int videoCallback(sceMpeg *mp, sceMpegCbDataStr *str, void *data);
int pcmCallback(sceMpeg *mp, sceMpegCbDataStr *str, void *data);
int vblankHandler(int);
int handler_endimage(int);
void loadModule(char *moduleName);

//#define MDLCHG_RESET
// ////////////////////////////////////////////////////////////////
//
// Main function
//
int main(int argc, char *argv[])
{
    u_int ret;
    char *name = NULL;
    int k;
    int isError = 0;

#ifdef MDLCHG_RESET
        sceSifInitRpc(0);
        while (sceSifRebootIop("host0:/usr/local/sce/iop/modules/"IOP_IMAGE_file) == 0){
                                   // ÉCÉÅÅ[ÉWÉtÉ@ÉCÉãñºÇèëÇ´çCÍCîCàCê¢ãCÇü                printf("iop reset error\n");
        }
        while(sceSifSyncIop() == 0);
#endif

    commandname = argv[0];

    // /////////////////////////////
    // 
    //  load motules
    // 
    sceSifInitRpc(0);
    sceSifInitIopHeap();                /* CD/DVD Streaming */

    loadModule(MODULEDIR "rspu2drv.irx");
    loadModule(MODULEDIR "sio2man.irx");
    loadModule(MODULEDIR "padman.irx");

    // /////////////////////////////
    // 
    //  CD/DVD Streaing Init
    // 
    ring_buf= sceSifAllocIopHeap((2048 * 80) + 16);      /* CD/DVD Streaming */
    ring_buf= (u_char *)((u_int)(ring_buf + 15) & ~0x0f);/* CD/DVD Streaming */

    sceCdInit(SCECdINIT);               /* CD/DVD Streaming */
    sceCdDiskReady(0);                  /* CD/DVD Streaming */
    sceCdStInit(80,5,(u_int)ring_buf);  /* CD/DVD Streaming */

    // /////////////////////////////
    // 
    //  Parse command line options
    // 
    for (k = 1; k < argc; k++) {
    	if (!strcmp(argv[k], "-noaudio")) {
	    isWithAudio = 0;
	} else if (argv[k][0] == '-') {
	    isError = 1;
	} else {
	    name = argv[k];
	}
    }
    if (isError || (name == NULL)) {
        usage();
	ERR_STOP;
    }

    // /////////////////////////////
    // 
    //  Initialize display
    // 
    dispCreate(&display, MAX_WIDTH, MAX_HEIGHT/2);
    dispClear(&display, 0x80404040);

    while ((ret = movie(name)) != MOVIE_ABORTED)
    	;

    dispDelete(&display);

    return 0;
}

// ////////////////////////////////////////////////////////////////
//
// Decode MPEG bitstream
//
// ret:
//   1: ok
//   0: error
//  -1: abort
static u_int movie(char *name)
{
    char bsfilename[256];
    static int count = 0;

    printf("========================== decode MPEG2 ============= %d ===========\n", count++);
    //sprintf(bsfilename, "host0:%s", name);      /* CD/DVD Streaming */
    sprintf(bsfilename, "%s", name);              /* CD/DVD Streaming */

    printf("bit stream file \"%s\"\n", bsfilename);

    initAll(bsfilename);

    readMpeg(&videoDec, &readBuf, bs_fd, bs_filesize);

    termAll();

    return controller_val;
}

// ////////////////////////////////////////////////////////////////
//
// Read MPEG data
//
// return value
//     1: normal end
//     -1: aborted
static int readMpeg(VideoDec *vd, ReadBuf *rb, int fd, int size)
{

    u_int ctrlmask =  SCE_PADRright | SCE_PADRdown;
    u_char *put_ptr;
    u_char *get_ptr;
    int putsize;
    int getsize;
    int readrest = size;
    int writerest = size;
    int count;
    int proceed;
    int isStarted = 0;
    u_int button_old = 0;
    u_int pushed = 0;
    u_char cdata[32];
    u_int err;                                         /* CD/DVD Streaming */

    // writerest > 4: to skip the last 4 bytes
    while (writerest > 4 && videoDecGetState(vd) != VD_STATE_END) {

    	// /////////////////////////////////////////////////
	//
	// Get controller information
	//
    	if (scePadRead(0, 0, cdata) > 0) {
	    controller_val = 0xffff ^ ((cdata[2] << 8) | cdata[3]);
	} else {
	    controller_val = 0;
	}
	pushed = (button_old ^ controller_val)
			& controller_val & ctrlmask;
	button_old = controller_val;
	if (pushed && vd->mpeg.frameCount > 10) {

	    // /////////////////////////////////////////////////
	    //
	    // Abort decoding
	    //
	    videoDecAbort(&videoDec);
	}

    	// /////////////////////////////////////////////////
	//
	// Put data on the read buffer
	//
        putsize = readBufBeginPut(rb, &put_ptr);
	if (readrest > 0 && putsize >= READ_UNIT_SIZE) {
        //  count = sceRead(fd, put_ptr, READ_UNIT_SIZE); /* CD/DVD Streaming */
            count= sceCdStRead(READ_UNIT_SIZE / 2048,     /* CD/DVD Streaming */
                       (u_int *)put_ptr, STMBLK, &err);  /* CD/DVD Streaming */
            count*= 2048;                                 /* CD/DVD Streaming */

	    readBufEndPut(rb, count);
	    readrest -= count;
	}

	switchThread();

    	// /////////////////////////////////////////////////
	//
	// De-multiplex and put data on video/audio input buffer
	//
	getsize = readBufBeginGet(rb, &get_ptr);
	if (getsize > 0) {

	    proceed = sceMpegDemuxPssRing(&vd->mpeg,
	    		get_ptr, getsize, rb->data, rb->size);

	    readBufEndGet(rb, proceed);
	    writerest -= proceed;

	}

	if (!isStarted && voBufIsFull(&voBuf) && isAudioOK()) {

	    startDisplay(1);		// start video
	    if (isWithAudio) {
		audioDecStart(&audioDec);	// start audio
	    }
	    isStarted = 1;
	}
    }

    // try to flush buffers inside decoder
    while (!videoDecFlush(vd)) {
	switchThread();
    }

    // wait till buffers are flushed
    while (!videoDecIsFlushed(vd)
    	&& videoDecGetState(vd) != VD_STATE_END) {

	switchThread();
    }

    endDisplay();
    if (isWithAudio) {
	audioDecReset(&audioDec);
    }

    return 1;
}

// /////////////////////////////////////////////////
//
// Switch to another thread
//
void switchThread()
{
    RotateThreadReadyQueue(DEF_PRIORITY);
}

// /////////////////////////////////////////////////
//
// Check audio
//
static int isAudioOK()
{
    return (isWithAudio)? audioDecIsPreset(&audioDec): 1;
}

// ////////////////////////////////////////////////////////////////
//
// Initialize all modules
//
static void initAll(char *bsfile)
{
    struct ThreadParam th_param;

    *D_CTRL = (*D_CTRL | 0x003);
    *D_STAT = 0x4; // clear D_STAT.CIS2
    // /////////////////////////////
    // 
    //  Create read buffer
    // 
    readBufCreate(&readBuf);

    // /////////////////////////////
    // 
    //  Initialize video decoder
    // 
    sceMpegInit();
    videoDecCreate(&videoDec,
    	mpegWork, sizeof(mpegWork),
    	viBufData, viBufTag, VIBUF_SIZE, timeStamp, VIBUF_TS_SIZE);

    // /////////////////////////////
    // 
    //  Initialize audio decoder
    // 
    sceSpu2RemoteInit();
    audioDecCreate(&audioDec, audioBuff, sizeof(audioBuff), IOP_BUFF_SIZE);

    ///////////////////////////////
    // 
    //  Choose stream to be played
    // 
    videoDecSetStream(&videoDec,
	    sceMpegStrM2V, 0, (sceMpegCallback)videoCallback, &readBuf);
    if (isWithAudio) {
	videoDecSetStream(&videoDec,
	    sceMpegStrPCM, 0, (sceMpegCallback)pcmCallback, &readBuf);
    }

    // /////////////////////////////
    // 
    //  Initialize video output buffer
    // 
    voBufCreate(&voBuf, UncAddr(voBufData), voBufTag, N_VOBUF);

    // /////////////////////////////
    // 
    //  Create 'default' thread
    // 
    th_param.entry = defMain;
    th_param.stack = defStack;
    th_param.stackSize = DEF_STACK_SIZE;
    th_param.initPriority = DEF_PRIORITY;
    th_param.gpReg = &_gp;
    th_param.option = 0;
    defaultTh = CreateThread(&th_param);
    StartThread(defaultTh, NULL);

    // /////////////////////////////
    // 
    //  Create docode thread
    // 
    th_param.entry = videoDecMain;
    th_param.stack = videoDecStack;
    th_param.stackSize = STACK_SIZE;
    th_param.initPriority = DEF_PRIORITY;
    th_param.gpReg = &_gp;
    th_param.option = 0;
    videoDecTh = CreateThread(&th_param);
    StartThread(videoDecTh, &videoDec);

    // /////////////////////////////
    // 
    //  Initialize controller
    // 
    {
	static int isFirst = 1;
	if (isFirst) {
	    scePadInit(0);
    	    scePadPortOpen(0, 0, controller_dma_buf);
	    isFirst = 0;
	}
    }

    // /////////////////////////////
    // 
    //  Open bitstream file
    // 
    //while ((bs_fd = openFile(bsfile, &bs_filesize)) < 0) {           /* CD/DVD Streaming */
    //  ErrMessage("File open error\n");                /* CD/DVD Streaming */
    //}                                                 /* CD/DVD Streaming */
    while (!sceCdSearchFile(&fp, bsfile)){                  /* CD/DVD Streaming */
        printf("sceCdSearchFile fail:%s:\n", bsfile);   /* CD/DVD Streaming */
    }                                                   /* CD/DVD Streaming */
    local_lsn= fp.lsn;                                  /* CD/DVD Streaming */
    bs_filesize= fp.size;                               /* CD/DVD Streaming */
    mode.trycount= 0;                                   /* CD/DVD Streaming */
    mode.spindlctrl= SCECdSpinStm;                      /* CD/DVD Streaming */
    mode.datapattern= SCECdSecS2048;                    /* CD/DVD Streaming */
    sceCdStStart(local_lsn, &mode);                     /* CD/DVD Streaming */

    // /////////////////////////////
    // 
    //  Set Interrupt handlers
    // 
    videoDec.hid_vblank = AddIntcHandler(INTC_VBLANK_S, vblankHandler, 0);
    EnableIntc(INTC_VBLANK_S);

    videoDec.hid_endimage = AddDmacHandler(DMAC_GIF, handler_endimage, 0);
    EnableDmac(DMAC_GIF);
}

// ////////////////////////////////////////////////////////////////
//
// Terminate all modules
//
static void termAll()
{
    readBufDelete(&readBuf);
    voBufDelete(&voBuf);

    TerminateThread(videoDecTh);
    DeleteThread(videoDecTh);

    TerminateThread(defaultTh);
    DeleteThread(defaultTh);

    DisableDmac(DMAC_GIF);
    RemoveDmacHandler(DMAC_GIF, videoDec.hid_endimage);

    DisableIntc(INTC_VBLANK_S);
    RemoveIntcHandler(INTC_VBLANK_S, videoDec.hid_vblank);

    videoDecDelete(&videoDec);
    audioDecDelete(&audioDec);

    sceCdStStop();		/* CD/DVD Streaming */
    //sceClose(bs_fd);          /* CD/DVD Streaming */
}

// ////////////////////////////////////////////////////////////////
//
// Main function of default thread
//
static void defMain(void)
{
    while (1) {
	switchThread();
    }
}

// ////////////////////////////////////////////////////////////////
//
// Print error message
//
void ErrMessage(char *message)
{
    printf("[ Error ] %s\n", message);
}

// ////////////////////////////////////////////////////////////////
//
// Usage
//
static void usage()
{
    printf(
	"usage: run %s [options] pss_file         (version %s)\n"
	"    < options >\n"
	"        -noaudio               decode video only\n"
	, commandname, VERSION
    );
}

