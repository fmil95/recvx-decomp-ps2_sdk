/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      USB Keyboard Sample (for EE)
 *
 *                          Version 0.10
 *                          Shift-JIS
 *
 *      Copyright (C) 2000 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                              usbkeybd.c
 *
 *        Version       Date            Design     Log
 *  --------------------------------------------------------------------
 *      0.10            Apr,13,2000     fukunaga   USB Keyboard sample
 * 
 */

#include <eekernel.h>
#include <eeregs.h>
#include <libgraph.h>
#include <stdio.h>
#include <sifdev.h>
#include <sifrpc.h>

#include "usbkeybd.h"

#define SSIZE 0x20
static sceSifClientData cdUsbKeybd;
static u_int sdata[SSIZE/sizeof(u_int)] __attribute__((aligned(64)));
/* static u_int rdata[SSIZE/sizeof(u_int)] __attribute__((aligned(64))); */

/*---------------------------------------------
  Function Name	: sceUsbKeybdInit
  function     	: RPCの登録
  Input Data	: none
  Output Data   : none
  Return Value	: 0(正常終了),not0(異常終了)
----------------------------------------------*/
int sceUsbKeybdInit(void)
{
	unsigned int i;
	
	sceSifInitRpc(0);

	while( 1 ) {
		if (sceSifBindRpc( &cdUsbKeybd, SCE_RPC_USB_KEYBD, 0) < 0) {
			printf("bind errr\n");
			while( 1 );
		}
		if (cdUsbKeybd.serve != 0) break;
		i = 0x10000;
		while(i --);
	}
	
	return 0;
}

/*---------------------------------------------
  Function Name	: sceUsbKeybdGetInfo
  function     	: キーボード接続情報の取得
  Input Data	: none
  Output Data   : info(キーボード接続情報)
                  ※64byteアライメントされたSSIZE分のバッファが必要
                   info[0]:最大接続可能数m
                   info[1]:現在の接続数
		   info[2]:キーボードNo.0の接続情報(1のとき接続中)
                     :
                   info[m+1]:キーボードNo.(m-1)の接続情報(1のとき接続中)
  Return Value	: 0(正常終了),not0(異常終了)
----------------------------------------------*/
int sceUsbKeybdGetInfo(u_char *info)
{
	int ret;
        
	ret = sceSifCallRpc(&cdUsbKeybd, 1, SIF_RPCM_NOWAIT,
	              &sdata[0],SSIZE, info, SSIZE, 0, 0);
	if (ret != 0) { return ret; }
	
	return 0;
}

/*---------------------------------------------
  Function Name	: sceUsbKeybdRead
  function     	: キーボードデータのリード
  Input Data	: no(キーボードNo) ※デバイスIDではありません。
  Output Data   : data(キーデータ) 
                  ※64byteアライメントされたSSIZE分のバッファが必要
                   data[0]:LED状態
                           bit0  :NUM LOCK
                           bit1  :CAPS LOCK
                           bit2  :SCROLL LOCK
                           bit3  :COMPOSE
                           bit4  :KANA
                           bit5-7:not used
                   data[1]:キーデータサイズ n (0のとき空データ)
		   data[2]:Modifier keys
                           bit0:Left-Ctrl
                           bit1:Left-Shift
                           bit2:Left-Alt(Win),Left-option(Mac)
                           bit3:Left-Win(Win),Left-Apple(Mac)
                           bit4:Right-Ctrl
                           bit5:Right-Shift
                           bit6:Right-Alt(Win),Right-option(Mac)
                           bit7:Right-Win(Win),Right-Apple(Mac)
		   data[3]:Reserved
                   data[4]:キーコード
                     :
                   data[2+n-1]:キーコード
  Return Value	: 0(正常終了),not0(異常終了)
----------------------------------------------*/
int sceUsbKeybdRead(u_int no,u_char *data)
{
	int ret;
	
	sdata[0] = no;
	
	ret = sceSifCallRpc(&cdUsbKeybd, 2, SIF_RPCM_NOWAIT,
	              &sdata[0],SSIZE, data, SSIZE, 0, 0);
	if (ret != 0) { return ret; }
	
	return 0;
}

/*---------------------------------------------
  Function Name	: sceUsbKeybdGetLocation
  function     	: キーボード接続位置の取得
  Input Data	: no(キーボードNo) ※デバイスIDではありません。
  Output Data   : location(キーボード接続位置情報)
                  ※64byteアライメントされたSSIZE分のバッファが必要
		   location[0]:本体のポート番号(存在しないときは０)
		   location[1]:HUB1のポート番号(存在しないときは０)
		   location[2]:HUB2のポート番号(存在しないときは０)
		   location[3]:HUB3のポート番号(存在しないときは０)
		   location[4]:HUB4のポート番号(存在しないときは０)
		   location[5]:HUB5のポート番号(存在しないときは０)
		   location[6]:常に０
  Return Value	: 0(正常終了),not0(異常終了)
----------------------------------------------*/
int sceUsbKeybdGetLocation(int no,u_char *location)
{
	int ret;
	
	sdata[0] = no;
        
	ret = sceSifCallRpc(&cdUsbKeybd, 3, SIF_RPCM_NOWAIT,
	              &sdata[0],SSIZE, location, SSIZE, 0, 0);
	if (ret != 0) { return ret; }
	
	return 0;
}

/*-----------------------------------------------------
  Function Name	: sceUsbKeybdSync
  function     	: 実行中のコマンドが終了するまで待つ
  Input Data	: mode(0:ブロック 1:ノンブロック)
  Output Data   : none
  Return Value	: 0(終了),1(実行中),-1(modeが0,1以外)
------------------------------------------------------*/
int sceUsbKeybdSync(int mode) 
{
        int i;

        switch(mode)
	  {
	  case 0:
	    i = 0;
	    while(sceSifCheckStatRpc((sceSifRpcData *) &cdUsbKeybd) != 0){i ++;};
	    //printf("loop cnt %x \n",i );
	    return 0;
	    break;
	  case 1:
	    return sceSifCheckStatRpc((sceSifRpcData *) &cdUsbKeybd);
	  }

	return -1;
}
