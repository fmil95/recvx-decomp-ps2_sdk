[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program to get data of the USB driver from IOP to EE

< Description of the Sample >
        In this program, the data of a USB mouse is read by EE.

        It is necessary to compile the following USB mouse sample 
        driver in IOP before executing this program.
        sce/iop/sample/usb/usbmouse

        Please plug USB mouse in either of the USB ports when executing 
	the program.

< File >
        main.c      : Sample of how to use USB mouse driver
        usbmouse.c  : Sample routine to get USB mouse data
        usbmouse.h  : Header file

< Activating the Program >
        % make          : Compiles the program
        % make run      : Executes the program

        After the compilation, the program is also executable with the
        following.

        % dsedb
        > run main.elf

< Messages >

	Messages are explained taking the following as examples.

        usbmouse3 : port=2,4 Button:00 X:  9 Y: -2 W: 12 [00 00 00]
        |_______|   |______| |_______| |_________| |___| |________|
            A           B        C          D        E       F

        A : Mouse number (0-7) assigned by IOP
            Not device ID number

        B : Port a USB mouse is plugged in
            Example B shows a mouse is plugged in port 2 of the DTL-T10000 
	    and port 4 of the HUB.

        C : Status of mouse button
            bit 0    : Button 1 (0: OFF, 1: ON)
            bit 1    : Button 2 (0: OFF, 1: ON)
            bit 2    : Button 3 (0: OFF, 1: ON)
            bits 3-7 : Device-specific

        D : USB mouse displacement
        
        E : Wheel displacement 
	    (Not displayed in the case of no wheel.)

        F : Device-dependent data 
	    (Not displayed in the case of no device-dependent data.)

<Using the Controller>
	None

<Note>
        None


