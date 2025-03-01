[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The Sample Program to Dump Static Descriptors of USB Devices


< Description of the Sample >
        This sample program is to dump the static descriptor
	of the USB device.  The sample can be used as shown below.

< File >

        usbdesc.c  	main program

< Using the Program >

1. Do not connect any device to the USB port of the PlayStation 2.
2. Execute $ make run
3. Insert the USB device, which you want to dump descriptors, into 
   the PlayStation 2.  The following will be displayed.

    New device plug-in!!!
    DEVICE-ID:2
    Dump end

4. See the current directory.
   "dev_id2.dsc" is the device descriptor file.

    dsidb R> quit
    $ ls
    Makefile      dev_id2.dsc   readme_j.txt  usbdesc.irx
    PathDefs      readme_e.txt  usbdesc.c     usbdesc.o
    $ 

