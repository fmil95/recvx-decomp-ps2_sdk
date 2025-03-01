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
 *                              usbmouse.c
 *
 *        Version       Date            Design     Log
 *  --------------------------------------------------------------------
 *      0.10            Jan,15,2000     hatto      Initial
 *      0.20            Apr,24,2000     fukunaga   Multi-USB Mouse
 * 
 */

#include <eekernel.h>
#include <eeregs.h>
#include <libgraph.h>
#include <stdio.h>
#include <sifdev.h>
#include <sifrpc.h>

#include "usbmouse.h"

#define SSIZE 0x20
static sceSifClientData cdUsbMouse;
static u_int sdata[SSIZE/sizeof(u_int)] __attribute__((aligned(64)));
/* static u_int rdata[SSIZE/sizeof(u_int)] __attribute__((aligned(64))); */

/*---------------------------------------------
  Function Name	: sceUsbMouseInit
  function     	: RPCの登録
  Input Data	: none
  Output Data   : none
  Return Value	: 0(正常終了),not0(異常終了)
----------------------------------------------*/
int sceUsbMouseInit(void)
{
	unsigned int i;
	
	sceSifInitRpc(0);

	while( 1 ) {
		if (sceSifBindRpc( &cdUsbMouse, SCE_RPC_USB_MOUSE, 0) < 0) {
			printf("bind errr\n");
			while( 1 );
		}
		if (cdUsbMouse.serve != 0) break;
		i = 0x10000;
		while(i --);
	}
	
	return 0;
}

/*---------------------------------------------
  Function Name	: sceUsbMouseGetInfo
  function     	: マウス接続情報の取得
  Input Data	: none
  Output Data   : info(マウス接続情報)
                  ※64byteアライメントされたSSIZE分のバッファが必要
                   info[0]:最大接続可能数m
                   info[1]:現在の接続数
		   info[2]:マウスNo.0の接続情報(1のとき接続中)
                     :
                   info[m+1]:マウスNo.(m-1)の接続情報(1のとき接続中)
  Return Value	: 0(正常終了),not0(異常終了)
----------------------------------------------*/
int sceUsbMouseGetInfo(u_char *info)
{
	int ret;
        
	ret = sceSifCallRpc(&cdUsbMouse, 1, SIF_RPCM_NOWAIT,
	              &sdata[0],SSIZE, info, SSIZE, 0, 0);
	if (ret != 0) { return ret; }
	
	return 0;
}

/*---------------------------------------------
  Function Name	: sceUsbMouseRead
  function     	: マウスデータのリード
  Input Data	: no(マウスNo) ※デバイスIDではありません。
  Output Data   : data(マウスデータ) 
                  ※64byteアライメントされたSSIZE分のバッファが必要
		   data[0]:マウスデータサイズ n (0のとき空データ)
		   data[1]:Button
                           bit0:Button1(0:OFF,1:ON)
                           bit1:Button2(0:OFF,1:ON)
                           bit2:Button3(0:OFF,1:ON)
                           bit3-7:Device-specific
		   data[2]:X
                   data[3]:Y
		   data[4]:Device-specific(Wheel)
		   data[5]:Device-specific
                     :
                   data[n]:Device-specific
  Return Value	: 0(正常終了),not0(異常終了)
----------------------------------------------*/
int sceUsbMouseRead(u_int no,char *data)
{
	int ret;
	
	sdata[0] = no;
	
	ret = sceSifCallRpc(&cdUsbMouse, 2, SIF_RPCM_NOWAIT,
	              &sdata[0],SSIZE, data, SSIZE, 0, 0);
	if (ret != 0) { return ret; }
	
	return 0;
}

/*---------------------------------------------
  Function Name	: sceUsbMouseGetLocation
  function     	: マウス接続位置の取得
  Input Data	: no(マウスNo) ※デバイスIDではありません。
  Output Data   : location(マウス接続位置情報)
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
int sceUsbMouseGetLocation(int no,u_char *location)
{
	int ret;
	
	sdata[0] = no;
        
	ret = sceSifCallRpc(&cdUsbMouse, 3, SIF_RPCM_NOWAIT,
	              &sdata[0],SSIZE, location, SSIZE, 0, 0);
	if (ret != 0) { return ret; }
	
	return 0;
}

/*-----------------------------------------------------
  Function Name	: sceUsbMouseSync
  function     	: 実行中のコマンドが終了するまで待つ
  Input Data	: mode(0:ブロック 1:ノンブロック)
  Output Data   : none
  Return Value	: 0(終了),1(実行中),-1(modeが0,1以外)
------------------------------------------------------*/
int sceUsbMouseSync(int mode) 
{
        int i;

        switch(mode)
	  {
	  case 0:
	    i = 0;
	    while(sceSifCheckStatRpc((sceSifRpcData *) &cdUsbMouse) != 0){i ++;};
	    //printf("loop cnt %x \n",i );
	    return 0;
	    break;
	  case 1:
	    return sceSifCheckStatRpc((sceSifRpcData *) &cdUsbMouse);
	  }

	return -1;
}

