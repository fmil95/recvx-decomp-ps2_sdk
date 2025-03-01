[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                    All Rights Reserved
 
The sample program for saving the contents of the frame buffer

<Description of the Sample>
	This program is for saving the contents of frame buffer drawn into
	that of the host machine.
	For drawing, the sample program in "sample/vu1/deform" is used.

<File>
        capture.c       : Main program
        metal.dsm       : Texture data
        sphere.dsm      : Object program (DMA packet)
        mime.vsm        : VU1 micro program

<Activating the Program>
	% make          : Compiles the program.
        % make run      : Executes the program.

	After the compilation, the program is also executable with the
	following.	
        % dsedb
        > run capture.elf

<Using the Controller>
        Circle button   : Stop rotating with the transformation 
			  continued.
        Cross button    : Finish the program after capturing

<Outline of the Processing>
        The outline of the capture processing is as follows.
        (See the sample program of "deform" for drawing processing).

        1. Presses the cross button.
	2. Draws the next frame in another frame buffer without updating
	   the matrix for animation.
        3. Calls the local function StoreImage().
        4. Opens the file for the host.
        5. Reserves the memory for saving the contents of the frame buffer
	   of odd and even fields.
        6. Local-Host transfers the contents of the even field.
	   (sceGsExecStoreImage())
        7. Local-Host transfers the contents of the odd field.
        8. Writes the transferred image into the files on the host odd and
 	   even alternately. (sceWrite())
        9. Frees the reserved memory.
        10.Frees the file descriptor.
        11.Finishes the program.

<Note>
        The output files are raw data for 32bit-rgba.

