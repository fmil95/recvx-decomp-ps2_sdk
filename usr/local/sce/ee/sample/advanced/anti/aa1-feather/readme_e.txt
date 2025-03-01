[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

AA1 (hardware antialiasing function of GS) Sample
(Without Z sort)

< Description of the Sample >
	
	This sample program is for an antialiasing effect with the AA1 flag.
  
	To make full use of the AA1 flag, it is ideal to sort the drawing
	polygon from the back to front.  However, sorting takes a long 
	time so it is often not a practical method.

	If sorting is not available and there is a polygon that is heavily
	corrupted, enough effect is gained by setting the AA1 flag to off.

	There is a way to avoid a problem that the AA1 LINE does not write 
	any value to the Z Buffer.  Set the AA1 to off in the first time 
	then in the second attempt, draw the AA1 LINE while the Z Test is 
	being executed.

	There is another AA feature to perform an alpha blending of the 
	current frame buffer while the frame buffer contents of an 
	immediately preceding field is being bilinear filtered.


< Files >
	aa1.c		: Main program
	devinit.c	: Initialization routine of a device
	make_packet.c	: Packet generation routine
	*.h		: Texture data
	work.dsm	: Object data (DMA packet)
	work.vsm	: VU1 microprogram

< Activating the Program >
	% make		: Compiles the program.
	
	After the compilation, the program is executable with the following.

	% dsedb
	> run aa1.elf


< Using the Controller >
	Up/Down directional button   	: Switches view points
	Circle button			: Increses the number of objects
	Eks button			: Decreases the number of objects
	Triangle button			: Decreases the number of lights
					  (gets darker)
	Square button			: Increses the number of lights
					  (gets brighter)

	R1 button			: AA1 ON <-> OFF
	R2 button			: Switches an object,
					  triangle <-> line
	L1 button			: AA - blending with the picture of 
					  an immediately preceding field
	L2 button			: AA - blending with the picture of 
					  an immediately preceding field 
					  (gets slightly brighter)


< Note >
	The effect of L1/L2 button is independent of the AA1 flag function.

	To remove a side effect of the AA1 flag, the AA1 flag should be set
	off if the polygon is corrupted when looked at from the view point 
	or is reversed.  Codes for this function are included in work.vsm.
	
	
	The calculation is as follows.

	1. Calculates the vertex position in the view coordinate and 
 	   saves the result in VF05.
	2. Calculates the normal of a polygon in the view coordinate and
	   saves the result in VF06
	3. Calculates the inner product of VF05*VF06 and saves the result
	   in VF29x.
	4. Substitutes VF05x as the distance from the view point to the 
	   vertex and saves the result in VF08x.
	5. Calculates VF29x*8.0f - VF08x.
	6. Prepares a PRMODE command with AA1 flag controlled by the sign 
	7. Sends a PRMODE command per vertex drawing.

	Set the AA1 flag to off in the case of the cosine of the angle 
	between the normal vector of a polygon and the eye to object 
	direction is below 1/8.
