[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

Refrection Mapping Specular Sample Program


< Description of the Sample >
        This sample program allows to perform the illumination calculation
	with specularity and the reflection mapping.

< File >

        buff0.dsm       :Texture transfer setting packet
        buff1.dsm       :TEX0_1 register setting packet
        r.vsm           :Microcode(no optimization) for specular 
			  reflection mapping
        r_opt.vsm       :Microcode(optimization) for specularl reflection
        main.c          :Main function 
        mathfunc.c      :Various numeric operations function 
        mathfunc.h      :Defines various parameters 
        mug.dsm         :Mug cup model data (Texture-Off)
        mug_tex.dsm     :Mug cup model data (Texture-On)
        packet.dsm      :Packet data to be circulated to path1/path2
        matrix.c        :Various matrices setting function
        reftex.dsm      :Texture for the reflection mapping

< Activating the Program >

        % make          :Compiles the program
        % make run      :Executes the program 

        After compilation, the program is executable with the following:

        % dsedb
        > run main.elf

< Using the Controller >
	Up, Down, Left, 	: Up and down, right and left 
	Right button		  rotation of the camera
				
	Triangle, Circle, 	: Up and down, right and left rotation
	Cross, Square button	  of the light
				   

< Note >
	None

