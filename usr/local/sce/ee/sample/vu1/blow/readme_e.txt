[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program for particle drawing using VU1

<Description of the Sample>
        This is a sample program, which draws with motion blur from 
	three-dimensional coordinates given in VU1 to particles. 
	The sample includes the blow and gas. Both are drawn using the
	same micro code.
<File>
        blow.c		: Main program
	blow.h		: Parameters for setting the particles for the 
			  blow
        data.dsm	: Particles and data packet of the reflection of 
			  the particles for the blow
	data_gas.dsm	: Particles and data packet of the reflection of 
			  the particles for the gas	
        draw.vsm	: Micro code to draw the ground
        firebit.dsm	: Texture of the particles
        fireref.dsm	: Texture of reflection of particles
	gas.h		: Parameters for setting particles for the gas
	gas_tex.dsm	: Texture of the gas
        grid.dsm	: Data packet of the ground
        p.vsm		: Micro code to draw particles
	p_opt.vsm	: Optimized version of p.vsm
        p2.vsm		: Micro code to draw reflection of particles
        part.dat	: Data string of particles
        part1.dsm	: Part.dat enclosed in VIFtag
        part10.dsm	: 10 "part1.dsm" files collected
	physics.c	: Function to calculate the trace of particles
        spart.dat	: Data string of reflection of particles
        spart1.dsm	: Spart.dat enclosed in VIFtag
        spart10.dsm	: 10 "spart1.dsm" files collected
        src.dsm		: Area to preserve the speed vector and 
			  color of particles
        srcpart.dat	: Data string included in src.dsm
        wood.dsm	: Texture of the ground

<Activating the program>
	% make		: Compiles the program for the blow.
	% make run	: Executes the program for the blow.
	
	% make -f Makefile.gas		:Complies the program for the gas.
	% make -f Makefile.gas run	:Executes the program for the gas.

	After compilation, the program is also executable with the 
	following.

	% dsedb
	> run blow.elf

<Using the Controller>
        Directional buttons	: Moves perspective.
        Circle button		: Stops moving particles.

<Note>
 	The outline of the processing is as follows:

        [blow.c]
        1. Sets double buffer and loads textures.
        2. Initializes parameters and particles.
        3. Sets transparency perspective conversion matrix and offset 
	   vector perpendicular to the line of sight (for particle 
	   expansion) within the packet.
        4. Draws the ground.
        5. Calculates the coordinates of the particles and  reflection of 
	   particles.  (SetParticlePosition for blow and gas)
        6. Draws reflection of particles.
        7. Draws particles.
        8. Returns to 3 above.

        The following processing is performed in the VU1Micro. 

        [draw.vsm]
        1. Sets the matrix of rotation x transparency perspective 
	   conversion.
        2. Loads various parameters (e.g. matrix) to the VU1 register.
        3. Loads the normal vector, vertex coordinates, ST value, 
	   and color.
        4. Performs transparency perspective conversion and lighting 
	   calculation and stores the results in the VU1Mem.
        5. Returns to 3 above and repeats 3 and 4 above for the number 
	   of vertices.
        6. XGKICKs the stored data (Gif Packet) to the GS.
           
        [p.vsm]
        1. Loads various parameters (offset, transparency perspective 
	   conversion matrix and ST, etc.) to the VU1 register. 
	2. Loads coordinates, speed vector, and color.
        3. Relocates the coordinates via the speed vector (for motion 
	   blur).
        4. Performs transparency perspective conversion to the 2 
	   points shown with the coordinates gained from 3 above and 
	   the coordinates gained by adding the offset to them.
        5. Creates a quadrangle (TriangleStrip with four points) whose 
	   diagonal line is a line segment connecting the 2 points in 4 
	   above.
        6. Stores the corresponding ST and color in the VU1Mem as a 
	   result of 5 above.
	7. Returns to 2 above and repeats 2 to 6 above for the number 
	   of particles.
        8. XGKICKs the stored data (Gif Packet).

        [p2.vsm]
 	This becomes a p.vsm file without motion blur.  It also performs 
	double buffering in the VU1Mem. 
	
	Note that p.vsm is not optimized.. 

