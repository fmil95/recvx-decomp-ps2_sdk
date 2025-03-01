[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved


Antialiasing by AA1 Line Redrawing (Without Z sort) Sample

<Description of the Sample>

	This program is for an antialiasing effect by the AA1 line redrawing.  

	The antialiasing effect is gained by redrawing the AA1 line 
	on the edge of a polygon.  With this sample, the line data is 
	generated for redrawings as well as the two-dimensional mesh data.
	

	With this feature the aliasing of the shadow figure part and a T-shape
	gap created at the transition point of the mesh LOD are reduced.


< Files >
	
	Makefile	: Make file
	clip.c		: Clip routine
	devinit.c	: Initialization routine of a device
	dma_util.c	: DMA routine
	gterrain.c	: Geographic feature generation routine 
	gterrain_packet.c  : Geographic feature packet generation routine
	main.c		: Main program
	matrix_util.c	: Matrix calculation routine
	misc_packet.c	: Other packet generation routine
	random.c	: Random number routine
	texload.c	: Texture loading routine 
	view_util.c	: Routine of matrix calculation from the view point
	terrain.h	: Structure 
	constant.h	: Constant 
	scale.h		: Scale dependent constant  
	testconst.h	: Constant for VU1 microprogram 
	*.raw		: Texture file
	work.dsm	: DMA packet
	fsub.vsm	: VU1 microprogram 


< Activating the Program >
	% make		: Compiles the program.
	
	After the compilation, the program is executable with the following. 
	% dsedb
	> run aa1.elf


<Using the Controller>
	Up directional button	: Move forward
	Down directional button	: Move backward
	Right directional button: Move right
	Left directional button	: Move left
	Circle button		: Rotate right
	Square button		: Rotate left
	Triangle button		: Rotate down
	Eks button		: Rotate up

	R1 button		: Move away
	R2 button		: Approach
	L1 button		: Move up
	L2 button		: Move down

	SELECT button +
	R1 button		: AA1 lines only
	L1 button		: Shading polygons + AA1 lines
	L2 button		: Shading polygons only
	R2 button		: Shading polygons + AA1 lines


< Description of the Program > 
	Refer to the mountain.ppt file for the program memory mapping etc.

