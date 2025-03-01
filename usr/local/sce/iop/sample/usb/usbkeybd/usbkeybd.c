/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      USB Keyboard Class Driver (for IOP)
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


#include <stdio.h>
#include <kernel.h>
#include <memory.h>
#include <sif.h>
#include <sifcmd.h>
#include <sifrpc.h>

#include <usb.h>
#include <usbd.h>

#include "usbkeybd.h"


ModuleInfo Module = {"USB KEYBOARD SAMPLE", 0x0201 };


/*********************************************************
                           Type 
**********************************************************/
typedef struct _unit {
        int    dev_id;
        int    number;  /* EEからのRPC要求を受けるためのINDEX */
	int    c_pipe; 
	int    d_pipe;
        int    payload;
        int    ifnum;
        int    as;
        int    count;
        u_char ledptn;

        /* Ring buffer */
	int rp;
        int wp;
        int rblen;
        u_char ringbuf[RINGBUF_SIZE][KEY_DATA_LEN+2];

	u_char data[0];  /* for interrupt transfer */
} UNIT;

typedef struct {
        int next_number;
	int n; // the number of usb keyboard connecting
	UNIT* con_unit[MAX_KEYBD_NUM];
} USBKEYBD_t;

/*********************************************************
                     Common Variables
**********************************************************/
static USBKEYBD_t USBKEYBD;

#define SSIZE 0x20
static unsigned int buffer [SSIZE/sizeof(u_int)];


/*********************************************************
                         Macros
**********************************************************/
#define err(p, f, r)	if(r) printf("usbkeybd%d: %s -> 0x%x\n", \
				(p)->number, (f), (r))

/*********************************************************
                         Prototype
**********************************************************/
static void sifrpc_thread();
static void * sifrpc_server(unsigned int fno, void *data, int size);
static void * get_connect_info(unsigned int fno, void *data, int size);
static void * read_keyboard(unsigned int fno, void *data, int size);
static void * get_device_location(unsigned int fno, void *data, int size);
static int usbkeybd_probe(int dev_id);
static int usbkeybd_attach(int dev_id);
static UNIT *unit_alloc(int dev_id, int payload, int ifnum, int as);
static void unit_free(UNIT *p);
static void set_config_done(int result, int count, void *arg);
static void set_interface_done(int result, int count, void *arg);
static void data_transfer(UNIT *unit);
static void data_transfer_done(int result, int count, void *arg);
static void led_transfer_done(int result, int count, void *arg);
static int usbkeybd_detach(int dev_id);

/*********************************************************
                      Program Start
**********************************************************/

static sceUsbdLddOps usbkeybd_ops = {
	NULL, NULL,
	"usbkeybd",
	usbkeybd_probe,
	usbkeybd_attach,
	usbkeybd_detach,
};

#define BASE_priority  50

/*---------------------------------------------
  Function Name	: start
  function     	: メイン関数
  Input Data	: none
  Output Data   : none
  Return Value	: RESIDENT_END(常駐終了),
                  NO_RESIDENT_END(非常駐終了)
----------------------------------------------*/
int start(void){
	int r;
	struct ThreadParam param;
	int th;
	int i;
	
	if(0 != (r = sceUsbdRegisterLdd(&usbkeybd_ops)))
	  printf("usbkeybd: sceUsbdRegisterLdd -> 0x%x\n", r);
	
	// init common variables
	USBKEYBD.next_number = 0;
	USBKEYBD.n = 0;
	for(i=0;i<MAX_KEYBD_NUM;i++) {
	  USBKEYBD.con_unit[i] = NULL;
	}
	
	// init thread
	param.attr         = TH_C;
	param.entry        = sifrpc_thread;
	param.initPriority = BASE_priority;
	param.stackSize    = 0x1000;
	param.option       = 0;
	th = CreateThread(&param);
	if (th > 0) {
		StartThread(th,0);
		return RESIDENT_END;
	}else{
		return NO_RESIDENT_END;
	}
	
	return(r);
}

/*---------------------------------------------
  Function Name	: sifrpc_thread
  function     	: EEからのRPCコールに応答するスレッド
  Input Data	: none
  Output Data   : none
  Return Value	: none
----------------------------------------------*/
static void sifrpc_thread()
{
	sceSifQueueData qd;
	sceSifServeData sd;
	
	sceSifInitRpc(0);
	sceSifSetRpcQueue(&qd, GetThreadId());
	sceSifRegisterRpc(&sd, SCE_RPC_USB_KEYBD, sifrpc_server,
			  (void *) &buffer[0],0,0,&qd);
	sceSifRpcLoop(&qd);
}

/*---------------------------------------------
  Function Name	: sifrpc_server
  function     	: EEからRPCコールがあったときに
                  呼ばれる関数
  Input Data	: fno(関数番号),
                  data(EEから受信したデータ)
                  size(EEから受信したデータサイズ)
  Output Data   : data(EEへ送信するデータ)
  Return Value	: data(EEへ送信するデータ)
----------------------------------------------*/
static void * sifrpc_server(unsigned int fno, void *data, int size)
{
        switch(fno) 
	  {
	  case 0x01:
	    //printf("Call get_connect_info()\n");
	    get_connect_info(fno,data,size);
	    break;
	  case 0x02:
	    //printf("Call read_keyboard()\n");
	    read_keyboard(fno,data,size);
	    break;
	  case 0x03:
	    //printf("Call get_device_location()\n");
	    get_device_location(fno,data,size);
	    break;
	  default:
	    printf("Illegal function No.%d from EE\n",fno);
	  }
	
	return data;
}

/*---------------------------------------------
  Function Name	: get_connect_info(for RPC)
  function     	: キーボードの接続情報の取得
  Input Data	: fno(関数番号),
                  data(EEから受信したデータ)
                  size(EEから受信したデータサイズ)
  Output Data   : data(EEへ送信するデータ)
  Return Value	: data(EEへ送信するデータ)
----------------------------------------------*/
static void * get_connect_info(unsigned int fno, void *data, int size)
{
	u_char *p;
	int i;
	
	p = (unsigned char *) data;
	
	*p++= MAX_KEYBD_NUM;
	*p++= USBKEYBD.n;
	for(i=0;i<MAX_KEYBD_NUM;i++) {
	  if (USBKEYBD.con_unit[i] != NULL) {
	    *p++ = 1;
	  } else {
	    *p++ = 0;
	  }
	}
	
	return data;
}

/*---------------------------------------------
  Function Name	: read_keyboard
  function     	: キーデータの取得(for RPC)
  Input Data	: fno(関数番号)
                  data(EEから受信したデータ)
                  size(EEから受信したデータサイズ)
  Output Data   : data(EEへ送信するデータ)
  Return Value	: data(EEへ送信するデータ)
----------------------------------------------*/
static void * read_keyboard(unsigned int fno, void *data, int size)
{
	u_char *p;
	UNIT *unit;
	int i;
	u_char *keybuf;
	
	p = (unsigned char *) data;
	if (p[0] > MAX_KEYBD_NUM) {
	  printf("Illegal index_no : %d\n",p[0]); //debug
	  p[0] = 0;  /* data length = 0 */
	  return data;
	}
	unit = USBKEYBD.con_unit[p[0]]; /* p[0]: keyboard number */
	if (unit == NULL) {
	  printf("USB keyboard %d is not connected!\n",p[0]);
	  p[0] = 0;  /* data length = 0 */
	  return data;
	}
	
	//printf("wp:%d rp:%d len:%d pl:%d\n",
	//       unit->wp,unit->rp,unit->rblen,unit->payload); //debug
	
	/*----- get from ringbuffer -----*/
	if (unit->rblen > 0) {
	  keybuf = &(unit->ringbuf[unit->rp][0]);
	  *p++ = keybuf[0]; /* ledptn */
	  *p++ = keybuf[1]; /* len */
	  for(i=0;i<unit->payload;i++) {
	    *p++ = keybuf[i+2];
	  }
	  if (++unit->rp >= RINGBUF_SIZE) { unit->rp = 0; }
	  unit->rblen--;
	} else {
	  *p++ = unit->ledptn; /* ledptn */
	  *p++ = 0;            /* len */
	  for(i=0;i<unit->payload;i++) { *p++ = 0; }
	}
	
	return data;
}

/*---------------------------------------------
  Function Name	: get_device_location(for RPC)
  function     	: デバイスの位置情報を取得
  Input Data	: fno(関数番号),
                  data(EEから受信したデータ)
                  size(EEから受信したデータサイズ)
  Output Data   : data(EEへ送信するデータ)
  Return Value	: data(EEへ送信するデータ)
----------------------------------------------*/
static void * get_device_location(unsigned int fno, void *data, int size)
{
	u_char *p;
	u_char locs[7];
	int i;
	int dev_id;
	int r;
	
	p = (unsigned char *) data;
	
	if (p[0] >= MAX_KEYBD_NUM) {
	  printf("Illegal USB keyboard number! : %d\n",p[0]);
	  for(i=0;i<8;i++) { *p++ = 0; }
	  return data;
	}
	
	if (USBKEYBD.con_unit[p[0]] == NULL) {
	  printf("USB keyboard %d is not connected!\n",p[0]);
	  for(i=0;i<8;i++) { *p++ = 0; }
	  return data;
	}
	
	dev_id = USBKEYBD.con_unit[p[0]]->dev_id;
	
	if(sceUsbd_NOERR != (r = sceUsbdGetDeviceLocation(dev_id, locs))){
		err(USBKEYBD.con_unit[p[0]], "sceUsbdGetDeviceLocation", r);
		for(i=0;i<8;i++) { *p++ = 0; }
		return data;
	}
	
	for(i=0;i<7;i++) { *p++ = locs[i]; }
	
	return data;
}



/*---------------------------------------------
  Function Name	: usbkeybd_probe
  function     	: LDD検出処理関数
  Input Data	: dev_id(デバイスID)
  Output Data   : none
  Return Value	: 0(検出したデバイスはこのLDDに対応しない)
                  1(検出したデバイスはこのLDDに対応)
----------------------------------------------*/
static int usbkeybd_probe(int dev_id){
	UsbDeviceDescriptor *ddesc;
	UsbInterfaceDescriptor *idesc;
	
	if(NULL == (ddesc = sceUsbdScanStaticDescriptor(dev_id, NULL,
			USB_DESCRIPTOR_TYPE_DEVICE)))
		return(0);
	if(ddesc->bDeviceClass != 0)  /* 0:HID Device Class */ 
		return(0);
	//printf("ddesc->bNumConfigurations:%d\n",ddesc->bNumConfigurations);
	if(ddesc->bNumConfigurations != 1)
		return(0);
	if(NULL == (idesc = sceUsbdScanStaticDescriptor(dev_id, ddesc,
			USB_DESCRIPTOR_TYPE_INTERFACE)))
		return(0);
	if(idesc->bInterfaceClass != 3  /* 3:HID Interface Class*/
	   || idesc->bInterfaceSubClass != 1  /* 1:Boot Interface subclass */
	   || idesc->bInterfaceProtocol != 1) /* 1:Keyboard */
	  {
	    return(0);
	  }
	return(1);
}

/*---------------------------------------------
  Function Name	: usbkeybd_attach
  function     	: LDD接続処理関数
                  (usbkeybd_probeの戻り値が1のときコールされる)
  Input Data	: dev_id(デバイスID)
  Output Data   : none
  Return Value	: 0(コンフィグレーション正常終了)
                  -1(コンフィグレーション失敗)
----------------------------------------------*/
static int usbkeybd_attach(int dev_id){
	UsbConfigurationDescriptor *cdesc;
	UsbInterfaceDescriptor *idesc;
	UsbEndpointDescriptor *edesc;
	UNIT *unit;
	int payload, r;
	u_char locs[7];
	
	if(NULL == (cdesc = sceUsbdScanStaticDescriptor(dev_id, NULL,
			USB_DESCRIPTOR_TYPE_CONFIGURATION)))
		return(-1);
//	printf("bNumInterfaces:%d\n",cdesc->bNumInterfaces);
//	if(cdesc->bNumInterfaces != 1) /* 通常は１ */
//		return(-1);

	if(NULL == (idesc = sceUsbdScanStaticDescriptor(dev_id, cdesc,
			USB_DESCRIPTOR_TYPE_INTERFACE)))
		return(-1);
	if(idesc->bNumEndpoints != 1)
		return(-1);
	
	if(NULL == (edesc = sceUsbdScanStaticDescriptor(dev_id, idesc,
			USB_DESCRIPTOR_TYPE_ENDPOINT)))
		return(-1);
	if((edesc->bEndpointAddress & USB_ENDPOINT_DIRECTION_BITS)
			!= USB_ENDPOINT_DIRECTION_IN)
		return(-1);
	if((edesc->bmAttribute & USB_ENDPOINT_TRANSFER_TYPE_BITS)
			!= USB_ENDPOINT_TRANSFER_TYPE_INTERRUPT)
		return(-1);

	payload = edesc->wMaxPacketSize0 | (edesc->wMaxPacketSize1 << 8);
	if(payload > KEY_DATA_LEN) { return -1;	}

	if(NULL == (unit = unit_alloc(dev_id, payload, idesc->bInterfaceNumber,
			idesc->bAlternateSetting)))
		return(-1);
	if(0 > (unit->c_pipe = sceUsbdOpenPipe(dev_id, NULL)))
		return(-1);
	if(0 > (unit->d_pipe = sceUsbdOpenPipe(dev_id, edesc)))
		return(unit_free(unit), -1);
	sceUsbdSetPrivateData(dev_id, unit);
	
	if(sceUsbd_NOERR != (r = sceUsbdSetConfiguration(unit->c_pipe,
			cdesc->bConfigurationValue, set_config_done, unit))){
		err(unit, "sceUsbdSetConfiguration", r);
		return(-1);
	}
	
	if(sceUsbd_NOERR != (r = sceUsbdGetDeviceLocation(dev_id, locs))){
		err(unit, "sceUsbdGetDeviceLocation", r);
		return(-1);
	}
	
	printf("dev_id:%d\n",dev_id);
	printf("usbkeybd%d: attached (port=", unit->number);
	for(r = 0; r < 7 && locs[r] != 0; r++)
		printf("%s%d", ((r)? ",": ""), locs[r]);
	printf(")\n");
	
	// update common data
	USBKEYBD.n++;
	USBKEYBD.con_unit[unit->number] = unit;
	
	return(0);
}

/*---------------------------------------------
  Function Name	: unit_alloc
  function     	: LDDの依存データの確保と作成
  Input Data	: payload(1フレーム中の最大パケット長), 
                  ifnum(インターフェース番号), 
                  as(代替設定)
  Output Data   : none
  Return Value	: NULL(メモリ確保に失敗)
                  not NULL(依存データのポインタ)
----------------------------------------------*/
static UNIT *unit_alloc(int dev_id, int payload, int ifnum, int as){
	UNIT *p;
	
	if (USBKEYBD.n >= MAX_KEYBD_NUM) return NULL;
	
	if(NULL != (p = AllocSysMemory(0, sizeof(UNIT) + payload, NULL))){
	        p->dev_id = dev_id;
		p->number = USBKEYBD.next_number;
	        while(1) {
		  ++USBKEYBD.next_number;
		  if (USBKEYBD.next_number >= MAX_KEYBD_NUM) {
		    USBKEYBD.next_number = 0;
		  }
		  if (USBKEYBD.con_unit[USBKEYBD.next_number] == NULL) {
		    break;
		  }
		}
		p->payload = payload;
		p->ifnum = ifnum;
		p->as = as;
		p->count = 0;
		p->ledptn = 0;
		p->rp = 0;
		p->wp = 0;
		p->rblen = 0;
	}
	return(p);
}

/*---------------------------------------------
  Function Name	: unit_free
  function     	: LDDの依存データの開放
  Input Data	: p(LDD依存データのポインタ)
  Output Data   : none
  Return Value	: none
----------------------------------------------*/
static void unit_free(UNIT *p){
	FreeSysMemory(p);
}

/*---------------------------------------------
  Function Name	: set_config_done
  function     	: 制御転送(SetConfiguration要求)のコールバック関数
  Input Data	: result(制御転送の結果Usbd_XXX), 
                  count(転送済みデータ数), 
                  arg(LDD依存データのポインタ)
  Output Data   : none
  Return Value	: none
----------------------------------------------*/
static void set_config_done(int result, int count, void *arg){
	UNIT *unit = arg;
	int r;

	err(unit, "sceUsbdSetConfiguration", result);
	r = sceUsbdSetInterface(unit->c_pipe, unit->ifnum, unit->as,
		set_interface_done, unit);
	err(unit, "sceUsbdSetInterface", r);
}

/*---------------------------------------------
  Function Name	: set_interface_done
  function     	: 制御転送(SetInterface要求)のコールバック関数
  Input Data	: result(制御転送の結果Usbd_XXX), 
                  count(転送済みデータ数), 
                  arg(LDD依存データのポインタ)
  Output Data   : none
  Return Value	: none
----------------------------------------------*/
static void set_interface_done(int result, int count, void *arg){
	UNIT *unit = arg;

	err(unit, "sceUsbdSetInterface", result);
	data_transfer(unit);
}

/*---------------------------------------------
  Function Name	: data_transfer
  function     	: インタラプト転送の要求
  Input Data	: unit(LDD依存データのポインタ)
  Output Data   : none
  Return Value	: none
----------------------------------------------*/
static void data_transfer(UNIT *unit){
	int r;

	r = sceUsbdInterruptTransfer(unit->d_pipe,
		unit->data, unit->payload, data_transfer_done, unit);
	err(unit, "sceUsbdInterruptTransfer", r);
}

/*---------------------------------------------
  Function Name	: data_transfer_done
  function     	: インタラプト転送のコールバック関数
  Input Data	: result(インタラプト転送の結果Usbd_XXX), 
                  count(転送済みデータ数), 
                  arg(LDD依存データのポインタ)
  Output Data   : none
  Return Value	: none
----------------------------------------------*/
static void data_transfer_done(int result, int count, void *arg){
	UNIT *unit = arg;
	int i;
	int r;
	u_char *keybuf;
	
	err(unit, "data_transfer_done:sceUsbdInterruptTransfer", result);
	if(result == sceUsbd_NOERR){
		printf("usbkeybd%d: count=%d led=%02X data=(",
			 unit->number, ++unit->count, unit->ledptn);
		for(i = 0; i < count; i++)
			printf(" %02x", unit->data[i]);
		printf(" )\n");
	} else {
	  data_transfer(unit);
	  return;
	}
	
	for(i=2; i < count; i++) {
	  switch(unit->data[i])
	    {
	    case 0x53: /* NUM LOCK */
	      unit->ledptn ^= 1<<0;
	      break;
	    case 0x39: /* CAPS LOCK */
#if (CAPS_LED_TYPE == MAC)
	      unit->ledptn ^= 1<<1;
#endif
#if (CAPS_LED_TYPE == WIN)
	      if (unit->data[0] & 0x22) {
		unit->ledptn ^= 1<<1;
	      }
#endif
	      break;
	    case 0x47: /* SCROLL LOCK */
	      unit->ledptn ^= 1<<2;
	      break;
	    case 0xE3: /* COMPOSE */
	      unit->ledptn ^= 1<<3;
	      break;
	    case 0x90: /* KANA */
	      unit->ledptn ^= 1<<4;
	      break;
	    }
	}
	
	r = sceUsbdControlTransfer(unit->c_pipe,  /* Pipe ID */
				   0x21,          /* bmRequestType */
				   0x09,          /* bRequest = SET_REPORT */
				   0x02*256+0x00, /* Report type = 0x02(Output)
						     Report ID   = 0x00 */
				   unit->ifnum,   /* Interface */
				   1,             /* Report Length = 1byte */
				   &unit->ledptn, /* LED DATA */
				   led_transfer_done, /* Callback */
				   unit);
	
	if(sceUsbd_NOERR != r){
		err(unit, "sceUsbdSetConfiguration", r);
	}
	
	/*----- put ringbuffer (for sifrpc) -----*/
	//printf("wp:%d rp:%d len:%d\n",unit->wp,unit->rp,unit->rblen); //debug
	if (unit->rblen < RINGBUF_SIZE) {
	  keybuf = &(unit->ringbuf[unit->wp][0]);
	  keybuf[0] = unit->ledptn;
	  keybuf[1] = (u_char)(count & 0xff);
	  for(i=0;i<count;i++) {
	    keybuf[i+2] = unit->data[i];
	  }
	  if (++unit->wp >= RINGBUF_SIZE) { unit->wp = 0; }
	  unit->rblen++;
	}
}

/*---------------------------------------------
  Function Name	: led_transfer_done
  function     	: LEDデータ転送のコールバック関数
  Input Data	: result(制御転送の結果Usbd_XXX), 
                  count(転送済みデータ数), 
                  arg(LDD依存データのポインタ)
  Output Data   : none
  Return Value	: none
----------------------------------------------*/
static void led_transfer_done(int result, int count, void *arg){
	UNIT *unit = arg;
	
	err(unit, "led_transfer_done", result);
	
	data_transfer(unit);
}

/*---------------------------------------------
  Function Name	: usbkeybd_detach
  function     	: LDD切断処理関数
  Input Data	: dev_id(デバイスID)
  Output Data   : none
  Return Value	: 0(正常終了)
                  -1(LDD依存データなし)
----------------------------------------------*/
static int usbkeybd_detach(int dev_id){
	UNIT *unit;
	
	if(NULL == (unit = sceUsbdGetPrivateData(dev_id)))
		return(-1);
	
	// update common data
	USBKEYBD.n--;
	USBKEYBD.con_unit[unit->number] = NULL;
	
	// free
	printf("usbkeybd%d: detached\n", unit->number);
	unit_free(unit);
	
	return(0);
}

