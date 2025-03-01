[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

EzBGM - BGM Playback with Disk Streaming 

< Description of the Sample >
	
	This sample allows to reproduce the sound data of the wav file 
	with streaming from a disk; therefore, two wav files can be played
	simultaneously and asynchronously.  The sound reproduction of each
	stream can be controlled by the controller.

	This sample processes streaming from the hard disk of the host.  
	When using the DTL-T10000 connected to the network, there are many
	cases that sound reproduction does not come smoothly due to the 
	data transfer delay via the network.
	The CD/DVD disk streaming is available with this sample program.

	The wav file must be 16 bits, 48 KHz and stereo or monoral data 
	in Windows format without compression.  This program switches the 
	playback mode whether it is stereo or monoral determining from the
	header information.

	The left and right channels are interleaved per sample in the stereo 
	wav data.  Since the left and right channels in SPU2 must be 
	interleaved every 512-byte, the stereo wav data must be changed to 
	the correspondent data format for the sound reproduction.  In this 
	sample, the PCM of wav data is rearranged to the PCM in the SPU2 
	format.  The load on the IOP is 2 % per file.
	
	This sample is associated with the sample of the IOP and is 
	functional in a pair.  Since the EE program loads the IOP sample 
	module automatically, build the IOP module (iop/sample/sound/ezbgm) 
	in advance. 

< File >
	main.c	
	bgm_rpc.c
	bgm_i.h
	m_stereo.wav
	ps_mono.wav

< Activating the Program >
	% make		:Compiles sample of the IOP (iop/sample/sound/ezbgm)
	% cd  ???	:Transfers to the sample directory of the EE 
			 (ee/sample/sound/ezbgm)
	% make		:Complies sample of the EE 
	% make run	:Executes the program

	The host HD plays the wav file by reading it.
	When the wav file is in the local CD/DVD drive,
	execute the following:

	% dsreset 0 0
	$ dsedb
	dsedb S> run main.elf 'cdrom0:\\M_STEREO.WAV;1' 'cdrom0:\\PS_MONO.WAV;1'

	In the dsedb command line, execute the following (when using the
	host HD):

	dsedb S> run main.elf host0:m_stereo.wav host0:ps_mono.wav

	Specify either one or two wav file(s) to the argument.

< Using the Controller >

        Circle button   :   Starts the first wav file play
        Triangle button :   Stops the first wav file play
        Cross button    :   Starts the second wav file play
        Square button   :   Stops the second wav file play
        START button    :   Suspends/Resumes the running file
	SELECT button   :   Exits the program 

< Note >
	None

