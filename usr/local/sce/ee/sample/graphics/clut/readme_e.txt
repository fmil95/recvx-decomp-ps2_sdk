[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program for displaying 4bit/8bit CLUT texture

<Description of the Sample>
        This program is for displaying 4bit/8bitCLUT texture.

<File>
        clut.c          : Main program
        tex8.dsm        : 8 bit texture data
        tex4.dsm        : 4 bit texture data

<Activating the Program>

        % make          : Compiles the program.
        % make run      : Executes the program.	
	
	After the compilation, the program is also executable with the
	following.

        % dsedb
        > run clut.elf

<Outline of the Processing>
        The outline of the processing is as follows.

        1. Initializes the settings for drawing.
        2. Loads 4-bit and 8-bit index image to the GS frame buffer.
        3. Generates the packets for odd/even fields.
        4. Sets the index array for setting 8-bit clut.
        5. Updates 4-bit and 8-bit clut based on the frame number for the 
	   clut animation and transfer to GS.
        6. Swaps the double buffer.
        7. Draws the left side sprite (8bit texture).
        8. Draws the right side sprite (4bit texture).
        9. Returns 5..

