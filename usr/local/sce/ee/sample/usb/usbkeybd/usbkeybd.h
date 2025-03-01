/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      USB Keyboard Library Sample (for EE)
 *
 *                          Version 0.10
 *                          Shift-JIS
 *
 *      Copyright (C) 2000 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                              usbkeybd.h
 *
 *        Version       Date            Design     Log
 *  --------------------------------------------------------------------
 *      0.10            Apr,13,2000     fukunaga   USB Keyboard sample
 * 
 */

#define SCE_RPC_USB_KEYBD	0x80000211

#define KEY_DATA_LEN 16

int sceUsbKeybdInit(void);
int sceUsbKeybdGetInfo(u_char *info);
int sceUsbKeybdRead(u_int no,u_char *data);
int sceUsbKeybdGetLocation(int no,u_char *location);
int sceUsbKeybdSync(int mode);
