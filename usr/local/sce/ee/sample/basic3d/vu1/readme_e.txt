[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Basic sample programs for the basic3d VU1 version 
==================================================

Aim in the basic3d VU1 version
--------------------------------------------------------------------------
These sample programs show the examples of the process for the transparent
transformation and packet generation implemented in EECore and VU0 with
the VU1 micro mode for fast processing. They also implement the double 
buffer processing of VUMem1 and provide the basics for realize the
faster geometry processing.
 

Description for the program
--------------------------------------------------------------------------
<Files>
	main.c		Main program
	mathfunc.c	Matrix operation functions for a transparent
			transformation etc. (CPUcore version)
	mathfunc.h	Header file
	cube.dsm	Object data (Cube)(VU1 packet format)
	torus.dsm	Object data (Torus/data divided)
			(VU1 packet format)
	torus1.dsm	Object data (Torus/data divided)
			(VU1 packet format)
	flower.dsm	Texture data
	basic.vsm	Execution instruction file in the VU1 micro mode
	vu1dump.c	Dump function in the local memory

<Compilation>
	% make

<Execution>
	% make run

	After the compilation, the program is also executable with the
	following.

	% dsedb
	> run main.elf

<Specification>
	Display a cube of a Triangle Strip with texture 
	Display a torus of a Triangle with texture
	(Switching with the select button)

	24-bit Z buffer
	3 light sources
	32-bit texture

<Controller operation>
	Up/Down directional button	:Rotation around x-axis (object)
	Right/Left directional button	:Rotation around y-axis (object)
	L1L2 button			:Rotation around z-axis (object)
        Square/Circle button		:Rotation around y-axis of the 
					 camera toward the world 
					 coordinate system
        Triangle/Cross button		:Rotation around x-axis of the 
					 camera toward the world 
					 coordinate system
        R1R2 button			:Forward and backward rotation of 
					 the camera
        select button			:Object switching

<Outline of the processing>
	1.Load texture
	2.Open the pad
	3.Set the matrix and light source matrix in the packet passed to
	  the VU1
	4.DMA kick to the GS
	5.Return 2.


	The following processing are executed in the VU1 micro program.
	1. Set the transparent transformation matrix for rotation x.
	2. Load some parameters (e.g. matrix) into the VU1 register.
	3. Load the normal line vector, two coordinates interpolated, the 
	   value of ST and the color.
	4. Calculate the coordinate interpolation, transparent 
	   transformation and light source and store the result in VU1mem.
	5. Return 3. Repeat the process the vertex number of times.
	6. XGKICK the stored data (Gif Packet) to GS

	Double buffering is executed in VU1Mem.


Data flow
--------------------------------------------------------------------------
<Initializing process>
Main RAM                                GS built-in RAM(4M)
  Texture data -----------------------> Expansion of the texture data 
  Expansion of the object data
  Expansion of the transparent transformation matrix etc.

<Execution process>
Main RAM                              VUMem1(16K)
  Object data -----------------------> Expansion of the packet data for 
  (Various matrixes    DMA transfer    the GS transfer(Geometry operation 
  and data before     	               and double buffer)
  geometry processing)	               (Primitive data(STQ,RGBA,XYZF))
 		  	                                |
                                                        |
					        	|
   					 	        |
GS built-in RAM(4M)    <--------------------------------+
 Writing to the frame and Z buffer            Data kick to GIF
 (Rendering)


Detail description of the program
--------------------------------------------------------------------------
<Variable/Structure/Function> (the function of mathfunc.c is excluded.)

        My_dma_start            :The leading element of the packet group 
				 sent to the VU1
        My_dma_next             :The second leading element of the packet 
				 sent to the VU1
        My_matrix               :The local_screen matrix in the packet
        My_rot_trans_matrix     :The local_world matrix in the packet
        My_light_matrix         :The local_light matrix in the packet
        My_cube_start           :The leading element of the cube packet 
				 data
        My_torus_start          :The leading element of the torus packet 
				 data
        My_texture1             :The head of the texture data

   	TexEnv          : The structure for generating the packet to 
			  transfer the sceGsTexEnv structure

        camera_p        : The position of the camera
        camera_zd       : The eye direction vector of the camera
        camera_yd       : The downward direction vector of the camera
        camera_rot      : The vector for rotation of the camera

        light0          : Light 0
        light1          : Light 1
        light2          : Light 2
        color0          : The color of the light 0
        color1          : The color of the light 1
        color2          : The color of the light 2

	ambient		: Ambient light

        obj_trans       : Translation vector of the object
        obj_rot         : Rotation vector of the object

        local_world     : The matrix converting the object Local 
			  coordinate into the World coordinate system
        world_view      : The matrix converting the World coordinate 
			  system into the View coordinate system
        view_screen     : The matrix converting the View coordinate system
	                  into the Screen coordinate system
        local_screen    : The matrix converting the Local coordinate into 
			  the Screen coordinate (Product of above 3 
			  matrixes)

        normal_light    : The matrix calculating the inner product between
             		  a normal line of the vertex and the light 
			  direction vector in the World coordinate system
                          (The light direction vector will be set).
	local_light     : The matrix that allows to calculate the inner 
			  product between the normal line of the vertex 
			  and the light direction vector in the Local 
			  coordinate system by multiplying the normal_
			  light matrix by the local_world matrix.
       	light_color     : The matrix produced by multiply-accumulating the
	                  inner product, generated by multiplying the 
			  local_light matrix by the normal line of the 
			  vertex, 
			  by each color of light (3 colors plus ambient 
			  light) 
         local_color     : The matrix generated by multiplying local_light
			   by light_color

        work            : The matrix for temporary work

        sceGsDBuff      db      : The structure for implementing the 
				  double buffer
	scePadData	pad	: The structure for reading the pad data
        sceGsLoadImage  gs_limage:The structure for transferring the 
                                  texture into the GS local memory
        TexEnv          texenv  :The structure generating the packet for 
                                 transferring the texture data

        void SetVu1PacketMatrix(void)
                        :The function setting the matrix in the packet 
			 sent to VU1

<Variables in the main function>

        fd              : The file descriptor receiving a return value 
			  when the pad is opened
        frame           : The switch for flipping the double buffer
	delta		: The variable for setting the angle of rotation
        obj_switch      : The toggle switch for switching the object
        toggle          : The variable for controlling the reading of the 
			  select button with the toggle

<The variables and functions provided with mathfunc.c> 

	sceSamp0IVECTOR[4]      : The int type array located in 128-bit 
				  boundary (element 4)
        sceSamp0FVECTOR[4]      : The float type array located in 128-bit 
				  boundary (element 4)
        sceSamp0FMATRIX[4][4]   : The float type array located in 128-bit
				  boundary (element 4x4)

See mathfunc.c for the detail description of the function. Most of the 
functions written in mathfunc.c are written by the Vu0 macro and are 
provided with libvu0.
In the VU1 version, the sceSamp0... function executed in the Core is used 
for a matrix calculation. The processing will be faster by converting the 
function as the VU0 version.
Note that the array type of the matrix is different between the Core 
version and VU0/VU1 versions. The Core version uses a general array, but
the VU0/VU1 versions adopt the transposed type to make use of the 
arithmetic function effectively.


Description of the programs (for initializing)
---------------------------------------------------------------------------
Description of the programs (for initializing)

	In the VU1 version, the object data is linked when compiling and 
	expanded to the memory. The file of *.dsm format is a packet 
	format that can be sent to VU1 directly. It can use the My_dma_
	start pointer as it is.

- Initialize the device
    sceGsResetPath();//Initialize the data path device (VIF1,VU1 and GIF)

	In the head of the program, all devices are to used are 
	initialized. Initialization is a must because if using a device 
	without it, unexpected results will occur.

- Set the drawing environment
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
        //Initialize the GS
    sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, 
             SCREEN_HEIGHT, SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);
        //Set the double buffer
    *(u_long *)&db.clear0.rgbaq = SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10,
                                                0x80, 0x3f800000);
    *(u_long *)&db.clear1.rgbaq = SCE_GS_SET_RGBAQ(0x10, 0x10, 0x10,
                                                0x80, 0x3f800000);

	The program sets the drawing environment for the GS after 
	initializing each device. Displaying on a screen, as well as the 
	setting for the controller which is synchronized using the VSync 
	is not allowed without this setting. Never forget the setting at 
	least once immediately after the initialization of the device.

- Image transfer for the texture
    sceGsSetDefLoadImage(&pTex,13340,IMAGE_SIZE / 64,SCE_GS_PSMCT32,
                        0, 0,IMAGE_SIZE, IMAGE_SIZE);
                        //Set LoadImage information
    FlushCache(0);
    sceGsExecLoadImage(&pTex, (u_long128 *)My_texture1);
                        //Execute LoadImage
    sceGsSyncPath(0, 0);//Wait for the data transfer to finish 

	The function sets LoadImage information to transfer the texture 
	image into the GS local memory. Then, by executing the 
	sceGsExecLoadImage(), specified image will be transferred to the
	GS local memory. Because this function uses the DMA internally, 
	the D cache is consistent with the main memory using the 
	FlushCache(0) before transferring. Finally, the function waits for
	the data transfer to finish using the sceGsSyncPath().

- Generate the packet for setting the environment for the texture
    FlushCache(0);
    // --- set texture env 1 ---
    sceGsSetDefTexEnv(&texenv.gs_tex, 0, 13340, 
                        IMAGE_SIZE / 64, SCE_GS_PSMCT32, 
                          8, 8, 0, 0, 0, 0, 1);
    SCE_GIF_CLEAR_TAG(&texenv.giftag); 
    texenv.giftag.NLOOP = 9;
    texenv.giftag.EOP = 1;
    texenv.giftag.NREG = 1;
    texenv.giftag.REGS0 = 0xe;
    sceGsSetDefAlphaEnv(&texenv.gs_alpha, 0);
    *(u_long *) &texenv.gs_alpha.alpha1 
                = SCE_GS_SET_ALPHA(0, 2, 0, 1, 0);
    *(u_long *) &texenv.gs_test 
                =  SCE_GS_SET_TEST(0, 0, 0, 0, 0, 0, 3, 0x3);
    texenv.gs_test1addr = (long)SCE_GS_TEST_1;
    *(u_long*)&texenv.gs_tex.clamp1 
                = SCE_GS_SET_CLAMP(0, 0, 0, 0, 0, 0); 

	The function generates the packet for setting the GS general 
	purpose	register TEX0_1 and CLAMP_1 related to the texture using 
	the sceGsSetDefTexEnv().

- Transfer the packet for setting the environment for the texture
    FlushCache(0);
    sceGsPutDrawEnv(&texenv.giftag);  //Transfer the data for setting the 
					texture
    sceGsSyncPath(0, 0);

	The program transfers the generated packet for setting the texture
	environment by executing the sceGsPutDrawEnv() function.

- Initialize the controller
    fd = open("pad:0", 0);

	Initialize the controller for 1p side. Then, the controller can 
	read the data to the specified buffer using the read(). The data
	of the controller is updated every VSync operation.

Description of the programs (for the main loop)
--------------------------------------------------------------------------
Acquiring the controller information, generating the matrix and packet
(a coordinate and transparent transformation, UV->STQ transformation and 
light source calculation) executed in the main loop 
will be described in the following section.

- Generate the View-Screen matrix
    sceSamp0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
                2048.0f, 2048.0f,1.0f, 16777215.0f,1.0f,65536.0f);

	The function calculates the View-Screen matrix using the 
	sceSamp0ViewScreenMatrix(). In this example, the distance from the
	viewpoint to the projection screen is 512, the aspect ratio of the
	screen is 1:0.47, the central point of the screen is (2048,2048) 
	in the GdPrimitive coordinate system, the value of the z buffer is
	1.0-16777215.0 and the value of z is 1.0-65536.0.

- Acquire the controller information
    // --- read pad ---
    read(fd, &pad, sizeof(pad));

    // --- object rotate & change view point ---
    if(pad.button & SCE_PADLdown){ 
                obj_rot[0] += deltta;
                if(obj_rot[0]>PI) obj_rot[0] -= 2.0f*PI; }
    if(pad.button & SCE_PADLup){ 
                obj_rot[0] -= delta;
                if(obj_rot[0]<-PI) obj_rot[0] += 2.0f*PI; }
    if(pad.button & SCE_PADLright){ 
                obj_rot[1] += delta;
                if(obj_rot[1]>PI) obj_rot[1] -= 2.0f*PI; }
    if(pad.button & SCE_PADLleft){ 
                obj_rot[1] -= delta;
                if(obj_rot[1]<-PI) obj_rot[1] += 2.0f*PI; }
    if(pad.button & SCE_PADL1){
                obj_rot[2] += delta;
                if(obj_rot[2]>PI) obj_rot[2] -= 2.0f*PI; }
    if(pad.button & SCE_PADL2){
                obj_rot[2] -= delta;
                if(obj_rot[2]<-PI) obj_rot[2] += 2.0f*PI; }
    if(pad.button & SCE_PADRdown){
                camera_rot[0] += delta;
                if(camera_rot[0]>PI*0.4) camera_rot[0] = 0.4f*PI; }
    if(pad.button & SCE_PADRup){
                camera_rot[0] -= delta;
                if(camera_rot[0]<-PI*0.4) camera_rot[0] = -0.4f*PI; }
    if(pad.button & SCE_PADRright){
                camera_rot[1] += delta; 
                if(camera_rot[1]>PI) camera_rot[1] -= 2.0f*PI; }
    if(pad.button & SCE_PADRleft){
                camera_rot[1] -= delta;
                if(camera_rot[1]<PI) camera_rot[1] += 2.0f*PI; }
    if(pad.button & SCE_PADR1){
                camera_p[2] -= delta*5;
                if(camera_p[2]<-100) camera_p[2] = -100; }
    if(pad.button & SCE_PADR2){
                camera_p[2] += delta*5;
                if(camera_p[2]>-10) camera_p[2] = -10; }
    if(!toggle && (pad.button & SCE_PADselect)){
         obj_switch = (++obj_switch)&0x01; toggle = 1;
    }else if(!(pad.button & SCE_PADselect)){toggle = 0;}

	The controller button information is acquired using the read(). In
	this example, the object, the angle of rotation for both sides of 
	the camera and the shift amount are updated based on the acquired 
	data. A left-hand side button for the controller is for the object
	operation while a right-hand side button for that is for the 
	camera operation. The select button is for object switching.

- Generate the Local-World matrix (rotation only)
    sceSamp0UnitMatrix(work);                   //Unit matrix 
    sceSamp0RotMatrix(local_world, work, rot);  //Rotating matrix 

	The Local-World matrix should be calculated for every object held 
	in the Local coordinate system. The affine transformation, such as
	the rotational and parallel translation, is used. In this sample 
	program, the rotating and parallel translation matrix is 
	calculated and the result from multiplying these two matrixes is 
	used as the Local-World matrix.
	In this sample program, however, the Local_Light matrix should be 
	set before setting the parallel translation (obj_trans) of the 
	third line. If the Local_Light matrix is generated using the 
	matrix including the parallel translation, the ambient light is 
	not correctly calculated. This is because the parallel translation
	and the ambient light are calculated using the forth column of the
	matrix. Therefore, generate the Local_Light matrix immediately 
	after setting the rotating matrix.
	Note that the forth element of the obj_trans should be set to 0.
	Otherwise, very critical value set in [4,4] element of the matrix
	will change.

- Generate the Local_Light matrix
    sceSamp0NormalLightMatrix( normal_light, light0,light1,light2);
    sceSamp0MulMatrix(local_light, normal_light,local_world);

	The function calculates the Normal-Light matrix from the three 
	light sources using the sceSamp0NormalLightMatrix(). Further, the 
	matrix is multiplied by the Local-World matrix to generate the 
	Local-Light matrix. As mentioned above, in the Local-World matrix 
	used here, do not set the obj_trans.

- Generate the Light_Color matrix
   sceSamp0LightColorMatrix(light_color, color0, color1, color2, ambient);
	The function calculates the Light_Color matrix from the light 
	source and environmental color using the 
	sceSamp0LightColorMatrix(). In the light source calculation, the 
	final vertex color on the screen is determined by multiplying the 
	Light_Color matrix after each lighting effect color is calculated.

- Generate the World-View matrix
    sceSamp0RotCameraMatrix(world_view, camera_p, camera_zd, camera_yd,
                                                         camera_rot);

	The function sceSamp0RotCameraMatrix() calls 
	sceSamp0CameraMatrix() internally. The function calculates 
	World-View matrix using the sceSamp0CameraMatrix(). The View 
	coordinate system defines the viewpoint (eye) as the origin point 
	(0,0,0), the eye direction as Z+, the rightward direction as X+ 
	and the downward direction as Y+ (right hand coordinate system).
	In this sample, it defines the viewpoint camera_p as (0,0,-25),
	viewpoint camera_zd as (0,0,1) and vertical camera_yd as (0,1,0)
	toward the World and generates the World-View matrix using these 
	vectors. The sceSamp0RotCameraMatrix() function internally rotates
	these vectors using the camera_rot in advance. The rotation of the
	camera is enabled by passing the result to the 
	sceSamp0CameraMatrix().

- Generate the Local-World matrix (rotational and parallel translation)
    sceSamp0TransMatrix(local_world, local_world, obj_trans);
        //Rotational and parallel translation

	As the generation of the Local_Light matrix is finished, the real 
	Local_World matrix is generated by adding the Local_World matrix 
	to the parallel translation elements (obj_trans).

- Generate the Local-Screen matrix
    sceSamp0MulMatrix(world_screen, view_screen, world_view);
    sceSamp0MulMatrix(local_screen, world_screen, local_world);

	First, calculate the World-Screen matrix using the View-Screen
	and the World-View matrix already calculated. The Local-Screen 
	matrix is calculated by multiplying the World-Screen matrix by the
	Local-World matrix. In the Local_World matrix used at this point,
	the elements of the rotation and the parallel translation should 
	be properly set.

- Set the value in the packet transferred to VUMem1
    SetVu1PacketMatrix();
    if(obj_switch==0) *((&My_dma_next)+1) = (u_int)(&My_cube_start);
    else if(obj_switch==1) *((&My_dma_next)+1) = (u_int)(&My_torus_start);

	Set the value to the matrix transferred to VU1 with 
	SetVu1PacketMatrix() function. The address of the object data
	that is to transferred to VU1 is set in DMAtag in the packet.

- Transfer the packet in VUMem1
    // --- kick dma to draw object ---
    *D1_QWC  = 0x00;
    *D1_TADR = (u_int)&My_dma_start & 0x0fffffff;
    *D_STAT = 2;
    FlushCache(0);
    *D1_CHCR = 1 | (1<<2) | (0<<4) | (1<<6) | (0<<7) | (1<<8);
    sceGsSyncPath(0, 0);

	Since the packet is transferred using the channel 1 here, the 
	value is set in the register directly. Set zero to D1_QWC for 
	transferring DMAtag and the packet transfer address to 
	My_dma_start with D1_TADR.
	Set the status bit of channel 1 with D_STAT and flush the cache
	before transferring to consist with the memory. Then start the
	DMA transfer after setting the value in the channel control 
	register of D1_CHCR. The data transferred to VU1 then include
	VIF micro code, VIF code and the object data. These are processed
	with one DMA transfer. The detail of the processing will be 
	described later section "Data processing in VU1 and VIF".

- Switch the drawing environment and wait for the V-Blank
    sceGsSetHalfOffset((frame&1) ? &db.draw1 : &db.draw0, 2048,2048,odev);
    odev = !sceGsSyncV(0);

	After switching the operating environment in every field, wait for
 	the V-Blank period to start. Afterward, return the head of the 
	loop and repeat the same operation.


Data processing in VU1 and VIF
--------------------------------------------------------------------------
Describe the summary of data flow sent to VU1

	Main RAM ---> DMA ---> VIF1 ---> MicroMem1 or VUMem1

The outline of the data flow is above. Describe the processing process in 
the following.
First, control the DMA to transfer the data with DMA. The DMA is 
controlled with sending DMAtag. The addition and generation processing 
for DMA is executed in *.dsm internally (e.g."DMAcnt *" and  "EndDmaData")
and the main loop. 

    if(obj_switch==0) *((&My_dma_next)+1) = (u_int)(&My_cube_start);
    else if(obj_switch==1) *((&My_dma_next)+1) = (u_int)(&My_torus_start);

For example, in this processing, the transfer object is exchanged by
specifying next in DMAtag for object switching and switching the
address in DMAtag.

Then the data pass VIF1. Many functions that can control the VU1 are 
implemented in VIF1. The codes for controlling VIF are implemented with
VIFcode and can be incorporated into the packet. In this sample program,
they are used in *.dsm file.

	Example:

	MPG    :Load the micro program (Send the data to uMem)
	-- data --
	.EndMPG

	unpack 4, 4, V4_32, 0, *    :Expand the following data and write
				     to VUMem
	-- data --
	.EndUnpack

In this sample program, the double buffer processing is executed with the 
micro code and the function of VIFcode in the following order.

	MSCAL 0       :Execute the micro program(Start from address 0)
	BASE 0        :Specify one of the leading addresses of the double
		       buffer to 0 (Set vif1 base register)
	OFFSET 512    :Specify another side of the leading address to 512
                       (Set vif1 offset register)

	unpack ......
	-- object data --
	.EndUnpack
	MSCNT     :Resume the execution of the micro code (drawing 
		   process)

  Show the process flow of the micro simply	

      Preparation --+-> Transparent transformation and light source
			processing (Generate and transfer the GIF packet) 
                 |                              |
                 |               |              |
                 +<----<----<----|<----<----<---+
                         restart | stop
                               MSCNT
                          (switch buffer)

The micro code waits until the following MSCNT instruction comes after the
preparation or one loop is processed. And a value is set in the following
buffer. When the MSCNT instruction is executed, the execution is resumed
(or the MSCNT instruction stalls until the micro program finishes) and 
data is processed.

When the MSCNT instruction is executed, the offset address is switched, or
the memory area for processing is switched, which allows the double buffer
processing.


Basic3d VU1 micro code
--------------------------------------------------------------------------
<Outline of the operation>
	1. Coordinate transformation in the World coordinate system 
	2. Transparent transformation 
	3. Lighting by the normal line vector of the vertex and parallel
	   light source 
	4. Texture mapping with perspective correction 


<Data placement in VUMem1>

   Address       Stored data                          Description
  ---------------------------------------------------------------------
    0 - 3      - Transparent transformation        Matrix data etc. shared
         	 matrix			    	   with the entire model
    4 - 7      - Coordinate transformation              
		 matrix
    8 - 11     - Parallel light source vector
   12 - 15     - Lighting color
  ----------------------------------------------------------------------
    0 - 511     [Source data]
                - GIFtag
                - Vertex coordinate
                - Normal line vector
                - Vertex color
                - Texture coordinate
                                                Double buffer area 0 in 
						VU1
                [The packet XGKICKed to GIF]
                - GIFtag
                - ST
                - RGBAQ
                - XYZF2
  -----------------------------------------------------------------------
   512 - 1023
                [Source data]
                - GIFtag
                - Vertex coordinate
                - Normal line vector
                - Vertex color
                - Texture coordinate
                                                Double buffer area 1 in
						VU1              
		[The packet XGKICKed to GIF]
                - GIFtag
                - ST
                - RGBAQ
                - XYZF2

  ---------------------------------------------------------------------

<Micro code>

;
;
; vu1 micro code for basic sample
;
; [Contents of the main registers]
;
; VF01 - VF04 : Transparent transformation matrix x : rotational and 
		translation matrix in the world coordinate system
; VF05 - VF08 : Directional vector of the parallel light source
; VF09 - VF11 : Colors of each light source
; VF19 : gif tag 
; VF20 : The vertex coordinate processed
; VF21 : The normal line vector in the vertex processed
; VF22 : The texture coordinate of the vertex processed
; VF23 : The color of vertex processed
;
;
; VI02 : The Leading data transferred from VUMEM to GIF with XGKICK (gif 
	 tag is attached.)
; VI03 : The lead pointer to the vertex data in VUMEM processed
; VI04 : The lead pointer to the normal line vector in VUMEM processed
; VI05 : The lead pointer to the texture coordinate in VUMEM processed
; VI06 : The lead pointer to the color of the vertex in VUMEM processed
; VI07 : The store pointer to RGBAQ data transferred to GIF
; VI08 : The store pointer to ST data transferred to GIF
; VI09 : The store pointer to XYZF2 data transferred to GIF
;
; VI10 : The number vertex processed (Loop counter)
;
;
; ---------------------------------------------------------------------


NOP IADDIU VI01, VI00, 0
NOP IADDIU VI02, VI00, 4
NOP IADDIU VI03, VI00, 16 
NOP BAL VI15, MulMatrix ; Transparent transformation matrix x : Calculate
			  the rotational and translation matrix in the 
			  world coordinate system
NOP NOP

NOP IADDIU VI01, VI00, 8 
NOP LQI VF09, (VI01++) ; Load parallel light source vector (8)
NOP LQI VF10, (VI01++)
NOP LQI VF11, (VI01++)
NOP LQI VF12, (VI01++)
NOP LQI VF05, (VI01++) ; Load colors of each light source (12) 
NOP LQI VF06, (VI01++)
NOP LQI VF07, (VI01++)
NOP LQI VF08, (VI01++)
NOP LQI VF01, (VI01++) ; Transparent transformation matrix x : Load 
			  rotational and translation matrix in the 
			  world coordinate system (16)
NOP LQI VF02, (VI01++)
NOP LQI VF03, (VI01++)
NOP LQI VF04, (VI01++)

NOP[E] NOP
NOP NOP
START0:
NOP XTOP VI01                 ; Read the value in TOP register in VIF for
			      ;	double buffering                
NOP ILWR.x VI10, (VI01)x      ; Read the value of nloop from GIFtag
NOP IADDIU VI11, VI00, 0x7fff ; Set the mask to acquire the loop 
				counter (the number of vertex)
NOP IAND VI10, VI11, VI10     ; Set VI10 in the loop counter including the
				number of vertex with masking 		
NOP LQ VF19, 0(VI01)          ; Read gif tag
NOP IADDIU VI03, VI01, 1      ; The pointer to the vertex data read
NOP IADD VI04, VI03, VI10     ; The pointer to the normal line vector 
				read
NOP IADD VI05, VI04, VI10     ; The pointer to the color of the vertex 
				data read
NOP IADD VI06, VI05, VI10     ; The pointer to the texture coordinate data
				read
NOP IADD VI02, VI06, VI10     ; The pointer of the position XGKICKed
NOP SQ VF19, 0(VI02)          ; Store gif tag
NOP IADDIU VI07, VI02, 2      ; The pointer to the position storing 
				calculated RGBAQ
NOP IADDIU VI08, VI02, 1      ; The pointer to the position storing
				calculated ST
NOP IADDIU VI09, VI02, 3      ; The pointer to the position storing
				calculated XYZF2

LOOP:
NOP LQI VF20, (VI03++)        ; Read the vertex data
NOP LQI VF21, (VI04++)        ; Read the normal line vector
NOP LQI VF22, (VI05++)        ; Read the color of the vertex
NOP LQI VF23, (VI06++)        ; Read the texture coordinate

; --- Coordinate and transparent transformation ---
MULAx.xyzw ACC, VF01, VF20x NOP   ; Execute the coordinate and transparent
				    transformation 
MADDAy.xyzw ACC, VF02, VF20y NOP 
MADDAz.xyzw ACC, VF03, VF20z NOP 
MADDw.xyzw VF25, VF04, VF20w NOP 
NOP NOP
NOP NOP
NOP NOP
NOP DIV Q, VF00w, VF25w          ; Calculate 1/w

; ---Inner product between parallel light source and normal line vector---
MULAx.xyzw ACC, VF05, VF21x NOP   ; Inner product
MADDAy.xyzw ACC, VF06, VF21y NOP 
MADDAz.xyzw ACC, VF07, VF21z NOP 
MADDw.xyzw VF13, VF08, VF21w NOP 
NOP NOP
NOP NOP
NOP NOP
MAXx.xyzw VF13, VF13, VF00x NOP ; Set the value less than 0.0f to 0.0f

; --- Multiply 1/w and store ---
MULq VF28, VF25, Q NOP       ; Multiply 1/w
NOP NOP
NOP NOP
NOP NOP
FTOI4 VF27, VF28 NOP         ; Convert to fixed point
NOP NOP
NOP NOP
NOP NOP
NOP SQ VF27, 0(VI09)         ; Store XYZF2 (unpacked mode)
NOP IADDIU VI09, VI09, 3     ; Increment the store pointer

; Read st, multiply it by Q and store(for perspective correction)
MULq.xyz VF27, VF23, Q NOP   ; Multiply ST by 1/w and set 1/w to Q
NOP NOP
NOP NOP
NOP NOP
NOP SQ VF27, 0(VI08)         ; Store STQ (unpacked mode)
NOP IADDIU VI08, VI08, 3     ; Increment the store pointer
NOP NOP
NOP NOP

; --- Calculate the color of the material in which illumination is 
      calculated and store ---
MULAx.xyzw ACC, VF09, VF13x NOP    ; Multiply the inner product by the 
				     color of the light source
MADDAy.xyzw ACC, VF10, VF13y NOP
MADDAz.xyzw ACC, VF11, VF13z NOP
MADDw.xyzw VF14, VF12, VF13w NOP
NOP NOP
NOP NOP
NOP NOP
MUL.xyz VF27, VF22, VF14 NOP       ; Multiply calculated color of light 
				     by the color of material
NOP NOP
NOP NOP
NOP NOP
FTOI0 VF26, VF27 NOP               ; Convert to integer 
NOP NOP
NOP NOP
NOP NOP
NOP SQ VF26, 0(VI07)               ; Store RGBAQ (unpacked mode)
NOP IADDIU VI07, VI07, 3           ; Increment the store pointer

; --- Loop ---
NOP IADDI VI10, VI10, -1           ; Decrement the loop counter
NOP IBNE VI10, VI00, LOOP          ; Repeat the loop

; --- Send the calculation result to GIF (XGKICK) and stop the execution 
      of the micro ---
NOP NOP 
NOP XGKICK VI02 ; XGKICK 
NOP[E] NOP      ; Stop the micro
NOP NOP 
NOP B START0    ; Jump to the next position to execute the micro
NOP NOP


; -------------------------------------------------------------------
; MulMatrix : Multiply matrix 
; VI01 : The address of the source matrix 1
; VI02 : The address of the source matrix 1
; VI03 : The address storing the calculated matrix
;

MulMatrix:
NOP LQI.xyzw VF08, (VI02++)
NOP LQI.xyzw VF04, (VI01++)
NOP LQI.xyzw VF05, (VI01++)
NOP LQI.xyzw VF06, (VI01++)
NOP LQI.xyzw VF07, (VI01++)
MULAx.xyzw ACC, VF04, VF08x LQI.xyzw VF09, (VI02++)
MADDAy.xyzw ACC, VF05, VF08y NOP
MADDAz.xyzw ACC, VF06, VF08z NOP
MADDw.xyzw VF12, VF07, VF08w NOP
MULAx.xyzw ACC, VF04, VF09x LQI.xyzw VF10, (VI02++)
MADDAy.xyzw ACC, VF05, VF09y NOP
MADDAz.xyzw ACC, VF06, VF09z NOP
MADDw.xyzw VF13, VF07, VF09w SQI.xyzw VF12, (VI03++)
MULAx.xyzw ACC, VF04, VF10x LQI.xyzw VF11, (VI02++)
MADDAy.xyzw ACC, VF05, VF10y NOP
MADDAz.xyzw ACC, VF06, VF10z NOP
MADDw.xyzw VF14, VF07, VF10w SQI.xyzw VF13, (VI03++)
MULAx.xyzw ACC, VF04, VF11x NOP
MADDAy.xyzw ACC, VF05, VF11y NOP
MADDAz.xyzw ACC, VF06, VF11z NOP
MADDw.xyzw VF15, VF07, VF11w SQI.xyzw VF14, (VI03++)
NOP NOP
NOP NOP
NOP NOP
NOP SQI.xyzw VF15, (VI03++)
NOP NOP
NOP JR VI15
NOP NOP


Description of the packet
--------------------------------------------------------------------------
Describe the packet (.dsm format) transferred to VU1 to calculate the 
transparent transformation and parallel light source with VU1 micor code.
An independent triangle and triangle strip are also allowed to use with  
setting GIFtag. When drawing many vertexes, transfer the packets from
GIFtag to MSCNT in parallel, which enables double buffer processing in 
VU1.
 <Example> Triangle strip cube ------------------------------------
.include "vumacros.h"
.global My_dma_start
.global My_matrix
.global My_light_matrix
.global My_rot_trans_matrix

My_dma_start:
.align 0
DMAcnt *     ; Transfer the micro code itself to VU1
MPG 0, *
.include "basic.vsm" 
.EndMPG
.EndDmaData

DMAcnt *
unpack 4, 4, V4_32, 0, *  ; Screen matrix
My_matrix:
fwzyx 0.000000, 0.000000, 0.000000, 35.752483
fwzyx 0.000000, 0.000000, -14.765776, 0.000000
fwzyx 0.050000, 4995000.000000, 102.400002, 102.400002
fwzyx 1.000000, 100000000.000000, 2048.000000, 2048.000000
.EndUnpack

unpack 4, 4, V4_32, 4, * ; Rotational and translation matrix in the 
			   world coordinate system
My_rot_trans_matrix:
fwzyx 0.0, -0.000000, -0.000000, 1.000000
fwzyx 0.0, -0.000000, 1.000000, 0.000000
fwzyx 0.0, 1.000000, 0.000000, 0.000000
fwzyx 1.0, 0.000000, 0.000000, 0.000000
.EndUnpack

unpack 4, 4, V4_32, 8, * ; Light source color (Parallel light source x3
			   and ambient light x1)
fxyzw 1.0, 0.0, 0.0, 0.0 ; Parallel light source1 (R,G,B)=(1.0, 0.0, 0.0)
fxyzw 0.0, 0.0, 1.0, 0.0 ; Parallel light source2 (R,G,B)=(1.0, 0.0, 0.0)
fxyzw 0.0, 0.0, 0.0, 0.0 ; Parallel light source3 (R,G,B)=(1.0, 0.0, 0.0)
fxyzw 0.2, 0.2, 0.2, 1.0 ; Ambient light (R,G,B) = (0.2, 0.2, 0.2)
.EndUnpack

unpack 4, 4, V4_32, 12, * ; Parallel light source vector(x3)
My_light_matrix:
fxyzw 0.0, -1.0, 0.0, 0.000000 ; Each column shows vector. In this 
example, Light1=(0.0, 1.0, 0.0). 
fxyzw 1.0, 0.0, 0.0, 0.000000  ; Light2 = (-1.0, 0.0, 0.0),
Light3=(0.0, 0.0, 0.0)
fxyzw 0.0, 0.0, 0.0, 0.000000  ; The forth column is for setting ambient
				 light.
fxyzw 0.0, 0.0, 0.0, 1.000000
.EndUnpack

MSCAL 0 ; Execute the micro
BASE 0 ; Set the base register of vif1 base 
OFFSET 512 ; Set the vif1 offset register
.EndDmaData


; ------------------------------------------------------------------------
; The model data that are to be drawn
; This is a cube (triangle strip) here.

DMAcnt *
unpack[r] 4, 4, V4_32, 0, *
iwzyx 0x00000000, 0x00000412, 0x300a4000, 0x0000800e ; giftag (with 
texture)
.EndUnpack
unpack[r] 4, 4, V4_32, 1, *
; The position of the vertex of the strip in the world coordinate system
fxyzw -5.0, -5.0, 5.0, 1.0
fxyzw -5.0, -5.0, -5.0, 1.0
fxyzw -5.0, 5.0, 5.0, 1.0
fxyzw -5.0, 5.0, -5.0, 1.0
fxyzw 5.0, 5.0, -5.0, 1.0
fxyzw -5.0, -5.0, -5.0, 1.0
fxyzw 5.0, -5.0, -5.0, 1.0
fxyzw -5.0, -5.0, 5.0, 1.0
fxyzw 5.0, -5.0, 5.0, 1.0
fxyzw -5.0, 5.0, 5.0, 1.0
fxyzw 5.0, 5.0, 5.0, 1.0
fxyzw 5.0, 5.0, -5.0, 1.0
fxyzw 5.0, -5.0, 5.0, 1.0
fxyzw 5.0, -5.0, -5.0, 1.0

; Normal line vector
fxyzw -1.0, 0.0, 0.0, 1.0
fxyzw -1.0, 0.0, 0.0, 1.0
fxyzw -1.0, 0.0, 0.0, 1.0
fxyzw -1.0, 0.0, 0.0, 1.0
fxyzw 0.0, 1.0, 0.0, 1.0
fxyzw 0.0, 0.0, -1.0, 1.0
fxyzw 0.0, 0.0, -1.0, 1.0
fxyzw 0.0, -1.0, 0.0, 1.0
fxyzw 0.0, -1.0, 0.0, 1.0
fxyzw 0.0, 0.0, 1.0, 1.0
fxyzw 0.0, 0.0, 1.0, 1.0
fxyzw 0.0, 1.0, 0.0, 1.0
fxyzw 1.0, 0.0, 0.0, 1.0
fxyzw 1.0, 0.0, 0.0, 1.0

; Color
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0
fxyzw 200.0, 200.0, 200.0, 128.0

; Texture coordinate
fxyzw 0.0, 0.0, 1.0, 0.0
fxyzw 0.0, 1.0, 1.0, 0.0
fxyzw 1.0, 0.0, 1.0, 0.0
fxyzw 1.0, 1.0, 1.0, 0.0
fxyzw 0.0, 1.0, 1.0, 0.0
fxyzw 1.0, 0.0, 1.0, 0.0
fxyzw 0.0, 0.0, 1.0, 0.0
fxyzw 1.0, 1.0, 1.0, 0.0
fxyzw 0.0, 1.0, 1.0, 0.0
fxyzw 1.0, 0.0, 1.0, 0.0
fxyzw 0.0, 0.0, 1.0, 0.0
fxyzw 0.0, 1.0, 1.0, 0.0
fxyzw 1.0, 0.0, 1.0, 0.0
fxyzw 1.0, 1.0, 1.0, 0.0

.EndUnpack
MSCNT  ; Resume the micro code (Drawing process)
.EndDmaData

DMAend

