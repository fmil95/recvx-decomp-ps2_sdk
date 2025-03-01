[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                 Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                    All Rights Reserved

The sample program for clipping (Backface&Viewvolume) (VU1)

<Description of the Sample>

	This is a sample program for the backface clipping and view-volume
	clipping of a polygon with VU1.	The sample program is for both 
	Triangle and Triangle strip. The processing is executed by the 
	polygon. Scissoring is not implemented.	Therefore, polygon will 
	lack if it protrudes a clipping area of at least one vertex.
	White lines on the screen show the clipping area, which can be 
	operated by the controller.
	Originally, the purpose of this area is that a polygon is not
	protruded this area by setting it to +/- 2048, the same number
	as the clipping area of GS. And the program does not display the 
	polygon at the back side of the viewpoint.

<File>

        main.c          : Main program
	main.h          : Prototype declaration in the main program
	defines.h	: Prototype declaration used in each *.c file
        vu1pack.c       : Function for generating the packet for the Vu1 
			  transfer
        cube.dsm        : Packet(data) file(cube) for the Vu1 transfer
        torus.dsm       : Packet(data) file(torus) for the Vu1 transfer
	basic.dsm       : File loading the Vu1 micro code
	basic.vsm       : Micro code for drawing without clipping
	clip.dsm        : File loading the Vu1 micro code
        clip.vsm        : Micro code for drawing implementing Backface
			  &View-volume Clipping
        clip_opt.vsm    : Optimized version of clip.vsm
        tex128.dsm      : Texture data
        tex64.dsm	: Texture data

<Activating the Program>

        % make          :Compiles the program.
        % make run      :Executes the program.	
	
	After the compilation, the program is also executable with the 
	following.

        % dsedb
        > run main.elf

<Using the Controller>

	Up/Down directional button	:Rotation around x-axis
				         (viewpoint) 
  	Left/Right directional button   :Rotation around y-axis
					 (viewpoint)	
 	L1L2    :       Translation around z-axis 
			(viewpoint, back and forth)
        Triangle/Cross button :  Modification of a clipping area 
				 (y direction)
        Square/Circle button  :  Modification of a clipping area 
				 (x direction)
        R1R2   	:   Modification of a clipping area (xy direction)
        select button    :Model switching (cube, torus)
	start button	 :ON/OFF switching in the clipping processing

<Note>

        The outline of the processing is as follows.

        ===== main.c =====
        main():

        1. Sets the double buffer and loads the texture
        2. Opens the pad and transfers the micro code
        3. Sets the blending coefficient and initializes the noise 
	   distribution for transformation
        4. Reads the information from the pad
	5. Generates the packet for the VU1 transfer
        6. Transfers the packet to VU1 and draws
	7. Returns 4.

	SetViewScreenClipMatrix():
	It calculates the matrix (view_clip) for projecting each vertex 
	to a clip coordinate system, as well as the matrix(view_screen)
	for the transparent transformation.
	A clip coordinate system is the coordinate system for projecting
	a view volume to the cube whose diagonal value is 
	(-1.0, -1.0, -1.0) and (1.0, 1.0, 1.0), which enables to clip the
	coordinate after projecting in each axis within the range in which
	the absolute value is 1.0. 

	SetGifClipLine():
	The function to generate the GIF packet for drawing the boundary 
	in the clipping area. It executes a transparent transformation 
	directly with Vu0 and generates the packet data for GIF here.

        ===== vu1packet.c =====
	The function group for generating the packet for the Vu1 transfer

	Vu1DmaPacket structure:
	The structure for generating and managing the packet for 
	transferring to Vu1.
	It manages each object data collectively and generates the 
	packet that can minimize the number of times for DMAKICKing.
	And the packet data are designed for operating with the double 
	buffer.

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

        ==== clip.vsm(VU1 micro code) ====

        [clip.vsm]

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
	7. Sets the flag (VI01) by projecting the vertex to the clip 
	   coordinate. (X,Y and Z all is within absolute value 1.0
	   range. The last three vertexes are judged by CLIP instruction.)
	8. Executes BackfaceClip and sets the flag.(VI12)  
	   (The last three coordinates for the transparent transformation 
	    is managed in VF22 and VF23. Calculates the normal vector of 
	    the surface and judges about positive or negative. In 
	    TriangleStrip, since positive and negative are detected 
	    alternately, the detection is taken place with the sign 
	    reversing.)
	9. Calculates summation of two flags and branches. Determines 
	   either XYZ2 or XYZ3 is kicked.
	10.Calculates the color of vertex from the normal line vector and
	   the inner product.	
	11. Loops the number of times of vertexes. (Returns 5.)
	12. Loops the number of times of the object blocks.
	    (Returns 4. Switches the buffer switch)
	13. Loops the number of times of the object (Returns 2.)

