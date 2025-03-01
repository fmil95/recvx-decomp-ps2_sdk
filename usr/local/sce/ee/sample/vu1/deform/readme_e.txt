[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Sample to Perform Interpolation between Two Figures in VU1

<Description of the Sample>

	This is a sample program, which performs transparency 
	perspective conversion, lighting calculation, and 
	interpolation between two geometric figures in the VU1. 

<File>
        deform.c	: Main program
        metal.dsm	: Texture data
        sphere.dsm	: Object data (DMA packet)
        mime.vsm	: VU1 microprogram

<Activating the Program>
	% make		: Compiles the program.
	% make run	: Executes the program.

	After the compilation, the program is also executable 
	with the following.
	% dsedb
	> run deform.elf

<Using the Controller>
        Circle button	: Stops rotation with transformation continued.

<Note>
        The outline of processing is as follows:

        1. Loads the texture.
        2. Calls the "open" function for the controller.
        3. Sets the matrix, interpolation weight, and lighting 
	   matrix in the packet to be passed to the VU1.
        4. Kicks the DMA to the VU1.
        5. Returns to 2 above.

        The following processing is performed in the VU1Micro:
        1. Sets the matrix of rotation x transparency perspective 
	   conversion.
        2. Loads various parameters (e.g. matrix) to the VU1 register.
        3. Loads a normal vector, two coordinates for interpolation,
	   an ST value, and a color.
        4. Performs the coordinate interpolation, transparency 
	   perspective conversion, and lighting calculation, 
	   and stores the results in the VU1Mem.
        5. Returns to 3 above and repeats 3 and 4 above for the number 
	   of vertices.
        6. XGKICKs the stored data (Gif Packet) to the GS.

        Double buffering in the VU1Mem is also performed.


