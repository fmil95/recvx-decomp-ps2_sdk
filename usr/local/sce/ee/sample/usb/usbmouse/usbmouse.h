/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      USB Mouse Sample (for EE)
 *
 *                          Version 0.20
 *                          Shift-JIS
 *
 *      Copyright (C) 2000 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                              usbmouse.h
 *
 *        Version       Date            Design     Log
 *  --------------------------------------------------------------------
 *      0.10            Jan,15,2000     hatto      Initial
 *      0.20            Apr,24,2000     fukunaga   Multi-USB Mouse
 * 
 */

#define SCE_RPC_USB_MOUSE	0x80000210

#define KEY_DATA_LEN 8

int sceUsbMouseInit(void);
int sceUsbMouseGetInfo(u_char *info);
int sceUsbMouseRead(u_int no,char *data);
int sceUsbMouseGetLocation(int no,u_char *location);
int sceUsbMouseSync(int mode);
