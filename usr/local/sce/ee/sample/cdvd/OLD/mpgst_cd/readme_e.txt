[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Sample Program to play back MPEG2 Stream with sound for "PlayStation" (PSS)
 - Specification of Streaming from CD/DVD Drive 


<Description of the Sample>
	This is a sample program to playback the MPEG2 Stream (PSS) 
	with the sound for "PlayStation".  The mpgst_cd plays back 
	the image and sound satisfying the following requirements.

	- Image -

	    Main Profile at Main Level(MP@ML)
	    Simple Profile at Main Level(SP@ML)
	    Maximum Size             : 720x480
	    Picture Structure        : Frame structure 
				       (progressive frame is preferable)
	    
	- Sound -
	    
	    Form                      : Straight PCM
	    Sample Length             : 16 bits
	    Endian                    : Little endian
	    Sampling Rate             : 48.0 KHz
	    Number of Channel         : 2
	    (L/R) Interleave Size     : 512

	The stream converter ps2str is used for multiplexing the image
	and sound.  The stream number of both the image and sound 
	played back by this program must be 0.  Specify 0 to the stream
	number for multiplexing.
	
	In this program, the thread for reading is different from that for 
	decoding.  While the main thread reads in sceRead(), another thread
	decodes MPEG2.

	The sound and image data is stored in the EE, demultiplexed, 
	and then returned to the IOP.  This program does not check the time
	stamp of the image and sound.  Therefore, a recovery operation when
	they do not synchronize is not incorporated.  This program aligns 
	the start of the image and sound data, then starts to play back.  
	The vblank determines the playback speed of the image, and the 
	playback speed of the SPU2 determines that of the sound.  There may
	be slight difference in the speed, but it is not a problem in the 
	normal playback.
	
	In a case where the underflow of the buffer in the process occurs 
	as reading or decoding cannot keep up, the decoded data up to that
	time is played back repeatedly. It means the last decoded picture 
	is displayed as an image. As for the sound, the data left in the 
	buffer is repeatedly played back.  At this time, the sound and 
	image may not synchronize.  As long as the buffer is full, no more
	data is read in; therefore, the buffer will not overflow.
	
	The function sceMpegGetPicture() uses SPR(Scratch Pad RAM) 
	internally.  Therefore, if using SPR in the other function, 
	the SPR must not be used while the sceMpegGetPicture() is
	performed.

< File >
	audiodec.c
	audiodec.h
	defs.h
	disp.c
	disp.h
	main.c
	read.c
	readbuf.c
	readbuf.h
	util.c
	vibuf.c
	vibuf.h
	videodec.c
	videodec.h
	vobuf.c
	vobuf.h

< Activating the Program >

	% make  	: Compiles the program

        Load the CD/DVD-ROM containing the MPG file into the local 
	CD/DVD drive.  The program can be executed from the console 
	following the steps below.

        % dsreset 0 0
        $ dsedb
        dsedb S> run main.elf '\\MPG file name;1'

< Using the Controller >

	Circle button: Plays back from the beginning 
	Cross button : Terminates playback

< Note >
	(a) The ps2str ver1.00 supports multiplexing the image encoded
	    in the constant bit rate (CBR) only.  The image encoded in
	    the variable bit rate (VBR) cannot be multiplexed 
	    appropriately.  VBR will be supported in the future release.
