[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Sample Program for the MPEG2 Stream Reproduction Using the IPU


<Description of the Sample>
	This sample program allows to reproduce the MPEG2(including MPEG1)
	stream using the IPU.  The reproducible MPEG2 stream is limited to
	the following streams with the frame structured picture.

	    Main Profile at Main Level(MP@ML)
	    Simple Profile at Main Level(SP@ML)

	This program currently does not support for reproducing the field 
	structured animation and the stream with the MPEG2 system information.  
	This sample program firstly reads the MPEG2 data into the memory,
	then sends the data to the IPU using the sceMpegAddBs() function
	to decodes the picture one by one.  The result is expanded to the 
	main memory, so DMA transfer it to the GS to be displayed on a 
	screen.
	
	The frame structured picture has the even and odd fields in one frame
	which are arrenged in staggered.  Since each field is switched at
	1/60 sec, the picture needs to be switched at 1/30 sec.  
	Therefore,it is specified in the program that the picture is 
	switched every 2 vblank.

	The function sceMpegGetPicture() uses the SPR(Scratch Pad RAM)
	internally.  Therefore, when using the SPR, it must be released
	while sceMpegGetPicture() is executed. 

<File>
        ezmeg.h
        display.c
        ldimage.c
        main.c
	ez.m2v

<Activating the Program>
	% make		:Compiles the program 
	% make run	:Exetutes the program

	After the compilation, the program is also executable with the 
	following:

 	% dsedb
	> run ezmpeg.elf ez.m2v

<Using the Controller>
	None

<Note>
	None
