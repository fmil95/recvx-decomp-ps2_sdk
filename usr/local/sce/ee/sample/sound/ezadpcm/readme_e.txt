[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

EzADPCM - BGM Playback with ADPCM Data Streaming from the Disk

< Description of the Sample >

	This sample allows to playback the sound data of the .VB (ADPCM) 
	file with streaming from a disk. Two VB files can be played back 
	simultaneously to compose the stereo BGM.

	This sample processes streaming from the hard disk of the host or 
	the local CD/DVD disk.  When using the DTL-T10000 connected to the
	network, it may occasionally fail to playback smoothly due to the 
	data transfer delay via the network.
	
	The data is a .VB (ADPCM) file containing only a waveform data.
	Top and end of the data must be in a loop.  The file size must be 
	an integral multiple of the buffer size set in the SPU2 local 
	memory.

	This sample corresponds to the sample of the IOP and is functional
	in a pair.  Since the EE program loads the IOP sample module
	automatically, execute the make command of the IOP module
	(iop/sample/sound/ezadpcm) in advance. 

< File >
	main.c	
	rpc.c
	ezadpcm.h

< Activating the Program >
	% make		: Compiles sample of the IOP 
			  (iop/sample/sound/ezadpcm/)
	% cd ???	: Transfer to the EE sample directory
		   	  (ee/sample/sound/ezadpcm/)
	% make		: Complies sample of the EE 
	% make run	: Executes the program

	After the execution of "make run", the .VB file 
	(ee/sample/spu2/rstream/tr1l(r)_pad.vb in the sample) are read 
	from the hard disk of the host by pressing the START button and 
	then the ADPCM data is played back.

	If the .VB file exists in a local CD/DVD drive, take the following
	procedures.

	% dsreset 0 0
	% dsedb
	dsedb S> run main.elf 'cdrom0:\\TR1L_PAD.VB;1' 'cdrom0:\\TR1R_PAD.VB;1'

	With the dsedb command line, take the following procedures.
	(in a case of the hard disk of the host)
	
	dsedb S> run main.elf host0:tr1l_pad.vb host0:tr1r_pad.vb

	The data must specify two .VB (ADPCM) files as the argument.

< Using the Controller >
        Up directional button    :   Volume up
        Down directional button  :   Volume down
        START button 		 :   Running file start(resume)/stop 
	SELECT button		 :   Program termination

< Note >
	None

