[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved


Antialiasing by PCRTC Merge Circuit Sample

<Description of the Sample>

This program is for an antialiasing effect with the PCRTC merge circuit.

The drawing area of a 640x448 VAG level in a dual buffer is used and the 
odd and even horizontal scanning lines are balanced via the merge circuit 
of PCRTC to be output. (1:1 blend)

To realize a 640 x 448 dual buffering, which accepts the program display 
area update to exceed 1/60 sec, the object is splited and the texture for
drawing is transferred to a common texture work area as necessary.


< Files >
	main.c		: Main program
	dma.c		: DMA control routine
	gscntrl.c	: GS control routine
	matrix.c	: Matrix control routine
	packet.c	: Packet generation & Link routine
	fractalobj.c	: Fractal display object generation routine
	base.h		: Setting parameter, data structure etc.
	vu1mpg.dsm	: VU1 microprogram packet
	vu1mpg.vsm	: VU1 microprogram

< Activating the Program >
	% make		: Compiles the program

	After the compilation, the program is executable with the following.

	% dsedb
	> run main.elf

<Using the Controller>
	Circle button		: Controls the position of the planet
	Square button		: Controls the position of the planet
	Triangle button		: Controls the position of the planet
	Eks  button		: Controls the position of the planet
	R1 button		: Controls the position of the planet
	R2 button		: Controls the position of the planet
	START button		: Resets the position of the planet
				  (home position)
	Up directional button	: Controls the eye position
	Down directional button	: Controls the eye position
	Right directional button: Controls the eye position
	Left directional button	: Controls the eye position
	L1 button		: Controls the eye position
	L2 button		: Controls the eye position
	SELECT button		: Resets the eye position (home position)
	Right analog stick	: Rotates the planet
	R3 button		: Resets the rotation of the planet and 
				  switches the AA1 line mesh 
				  display/non-display	
	Left analog stick	: Rotates the light source
	L3 button 		: Resets the rotation of the light source 
				  and switches the AA by PCRTC on/off


< Note >
	The AA effect is hardly recognized without displaying the AA1 
	line mesh.

< Explanation of the Program > 
	Refer to the pcrtc-blend.ppt document.
