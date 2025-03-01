[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved


Reflection Mapping/Specular Sample
	- Antialiasing by sub-pixel deviations

< Description of the Sample >
        This sample program allows the specular illumination calculation
	and the reflection mapping.

	The antialiasing is achieved when drawings have performed four
	times with each drawing is displaced by sub-pixel.
	640x224 PSMCT32


< Files >

        buff0.dsm       :Texture transfer setting packet
        buff1.dsm       :TEX0_1 register setting packet
        r.vsm           :Microcode (no optimization) for specular 
			  reflection mapping
        r_opt.vsm       :Microcode (optimization) for specular reflection
        main.c          :Main function 
        mathfunc.c      :Various numeric operations function 
        mathfunc.h      :Defines various parameters 
        mug.dsm         :Mug cup model data (Texture-Off)
        mug_tex.dsm     :Mug cup model data (Texture-On)
        packet.dsm      :Packet data to be transferred to path1/path2
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
	L1 button		: Shading display
	L2 button		: Line display
	R1 button		: Moves camera forward 
	R2 button		: Moves camera backward 

< Note >
	None

<GS buffers>
	0-69,70-139	double buffers
	140-209		z buffer
	210-279		drawing buffer (accumulation buffer)

