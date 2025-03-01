[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program for MIPMAP

<Description of the Sample>
       This is the sample program for drawing using MIPMAP

<File>
        256.dsm
        128.dsm
        64.dsm
        32.dsm
        16.dsm
        8.dsm
        4.dsm           : Texture for each MIP level
        main.c          : Main function
        mathfunc.c      : Function for various numerical operations
        rect.dsm        : Packet for drawing the rectangular
        basic.vsm       : Micro code for drawing
        mathfunc.h      : Defines various parameters

<Activating the Program>

        % make          : Compiles the sample program for MIPMAP.
        % make run      : Executes the sample program for MIPMAP.	
	
	After the compilation, the program is also executable with the
	following.

        % dsedb
        > run main.elf

<Using the Controller>

        Circle button        : Stops the back and forth movement of the 
             		       rectangular

<Note>
        The outline of the processing is as follows.

        [main.c]

        1. Sets the double buffer and loads the texture in each MIP level.
	2. Sets the registers for textures (TEX0_1, TEX2_1, MIPTBP1_1 and 
	   MIPTBP2_1 etc.).
	3. Acquires the input value from the pad and set the parameters 
	   depending on the value.
        4. Sets MIPMAP on (TEX1_1).
        5. Sets the matrix.
        6. Draws the MIPMAPed rectangular (right).
        7. Sets MIPMAP off (TEX1_1).
        8. Sets the matrix.
        9. Draws the unMIPMAPed rectangular (left).
        10.Returns 3..

        The following processing is executed in the VU1 micro.

        [basic.vsm]

        1. Sets the rotational and transparent transformation matrixes.
        2. Loads various parameters (e.g. matrix) to the VU1 register.
        3. Loads the normal line vector, vertex coordinate, ST value and 
	   color.
        4. Executes the transparent transformation and calculates the 
	   light source, Q value and so on and stores the result into 
	   VU1Mem.
        5. Returns 3. and repeats the number of times of the vertexes.
        6. XGKICKs the stored data (Gif Packet) to GS.

