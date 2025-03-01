[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
            Copyright (C) 1999, 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

MIDI Playback with Hardware Synthesizer

<Description of the Sample>
	This sample program is for the MIDI sequencer(modmidi) and the
	hardware synthesizer(modhsyn).  This is the simplest sample for
	the use of csl module.

	The sample reads two set of sq file (MIDI sequence) and bank
	binary file (tone) to play sounds.  These files can be created
	with SMF2SQ and JAM provided by SCE.

	libsd.irx of the IOP must be running.  
	The loading status is displayed on dsicons.  
	Check the dsicons whether to these are loaded if not functioning well.

	
<File>
	main.c	
	sakana.sq
	sakana.hd
	sakana.bd
	overload.sq
	overload.hd
	overload.bd

<Activating the Program>
	% make		:Compiles the program
	% make run	:Executes the program

	The program works good if two songs are played at the same
	time.

<Using the Controller>
	None

<Note>
	None
