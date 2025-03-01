[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                 Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                    All Rights Reserved

Clipping sample program (VU0)

<Description of the Sample>

	This program allows to display twenty-seven cubes, clip by 
	the object with the display area using the boundary box and clip
	by the polygon with the drawing area of GS.
	This program sets the clipping area smaller than displaying 
	one so that you can understand the clipping in the boundary box
	easily.
	Displaying the far or near clip side by alpha blending and 
	switching it are also possible.

<File>
        main.c          Main program
        vu0.c      	Matrix operation functions for a coordinate and 
			transparent transformation etc. 
        cube.s          Object data (cube)
        torus1.s        Object data (torus, data divided)
        flower.dsm   	Texture data

<Activating the Program>
	% make		: Compiles the program.
	% make run	: Executes the program.
	
	After the compilation, the program is also executable with the 
	following.

	% dsedb
	> run main.elf

<Using the Controller>
	Up/Down directional button	: Rotation around x-axis (object)
	Left/Right directional button	: Rotation around y-axis (object)
	L1L2				: Rotation around z-axis (object)

	Triangle/Cross button	: Rotation around x-axis (viewpoint)	
	Square/Circle button	: Rotation around y-axis (viewpoint)
	R1R2			: Translation around z-axis (viewpoint)
	Left analog stick	: Translation around x and y axis 
				  (viewpoint)

	SELECT			: Switching of the object (CUBE/TORUS)
	START			: On/off switching of the object clip
	Left analog stick	: On/off switching of the clip with the GS
				  drawing area by pushing the left analog stick
				  (*1)
	Right analog stick	: Near/Far switching of the object clip side 
				  by pushing the right analog stick

<Note>
	For this sample program, execute it with the ANALOG mode button 
	on.

	(*1) Protrude the object from the screen widely, or go beyond 
	the drawing area of GS using the R2 button with the object clip 
	off. If the left analog stick is switched to on, protruded polygon
	is clipped.
	
