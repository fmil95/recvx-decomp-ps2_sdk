[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

The sample program for the texture swapping

<Description of the Sample>
	This is the sample program for drawing and swapping the texture 
	simultaneously.

<File>
        buff0.dsm       : Packet for setting the transfer status 0 of 
			  the triple buffer
        buff1.dsm       : Packet for setting the transfer status 1 of 
			  the triple buffer
        buff2.dsm       : Packet for setting the transfer status 2 of 
			  the triple buffer
        grid.dsm        : Model of the ground
        m.vsm           : Micro code for drawing: not optimized
        m_opt.vsm       : Micro code for drawing: optimized
        main.c          : Main function
        mat.dsm         : Texture of the ground
        mathfunc.c      : Various numeric operation functions
        mathfunc.h      : Definition of various parameters
        matrix.dsm      : Packet template for matrixes
        no[1-4].dsm     : Textures 1 to 4
        packet.dsm      : Packet data flown to path1/path2
        phys.c          : Function for calculating the ball position and 
			  setting into the packet
        sphere.dsm      : Model of the ball
        tex_swap.h      : Sample parameters and a prototype declaration

<Activating the program>
        % make          : Complies the sample program for the texture 
			  swapping
        % make run      : Executes the sample program for the texture
		  	  swapping

        After the compilation, the program is also executable with the
	following.

        % dsedb
        > run main.elf

<Using the controller>
	Up/Down/Left/Right directional button	 : Rotation of the viewpoint
        Start button       			 : Resets the ball

<Notes>
        The outline of the processing is as follows.

        [main.c]
        1. Sets the double buffer and initializes parameters.
        2. Sets registers associated with the texture.
        3. Swaps the double buffer.
        4. Calculates the ball coordinates and sets the value to the 
	   matrix. 
        5. Acquires the input value from the pad and sets the viewpoint 
	   depending on the value.
        6. Starts DMA ch-1 and draws.
        7. Returns to step 3.

        The following processing is performed in VU1 micro.

        [m.vsm]
        1. Sets the rotational and transparent transformation matrixes.
        2. Loads various parameters (e.g. matrix) into VU1 register.
        3. Loads the normal line vector, vertex coordinates, ST value and
	   colors.
        4. Executes the transparent transformation and calculates the 
	   light source, Q value and so on and stores the result into 
	   VU1Mem
        5. Returns to step 3 and repeats it as many as the vertexes.
        6. XGKICKs the stored data (Gif Packet) to GS.

        [packet.dsm]
	The following is about the packet flow.

        1. Loads the micro code.
        2. Sets the environment for drawing the ground and the texture
	   buffer.
        3. Transfers the texture of the ground.
        4. Draws the ground.
        5. Transfers the matrix for drawing Ball 1.
        6. Sets the buffer for transferring the texture of Ball 1.
        7. Transfers the texture of Ball 1.
        8. Draws Ball 1.
        9. Repeats step 5 to 8 as many as the balls. The texture of Ball
	   transfers 4 different types of textures in sequence. (256x256,
	   32bitRGBA)


