/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      USB Mouse Class Driver
 *
 *                          Version 0.20
 *                          Shift-JIS
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                              usbmouse.c
 *
 *        Version       Date            Design     Log
 *  --------------------------------------------------------------------
 *      0.10            Jan, 2,2000     mka        Initial
 *      0.11            Jan,15,2000     hatto      Using SifRpc
 *      0.12            Apr,14,2000     mka        Using sceUsbdGetDeviceLocation()
 *      0.20            Apr,24,2000     fukunaga   Multi-MOUSE
 */

#include <stdio.h>
#include <kernel.h>
#include <memory.h>
#include <sif.h>
#include <sifcmd.h>
#include <sifrpc.h>

#include <usb.h>
#include <usbd.h>

#include "usbmouse.h"

ModuleInfo Module = {"USB MOUSE SAMPLE", 0x0202 };


typedef struct _unit {
	int dev_id;
        int number; /* EEからのRPC要求を受けるためのINDEX */
        int c_pipe;
        int d_pipe;
        int payload;
        int ifnum;
        int as;
        int tcount; /* 接続されてから行ったデータ転送の総回数 */
  
        /* Ring buffer for RPC */
	int rp;
        int wp;
        int rblen;
        u_char ringbuf[RINGBUF_SIZE][MOUSE_DATA_LEN];
  
        /* for interrupt transfer */
	u_char data[0];  
} UNIT;

typedef struct {
        int next_number;
	int n; // the number of Usb mouse connecting
	UNIT* con_unit[MAX_MOUSE_NUM];
} USBMOUSE_t;



USBMOUSE_t USBMOUSE;

#define SSIZE 0x20
static unsigned int buffer[SSIZE];



#define err(p, f, r)	if(r) printf("usbmouse%d: %s -> 0x%x\n", \
				(p)->number, (f), (r))



static void sifrpc_thread();
static void * sifrpc_server(unsigned int fno, void *data, int size);
static void * get_connect_info(unsigned int fno, void *data, int size);
static void * read_mouse(unsigned int fno, void *data, int size);
static void * get_device_location(unsigned int fno, void *data, int size);
static int usbmouse_probe(int dev_id);
static int usbmouse_attach(int dev_id);
static UNIT *unit_alloc(int dev_id, int payload, int ifnum, int as);
static void unit_free(UNIT *p);
static void set_config_done(int result, int count, void *arg);
static void set_interface_done(int result, int count, void *arg);
static void data_transfer(UNIT *unit);
static void data_transfer_done(int result, int count, void *arg);
static int usbmouse_detach(int dev_id);



static sceUsbdLddOps usbmouse_ops = {
	NULL, NULL,
	"usbmouse",
	usbmouse_probe,
	usbmouse_attach,
	usbmouse_detach,
};

#define BASE_priority  50



int start(void){
	int r;
	struct ThreadParam param;
	int th;
	int i;
	
	if(0 != (r = sceUsbdRegisterLdd(&usbmouse_ops)))
		printf("usbmouse: sceUsbdRegisterLdd -> 0x%x\n", r);
	
	// init common variables
	USBMOUSE.next_number = 0;
	USBMOUSE.n = 0;
	for(i=0;i<MAX_MOUSE_NUM;i++) {
	  USBMOUSE.con_unit[i] = NULL;
	}
	
	// init thread
	param.attr         = TH_C;
	param.entry        = sifrpc_thread;
	param.initPriority = BASE_priority;
	param.stackSize    = 0x800;
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


static void sifrpc_thread()
{
	sceSifQueueData qd;
	sceSifServeData sd;

	sceSifInitRpc(0);
	sceSifSetRpcQueue(&qd, GetThreadId());
	sceSifRegisterRpc(&sd, SCE_RPC_USB_MOUSE, sifrpc_server,
			  (void *) &buffer[0],0,0,&qd);
	sceSifRpcLoop(&qd);
}


static void * sifrpc_server(unsigned int fno, void *data, int size)
{
        switch(fno) 
	  {
	  case 0x01:
	    	    get_connect_info(fno,data,size);
	    break;
	  case 0x02:
	    	    read_mouse(fno,data,size);
	    break;
	  case 0x03:
	    	    get_device_location(fno,data,size);
	    break;
	  default:
	    printf("Illegal function No.%d from EE\n",fno);
	  }
	
	return data;
}


static void * get_connect_info(unsigned int fno, void *data, int size)
{
	u_char *p;
	int i;
	
	p = (unsigned char *) data;
	
	*p++= MAX_MOUSE_NUM;
	*p++= USBMOUSE.n;
	for(i=0;i<MAX_MOUSE_NUM;i++) {
	  if (USBMOUSE.con_unit[i] != NULL) {
	    *p++ = 1;
	  } else {
	    *p++ = 0;
	  }
	}
	
	return data;
}


static void * read_mouse(unsigned int fno, void *data, int size)
{
	unsigned char *p;
	UNIT *unit;
	int i;
	u_char *mousebuf;

	p = (unsigned char *) data;
	if (p[0] > MAX_MOUSE_NUM) {
	  printf("Illegal index_no : %d\n",p[0]); 	  p[0] = 0;  /* data length = 0 */
	  return data;
	}
	unit = USBMOUSE.con_unit[p[0]]; /* p[0]: mouse number */
	if (unit == NULL) {
	  printf("USB mouse %d is not connected!\n",p[0]);
	  p[0] = 0;  /* data length = 0 */
	  return data;
	}
	
		//       unit->wp,unit->rp,unit->rblen,unit->payload); //debug
	

	if (unit->rblen > 0) {
	  mousebuf = &(unit->ringbuf[unit->rp][0]);
	  *p++ = mousebuf[0]; /* len */
	  for(i=0;i<unit->payload;i++) {
	    *p++ = mousebuf[i+1];
	  }
	  if (++unit->rp >= RINGBUF_SIZE) { unit->rp = 0; }
	  unit->rblen--;
	} else {
	  *p++ = 0;           /* len */
	  for(i=0;i<unit->payload;i++) { *p++ = 0; }
	}
	
	return data;
}


static void * get_device_location(unsigned int fno, void *data, int size)
{
	u_char *p;
	u_char locs[7];
	int i;
	int dev_id;
	int r;
	
	p = (unsigned char *) data;
	
	if (p[0] >= MAX_MOUSE_NUM) {
	  printf("Illegal USB mouse number! : %d\n",p[0]);
	  for(i=0;i<8;i++) { *p++ = 0; }
	  return data;
	}
	
	if (USBMOUSE.con_unit[p[0]] == NULL) {
	  printf("USB mouse %d is not connected!\n",p[0]);
	  for(i=0;i<8;i++) { *p++ = 0; }
	  return data;
	}
	
	dev_id = USBMOUSE.con_unit[p[0]]->dev_id;
	
	if(sceUsbd_NOERR != (r = sceUsbdGetDeviceLocation(dev_id, locs))){
		err(USBMOUSE.con_unit[p[0]], "sceUsbdGetDeviceLocation", r);
		for(i=0;i<8;i++) { *p++ = 0; }
		return data;
	}
	
	for(i=0;i<7;i++) { *p++ = locs[i]; }
	
	return data;
}


static int usbmouse_probe(int dev_id){
	UsbDeviceDescriptor *ddesc;
	UsbInterfaceDescriptor *idesc;

	if(NULL == (ddesc = sceUsbdScanStaticDescriptor(dev_id, NULL,
			USB_DESCRIPTOR_TYPE_DEVICE)))
		return(0);
	if(ddesc->bDeviceClass != 0 || ddesc->bNumConfigurations != 1)
		return(0);
	if(NULL == (idesc = sceUsbdScanStaticDescriptor(dev_id, ddesc,
			USB_DESCRIPTOR_TYPE_INTERFACE)))
		return(0);
	if(idesc->bInterfaceClass != 3
			|| idesc->bInterfaceSubClass != 1
			|| idesc->bInterfaceProtocol != 2)
		return(0);
	return(1);
}


static int usbmouse_attach(int dev_id){
	UsbConfigurationDescriptor *cdesc;
	UsbInterfaceDescriptor *idesc;
	UsbEndpointDescriptor *edesc;
	UNIT *unit;
	int payload, r;
	u_char locs[7];

	if(NULL == (cdesc = sceUsbdScanStaticDescriptor(dev_id, NULL,
			USB_DESCRIPTOR_TYPE_CONFIGURATION)))
		return(-1);
	if(cdesc->bNumInterfaces != 1)
		return(-1);

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

	printf("usbmouse%d: attached (port=", unit->number);
	for(r = 0; r < 7 && locs[r] != 0; r++)
		printf("%s%d", ((r)? ",": ""), locs[r]);
	printf(")\n");

		// update common data
	USBMOUSE.n++;
	USBMOUSE.con_unit[unit->number] = unit;

	return(0);
}


static UNIT *unit_alloc(int dev_id, int payload, int ifnum, int as){
	UNIT *p;

	if (USBMOUSE.n >= MAX_MOUSE_NUM) return NULL;
	
	if(NULL != (p = AllocSysMemory(0, sizeof(UNIT) + payload, NULL))){
	        p->dev_id = dev_id;
		p->number = USBMOUSE.next_number;
	        while(1) {
		  ++USBMOUSE.next_number;
		  if (USBMOUSE.next_number >= MAX_MOUSE_NUM) {
		    USBMOUSE.next_number = 0;
		  }
		  if (USBMOUSE.con_unit[USBMOUSE.next_number] == NULL) {
		    break;
		  }
		}
		p->payload = payload;
		p->ifnum = ifnum;
		p->as = as;
		p->tcount = 0;
		p->rp = 0;
		p->wp = 0;
		p->rblen = 0;
	}
	return(p);
}


static void unit_free(UNIT *p){
	FreeSysMemory(p);
}


static void set_config_done(int result, int count, void *arg){
	UNIT *unit = arg;
	int r;

	err(unit, "sceUsbdSetConfiguration", result);
	r = sceUsbdSetInterface(unit->c_pipe, unit->ifnum, unit->as,
		set_interface_done, unit);
	err(unit, "sceUsbdSetInterface", r);
}


static void set_interface_done(int result, int count, void *arg){
	UNIT *unit = arg;

	err(unit, "sceUsbdSetInterface", result);
	data_transfer(unit);
}


static void data_transfer(UNIT *unit){
	int r;

	r = sceUsbdInterruptTransfer(unit->d_pipe,
		unit->data, unit->payload, data_transfer_done, unit);
	err(unit, "sceUsbdInterruptTransfer", r);
}


static void data_transfer_done(int result, int count, void *arg){
	UNIT *unit = arg;
	int i;
	u_char *mousebuf;
	
	err(unit, "sceUsbdInterruptTransfer", result);
	if(result == sceUsbd_NOERR){
		printf("usbmouse%d: count=%d data=(",
			 unit->number, ++unit->tcount);
		for(i = 0; i < count; i++)
			printf(" %02x", unit->data[i]);
		printf(" )\n");
	}

	
		if (unit->rblen < RINGBUF_SIZE) {
	  mousebuf = &(unit->ringbuf[unit->wp][0]);
	  mousebuf[0] = (u_char)(count & 0xff);
	  for(i=0; i<count; i++) {
	    mousebuf[i+1] = unit->data[i];
	  }
	  if (++unit->wp >= RINGBUF_SIZE) { unit->wp = 0; }
	  unit->rblen++;
	}

	data_transfer(unit);
}


static int usbmouse_detach(int dev_id){
	UNIT *unit;

	if(NULL == (unit = sceUsbdGetPrivateData(dev_id)))
		return(-1);
	
	// update common data
	USBMOUSE.n--;
	USBMOUSE.con_unit[unit->number] = NULL;
	
	// free
	printf("usbmouse%d: detached\n", unit->number);
	unit_free(unit);
	
	return(0);
}

