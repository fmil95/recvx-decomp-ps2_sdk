[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Sample Program to decode the MPEG2 Stream for PlayStation (PSS) 
and Color Space Conversion with the VU1


< Description of the Sample >

	mpegvu1 is a sample program for playing back the MPEG2 stream 
	for the PlayStation (PSS). The mpegvu1 converts the color 
	space (YCbCr->RGB) using the VU1 instead of the IPU.  

	The IPU color space conversion function treats the values of 
	adjacent four points of different color components as the same.
	Since these values in the progressive frame have been sampled 
	simultaneously in both vertical and horizontal directions, there 
	should not be problems in using this method.

	However, when the picture in the interlace frame or the field 
	structure is displayed, fields sampled at different time are 
	located alternately in the vertical direction.  If a similar 
	color space conversion is carried out, the color may slightly 
	blur, or the picture may have a streak.

	The mpegvu1 is a sample program which realizes the high-precision
	color space conversion with the VU1 instead of the IPU.  
	When decoding the MPEG2 file, using this method will bring a
	better-quality image, if the VU1 has space for it.

	The mpegvu1 plays back images satisfying the following
	requirements.

	- Image -

	    Main Profile at Main Level(MP@ML)
	    Simple Profile at Main Level(SP@ML)
	    Maximum Size               :720x480

	In the mpegvu1, the picture structure is categorized as follows.
	The color space is converted using different VU1 micro codes.

	    (1) progressive frame structure
	    (2) interlace frame structure
	    (3) field structure
	
	After the decoding process, which of the above three applies to
	the bit stream can be determined by referring to the member of
	the structure, sceMpeg. 
	(refer to the part of the csct calculation in videodec.c)
	
	mpegvu1 plays back the PSS file in the hard disk drive of the host
	or the CD/DVD drive of the DTL-T10000.  The device using for 
	playback can be specified by the device name (host0:/cdrom0:) at 
	the top of the file name.

	When pause or resume is executed, a maximum of 512 samples for
	sound (0.0107 sec at 48KHz) may be dropped out.

	This program is structured in the same way as a sample program, 
        mpegstr.  For details, refer to the mpegstr.
	    
< File >
	audiodec.c
	audiodec.h
	cscvu1.c
	cscvu1.h
	defs.h
	disp.c
	disp.h
	main.c
	read.c
	readbuf.c
	readbuf.h
	strfile.c
	strfile.h
	util.c
	vibuf.c
	vibuf.h
	videodec.c
	videodec.h
	vobuf.c
	vobuf.h
	vu1.dsm
	yuvfl.dsm
	yuvfl.vsm
	yuvfrfl.dsm
	yuvfrfl0.vsm
	yuvfrfl1.vsm
	yuvprg.dsm
	yuvprg0.vsm
	yuvprg1.vsm

< Activating the Program >

	% make		: Compiles the program
	% make run	: Executes the program (with sound)
	% make noaudio	: Executes the program (without sound)
	% make help     : Displays the help information 
			  (File name specification procedure can be found)

	If the PSS file exists in the hard disk drive of the host, the 
	following steps can be taken.

	% dsreset 0 0
	% dsedb
	dsedb S> run main.elf 'host0:XXX.pss'


	If the PSS file exists in the CD/DVD drive of the DTL-T10000, the 
	following steps can be taken.

	% dsreset 0 0
	% dsedb
	dsedb S> run main.elf 'cdrom0:\\XXX.PSS;1'

< Using the Controller >

	Circle button: Plays back from the beginning 
	Cross button : Terminates playback
	Square button: pause/resume

< Notes >

	(a) When the streaming from the host is processed using the 
	    DTL-T10000, the playback may become unstable due to the 
	    network load.
	    When processing the streaming via network, note the following:
		Connects with 100Base-T
		Avoid connection via hub etc.

	(b) The sample data image in /usr/local/sce/data/movie/sample.pss 
	    is partly unstable.
