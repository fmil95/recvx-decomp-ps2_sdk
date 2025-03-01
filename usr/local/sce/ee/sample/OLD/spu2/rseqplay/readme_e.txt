[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program for reproducing MIDI sequence

<Description of the Sample>
	It reproduces MIDI sequence from EE using the interim sound 
	library, librspu2.

	Specifies SEQ, vh and vb files in SEQ_FILENAME, VH_FILENAME and 
	VB_FILENAME respectively in the define line of main.c. 

	These file formats are the same as those of current PlayStation.
	Uses the current tool for generating.

<File>
	main.c	
	fuga.seq
	simple.vh
	simple.vb

<Activating the Program>
	% make		: Compiles the program.
	% make run	: Complies the program.
	
	After the compilation, the program is also executable with the 
	following.

	% dsedb
	> run main.elf

	The program is normal if fuga can be reproduced by the note of 
	piano.


<Using the Controller>
	None

<Note>
	None
