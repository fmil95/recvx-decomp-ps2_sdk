[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program for USB mouse

< Description of the Sample >
        This program is a sample driver for USB mouse and also a sample 
        program presenting how to use USB driver (USBD).

        This driver is used for the following EE sample program.
        sce/ee/sample/usb/usbmouse

        When executing only this IOP program, printf messages can be 
	obtained using dsidb for confirmation.


        Please plug USB mouse in either of the USB ports when executing 
	the program.

<Specifications of the Sample>
        1. A maximum of 8 USB mice can be connected at one time.
           This value can be changed by using a macro, MAX_MOUSE_NUM, in 
	   "usbmouse.h".
        2. A ring buffer provided for each mouse assists in receiving EE 
	   data without missing any part.  The size of a ring buffer is 
	   defined by a macro, RINGBUF_SIZE, in "usbmouse.h". 
	    
< File >

        usbmouse.c  : Main program
        usbmouse.h  : Header file

< Activating the Program >

        % make                                     : Compiles the program
        % dsreset 0 0                              : Resets the program
        % dsistart /usr/local/sce/iop/modules/usbd.irx
        % dsistart usbmouse.irx                    : Executes the program

        In the case of confirming printf messages:
        % dsidb
        > reset 0 0
        > mstart /usr/local/sce/iop/modules/usbd.irx
        > mstart usbmouse.irx

<Messages>
        
	[Immediately after a mouse is connected]
          usbmouse0: attached (port=1)
          |_______|            |____|
              A                   B

          A : Mouse number assigned dynamically
          B : Port number a USB mouse is plugged in

        [Thereafter until the mouse is disconnected]
	  usbmouse0: count=1 data=( 00 0f 0f )
          |_______|  |_____|        |______|
              C         D               E

          C : Mouse number dynamically assigned when a USB mouse is plugged in
          D : The count of data receiving from the mouse
          E : Raw data transferred from the mouse
              byte 0    : Button
                            bit 0    : Button 1
                            bit 1    : Button 2
                            bit 2    : Button 3
                            bits 3-7 : Device-specific
              byte 1    : X (signed char)
              byte 2    : Y (signed char)
              byte 3    : Wheel (Device-specific)
              bytes 4-n : Device-specific

<Note>
        For further information on USB mouse, refer to 
	"Human Interface Devices" distributed on 
	"http://www.usb.org/developers/devclass_docs.html".

