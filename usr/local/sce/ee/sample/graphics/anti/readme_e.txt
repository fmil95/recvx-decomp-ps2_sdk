[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved


Anti-Aliasing Sample (Sort & Polygon Clip) 

<Description of the Sample>

	This sample program is for the safe use of the GS
	anti-aliasing function.

	Anti-aliasing smoothes the edge of primitive with alpha
	blending.  The Z value rewrite of the pixel which is alpha 
	blended at the boundary of primitive is not performed (the
	z test is conducted, however).  Therefore, for an ideal 
	anti-aliasing implementation, it is necessary to have the
	drawing primitive be sorted in distant order.
	
	Practically, however, it is difficult to sort 100% the
	individual polygon.  Therefore, in this sample, the drawing
	object is divided into some portions then sorted every 
	block to be rendered.  

	Also, with the anti-aliasing, the z test is conducted up to
	the area 1 pixel outside of the primitive boundary and then
	rendering is performed (no z value write).  Therefore, when
	obtaining the z value of the pixel out of the boundary by
	interpolation, the calculation result occasionally causes
	overflow (exceeds the Zmax value).  Consequently, this pixel 
	is rendered protruding through the front polygon and unnatural 
	flicker occurs.  To be more specific, this problem occurs when the
	inclination of the polygon is high (when polygon is flat).  
	
	To avoid this problem, with this sample, the polygon with small 
	area is clip processed adopting the BackFaceClipping process
	aiming not to render polygons with high inclination.

<File>

        main.c          : Main program 
	mathfunc.h	: Header file of matrix & math operations
	mathfunc.c	: Header file of matrix & math operations
	packet.dsm	: Packet file of data used in VU1
	aa_clip.vsm	: VU1 microcode  
	cup0.dsm	: Object data (cop split 0-6) 
	cup1.dsm	:
	cup2.dsm	:
	cup3.dsm	:
	cup4.dsm	:
	cup5.dsm	:
	cup6.dsm	:

<Activating the Program>

        % make          :Compiles the program
        % make run      :Executes the program

        After compilation, executable with the following

        % dsedb
        > run main.elf

<Using the Controller>

  Directional button up/down :  Rotation around x-axis (viewpoint)
  Directional button left/right:  Rotation around y-axis (viewpoint)
  L1L2    :  Translation around z-axis  (viewpoint, back and forth)
  R1      :  Anti-aliasing OFF
  R2	  :  Clipping OFF the flat polygon


<Note>

    [Important] 

	When calculating the area of polygon with the GS coordinate
	system and clipping, care must be taken for the threshold
	computation.  For example, when the computation is perfomed with
	the fixed threshold level, the polygon disappears if the 
	viewpoint and object are located away.  To avoid this happens,
	it is necessary to controll the threshold value taking the distance
	between the polygon and viewpoint into a consideration.  The object
	becomes small inversely with the distance.  Consequently, the
	area becomes small inversely with the square, therfore, the
	desired polygon to clip is almost flat or rather straight 
	line.  The idea is that the exterior product value gradually
	decreases (be inversely proportional to the distance) linearly.
	In this sample, the clip area value is calculated according to
	the inversely proportional distance between the typical point and 
	viewpoint of the object.
	
	The outline of this process is as follows:

        ===== main.c =====
       main():

        1. Sets the double buffer
	2. Sets the alpha blending register(ALPHA1) of GS
        3. Opens the pad and reads the information from the pad
	4. Sets the matrix 
	5. Z sorts the individual block of objects  
	6. Creates the packet for VU1 transfer according to the 
	   sort result (DMAref tag swap)
	7. Calculates the clip area value and inserts it into a packet
        8. Transfers the packet to VU1 and renders
	9. Returns to 3


    VU1 Micro Code Process 

        [aa_clip.vsm]

        1. Sets various matrixes to the register VF01 - VF16
        2. Setsu various counters (strip vertex number etc.) 
        3. Sets the GIF tag load/store, store pointers of 
	   RGBAQ, ST and XYZF
        4. Perspective transforms the vertex   
        5. Calculates the luminance value from the inner product
	   of normal vector and light vector
        6. Obtains the vertex color from the luminance value and 
	   light color
        7. Calculates the exterior product from three vertexes 
	   after perspective transformation and then the area of
	   polygons
        8. When no more than a threshold value, sets XYZ3 flag and clip
        9. Loops 1-8 the number of times of vertex and XGKICK
