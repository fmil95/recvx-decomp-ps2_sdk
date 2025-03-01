[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program for a stream reproduction by the voice

<Description of the Sample>
	The program is for a stream reproduction by the voice of SPU2 from 
	EE using the interim sound library, librspu2.

<File>
	main.c	
	tr1l_pad.vb
	tr1r_pad.vb

<Activating the Program>
	% make		: Compiles the program.
	% make run	: Executes the program.
	
	After the compilation, the program is also executable with the
	following.

	% dsedb
	> run main.elf

	The program is normal if the music can be reproduced 
	stereophonically.

<Using the Controller>
	None

<Note>
	None
