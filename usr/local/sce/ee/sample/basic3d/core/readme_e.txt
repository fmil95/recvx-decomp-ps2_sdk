[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Basic sample programs for the basic3d Core version 
===================================================

Aim in the basic3d Core version
--------------------------------------------------------------------------
These sample programs implement mathematical functions for a matrix 
operation and transparent transformation using only EE Core and deepen the
understanding of the mathematical basics related to 3d graphics. They also
implement the function operationally equal to that used in the basic3d vu0
and vu1 version to help implement the mathematical functions to vu0 and 
vu1. In addition, by generating the packet that passes the operational 
result directly to the GS, the program confirms the DMA operation as well 
as the basic specification for the GS operation.

Programs for the Core version
--------------------------------------------------------------------------
<Files>
	core.c		Main program
	mathfunc.c	Matrix operation functions for a transparent 	
			transformation etc. (Core version)
	mathfunc.h	Header file of matrix operation functions for a 
			transparent transformation etc.
	cube.s		Object data (Cube)
	torus1.s	Object data (Torus/data divided)
	flower.dsm	Texture data

<Compilation>
	% make

<Execution>
	% make run

	After the compilation, the program is also executable 
	with the following.

	% dsedb
	> run core.elf

<Specification>
        Display a cube of a Triangle Strip with texture 
	Display a torus of a Triangle with texture 
	(Switching with the select button)

	24-bit Z buffer
	3 light sources
	32-bit texture

<Controller operation>
	Up/Down directional button:	Rotation around x-axis of the 
					object
        Right/Left directional button:	Rotation around y-axis of the 
					object
        L1L2 button:			Rotation around z-axis of the 
					object
        Square/Circle button:		Rotation around y-axis of the 
					camera toward the world coordinate
					system
        Triangle/Cross button:		Rotation around x-axis of the 
					camera toward the world coordinate
 					system
        R1R2 button:			Forward and backward rotation of 
					the camera
        select button:			Object switching

<Outline of the processing>
	1.Load texture
	2.Open the pad
	3.Set the transparent transformation and light source calculation
	  matrix and so on
	4.Execute the transparent transformation and light source 
	  calculation to generate the packet passed to the GS
	6.DMA kick to the GS
	7.Return 2.

Data flow
--------------------------------------------------------------------------
<Initializing process>
Main RAM                          	GS built-in RAM(4M)
  Texture data -----------------------> Expansion of the texture data 
  Expansion of the object data
  Expansion of the transparent transformation matrix etc.

<Execution process>
Main RAM                                Scratch pad (16K)
  Object data ------------------------> Expansion of the packet data for 
					the GS transfer(Primitive data 
     	    Transparent transformation/	      (STQ,RGBA and  XYZF) array)
 	    Lighting calculation                           |
	    (CPUcore geometry)                             |                
              		                                   |
                        	                           |
GS built-in RAM (4M) <-------------------------------------+
 Writing to the frame and Z buffer      DMA transfer (DMAtag and GIFtag 
 (Rendering)			         are attached.)
 

Detail description of the program
--------------------------------------------------------------------------
- Variable/Structure/Function (the function of mathfunc.c is excluded.)

        SampleCubeDataHead[]     : The pointer to the object data (cube)
        SampleTorus1DataHead[]   : The pointer to the object data (torus)
        My_texture1[]            : The pointer to the texture data

        TexEnv          : The structure for generating the packet to 
			  transfer the sceGsTexEnv structure
        QWdata          : The union to process 128-bit data easily
        GifPacket       : The structure for generating the packet 
			  transferred to the Gif(GS)
        ObjData         : The structure managing the object data expanded 
			  in the memory

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

        ambient         : Ambient light

        obj_trans       : The moving vector of the object
        obj_rot         : The rotation vector of the object
 
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
			  vertex, by each color of light (3 colors plus 
			  ambient light)  
        local_color     : The matrix generated by multiplying local_light 
			  by light_color

        work            : The matrix for temporary work

        scePadData      pad     : The structure for reading the pad data
        sceGsDBuff      db      : The structure for implementing the 
				  double buffer
	sceGsLoadImage  pTex    : The structure for transferring the 
				  texture into the GS local memory
        sceDmaChan      *dmaVif : The structure for data transfer with DMA
        TexEnv          texenv  : The structure generating the packet for 
				  transferring the texture data
	LoadObj(ObjData *o, u_int *Head)
                        : The function for registering the object data 
			  expanded in the main RAM to the ObjData 
			  structure and operating them.	
       	ReleaseObj(ObjData *o)
                        : The function for releasing the registered object
	                  data
       	MakePacket(ObjData *obj, int num)
                        : The function for generating the packet that 
			  executes transparent transformation and light 
			  source calculation and transfers the result to 
			  the GS.

- Variables in the main function

	i, j            : The counter used for "for loop" etc.
        fd              : The file descriptor receiving a return value 
			  whenthe pad is opened
        fr              : The switch for flipping the double buffer
        delta           : The variable for setting the angle of rotation
        obj[2]          : The variable for registering the object 
			  (cube&torus)
        obj_switch      : The toggle switch for switching the object
        toggle          : The variable for controlling the reading of the 
			  select button with the toggle

- The variables and functions provided with mathfunc.c 

	sceSamp0IVECTOR[4]      : The int type array located in 128-bit 
				  boundary (element 4)
        sceSamp0FVECTOR[4]      : The float type array located in 128-bit 
				  boundary (element 4)
        sceSamp0FMATRIX[4][4]   : The float type array located in 128-bit
				  boundary (element 4x4)

See mathfunc.c for the detail description of the function. Most of the 
functions written in mathfunc.c are written by the Vu0 macro and are 
provided with libvu0. The function name implemented  is sceSamp0... in the
Core version, while sceVu0... in the VU0 version.
Note that the array type of the matrix is different between the Core 
version and VU0/VU1 versions. The Core version uses a general array, but
the VU0/VU1 versions adopt the transposed type to make use of the 
arithmetic function effectively.


Description of the programs (for initializing)
--------------------------------------------------------------------------
- Read the object data
    LoadObj(&obj[0], SampleCubeDataHead);
    LoadObj(&obj[1], SampleTorus1DataHead);

	Read the object data used as the sample program from the structure
	of SampleCubeDataHead and SampleCubeDataHead to ObjData sturucrue.

- Initialize the device
    sceGsResetPath();	//Initialize the data path device 
			  (VIF1,VU1 and GIF)
    sceDmaReset(1);	//Initialize the DMA (DMA)

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
	//Set the LoadImage information
    FlushCache(0);
    sceGsExecLoadImage(&pTex, (u_long128 *)My_texture1);
	//Execute the LoadImage
    sceGsSyncPath(0, 0); //Wait for the data transfer to finish 
			
	The function sets the LoadImage information to transfer the 
	texture image into the GS local memory. Then, by executing the 
	sceGsExecLoadImage(), specified image will be transferred to the
	GS local memory. Because this function uses the DMA internally, 
	the D cache is consistent with the main memory using the 
	FlushCache(0) before transferring. Finally, the function waits for
	the data transfer to finish using the sceGsSyncPath().

- Genarate the packet for setting the environment for the texture
    FlushCache(0);
    texenv.size = sceGsSetDefTexEnv(&texenv.gs_tex, 0, 13340, 
        IMAGE_SIZE / 64,SCE_GS_PSMCT32, 8, 8, 0, 0, 0, 0, 1);
        	//Generate the data for setting the texture
    SCE_GIF_CLEAR_TAG(&texenv.giftag);
    texenv.giftag.NLOOP = texenv.size;
    texenv.giftag.EOP = 1;
    texenv.giftag.NREG = 1;
    texenv.giftag.REGS0 = 0xe;
    *(u_long*)&texenv.gs_tex.clamp1
        = SCE_GS_SET_CLAMP(0, 0, 0, 0, 0, 0);

	The function generates the packet for setting the GS general 
	purpose	register TEX0_1 and CLAMP_1 related to the texture using 
	the sceGsSetDefTexEnv().

- Transfer the packet for setting the environment for the texture
    FlushCache(0);
    dmaVif = sceDmaGetChan(SCE_DMA_GIF);
    dmaVif->chcr.TTE = 1;
    sceDmaSendN(dmaVif, &texenv.giftag, texenv.size+1);
			//Transfer the data for setting the texture
    sceGsSyncPath(0, 0); //Wait for the data transfer to finish 
				
	The program transfers the generated packet for setting the texture
	environment by executing the Normal DMA via PATH3.

- Initialize the controller
    fd = open("pad:0", 0);

	Initialize the controller for 1p side. Then, the controller can 
	read the data to the specified buffer using the read(). The data
	of the controller is updated every VSync operation.


Description of the programs (for the main loop)
--------------------------------------------------------------------------
- Generate the View-Screen matrix
    sceSamp0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
                2048.0f, 2048.0f,1.0f, 16777215.0f,1.0f,65536.0f);

	The function calculates the View-Screen matrix using the 
	sceSamp0ViewScreenMatrix(). In this example, the distance from the
	viewpoint to the projection screen is 512, the aspect ratio of the
	screen is 1:0.47, the central point of the screen is (2048,2048) 
	in the GdPrimitive coordinate system, the value of the z buffer is
	1.0-16777215.0 and the value of z is 1.0-65536.0.
	Initialization is executed outside the main loop in the Core 
	version, because this matrix is seldom changed during the 
	execution.

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

- Generate the packet
    MakePacket(&obj[obj_switch], i);

	The packet is generated in the MakePacket() internally. The 
	argument obj is the pointer to the ObjData structure of the 
	displayed object. The displayed object can be easily changed by 
	switching the pointer.

- Set the packet initially (MakePacket())
    pack->size = 0;
    pack->buf = (QWdata *)0x70000000;//SPR

	The writing address of the packet is specified in 0x70000000. 
	Because it is the head of the scratch pad area, note that the size
 	of the packet should not exceed the 16-KB, the size of the scratch
	pad. Even when generating the packet in the main memory, the value
	that can be set to the QWC during the DMA transfer is restricted.
	Note that the size of the packet should not exceed 1MB. The torus 
	data in this sample program are split up into the 4-KB or so 
	blocks in order not to exceed the capacity of the double buffer of
	the VUMem1 used in the VU1 version.

- Generate the DMAtag and GIFtag (MakePacket())
    //DMAtag
   pack->buf[pack->size].ul128 = (u_long128)0;
   pack->buf[pack->size++].ui32[0] = 0x70000000|(obj->vertexNum[num]*3+1);
    //GIFtag
   pack->buf[pack->size].ul64[0]
        = SCE_GIF_SET_TAG(obj->vertexNum[num], 1, 1,obj->prim,
                                                SCE_GIF_PACKED,3);
   pack->buf[pack->size++].ul64[1] = 0x0412L;

	In the head of the packet, the DMAtag and GIFtag should be 
	attached. DMA uses the Source Chain mode. The size of the packet 
	that will be sent equals the number of the vertex x3 (STQ, RGBA,
	and XYZF2) plus 1(GIFtag) and the ID of the DMAtag is end 
	(0x70000000) (because the number of the DMA packet is only one).
	The GIF uses the PACKED mode(SCE_GIF_PACKED) and the primitive is 
	set in the GIF tag (obj->prim). The value of the REGS is 0x412
	(STQ,RGBA and XYZF2).
- Transparent transformation
    Q = sceSamp0RotTransPers(v01, local_screen, vertex[j], 1);
    sceSamp0ScaleVector(tex, texUV[j], Q); //for Perspective correction

	The sceSamp0RotTransPers() function is the function for the 
	transparent translation. Detail description is written in 
	mathfunc.c. The output result is an int type array, which is a 
	coordinate system of the GS and can be passed to the GS directly. 
	The return value Q, which is a float type, is a reciprocal of the 
	forth element of v01. This is for a perspective correction and 
	used for the texture coordinate (S,T,Q). In the texUV, the UV 
	coordinate of the texture (U, V, 1.0 ,0) is set. The coordinate of
	the STQ texture is calculated by multiplying these values by Q.

- Light source calculation
    sceSamp0NormalColorVector(c01, local_light, light_color,
                         normal[j], color[j]);

	The color of the vertex after calculating the light source is 
	determined by the normal line vector of the vertex / the color 
	information and	the light vector  / the color information. The 
	value which can be passed to GS directly is returned. In the 
	function internally, when calculating the inner product between
	the light vector and the normal line vector of the vertex, the 
	value of the inner product is clipped between 0.0 and 1.0 once. 
	After the three lighting effects are added, the result is clipped 
	between 0.0 and 255.0. 

- Store to the packet
    pack->buf[pack->size++].ul128 = *((u_long128*)tex);
    pack->buf[pack->size++].ul128 = *((u_long128*)c01);
    pack->buf[pack->size++].ul128 = *((u_long128*)v01);

	Calculated values are stored to the packet in the following order
	;(S,T,Q),(R,G,B,A)and (X,Y,Z,F). 	

- Transfer to the GS
    MakePacket(&obj, i);
    FlushCache(0);
    sceDmaSend(dmaVif, (u_long128*)((u_int)obj.pack[i].buf|0x80000000));
    sceGsSyncPath(0, 0);

	Transfer the packet of the primitive generated by the MakePacket()
	by executing the Source Chain DMA via PATH3.
	Because the size of the packet generated at a time is restricted,
	the transfer is repeated the same number of times as the number of
	the split blocks of the object. 

- Switch the drawing environment and wait for the V-Blank
    if(fr&0x01)
        sceGsSetHalfOffset(&db.draw1,2048,2048,sceGsSyncV(0)^0x01);
    else
        sceGsSetHalfOffset(&db.draw0,2048,2048,sceGsSyncV(0)^0x01);

	After switching the operating environment in every field, wait for
 	the V-Blank period to start.

