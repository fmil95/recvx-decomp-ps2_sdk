[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Sample of Transparency Perspective Conversion and Lighting Calculation 
Using VU1

<Description of the Sample>
	This is a sample program to perform transparency perspective 
	conversion and lighting calculation in the VU1 at a high 
	speed. 

<File>
	sample.c	: Main program
	devinit.c	: Initialization routine of device
	tex.h		: Texture data
	work.dsm	: Object data (DMA packet)
	work.vsm	: VU1 microprogram

<Activating the Program>
	% make		: Compiles the program.
	% make run	: Executes the program.
	
	After the compilation, the program is also executable 
	with the following. 
	% dsedb
	> run sample.elf


<Using the Controller>
	Up directional button	: Forward acceleration
	Down directional button	: Backward acceleration
	Select button		: Stops while pressed.

<Note>
	The outline of the processing is as follows:

	1. Prepares 2 path1 DMA packet areas (dpmem[2]).
	2. Prepares a matrix necessary for object drawing in the SPRAM
           (SetMatrix()).
	3. Transfers the matrix of 2 above to the DMA packet area 
	   (SetMatrix()).
	4. Waits for the drawing of the previous frame and the VSync 
	   (sceGsSyncPath(), sceGsSyncV()).
	5. DMA-transfers the DMA packet in ready state to path1.
	6. Returns to 2 above.




