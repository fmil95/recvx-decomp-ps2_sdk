[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

EzMIDI - Playing Music and Producing Sound Effects with MIDI


<Description of the Sample>
	This sample allows to play music and produce sound effects by using 
	csl_hsyn, csl_midi, and csl_msin.

	This sample is associated with the sample of the IOP and is functional 
	as a set.  Since the EE program loads the IOP sample module 
	automatically, build the IOP module (iop/sample/sound/ezmidi) in 
	advance.

	libsd can be used to produce sound effects.  However, this sample 
	uses csl_hsyn.  csl_hsyn produces sound by generating midi-stream with
	csl_msin on the EE and transferring it to the IOP.

	This method has the advantages of being able to make the most of 
	JAM file and not requiring the EE to manage the SPU2 voices.  Setting 
	a maximum number of voices and their priorities to the two midi-stream 
	ports for BGM and sound effects enables voice control for guaranteed 
	maximum of n voices for sound effects and playing music with the 
	remaining (48-n) voices.    

	To minimize the communications between the EE and IOP, only when a new 
	sound effect is requested, midi-stream buffer data for sound effect 
	is transferred to the IOP once per frame.  The EE is not concerned 
	with the results, since voice control is left to csl_hsyn and the IOP 
	loops repeated at 240 MHz, several times faster than the EE loops, 
	never miss receiving data from the buffer.

	EzBGM supports split loading of BD, and can transfer large BDs to 
	the SPU2 local memory without pressing the IOP memory.

	Channel 1 of DMA is used for the BD transfer to the memory of SPU2.
	When I/O blocks are simultaneously transferred, be sure not to 
	allocate one channel to multiple block transfers.

<File>
	main.c	
	midi_rpc.c
	ezmidi_i.h
	sakana.sq
	sakana.hd
	sakana.bd
	eff.hd
	eff.bd

<Activating the Program>
	% make  	: Complies sample of the IOP (iop/sample/sound/ezmidi/)
	% cd  ???	: Moves to the sample directory of the EE
			  (ee/sample/sound/ezmidi/)
	% make		: Compiles sample of the EE
	% make run	: Executes the program

	Music is played.  Sound effects are produced by pressing directional 
	buttons.

<Using the Controller>
        Cross button			:   Resumes playing music
        Square button			:   Stops playing music
	Up directional button		:   Sound effect (Telephone)
	Right directional button	:   Sound effect (Right/Comedy)
	Left directional button   	:   Sound effect (Left/Comedy)
	Down directional button   	:   Sound effect (Glass)
        START button  			:   Suspends/Resumes playing music
	SELECT button 			:   Exits the program

<Note>
	Since MIDI message format has been created for music, it has 
	the following disadvantages when used for sound effects.

	- Generated voices are managed by channel and key numbers only.  
	  Therefore, it is impossible to continuously pressing the same key 
	  and key-off the individual voice.
	- Controls such as panpot are applied to the entire channel.

	To fix these disadvantages, an extended MIDI message has been defined
	by performing some unique extensions.  For the format, refer to 
	CSL Overview .  For APIs (e.g. sceMSIn_PutHsMsg), refer to 
	CSL MIDI Stream Generation Reference .
	
  
