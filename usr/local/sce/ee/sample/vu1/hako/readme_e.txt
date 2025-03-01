[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Sample of Transparency Perspective Conversion Using VU1

<Description of the Sample>
	This is a sample program, which only performs transparency 
	perspective conversion in the VU1.

<File>
	sample.c	Main program
	work.dsm	Object data (DMA packet)
	work.vsm	VU1 microprogram


<Activating the Program>
	% make		: Compiles the program.
	% make run	: Executes the program.

	After the compilation, the program is also executable 
	with the following.
	% dsedb
	> run sample.elf

<Using the Controller>
	Not defined.

<Note>
	The outline of the processing is as follows:

	1. Prepares the rotation matrix for the object desired 
	   to be displayed. (RotMatirx())
	2. Rewrites the rotation matrix for the DMA packet (work.dsm). 
	   (SetMatrix())
	3. DMA-transfers the DMA packet in ready state to path1.
	4. Waits for the drawing of the previous frame and the VSync. 
	   (sceGsSyncPath(), sceGsSyncV())
	5. Returns to 2 above.

	Note that "work.vsm" is not optimized because of readability.

 

