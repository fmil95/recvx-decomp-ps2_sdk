[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

MIDI Playback with Software Synthesizer


<Description of the Sample>
	This is a sample program for the MIDI sequencer (modmidi), 
	software synthesizer (modhsyn) and EE line out (liblout).

	This sample is associated with the sample of the IOP and is 
	functional in a pair.  Since the EE program loads the IOP sample 
	module automatically, build the IOP module (iop/sample/sound/sqsoft) 
	in advance.  

	The phoneme data for the software synthesizer uses data/sound/sce.ssb.
	This is an evaluation version and will be refined in future.

	The GM or GS compliant songs can be played as they are.  However, 
	since the program does not have a concept of "drum part", it is
	necessary to set BankChange(MSB) = 120 to the channel of the
	drum part.  For details, refer to the readme for phoneme data.
	
	The program operation is as follows:

	* The EE program starts, and loads necessary modules.

	* The EE program is initialized.
 
	* The EE loads and activates the IOP sample module.  
	  Then, the EE enters into a state of waiting for IOP data.

	* The IOP loads sq and starts sequence. AS the Stream MIDI, the 
	  output from modmidi is input to modssyn, it is transferred to the 
	  EE with time code added.  (Data is in the original format.)  

	* When MIDI data is received, the EE performs operations 
	  based on the data and outputs it as Stream-PCM data.

	* The Stream-PCM data is input to liblout and then transferred to 
	  the I/O block of the SPU2.  The libsdr feature is used internally. 


<File>
	main.c	

<Activating the Program>
	% make		:Compiles sample of the IOP (iop/sample/sound/sqsoft)
	% cd  ???	:Transfers to the sample directory of the EE 
			 (ee/sample/sound/sqsoft)
	% make		:Complies sample of the EE 
	% make run	:Executes the program

	The operation is normal if a song is played.

<Using the Controller>
	None

