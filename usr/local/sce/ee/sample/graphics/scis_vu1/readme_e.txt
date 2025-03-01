[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Scissoring Sample Program (VU1 2nd Version)

<Description of the Sample>
	This sample program allows to cut the polygon at the clipping 
	boundary by using VU1 enabling the new polygon which does not 
	protrude the boundary is generated and displayed.  
	The sample program is for both Triangle and Triangle strip.  
	The yellow line on the screen describes	the clipping area 
	which can be operated by the controller. 
	Normaly, this area should be set to the frame of (x, y)=(+/-2048) 
	to refrain the polygon from protruding the clipping area of GS.  
	Since the Z buffer effective range (near surface and far surface 
	of the z direction) should also be defined, cuts the polygon with 
	care for not to	protrude this range.  The object which is cut 
	front plane has a section view.  Generally, scissoring is used on 
	purpose of preventing the polygon close to the viewpoint from 
	displaying in the unnaturally chipped state. 
       
	(eg.) ground, road, wall

        VU1's performance has been highly improved in comparison with CORE&VU0.
	The VU1code size has also become much larger.  Since the function 
	branch 	by stack saving of the register value is processed in this
	version, this may be utilized as an example of the large-scale 
	VU1 program.  
	
<File>

        main.c          : Main program
        main.h          : Prototype declaration in the main program
        defines.h       : Prototype declaration used in each *.c file
        cube.dsm        : Object data (cube)
        torus51.dsm     : Object data (doughnut)
	grid1800.dsm    : Object data (concavo-convex plane)
        tex128.dsm      : Texture data
	scissor.dsm     : Packet for VU1 micro code transfer
	scissor.vsm     : VU1 micro code 
	sci_opt.vsm     : VU1 micro code (optimized)
	sci_tri.vsm     : VU1 micro code (specifically for the optimized
			  Triangle Model) 
	sci_str.vsm	: VU1 micro code (specifically for the optimized
			  Strip Model)
	vu1pack.c	: Packet-making function for VU1 transfer

<Activating the Program>

        % make          : Compiles the program
        % make run      : Executes the program

        After the compilation, the program is also executable with the 
	following:

        % dsedb
        > run main.elf

<Using the Controller>

        Up/Down directional button    : Rotation around x-axis (viewpoint) 
        Left/Right directional button : Rotation around y-axis (viewpoint)
	L1L2    :       Translation around z-axis  (viewpoint, back and forth)
        Triangle/Cross button: Modification of a clipping area (y direction)
        Square/Circle button :  Modification of a clipping area (x direction)
        R1R2    :       Modification of a clipping area (xy direction)
        SELECT button     : Model switching (cube, torus)
        START button      : Switching the scissoring polygon display
                            (TriangleFan or LineStrip)

<Note>

Improvement in 2nd Version

In the previous version, slight gaps were generated between scissoring 
polygons due to a computational error.  
In this version, a method of avoiding this problem has been adopted.  
A code which slightly magnifies scissoring polygons has been inserted 
immediately after the PUSHTRIANGLE: label in the source, scissor. vsm.  
Note that an adjustment of the magnification rate 1.01 might be required 
depending on the usage.       

     [Important] 
	1. As to associating the boundary surface of nearZ and farZ with 
	the Zbuffer value in the view volume, there are a few points 
	attention should be paid.  Normally, the arithmetic operations are
	performed in floating-point numbers in both nearZ surface and farZ
	surface.
	However, since Zbuffer value is represented in integer, nearZ 
	surface and farZ surface is changed to be the integer 
	representation by specific instruction such as a FTOI instructon.  
	Generally, association is made using the marginal value so that 
	the desired z value in integer representation is obtained.  

	For example, in the case of 24bits Zbuffer, setting is made
	(16777215.0->FTOI->0xffffff) so that the point on the nearZ 
	surface be 16777215.0 (eg.Z=1.0) by the floating-point operation.

        However, the fixed-point part is 23 bits in the 32bits 
	floating-point number representation, and reliable significant 
	digits will be approximately 6 to 7 digits at the most.  
	Therefore, the result of the 1.0->16777215.0 operation may largely
	exceed 16777215.0 due to the calculational error.  In such a case,
	the Zbuffer value falls in error which results in zero.  To make 
	sure to avoid this happens, it is effective to set the 
	corresponding value of the nearZ surface to 16777000.0 by 
	sacrifysing the Zbuffer width.  With the program like this sample,
	if it is critical to the nearZ surface, the extra attention should
	be paid to the calculation precision of the Z value in the Zbuffer
       
	2. To calculate the cross section, obtains the point of 
	intersection of each edges of polygon and the cut surface.
	At the same time, the vertex information (vertex,normal,color,
	textureUV) needs to be interpolated.  Especially, attention should
	be paid to the normal interpolation. 
	
	If polygon is displayed with gouraud shading, the color of the 
	point on the edge of the polygon is obtained by the endpoint which
	is linear interpolation of the vertex color.  Therefore, if normal
	is linear interpolated without careful consideration, the correct 
	lighting color cannot be obtained.

	To avoid this problem, linear interpolate the color to be passed 
	to GS, after calculating the lighting color of the vertex.  
	Consequently, the interpolation necessary elements are (vertex, 
	GScolor, textureUV).

    Outline of the Procedure

        The new polygon which is generated by cutting generates and draws 
	by TriangleFan on the assumption that it is convex polygon.
        
	The triangle polygon consists of 3 edges.  If this is cut by a 
	surface,each two edges have a point of intersection, and a new 
	polygon can be created by connecting these two points of 
	intersection.  The new polygon 	is displayable by TriangleFan.  
	Then, cuts this polygon by another surface.  Now, this polygon has
	two points of intersection of the two edges which can create 
	another new polygon by being connected.  If the	cutting process is
	executed recursively, a polygon which fits in the desired area 
	(within a cross section group) is eventually generated.
        
	With this sample, the cutting process is executed in each of the 
	6 clip 	surfaces (+x,-x, +y,-y, +z,-z) to generate the final 
	scissoring polygon.

     (Reference) Computer Graphics --princeples and practice--
                            SECOND EDITION
                 authors:  James Foley, Andries van Dam,
                         Steven Feiner, John Hughes
                 publisher: ADDISON-WESLEY PUBLISHING COMPANEY 
             
                 Section 3.14 Clipping Polygons(pp124-127)

The outline of the processing is as follows.

        ===== main.c =====
        main():

        1. Sets the double buffer and loads the texture
        2. Opens the pad and transfers the micro code
        3. Reads the information from the pad
	4. Generates the packet for the VU1 transfer
        5. Transfers the packet to VU1 and draws
	6. Returns to 3.

	
	SetViewScreenClipMatrix():
	It calculates the matrix (view_clip) for projecting each vertex 
	to a clip coordinate system, as well as the matrix(view_screen)
	for the transparent transformation.  It also calculates the 
	matrix for the transformation from the clip coordinate to the 
	screen coordinate.

	SetGifClipLine():
	The function to generate the GIF packet for drawing the boundary 
	in the clipping area. It executes a transparent transformation 
	directly with Vu0 and generates the packet data for GIF here.

        ===== vu1packet.c =====
	The function group for generating the packet for the Vu1 transfer

	Vu1DmaPacket structure:
	The structure for generating and managing the packet for 
	transferring to Vu1.
	
	InitVu1DmaPacket():
	The function for initializing Vu1DmaPacket structure. Two pointers
	to a management table (packet) is passed as arguments for double
	buffering.
	
	GetVu1ObjHead():
	It returns the first address of the packet currently enabled.

	AddVu1ObjPacket():
	It registers additionally the object data (already converted to 
	the packet format for VU1) compiled with *.dsm file and presented 
	in the memory. 

	CloseVu1ObjPacket():
	It terminates the packet currently enabled and switches the double
	buffer. It can be used to DMAKICK because it returns the address
	of the packet available for the returned value.

	GetVu1EnvHead():
	It returns the pointer to the structure for presetting the matrix
	data to VU1.

	CloseVu1EnvPacket():
	It closes the matrix data (packet) used in VU1 and returns the 
	pointer to the packet.

       ==== scissor.vsm(VU1 micro code) ====

        [scissor.vsm]
	
	1. Sets various matrixes to register VF01 to VF16 and 
	   precalculates.
	2. Sets the flag for judging the drawing attribute (Triangle or 
	   TriangleStrip).
	3. Sets each counter (e.g. the number of strip vertex).
	4. Loads and stores GIF tag and sets the store pointer for RGBAZ,
	   ST and XYZF. 
	5. Executes the transparent transformation of the vertex.
	6. Calculates the inner product between the normal line vector and
	   light vector.
	7. Obtains the vertex color from the inner product of the normal 
	   line vector.
	8. Calculats the texture coordinates (S,T,Q).
        9. Sets the flag (VI01) by projecting the vertex to the clip 
	   coordinate. (X,Y and Z all is within absolute value |W|
	   range. The last three vertexes are judged by CLIP instruction.)
	10. Branches to the scissoring routine (S1) if the applicable 
	    polygon is overstriding the cut surface.
        11. Determines either XYZ2 or XYZ3 is kicked.  Stores. 
        12. Loops the number of times of vertexes. (Returns to 5)
	13. XGKICK,GSrendering
        14. Loops the number of times of the object blocks.
	    (Returns to 4.  Switches the buffer switch)

	S1. Saves VF register and VI register to the stuck buffer.
	S2. Passes the recived polygon (considered as TriangleFan) to the
	    cutting routine (T1) and branches.
        S3. Repeats until processing of the 6 cut surfaces (+x,-x,+y,-y,
	    +z,-z) are completed.
	S4. Perspective transforms the final TriangleFan and XGKICK 
	S5. Returns to the main routine 11.

        T1. Traces each vertex and determines if any points of 
	    intersection between the cut surfaces exists. 
	T2. Registers the end information in the next TriangleFan list 
	    from the edge state. 
            - Do not register if the endpoint A and B of the edge are outer.
            - Registers if both endpoints of A and B of the edges are inner.
            - If the endpoint A of the edge is outer and the endpoint B is
	      inner, registers the point of intersection.
            - If the endpoint A of the edge is inner and the endpoint B is
	      outer, registers the two points of intersection of A.
        T3. Repeats until the edge is one-cycled and returns to S2 when 
	    the new TriangleFan list is created. 

