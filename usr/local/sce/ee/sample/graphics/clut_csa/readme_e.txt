[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0

                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program for controlling a load of 4-bit texture CLUT to a
temporary buffer with csa/cld field.

<Description of the Sample>
	This program is the sample program for controlling a load of 4-bit
	texture CLUT to a temporary buffer with csa/cld field.
	4-bit CLUT is loaded in a temporary buffer every 16 CLUT and 3 
	sets of the CLUTs are provided in advance.

<File>
        clut_csa.c      : Main program
        tex4.dsm        : 4-bit texture data and 48 CLUT data

<Activating the program>
        % make          : Compiles the program.
        % make run      : Executes the program.

        After the compilation, the program is also executable with the
	following.

        % dsedb
        > run clut_csa.elf

<The outline of the processing>
        The outline of the processing is as follows.

        1. Initializes a drawing environment and DMA.
        2. Loads the 4 index images into GS frame buffer. (My_indeximg4)
        3. Loads the CLUTs into the frame buffer. (My_clut4_1, 4_2, 4_3)
        4. Initializes the drawing packet.
        5. Swaps the double buffer.
        6. Sets the packet every frame.
	6-1. Sets for loading 16 4-bit CLUTs into a temporary buffer as a
	     8-bit CLUT. If they are the same as 4-bit CLUTs sets in the 
	     previous filed (If CBP is the same), they are not loaded.
	6-2. Specifies 4-bit CLUT for drawing the current frame depending 
	     on the offset values that csa field indicates without loading
	     CLUT from CLUT buffer. (0-15)
        7. Draws sprites.
        8. Returns to step 5.


