[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

The Sample Program for the Spotlight

< Description of the Sample >

	This sample allows to demonstrate the spotlight.  Generally,
 	the spotlight indicates the directional point light source. 
	Usually, the spotlight is designed not to diffuse light, it gives 
	the feeling that the degree of the luminous intensity attenuation 
	is small.  It is recommended to adjust the light attenuation function 
	according to the use.  In this sample, the light attenuates inversely
	proportional to the distance.  This sample supports three spotlights.

< File >

        main.c          : Main program
        main.h          : Prototype declaration of the main program
        defines.h       : Prototype declaration used for various .c files
	vu1pack.c       : Data packet generation function programs for the
			  VU1 processing
	tex256.dsm      : Texture data file 
        cube.dsm        : Object data (cube)
	torus.dsm       : Object data (torus 1024polygon)
	grid200.dsm     : Object data (plane 200polygon)
	grid1800.dsm    : Object data (concave/convex plane 1800polygon)
	spot.dsm        : Packet for the VU1 micro code transfer
	sp3.vsm         : VU1 micro code (inversely proportional to
			  the distance)
	sp3_opt.vsm     : VU1 micro code (optimization version)
	basic.vsm       : VU1 micro code (for cube rendering)

< Activating the Program >

        % make          : Compiles the program
        % make run      : Executes the program

        After the compilation, the program is also executable with the 
	following.

        % dsedb
        > run main.elf

< Using the Controller >

        Up/Down directional button    : Operating the red spotlight direction 
					(back and forth) 
        Right/Left directional button : Operating the red spotlight direction 
					(side to side)
        L1/L2 button 		: Operating the blue and green spotlight 
				  direction (up and down)
        Triangle/Cross button 	: Operating the red spotlight source
				  (back and forth)
        Square/Circle button	: Operating the red spotlight source
				  (side to side)
        R1/R2 button		: Operating the red spotlight source
				  (up and down)
        SELECT button 		: Switching the model (plane, concave/convex
				  plane)

< Note >

    Overview of the calculation method

	The light source coordinate and the light beam vector (light 
	directivity) are necessary for calculating the spotlight.
	With the inner product (cos(p)) value of the light beam vector and 
	the vector between the light source and the vertex, the light beam 
	directional intensity is calculated.  The functions for this
	calculation are as follows;

	Vertex_point(VP) : Polygon vertex coordinate 
	Vertex_color(VC) : Polygon vertex color (before color calculation)
	Vertex_normal(VN): Polygon vertex normal vector 
	Light_point(LP)  : Light source coordinate 
	Light_color(LC)  : Light source color
	Light_vector(LV) : Light beam vector
	Intensity(I)     : Intensity of the light beam directivity (extent) 
	a, b, c          : Coefficient
	.               : Inner product
	*                : Product

	Intensity(I) = (cos(p)^2 - a) * b ;
	     : cos(p) = (VP-LP).LV / |VP-LP|  : |LV| = 1
	     a and b must be the constant which meets with (1 - a) * b = 1
		

	By taking the graphical form, it is obvious this function is with the
	value in center, that is to say the value whose direction of 
	directivity is high.  It is recommended to use the (a, b) in the
	range of approximately (0.8, 5) to (0.99, 100).

	The principal cause for using this function is that it is less 
	computation.  Since the square of cos(p) is used, it is not 
	necessary to get the root for normalization. Therefore, the optimized
	computation of micro code is 43 instructions per vertex, and the 
	computational speed is approximately more than twice faster than that 
	of the normal parallel light source micro code.

	Vertex color equation
	  (RGB) = I * (VP-LP)/|VP-LP|.VN * c/|VP-LP| * LC * VC
	      = I * c/|VP-LP|^2 * ((VP-LP).VN) * LC * VC

	
	1/|VP-LP|^2 is used in I, the computational result of 1/|VP-LP|^2
	can be effectively used twice in the final equation.

	Note that the normalization is necessary in each step of the actual 
	calculation.


    < Remarks > 

 	1. The object is normally defined in the local coordinates.
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
	   carried out each polygon vertex.  The spotlight has 
	   directivity so the light and shade boundary is created between
	   the light extent and its outer area.  With linear interpolating
	   (gouraud shading) between the vertices of polygons, the clear 
	   rendering cannot be realized.  Especially, this problem is 
	   prominent when the polygon is coarse.
	   Keep in mind about the resolution of polygons in order to 
	   retain a constant quality in the spotlight calculation.
