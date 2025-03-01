[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program to get data of the USB driver from IOP to EE

< Description of the Sample >
        In this program, the data of a USB keyboard is read by EE.

        It is necessary to compile the following USB keyboard sample 
        driver in IOP before executing this program.
        sce/iop/sample/usb/usbkeybd

        Please plug a USB keyboard in either of the USB ports when executing 
	the program.

< Files >
        main.c        : Sample of how to use USB keyboard driver
        usbkeybd.c    : Library-style USB keyboard acquisition routine (sample)
        usbkeybd.h    : Header file 


< Activating the Program >
        $ make          : Compiles the program
        $ make run      : Executes the program

        After the compilation, the program is also executable with the
        following.
        $ dsedb
        > run main.elf

< About Display >

	The following is an example of the display.

        usbkeybd1 : port=2,1,1 : LED[00] KEY[00 00 00 00 00 00 00 00 ]
        |_______|   |________|   |_____|     |______________________|
            A            B          C                    D

        A : Keyboard number (0-7) allocated by IOP (not Device ID).

        B : A port to which the USB keyboard is pluged.

	    The example indicates that the keyboard is pluged into 
	    the USB port 2 of the console --> port 1 of the 1st hub (branch
	    off the console) --> port 1 of the 2nd hub (branch off the 
	    1st hub)

        C : The state of LED is lit up
              bit0  :NUM LOCK
              bit1  :CAPS LOCK
              bit2  :SCROLL LOCK
              bit3  :COMPOSE
              bit4  :KANA
              bit5-7:not used

        D : The raw data sent by the USB keyboard interrupt transfer.
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
        None
