/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *              Emotion Engine Library Sample Program
 *
 *                        - easy mpeg -
 *
 *                         Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                            main.c
 *                   main function for ezmpeg
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *       0.10           Aug.14.1999     umemura     the first version
 */

#include <stdio.h>
#include <eekernel.h>
#include <malloc.h>
#include <sifdev.h>
#include <libgraph.h>
#include <libipu.h>
#include <libmpeg.h>
#include "ezmpeg.h"

#define MAX_MBX         (MAX_WIDTH/16)
#define MAX_MBY         (MAX_HEIGHT/16)
#define PK_SIZE         (MAX_MBX * MAX_MBY * 6 + 10)
#define SCREEN_PK_SIZE  (MAX_WIDTH/16 * MAX_HEIGHT/16 * 6 + 10)
#define BSDMASIZE       (1024*1024 - 16)
#define FBP1	((bound(MAX_WIDTH, 64) * bound(MAX_HEIGHT, 32))>>11)
#define ERR_STOP while(1)

char *commandname = NULL;
char *VERSION = "1.0";

// ///////////////////////////////////////////////////////////////////
// 
//  Data for uncached access
// 
static u_int path3tag[2][bound(PK_SIZE*4, 64)] bss_align(64);
static sceIpuRGB32 rgb32[MAX_MBX * MAX_MBY] bss_align(64);
static u_int backTileTags[bound(SCREEN_PK_SIZE*4*2, 64)] bss_align(64);
static u_int backTile[16*16] bss_align(64);

// ///////////////////////////////////////////////////////////////////
// 
//  Work space for MPEG decoder
// 
static u_char mpegBuff[SCE_MPEG_BUFFER_SIZE(MAX_WIDTH, MAX_HEIGHT)];

// ///////////////////////////////////////////////////////////////////
// 
//  Static functions
// 
static void playMpegFile(char *filename);
static void playMpeg(sceMpeg *mp, u_long128 *p, int size);
static int readFile(char *filename, unsigned char **data, int *size);
static void clearBackground(u_int val);
static void usage();

// ///////////////////////////////////////////////////////////////////
// 
//  Main function of ezmpeg
// 
int main(int argc, char *argv[])
{
    char bsFilename[256];

    commandname = argv[0];

    if (argc != 2) {
	usage();
    	ERR_STOP;
    }
    sprintf(bsFilename, "host:%s", argv[1]);

    // //////////////////////////////////////
    // 
    //  Playback mpeg file specified by bsFilename
    // 
    playMpegFile(bsFilename);

    return 0;
}

// ///////////////////////////////////////////////////////////////////
// 
//  Playback mpeg file specified by 'filename'
// 
static void playMpegFile(char *filename)
{
    u_long128 *bs;
    int bsSize;
    int ret;
    int i;
    sceMpeg mp;

    // //////////////////////////////////////
    // 
    //  Read IPU movie file
    // 
    ret = readFile(filename, (u_char**)&bs, &bsSize);
    if (!ret) {
    	return;
    }
    printf("%s(%dbyte)\n", filename, bsSize);

    // //////////////////////////////////////
    // 
    //  Initialize GS
    // 
    sceGsResetGraph(
    	0,	// all reset
	1,	// interlace
	2,	// NTSC
	0	// field
    );
    sceGsResetPath();

    // //////////////////////////////////////
    // 
    //  Initialize IPU and MPEG library
    // 
    sceMpegInit();

    // ////////////////////////////
    // 
    //  Clear background
    // 
    clearBackground(0x80404040);

    // ////////////////////////////
    // 
    //  Create MPEG decoder
    // 
    sceMpegCreate(&mp, mpegBuff, sizeof(mpegBuff));

    // ////////////////////////////
    // 
    //  Set vblank handler
    // 
    AddIntcHandler(INTC_VBLANK_S, vblankHandler, 0);
    EnableIntc(INTC_VBLANK_S);

    for (i = 0; ; i++) {

	printf("------------ %d ------------\n", i);
	playMpeg(&mp, bs, bsSize);

	// ////////////////////////////
	// 
	//  Reset MPEG decoder to the initial state
	// 
	sceMpegReset(&mp);
    }

    DisableIntc(INTC_VBLANK_S);

    // ////////////////////////////
    // 
    //  Delete MPEG decoder
    // 
    sceMpegDelete(&mp);

    free(bs);
}

// ///////////////////////////////////////////////////////////////////
// 
//  Playback mpeg data on memory
// 
static void playMpeg(sceMpeg *mp, u_long128 *bs, int size)
{
    int ret;
    int drawid = 1;
    sceGsDispEnv disp;

    // ////////////////////////////
    // 
    //  Initilize display
    // 
    sceGsSetDefDispEnv(
    	&disp,
	SCE_GS_PSMCT32,
	DISP_WIDTH,
	DISP_HEIGHT,
	0,
	0
    );
    if (MAX_WIDTH > DISP_WIDTH) {
	disp.dispfb.DBX = (MAX_WIDTH - DISP_WIDTH) / 2;
    }
    if (MAX_HEIGHT > DISP_HEIGHT) {
	disp.dispfb.DBY = (MAX_HEIGHT - DISP_HEIGHT) / 2;
    }
    disp.dispfb.FBW = bound(MAX_WIDTH, 64) / 64;
    sceGsPutDispEnv(&disp);

    // ////////////////////////////
    // 
    //  Add bs data to MPEG decoder
    // 
    sceMpegAddBs(mp, bs, size);

    // ////////////////////////////
    // 
    //  Wait for even field
    // 
    startDisplay(1);
    
    // ////////////////////////////
    // 
    //  Main loop to decode MPEG bitstream
    // 
    while (!sceMpegIsEnd(mp)) {

	// ////////////////////////////
	// 
	//  Get decoded picture in sceIpuRGB32 format
	// 
        while ((ret = sceMpegGetPicture(mp, rgb32, MAX_MBX*MAX_MBY)) < 0) {
	    printf ("sceMpegGetPicture Failed\n");
	    ERR_STOP;
	}

	// ////////////////////////////
	// 
	//  Initialize DMA tags for path3 (uncached access)
	// 
	if (mp->frameCount == 0) { // for the first frame

	    int origin_x = (MAX_WIDTH - mp->width)/2;
	    int origin_y = (MAX_HEIGHT - mp->height)/2;
	    setLoadImageTags(path3tag[0], rgb32,
		origin_x, origin_y, mp->width, mp->height);
	    setLoadImageTags(path3tag[1], rgb32,
		origin_x, origin_y + MAX_HEIGHT,
		mp->width, mp->height);
	}

	// ////////////////////////////
	// 
	//  Load decoded image to GS via path3
	// 
	sceGsSyncPath(0, 0);
	loadImage(path3tag[drawid]);

	// ////////////////////////////
	// 
	//  Wait until two vblank happen
	// 
	syncDisplay(mp->frameCount);

	// ////////////////////////////
	// 
	//  Switch display
	// 
	if (drawid) {
	    disp.dispfb.FBP = FBP1; // display bottom buffer
	    drawid = 0;
	} else {
	    disp.dispfb.FBP = 0x00; // display top buffer
	    drawid = 1;
	}
	sceGsPutDispEnv(&disp); // switch display
    }

    endDisplay();
}

// ///////////////////////////////////////////////////////////////////
// 
//  Read data from a file
// 
static int readFile(char *filename, unsigned char **data, int *size)
{
    int fd;
    int ret = 1;
    int readcount;

    // //////////////////////////////////////
    // 
    //  open a file to read and check its size
    // 
    fd = sceOpen(filename, SCE_RDONLY);
    if (fd < 0) {
    	printf("Cannot open %s\n", filename);
	return 0;
    }
    *size = sceLseek(fd, 0, SCE_SEEK_END);
    if (*size < 0) {
    	printf("sceLseek() fails (%s)\n", filename);
	sceClose(fd);
	return 0;
    }

    ret = sceLseek(fd, 0, SCE_SEEK_SET);
    if (ret < 0) {
    	printf("sceLseek() fails (%s)\n", filename);
	sceClose(fd);
	return 0;
    }

    // //////////////////////////////////////
    // 
    //  Allocate memory dynamically
    // 
    *data = (u_char*)memalign(64, *size);
    if (*data == NULL) {
    	printf("memalign(%d) fails\n", *size);
	sceClose(fd);
	return 0;
    }

    // //////////////////////////////////////
    // 
    //  read bs data
    // 
    readcount = sceRead(fd, *data, *size);
    if (readcount != *size) {
	printf("Cannot read %s\n", filename);
	sceClose(fd);
	free(*data);
	return 0;
    }

    sceClose(fd);
    return 1;
}

// ///////////////////////////////////////////////////////////////////
// 
//  Clear background of display
// 
static void clearBackground(u_int val)
{
    int i;
    u_int *p = (u_int*)UncachedAddr(backTile);

    for (i = 0; i < 16*16; i++) {
        p[i] = val;
    }
    setLoadImageTagsTile(backTileTags, backTile,
    	0, 0, MAX_WIDTH, MAX_HEIGHT*2);
    loadImage(backTileTags);
    sceGsSyncPath(0, 0);
}

// ///////////////////////////////////////////////////////////////////
// 
//  Print usage of this program
// 
static void usage()
{
    printf(
	"usage: run %s mpeg2_file          (version %s)\n"
	, commandname, VERSION);
}

