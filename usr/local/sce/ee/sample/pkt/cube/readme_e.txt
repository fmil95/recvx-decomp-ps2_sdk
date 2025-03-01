[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

Sample to Display Three-Dimensional Polygons

<Description of the Sample>	
	This is a sample program, which displays a cube by means of
  	transparency perspective conversion and rotates each of
 	the x, y and z axes independently with the controller. 
	This sample program also enables a camera to move centering 
	on the x and y axes. 

<File>	
	main.c

<Activating the Program>	
	% make		: Compiles the program.	
	% make run	: Executes the program.	

	After the compilation, the program is also executable 	
	with the following. 	
	% dsedb	
	> run main.elf

<Using the Controller>	
	Up/down directional button	: Rotates the x axis.
	Left/right directional button	: Rotates the y axis.
	L1/L2 button			: Rotates the z axis.
	R1/R2 button			: the cube moves in the z axis 
					  direction. 
	Triangle/Cross button		: the camera rotates the x axis.
	Circle/Square button 		: the camera rotates the y axis.
	START button			: Resets


