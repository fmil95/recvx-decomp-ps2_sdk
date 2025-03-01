[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample driver for USB keyboard

< Description of the Sample >
        This program is a sample driver for USB keyboard and it also 
        indicates how to use the USB driver (USBD).
	
 	This driver is used for the following EE sample program.
        sce/ee/sample/usb/usbkeybd

        When executing the IOP driver only, the display can be checked
	with dsidb.


< Specification of the Sample >
        1. A maximum of 8 USB keyboards can be plugged at the same time.
	   The number of USB keyboards to be plugged can be set with the
	   MAX_KEYBD_NUM macro of "usbkeybd.h".

	2. A ring buffer is provided for every keyboard so that EE can 
	   get the data without a fail.  The ring buffer size is defined
	   in the RINGBUF_SIZE macro of "usbkeybd.h".

        3. LED can be controlled with this sample driver.
	   There are two ways of lighting CAPS LED up.
	   1. CAPS-LED is lit up by pressing the CAPS key as in Macintosh.
	   2. CAPS-LED lit up by pressing the CAPS+Shift keys as in Windows.
	      This is defined in the CAPS_LED_TYPE macro of "usbkeybd.h".

< Files >

        usbkeybd.c  Main program
        usbkeybd.h  Header file 

< Activating the Program >

        % make                                          : Compiles the program
        % dsreset 0 0                                   : Resets the program
        % dsistart /usr/local/sce/iop/modules/usbd.irx
        % dsistart usbkeybd.irx                         : Executes the program

        To check display:
        % dsidb
        > reset 0 0
        > mstart /usr/local/sce/iop/modules/usbd.irx
        > mstart usbkeybd.irx

< About Display >

        Examples of displays: 

        [When a keyboaord is plugged into a USB port]

          dev_id:7  <--- A 
          usbkeybd1: attached (port=2,1,1)
          |_______|            |________|
              B                     C

	  A : Device ID(1-127)
          B : Keyboard number (0-7) allocated by IOP.
          C : A port to which the USB keyboard is pluged.

	    The example indicates that the keyboard is pluged into
	    the USB port 2 of the console --> port 1 of the 1st hub (branch
	    off the console) --> port 1 of the 2nd hub (branch off the 1st hub)
        
        [When the data of a keyboard is being received]

          usbkeybd1: count=120 led=00 data=( 00 00 00 00 00 00 00 00 )
          |_______|  |_______| |____|        |_____________________|
              D          E        F                     G

          D : Keyboard number allocated by this sample driver.(not Device ID)
          E : The number of receiving data from this keyboard.
          F : The state of LED is lit up
                bit0  :NUM LOCK
                bit1  :CAPS LOCK
                bit2  :SCROLL LOCK
                bit3  :COMPOSE
                bit4  :KANA
                bit5-7:not used
          G : The raw data sent by the USB keyboard interrupt transfer.
	      Some keyboards may send longer data than this.

                byte0 :Modifier keys
                         bit0:Left-Ctrl
                         bit1:Left-Shift
                         bit2:Left-Alt(Win),Left-option(Mac)
                         bit3:Left-Win(Win),Left-Apple(Mac)
                         bit4:Right-Ctrl
                         bit5:Right-Shift
                         bit6:Right-Alt(Win),Right-option(Mac)
                         bit7:Right-Win(Win),Right-Apple(Mac)
                byte1 : Reserve
                byte2 : Key code
                  :
                byte7 : Key code

	For details of key codes, please refer to "HID Usage Tables Document"
	at "http://www.usb.org/developers/devclass_docs.html" on the web. 

< Using the Controller >
	None

< Note >
	For further information on USB keyboards, please refer to "Human 
	Interface Devices" at "http://www.usb.org/developers/devclass_docs.html"
	on the web. 
 