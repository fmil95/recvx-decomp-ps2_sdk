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
  function     	: RPC�̓o�^
  Input Data	: none
  Output Data   : none
  Return Value	: 0(����I��),not0(�ُ�I��)
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
  function     	: �}�E�X�ڑ����̎擾
  Input Data	: none
  Output Data   : info(�}�E�X�ڑ����)
                  ��64byte�A���C�����g���ꂽSSIZE���̃o�b�t�@���K�v
                   info[0]:�ő�ڑ��\��m
                   info[1]:���݂̐ڑ���
		   info[2]:�}�E�XNo.0�̐ڑ����(1�̂Ƃ��ڑ���)
                     :
                   info[m+1]:�}�E�XNo.(m-1)�̐ڑ����(1�̂Ƃ��ڑ���)
  Return Value	: 0(����I��),not0(�ُ�I��)
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
  function     	: �}�E�X�f�[�^�̃��[�h
  Input Data	: no(�}�E�XNo) ���f�o�C�XID�ł͂���܂���B
  Output Data   : data(�}�E�X�f�[�^) 
                  ��64byte�A���C�����g���ꂽSSIZE���̃o�b�t�@���K�v
		   data[0]:�}�E�X�f�[�^�T�C�Y n (0�̂Ƃ���f�[�^)
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
  Return Value	: 0(����I��),not0(�ُ�I��)
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
  function     	: �}�E�X�ڑ��ʒu�̎擾
  Input Data	: no(�}�E�XNo) ���f�o�C�XID�ł͂���܂���B
  Output Data   : location(�}�E�X�ڑ��ʒu���)
                  ��64byte�A���C�����g���ꂽSSIZE���̃o�b�t�@���K�v
		   location[0]:�{�̂̃|�[�g�ԍ�(���݂��Ȃ��Ƃ��͂O)
		   location[1]:HUB1�̃|�[�g�ԍ�(���݂��Ȃ��Ƃ��͂O)
		   location[2]:HUB2�̃|�[�g�ԍ�(���݂��Ȃ��Ƃ��͂O)
		   location[3]:HUB3�̃|�[�g�ԍ�(���݂��Ȃ��Ƃ��͂O)
		   location[4]:HUB4�̃|�[�g�ԍ�(���݂��Ȃ��Ƃ��͂O)
		   location[5]:HUB5�̃|�[�g�ԍ�(���݂��Ȃ��Ƃ��͂O)
		   location[6]:��ɂO
  Return Value	: 0(����I��),not0(�ُ�I��)
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
  function     	: ���s���̃R�}���h���I������܂ő҂�
  Input Data	: mode(0:�u���b�N 1:�m���u���b�N)
  Output Data   : none
  Return Value	: 0(�I��),1(���s��),-1(mode��0,1�ȊO)
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

