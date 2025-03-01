[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Sample Program to play back MPEG2 Stream with sound for "PlayStation" (PSS)


<Description of the Sample>

	mpegstr is a sample program for playing back the MPEG2 Stream (PSS) 
	with the sound for "PlayStation".  The mpegstr plays back the 
	image and sound satisfying the following requirements.

	- Image -

	    Main Profile at Main Level(MP@ML)
	    Simple Profile at Main Level(SP@ML)
	    Maximum Size             : 720x480
	    Picture Structure        : Frame structure
				       (progressive frame is preferable)
	    
	- Sound -
	    
	    Syntax                 : Straight PCM
	    Sample Length          : 16bits
	    Endian                 : Little endian
	    Sampling Rate          : 48.0KHz
	    Number of Channel      : 2
	    (L/R) Interleave Size  : 512

	mpegstr plays back the PSS file in the hard disk drive of the host
	or the CD/DVD drive of the DTL-T10000.  The device using for 
	playback can be specified by the device name (host0:/cdrom0:) at 
	the top of the file name.

	The stream converter ps2str is used for multiplexing the image
	and sound.  The stream number of both the image and sound must be
	0 in this program.  Specify 0 to the stream number for multiplexing.

	In this program, there are individual threads for reading and
	decoding.  While the main thread reads in sceRead()/sceCdStRead(),
	another thread decodes MPEG2.

	The sound and image data is stored in the EE, demultiplexed, 
	and then returned to the IOP.  This program does not check the time
	stamp of the image and sound.  Therefore, a recovery operation when
	they do not synchronize is not incorporated.  This program aligns 
	the start of the image and sound data, then starts to play back.  
	The vblank determines the playback speed of the image, and the 
	playback speed of the SPU2 determines that of the sound.  There may
	be slight difference in the speed, but it does not affect the normal
	playback.

	When pause or resume is executed, a maximum of 512 samples for
	sound (0.0107 sec at 48KHz) may be dropped out.

	In a case where the buffer underflow occurs in the middle as 
	reading or decoding cannot keep up, the decoded data up to that 
	point is played back repeatedly. It means the last decoded picture
	is displayed as an image.  As for the sound, the data left in the 
	buffer is repeatedly played back.  At this time, the sound and 
	image may not synchronize.  As long as the buffer is full,
	no more	data is read in; therefore, the buffer will not overflow.


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
	strfile.c
	strfile.h
	util.c
	vibuf.c
	vibuf.h
	videodec.c
	videodec.h
	vobuf.c
	vobuf.h

< Activating the Program >
	% make		: Compiles the program
	% make run	: Executes the program (with sound)
	% make noaudio  : Executes the program (without sound)
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
