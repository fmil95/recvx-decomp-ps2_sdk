[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
		  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

The Sample Program for the Point Lighting

< Description of the Sample >

	This sample allows to process the point lighting.
	There are two types of the light sources in this sample.  
	The point lighting generally attenuates in liminous intensity 
	inversely proportional to the square of the distance.  However,
	the lighting which attenuates in liminous intensity inversely 
	proportional to the distance is also available in this sample 
	for a visual effect.
        

< File >

        main.c          : Main program
        main.h          : Prototype declaration of a main program
        defines.h       : Prototype declaration used for various .c files 
	vu1pack.c       : Data packet generation function programs for the 
			  VU1 processing
	tex256.dsm      : Texture data file 
        cube.dsm        : Object data (cube)
        torus.dsm       : Object data (torus)
	grid200.dsm     : Object data (plane 200polygon)
	grid1800.dsm    : Object data (concave/convex plane 1800polygon)
	plight.dsm      : Packet for the VU1 micro code transfer 
	pl.vsm          : VU1 micro code (inversely proportional to the
			  distance)
	pl_opt.vsm      : VU1 micro code (optimization version)
	pl2.vsm         : VU1 micro code (inversely proportional to the
			  square of the distance)
	pl_opt2.vsm     : VU1 micro code (optimization version of pl2.vsm)
	basic.vsm       : VU1 micro code (for cube rendering)


< Activating the Program >

        % make          : Compiles the program
        % make run      : Executes the program

        After the compilation, the program is also executable with the 
	following.

        % dsedb
        > run main.elf

< Using the Controller >

        Up/Down directional button 	: Operating the red light source
				    	 (back and forth)
        Right/Left directional button 	: Operating the red light source
					 (side to side)
        L1/L2 button		: Operating the red light source (up and down)
        Triangle/Cross button	: Operating the blue light source 
				  (back and forth)
        Square/Circle button	: Operating the blue light source (side to side)        R1/R2 button		: Operating the blue light source (up and down)
        SELECT button 		: Switching the model (plane, concave/convex
				  plane)
        START button		: Switching the attenuation function (in file 
				  pl.vsm/pl2.vsm) of the liminous intensity
		      
< Note >

    Overview of the calculation method

	For the color calculation of each vertex, get the inner product of
	the normal line vector between the light source and vertices to 
	determine the intensity of diffused reflective light.  The distance 
	from the light source also has the effect on the luminance intensity.  
	The equation is briefly described in the following;
	
	Vertex_point(VP) : Polygon vertex coordinate
	Vertex_color(VC) : Polygon vertex color (before color calculation)
	Vertex_normal(VN): Polygon vertex normal line vector
	Light_point(LP)  : Light source coordinate
	Light_color(LC)  : Light source color
	coef             : Coefficient
	.                : Inner product
	*                : Products between each vector elements
	
	Attenuates inversely proportional to the distance 
	  (RGB) = ( ((VP-LP)/|VP-LP|.VN * coef/|VP-LP|) * LC ) * VC
	      = coef/|VP-LP|^2 * ((VP-LP).VN) * LC * VC
	
	Attenuates inversely proportional to the square of the distance
 	  (RGB) = ( ((VP-LP)/|VP-LP|.VN * coef/|VP-LP|^2) * LC ) * VC
	      = coef/|VP-LP|^3 * ((VP-LP).VN) * LC * VC

	Note that the normalization is necessary in each step of
	the actual calculation.


    < Remarks > 

	1. The object is normally defined in the local coordinates
	   On the contrary, the point light source coordinate 
	   is defined in the world coordinate in most cases.
	   Since the data to be transferred to VU1 is from the object to 
	   the local coordinate, the vector cannot be obtained between the 
	   point light source and vertices in the world coordinate.

	There are two ways to avoid this problem.
	
	The first method is to divide the conversion matrix of Local to
	Screen into two rows, Local to World, World to Screen during 
	perspective conversion, and obtain the vector during the 
	process.  However, this requires the VU1 micro code an extra
	calculation; therefore, this method is not recommended.

	The second method is to convert the point light source 
	into World to Local in advance, then deliver the point light
	source to VU1.  This method is used in this sample since it does 
	not put the calculation load on VU1.
		

	2. The color calculation using the point light source needs to be
	   carried out in each polygon vertex.  Generally, the luminous
	   intensity is propotional to the reciprocal of the (square of) 
	   the distance. For this reason, the accurate color pixel cannot 
	   be calculated by linear interpolating (gouraud shading) the 
	   color between the vertices of polygons.  Especially, this problem
	   is prominent when the polygon is coarse.  Keep in mind about the 
	   resolution of the polygon in order to retain a constant quality 
	   in the point light source calculation.

