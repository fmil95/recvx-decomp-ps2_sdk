[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Bump Mapping Sample Program   

<Description of the Sample>
       This is a sample program for the pseudo bump mapping graphics.

<File>

        grid.dsm        : Model packet of grid 
        m.vsm           : Micro code which calculates by giving the given 
			  offset by st
        m_opt.vsm       : Optimized version of m.vsm
        main.c          : Main
        mat.dsm         : Texture data of mat pattern 
        packet.dsm      : Packet for setting GS or perspective transformation
			  matrix
        textbump.dsm    : Bump texture of the character "BUMP MAP"
        wavebump.dsm    : Wave pattern bump texture 

<Activating the Program>

        % make          : Compiles the bump map sample 
        % make run      : Executes the bump map sample 

        After the compilation, the program is also executable with the 
	following:

        % dsedb
        > run main.elf

<Using the Controller>
        Directional button      : Moving the view point 
        Circle, Square button   : Changing direction of the light
        start button		: Bumping ON/OFF
        select button		: Switching the bump texture

<Note>
        The bump mapping process in this sample is as follows:
        1. Draws the original model with the patterned texture.
        2. Draws the same model from the drawn frame buffer with the bump 
	   texture, starting from the frame buffer color.  Slightly slides 
	   the texture coordinate st towards the light source from the 
	   original position.
        3. Draws the model with the bump texture without sliding st.
	   This time, color is added to the frame buffer.
           
	Light and dark appears to the original pattern as much as the st
	value is displaced in 2. and 3. which provides the bump-like 
	effect.

        The outline of the processing is as follows:

        [main.c]
        1. Resets the parameter of graphics
        2. Sets the double buffer
        3. Sets the parameter of texture 
        4. Loads the micro code and texture 
        5. Sets each parameter from the pad input
        6. Kicks Drawing DMA

        The following is processed in the VU 1 micro: 

        [m.vsm]
        1. Sets the rotation x perspective transformation matrix.
        2. Loads parameters(matrix etc.) to the VU1 register.
        3. Loads normal line vector, vertex coordinates, ST value and colors.
        4. Calculates the perspective transformation, light source calculation 
	   and Q value, then stores the result in VU1Mem.  At the same time,
	   adds the offset which is set to the st value.
        5. Returns to 3 and repeats the number of the vertex.
        6. XGKICKs the stored data (Gif Packet) to GS.

