[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The Sample Program for Reproducing the MIDI Sequence


<Description of the Sample>
	It reproduces the MIDI sequence on the IOP using the interim sound
	library, libspu2 and libsnd2.

	These file formats are the same as those of current PlayStation.
	Uses the current tool for generating.

<File>
	main.c	
	fuga.seq
	simple.vh
	simple.vb

<Activating the Program>
	% make		: Compiles the program.
	% make run	: Executes the program.

	The program is normal if fuga can be reproduced by the note of 
	piano.

<Using the Controller>
	None

<Note>
	None
