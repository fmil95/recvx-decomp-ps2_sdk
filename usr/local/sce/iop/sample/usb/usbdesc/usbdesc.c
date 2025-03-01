/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *      USB Static Descriptor Printer
 *
 *                          Version 0.11
 *                          Shift-JIS
 *
 *      Copyright (C) 1998-2000 Sony Computer Entertainment Inc.
 *                       All Rights Reserved.
 *
 *                             usbdesc.c
 *
 *        Version       Date            Design     Log
 *  --------------------------------------------------------------------
 *        0.10          Apr,28,2000     fukunaga   Initial
 *        0.11          July,21,2000    fukunaga
 */

#include <stdio.h>
#include <kernel.h>
#include <sys/file.h>
#include <memory.h>

#include <usb.h>
#include <usbd.h>


ModuleInfo Module = {"USB_DESCRIPTOR_PRINTER", 0x0201 };





static int fd;


#define err(p, f, r)	if(r) printf("usbdesc%d: %s -> 0x%x\n", \
				(p)->number, (f), (r))



static int usbdesc_probe(int dev_id);
static void dump_device_desc(UsbDeviceDescriptor *desc);
static void dump_configration_desc(UsbConfigurationDescriptor *desc);
static void dump_interface_desc(UsbInterfaceDescriptor *desc);
static void dump_endpoint_desc(UsbEndpointDescriptor *desc);
static void dump_class_specific_desc(u_char *desc);
static int usbdesc_attach(int dev_id);
static int usbdesc_detach(int dev_id);



static sceUsbdLddOps usbdesc_ops = {
	NULL, NULL,
	"usbdesc",
	usbdesc_probe,
	usbdesc_attach,
	usbdesc_detach,
};

#define BASE_priority  32



int start(void){
	int r;
	
	if(0 != (r = sceUsbdRegisterLdd(&usbdesc_ops)))
		printf("usbdesc: sceUsbdRegisterLdd -> 0x%x\n", r);
	
	return(r);
}


static int usbdesc_probe(int dev_id){
        char* desc = NULL;
	int i;
	char filename[256];
	
	sprintf(filename,"host1:dev_id%d.dsc",dev_id);

	fd = open(filename,O_WRONLY | O_CREAT);
	if (fd == 0) {
	  printf("Cannot open file\n");
	  return (0);
	}
	
	printf("New device plug-in!!!\n");
	printf("DEVICE-ID:%d\n",dev_id);

	fdprintf(fd,"-------------------------------------------\n");

	while (1) 
	  {
	    if(NULL == (desc = sceUsbdScanStaticDescriptor(dev_id, desc,0)))
	      { break; }
	    switch(desc[1]) 
	      {
	      case 1:
		dump_device_desc((UsbDeviceDescriptor*)desc);
		break;
	      case 2:
		dump_configration_desc((UsbConfigurationDescriptor*)desc);
		break;
	      case 3:
		fdprintf(fd,"STRING-DESCRIPTOR"); /* ‚±‚±‚É•ªŠò‚·‚é‚±‚Æ‚Í‚ ‚è‚¦‚È‚¢ */
		break;
	      case 4:
		dump_interface_desc((UsbInterfaceDescriptor*)desc);
		break;
	      case 5:
		dump_endpoint_desc((UsbEndpointDescriptor*)desc);
		break;
	      default:
		dump_class_specific_desc(desc);
		break;
	      }
	    for(i=0;i<100000;i++); /* wait for printf */
	  }
	
	fdprintf(fd,"-------------------------------------------\n");
	
	close(fd);
	
	printf("Dump end\n");
	
	return(0);
}


static void dump_device_desc(UsbDeviceDescriptor *desc)
{
        fdprintf(fd,"\nDEVICE DESCRIPTOR\n");
	
	fdprintf(fd," bLength:%02Xh(%d)\n",desc->bLength,desc->bLength);
	fdprintf(fd," bDescriptorType:%02Xh(%d)\n",
	       desc->bDescriptorType,desc->bDescriptorType);

	fdprintf(fd," bcdUSB:%04Xh\n",desc->bcdUSB);
	fdprintf(fd," bDeviceClass:%02Xh(%d)\n",desc->bDeviceClass,desc->bDeviceClass);
	fdprintf(fd," bDeviceSubClass:%02Xh(%d)\n",
	       desc->bDeviceSubClass,desc->bDeviceSubClass);
	fdprintf(fd," bDeviceProtocol:%02Xh(%d)\n",
	       desc->bDeviceProtocol,desc->bDeviceProtocol);
	fdprintf(fd," bMaxPacketSize0:%02Xh(%d)\n",
	       desc->bMaxPacketSize0,desc->bMaxPacketSize0);
	fdprintf(fd," idVendor:%04Xh\n",desc->idVendor);
	fdprintf(fd," idProduct:%04Xh\n",desc->idProduct);
	fdprintf(fd," bcdDevice:%04Xh\n",desc->bcdDevice);
	fdprintf(fd," iManufacturer:%02Xh(%d)\n",
	       desc->iManufacturer,desc->iManufacturer);
	fdprintf(fd," iProduct:%02Xh(%d)\n",desc->iProduct,desc->iProduct);
	fdprintf(fd," iSerialNumber:%02Xh(%d)\n",
	       desc->iSerialNumber,desc->iSerialNumber);
	fdprintf(fd," bNumConfigurations:%02Xh(%d)\n",
	       desc->bNumConfigurations,desc->bNumConfigurations);
	
}


static void dump_configration_desc(UsbConfigurationDescriptor *desc)
{
        u_short wTotalLength;

        fdprintf(fd,"\nCONFIGURATION DESCRIPTOR\n");
	
	fdprintf(fd," bLength:%02Xh(%d)\n",desc->bLength,desc->bLength);
	fdprintf(fd," bDescriptorType:%02Xh(%d)\n",
	       desc->bDescriptorType,desc->bDescriptorType);
	
	wTotalLength = desc->wTotalLength0 + desc->wTotalLength1 * 256;
	fdprintf(fd," wTotalLength:%04Xh(%d)\n",wTotalLength,wTotalLength);
	fdprintf(fd," bNumInterfaces:%02Xh(%d)\n",
	       desc->bNumInterfaces,desc->bNumInterfaces);
	fdprintf(fd," bConfigurationValue:%02Xh(%d)\n",
	       desc->bConfigurationValue,desc->bConfigurationValue);
	fdprintf(fd," iConfiguration:%02Xh(%d)\n",
	       desc->iConfiguration,desc->iConfiguration);
	fdprintf(fd," bmAttribute:%02Xh(%d)\n",desc->bmAttribute,desc->bmAttribute);
	fdprintf(fd," MaxPower:%02Xh(%d)\n",desc->MaxPower,desc->MaxPower);
}


static void dump_interface_desc(UsbInterfaceDescriptor *desc)
{
        fdprintf(fd,"\nINTERFACE DESCRIPTOR\n");
  
	fdprintf(fd," bLength:%02Xh(%d)\n",desc->bLength,desc->bLength);
	fdprintf(fd," bDescriptorType:%02Xh(%d)\n",
	       desc->bDescriptorType,desc->bDescriptorType);

	fdprintf(fd," bInterfaceNumber:%02Xh(%d)\n",
	       desc->bInterfaceNumber,desc->bInterfaceNumber);
	fdprintf(fd," bAlternateSetting:%02Xh(%d)\n",
	       desc->bAlternateSetting,desc->bAlternateSetting);
	fdprintf(fd," bNumEndpoints:%02Xh(%d)\n",
	       desc->bNumEndpoints,desc->bNumEndpoints);
	fdprintf(fd," bInterfaceClass:%02Xh(%d)\n",
	       desc->bInterfaceClass,desc->bInterfaceClass);
	fdprintf(fd," bInterfaceSubClass:%02Xh(%d)\n",
	       desc->bInterfaceSubClass,desc->bInterfaceSubClass);
	fdprintf(fd," bInterfaceProtocol:%02Xh(%d)\n",
	       desc->bInterfaceProtocol,desc->bInterfaceProtocol);
	fdprintf(fd," iInterface:%02Xh(%d)\n",desc->iInterface,desc->iInterface);

}


static void dump_endpoint_desc(UsbEndpointDescriptor *desc)
{
        u_short wMaxPacketSize;
	char dmystr[128];
	
        fdprintf(fd,"\nENDPOINT DESCRIPTOR\n");
	
	fdprintf(fd," bLength:%02Xh(%d)\n",desc->bLength,desc->bLength);
	fdprintf(fd," bDescriptorType:%02Xh(%d)\n",
	       desc->bDescriptorType,desc->bDescriptorType);
	
	if (desc->bEndpointAddress & 0x80) {
	  sprintf(dmystr,"In");
	} else {
	  sprintf(dmystr,"Out");
	}
	fdprintf(fd," bEndpointAddress:%02Xh(%d) [%s]\n",
		 desc->bEndpointAddress,desc->bEndpointAddress,dmystr);
	
	switch(desc->bmAttribute & 0x03) {
	case 0:
	  sprintf(dmystr,"Control");
	  break;
	case 1:
	  sprintf(dmystr,"Isochronous");
	  break;
	case 2:
	  sprintf(dmystr,"Bulk");
	  break;
	case 3:
	  sprintf(dmystr,"Interrupt");
	  break;
	}
	fdprintf(fd," bmAttribute:%02Xh(%d) [%s]\n",
		 desc->bmAttribute,desc->bmAttribute,dmystr);
	
	wMaxPacketSize = desc->wMaxPacketSize0 + desc->wMaxPacketSize1 * 256;
	fdprintf(fd," wMaxPacketSize:%04Xh(%d)\n",wMaxPacketSize,wMaxPacketSize);
	fdprintf(fd," bInterval:%02Xh(%d)\n",desc->bInterval,desc->bInterval);

}


static void dump_class_specific_desc(u_char *desc)
{
        int i;

        fdprintf(fd,"\nCLASS-SPECIFIC DESCRIPTOR\n");

	fdprintf(fd," bLength:%02Xh(%d)\n",desc[0],desc[0]);
	fdprintf(fd," bDescriptorType:%02Xh(%d)\n",desc[1],desc[1]);

	for(i=2;i<desc[0];i++) {
	  fdprintf(fd," %02Xh(%d)\n",desc[i],desc[i]);
	}

}


static int usbdesc_attach(int dev_id){
	return(-1);
}


static int usbdesc_detach(int dev_id){
	return(0);
}

