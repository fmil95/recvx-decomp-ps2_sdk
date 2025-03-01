[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved


One Path Reflection Mapping/Specular Sample
(with AA1 line antialiasing - single drawing)


< Description of the Sample >

        This sample program is for the specular illumination calculation
	and the reflection mapping.  The antialiasing by AA1 lines has
	been newly added.

< File >

        buff0.dsm       :Texture transfer setting packet
        buff1.dsm       :TEX0_1 register setting packet
        r.vsm           :Microcode (no optimization) for specular 
			 reflection mapping
        r_opt.vsm       :Microcode (optimization) for specular reflection
        main.c          :Main function 
        mathfunc.c      :Various numeric operations function 
        mathfunc.h      :Defines various parameters 
	add_object.c	:Object Generation
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


	Up directional button	: Rotates the camera upward
	Down directional button	: Rotates the camera downward
	Right directional button: Rotates the camera rightward
	Left directional button	: Rotates the camera leftward
	Triangle button		: Rotates the light upward
	Eks button		: Rotates the light downward
	Square button		: Rotates the light rightward
	Circle button		: Rotates the light leftward
	L1 button		: Draw AA1 lines <-> not
	L2 button		: Texture Function Toggle
				  Modulate -> Decal -> Highlight 
				  -> Highlight2
	R1 button		: Moves camera forward 
	R2 button		: Moves camera backward 
	START button		: Texture mapping ON <-> OFF
	SELECT button		: Draw only AA1 lines <-> draw with shading

< Note >
	Antialiasing by drawing AA1 lines over a reflected object.
	AA1 lines are not sorted, so sometimes some artifacts appear
	but seemed to be negligible.

	With refmap-AAline, the reflection mapping is redrawn 
	to make a reflection, however, with this sample, a single 
	drawing makes the similar reflection by using the highlight 
	of the texture function.  Therefore, better antialiasing effect
	is realized.
