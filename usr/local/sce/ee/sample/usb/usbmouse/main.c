/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      USB Mouse Sample (for EE)
 *
 *                          Version 0.21
 *                          Shift-JIS
 *
 *      Copyright (C) 2000 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                              main.c
 *
 *        Version       Date            Design     Log
 *  --------------------------------------------------------------------
 *      0.10            Jan,15,2000     hatto      Initial
 *      0.20            Apr,24,2000     fukunaga   Multi-USB Mouse
 *      0.21            June,19,2000    fukunaga   
 * 
 */

#include <eekernel.h>
#include <eeregs.h>
#include <libgraph.h>
#include <stdio.h>
#include <sifdev.h>
#include <sifrpc.h>

#include "usbmouse.h"


static u_char info[0x80] __attribute__((aligned(64)));
static u_char location[0x80] __attribute__((aligned(64)));
static char mdata[0x80] __attribute__((aligned(64)));


int main()
{
    int i,j,r,ret;

    sceSifInitRpc(0);

    // loading usb driver
    while(sceSifLoadModule("host0:/usr/local/sce/iop/modules/usbd.irx",
			0, NULL) < 0){
	printf("Can't load module usbd\n");
    }

    // loading usb mouse driver
    while(sceSifLoadModule("host0:/usr/local/sce/iop/sample/usb/usbmouse/usbmouse.irx",0,NULL) < 0){
	printf("Can't load module usbmouse\n");
    }
    
    // init usb mouse interface
    sceUsbMouseInit();

    sceGsResetPath();

    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);

    printf("USB Mouse Sample\n");

    while ( 1 ) {

	// get mouse info
	ret = sceUsbMouseGetInfo(info);
	if (ret != 0) {
	  printf("Error%d : sceUsbMouseGetInfo\n",ret);
	  continue;
	}
	sceUsbMouseSync(0);
#if 0 /* debug */
	for(i=0; i<info[0]+2; i++) {
	  printf("%02X ",info[i]);
	}
	printf("\n");
#endif

	// reading mouse
	for(i=0; i<info[0]; i++) {
	  if (info[i+2] == 0) continue;
	  
	  ret = sceUsbMouseGetLocation(i,location);
	  if (ret != 0) {
	    printf("Error%d : sceUsbMouseGetLocation\n",ret);
	    continue;
	  }
	  sceUsbMouseSync(0);
	  
	  while(1) { /* IOPのリングバッファが空になるまで回るループ */
	    ret = sceUsbMouseRead(i,mdata);
	    if (ret != 0) {
	      printf("Error%d : sceUsbMouseRead\n",ret);
	      break;
	    }
	    sceUsbMouseSync(0);
	    if (mdata[0] == 0) { break; } /* Ring buffer empty ? */
	    
	    printf("usbmouse%d : ",i);
	    
	    printf("port=");
	    for(r = 0; r < 7 && location[r] != 0; r++) {
	      printf("%s%d", ((r)? ",": ""), location[r]);
	    }
	    
	    printf(" Button:%02X",mdata[1]);
	    printf(" X:%3d",mdata[2]);
	    printf(" Y:%3d",mdata[3]);
	    if (mdata[0] >= 4) {
	      printf(" W:%3d",mdata[4]);
	    }
	    if (mdata[0] >= 5) {
	      printf(" [");
	      for (j=5;j<=mdata[0];j++) {
		printf(" %02X",mdata[j]);
	      }
	      printf("]");
	    }
	    printf("\n");
	  } /* end of "while(1)" */

	} /* end of "for(i=0; i<info[0]; i++)" */

	sceGsSyncV(0);

    } /* end of "while(1)" */
}
