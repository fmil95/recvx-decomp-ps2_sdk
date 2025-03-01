[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved


Basic sample programs for the basic3d VU0 version 
==================================================

Aim in the basic3d VU0 version
--------------------------------------------------------------------------
These sample programs intend to deepen the understanding of the 
mathematical functions for a matrix operation and transparent 
transformation and mathematical basics related to 3d graphics.
In addition, those shows the example rewriting to the VU0 macro 
instruction as the first step to fast processing.
As a coprocessor instruction, the VU0 macro instruction is allowed to use
as an assembler format. Also, since that can process maximum four data at 
a time, the affinity with a matrix operation and a vector operation is
high.

This basic3d VU0 version intends to deepen the understanding of the use of
the VU0 macro by showing the example converting the matrix operation etc. 
in the Core version using the VU0 macro instruction. And we expect these 
programs will help accelerate the program further when using the VU0 or 
VU1 micro instructions.

Description for the program
--------------------------------------------------------------------------
<Files>
	main.c		Main program
	vu0.c		Matrix operation functions for a coordinate and 
			transparent transformation etc. 
	cube.s		Object data (Cube)
	torus.s		Object data (Torus/data divided)
	torus1.s	Object data (Torus/data divided)
	flower.dsm	Texture data
                        
<Starting>
	% make		: Compile
	% make run	: Execute

	After the compilation, the program is also executable with the 
	following.

	% dsedb
	> run main.elf

<Controller operation>
	Up/Down directional button	:Rotation around x-axis (object)
	Right/Left directional button	:Rotation around y-axis (object)
	L1L2 button			:Rotation around z-axis (object)
	
	Triangle/Cross button		:Rotation around the x-axis
					 (viewpoint)
	Square/Circle button		:Rotation around the y-axis
					 (viewpoint)
	R1R2 button			:Translation around the z-axis
					 (viewpoint)

	SELECT button			:Object switching (CUBE/TORUS)

<Specification>
	Display a cube of a Triangle Strip with texture 
	Display a torus of a Triangle with texture 
	24-bit Z buffer
	3 light sources
	32-bit texture

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
Main RAM                                GS built-in RAM(4M)
  Texture data ------------------------>Expansion of the texture data 
  Expansion of the object data
  Expansion of the transparent transformation matrix etc.

<Execution process>
Main RAM                                Scratch pad (16K)
 Object data--------------------------->Expansion of the packet data for 
					the GS transfer (Primitive data 
     	   Transparent transformation/        (STQ,RGBA and XYZF array))
 	   Lighting calculation                            |
	    (VU0core geometry)                             |    
              		                                   |
                        	                           |
GS built-in RAM (4M) <-------------------------------------+
 Writing to the frame and Z buffer      DMA transfer (DMAtag and GIFtag 
 (Rendering)			        are attached.)


Detail description of the program
--------------------------------------------------------------------------
<Variable/Structure/Function> (the function of mathfunc.c is excluded.)

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

	ambient		: Ambient light

        obj_trans       : Translation of the object
        obj_rot         : Rotation of the object

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
			  (It is not actually allowed to use because the 
			  the value of the inner product should be clipped
			  between 0.0 and 1.0 after generating the inner 
			  product vector.)

        work            : The matrix for temporary work

        scePadData      pad     : The structure for reading the pad data
        sceGsDBuff      db      : The structure for implementing the 
				  double buffer
	sceGsLoadImage  pTex    : The structure for transferring the 
				  texture into the GS local memory
	sceDmaChan      *dmaVif	: The structure for data transfer with DMA
	TexEnv          texenv	: The structure generating the packet for 
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

<Variables in the main function>
	i, j            : The counter used for "for loop" etc.
        fd              : The file descriptor receiving a return value 
			  when the pad is opened
	fr		: The switch for flipping the double buffer
	delta		: The variable for setting the angle of rotation
	obj		: The variable for registering the object


Description of the programs (for initializing)
--------------------------------------------------------------------------
Description of the programs (for initializing)
    LoadObj(&obj[0], SampleCubeDataHead);
    LoadObj(&obj[1], SampleTorus1DataHead);

	Read the object data from SampleCubeDataHead and
	SampleTorus1DataHead.

- Initialize the device
    sceGsResetPath();	//Initialize the data path device
			  (VIF1,VU1 and GIF)
    sceDmaReset(1);	//Initialize the DMA (DMA)
    sceVpu0Reset();	//Initialize the VU0 (VIF0 and VU0)

	In the head of the program, all devices are to used are 
	initialized. Initialization is a must because if using a device 
	without it, unexpected result will occur.

- Set the drawing environment
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
		//Initialize the GS
    sceGsSetDefDBuff(&db, SCE_GS_PSMCT32, SCREEN_WIDTH, SCREEN_HEIGHT, 
			SCE_GS_ZGEQUAL, SCE_GS_PSMZ24, SCE_GS_CLEAR);
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
    sceGsSyncPath(0, 0);	//Wait for the data transfer to finish

	The function sets the LoadImage information to transfer the 
	texture image into the GS local memory. Then, by executing the 
	sceGsExecLoadImage(), specified image will be transferred to the
	GS local memory. Because this function uses the DMA internally, 
	the D cache is consistent with the main memory using the 
	FlushCache(0) before transferring. Finally, the function waits for
	the data transfer to finish using the sceGsSyncPath(). In this 
	process, the image transfer to the GS local memory is finished.

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
Acquiring the controller information, generating the matrix and generating
the packet(a coordinate and transparent transformation, UV->STQ 
transformation and light source calculation) executed in the main loop 
will be described in the following section.

-Generate the View-Screen matrix
    sceVu0ViewScreenMatrix(view_screen, 512.0f, 1.0f, 0.47f,
	    2048.0f, 2048.0f,1.0f, 16777215.0f,1.0f,65536.0f);

	The function calculates the View-Screen matrix using the 
	sceSamp0ViewScreenMatrix(). In this example, the distance from the
	viewpoint to the projection screen is 512, the aspect ratio of the
	screen is 1:0.47, the central point of the screen is (2048,2048) 
	in the GdPrimitive coordinate system, the value of the z buffer is
	1.0-16777215.0 and the value of z is 1.0-65536.0.

- Acquire the controller information
    if(pad.button & SCE_PADLdown){
        obj_rot[0] += delta; if(obj_rot[0]>PI) obj_rot[0] -= 2.0f*PI; }
    if(pad.button & SCE_PADLup){
        obj_rot[0] -= delta; if(obj_rot[0]<-PI) obj_rot[0] += 2.0f*PI; }
    if(pad.button & SCE_PADLright){
        obj_rot[1] += delta; if(obj_rot[1]>PI) obj_rot[1] -= 2.0f*PI; }
    if(pad.button & SCE_PADLleft){
        obj_rot[1] -= delta; if(obj_rot[1]<-PI) obj_rot[1] += 2.0f*PI; }
    if(pad.button & SCE_PADL1){
        obj_rot[2] += delta; if(obj_rot[2]>PI) obj_rot[2] -= 2.0f*PI; }
    if(pad.button & SCE_PADL2){
        obj_rot[2] -= delta; if(obj_rot[2]<-PI) obj_rot[2] += 2.0f*PI; }
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
	if(camera_rot[1]<-PI) camera_rot[1] += 2.0f*PI; }
    if(pad.button & SCE_PADR1){
        camera_p[2] -= delta*5; if(camera_p[2]<-100) camera_p[2] = -100; }
    if(pad.button & SCE_PADR2){
        camera_p[2] += delta*5; if(camera_p[2]>-10) camera_p[2] = -10; }
    if(!toggle && (pad.button & SCE_PADselect)){
        obj_switch = (++obj_switch)&0x01; toggle = 1;
    }else if(!(pad.button & SCE_PADselect)){toggle = 0;}

	The controller button information is acquired using the read(). In
	this example, the object, the angle of rotation for both sides of 
	the camera and the shift amount are updated based on the acquired 
	data. A left-hand side button for the controller is for the object
	operation while a right-hand side button for that is for the 
	camera operation.

- Generate the Local-World matrix (rotation only)
    sceVu0UnitMatrix(work);                       //Unit matrix 
    sceVu0RotMatrix(local_world, work, obj_rot);  //Rotating matrix

	The Local-World matrix should be calculated for every object held 
	in the Local coordinate. The affine transformation, such as the 
	rotational and parallel translation, is used. In this sample 
	program, the rotational and parallel translation matrix is 
	calculated and the result from multiplying these two matrixes is 
	used as the Local-World matrix.	However, the Local_Light matrix 
	should be set before setting the parallel translation (obj_trans)
	of the third line. If the Local_Light matrix is generated using 
	the matrix including the parallel translation, the ambient light 
	is not correctly calculated. This is because the parallel 
	translation and the ambient light are calculated using the forth 
	column of the matrix. Therefore, generate the Local_Light matrix 
	immediately after setting the rotating matrix.
	Note that the forth element of the obj_trans should be set to 0.
	Otherwise, very critical value set in [4,4] element of the matrix
	will change.

- Generate the Local_Light matrix
    sceVu0NormalLightMatrix( normal_light, light0,light1,light2);
    sceVu0MulMatrix(local_light, normal_light,local_world);

	The function calculates the Normal-Light matrix from the three 
	light sources using the sceSamp0NormalLightMatrix(). Further, the 
	matrix is multiplied by the Local-World matrix to generate the 
	Local-Light matrix. As mentioned above, the Local_World matrix 
	used here should be the matrix in which only the rotation is 
	set.

- Generate the Light_Color matrix
    sceVu0LightColorMatrix( light_color, color0, color1, color2, ambient);

	The function calculates the Light_Color matrix from the light 
	source and environmental color using the 
	sceSamp0LightColorMatrix(). In the light source calculation, the 
	final vertex color on the screen is determined by multiplying the 
	Light_Color matrix after each lighting effect color is calculated.

- Generate the Local-World matrix (rotational and parallel translation)
    sceVu0TransMatrix(local_world, local_world, obj_trans);
	//Rotational and parallel translation matrix

	As the generation of the local_light matrix is finished, the 
	function adds the elements of the parallel translation to the 
	local_world before generating the local_screen matrix.

- Generate the World-View matrix
    sceVu0RotCameraMatrix(world_view, camera_p, camera_zd,camera_yd,
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
	vectors. In the sceVu0RotCameraMatrix() function internally, 
	these vector rotates with the camera_rot and the result is passed
	to the sceVu0CameraMatrix().

- Generate the Local-Screen matrix
    sceVu0MulMatrix(work, world_view, local_world);	    //Local-View
    sceVu0MulMatrix(local_screen, view_screen, work);       //Local-Screen

	First, calculate the Local-View matrix using the World-View 
	matrix and the Local-World matrix already calculated. The Local-
	Screen matrix is calculated by multiplying the View-Screen 
	matrix by the Local-World matrix.

- Generate the packet
    MakePacket(&obj[obj_switch], i);

	The packet is generated in the MakePacket() internally. The 
	detail will be described later.

- Packet transfer to the GS
    FlushCache(0);
    sceDmaSend(dmaVif,(u_long128*)((u_int)pBase&0x3ff0|0x80000000));
    sceGsSyncPath(0, 0);

	Transfer the packet of the primitive generated by the MakePacket()
	by executing the Source Chain DMA via PATH3.

- Switch the drawing environment and wait for the V-Blank
    if(fr&0x01)
        sceGsSetHalfOffset(&db.draw1,2048,2048,sceGsSyncV(0)^0x01);
    else
        sceGsSetHalfOffset(&db.draw0,2048,2048,sceGsSyncV(0)^0x01);

	After switching the operating environment in every field, wait for
 	the V-Blank period to start.


Description of the programs (for generating the packet)
--------------------------------------------------------------------------
The contents of the packet generation processing in the MakePacket() will
be described in the following sections.

- Initialize the packet
    pack->size = 0;
    pack->buf = (QWdata *)0x70000000;//SPR

	Set the writing position of the packet to the head. The area for
	generating the packet is set in the head of the scratch pad. When
	using it, note that the size of the packet should not exceed the 
	16-KB, the size of the scratch pad. Even when using the main
	memory, note that the size of the packet should not exceed 1MB
	since the value that can be set to the QWC during the DMA transfer
	is restricted.

- Generate the DMAtag and GIFtag    //DMAtag
    pack->buf[pack->size].ul128 = (u_long128)0;
    pack->buf[pack->size++].ui32[0]
        = 0x70000000 | (obj->vertexNum[num]*3 + 1);

    //GIFtag
    pack->buf[pack->size].ul64[0]
	= SCE_GIF_SET_TAG(obj->vertexNum[num], 1, 1,obj->prim,
          SCE_GIF_PACKED,3);
    pack->buf[pack->size++].ul64[1] = 0x0412L;

	In the head of the packet, the DMAtag and GIFtag should be 
	attached. DMA uses the Source Chain mode. The size of the packet 
	that will be sent equals the number of the vertex x3 (STQ, RGBA 
	and XYZF2) plus 1(GIFtag) and the ID of the DMAtag uses end 
	(because the number of the DMA packet is only one). The GIF uses 
	the PACKED mode and the primitive is set in the GIF tag. The value
	of the REGS is 0x412 (STQ,RGBA and XYZF2).

- Generate STQ, ARGBA and XYZ
    sceVu0RotTransPersNClipCol(&pack->buf[pack->size].ul128, local_screen,
         vertex,normal, texUV,color, local_light,light_color, 
         obj->vertexNum[num]);

	The function calculates the contents of the packet(STQ, ARGBA and
	XYZ) using the sceVu0RotTransPersNClipCol().

Description of the programs (for VU0 macro instructions)
--------------------------------------------------------------------------
The processing of the sceVu0RotTransPersNClipCol() will be described in 
the following section. This is the main part of these sample programs
since the function executes the coordinate and transparent transformation
and the light source calculation using the VU0 macro instructions.
See vu0.c.

- Set the matrixes
    Set the Local-World matrix
    lqc2	vf4,0x0(%1)	#set local_world matrix[0]
    lqc2	vf5,0x10(%1)	#set local_world matrix[1]
    lqc2	vf6,0x20(%1)	#set local_world matrix[2]
    lqc2	vf7,0x30(%1)	#set local_world matrix[3]
    Set the Local-Light matrix
    lqc2	$vf17,0x0(%6)	#set local_light matirix[0]
    lqc2	$vf18,0x10(%6)	#set local_light matirix[1]
    lqc2	$vf19,0x20(%6)	#set local_light matirix[2]
    Set the Light-Color matrix
    lqc2	$vf21,0x0(%7)	#set light_color matrix[0]
    lqc2	$vf22,0x10(%7)	#set light_color matrix[1]	
    lqc2	$vf23,0x20(%7)	#set light_color matrix[2]
    lqc2	$vf20,0x30(%7)	#set light_color matrix[3]		

	First, set the Local-World matrix, Normal-Light matrix and Light-
	Color matrix in the VF register of the VU0.

- Read the vertex, normal line, color of vertex and ST
    lqc2	vf8,0x0(%2)	#load XYZ
    lqc2	$vf24,0x0(%4)	#load NORMAL
    lqc2	$vf25,0x0(%5)	#load COLOR
    lqc2	$vf27,0x0(%8)	#load ST

	Read the coordinate of the vertex, normal line, color of the 
	vertex and the coordinate of the texture necessary to generate
	the packet into the register.

- Coordinate transformation
    # (X0,Y0,Z0,W0)=[SCREEN/LOCAL]*(X,Y,Z,1)
    vmulax.xyzw     ACC, vf4,vf8
    vmadday.xyzw    ACC, vf5,vf8
    vmaddaz.xyzw    ACC, vf6,vf8
    vmaddw.xyzw     vf12,vf7,vf8

	Calculate the screen coordinate by multiplying the Local-Screen
	matrix by the vertex coordinate. In this case, the value W is the 
	same as the value Z in the View coordinate system.

- Transparent transformation
    # (X1,Y1,Z1,1)=(X0/W0,Y0/W0,Z0/W0,W0/W0)
    vdiv    Q,vf0w,vf12w
    vwaitq
    vmulq.xyzw	vf12,vf12,Q
    vftoi4.xyzw	vf13,vf12

	Multiply the screen coordinate by 1/w. Then convert it into the 
	fixed point value for the GIF packet. 1/w used here is kept for
	multiplying by the texture coordinate later.

- Calculate the lighting effect
    # (L1,L2,L3)=[LLM](Nx,Ny,Nz)
    # LLM: Local light matrix
    # L1,L2,L3: Lighting effect
    # Nx,Ny,Nz: Normal line vector
    vmulax.xyzw    ACC, $vf17,$vf24
    vmadday.xyzw   ACC, $vf18,$vf24
    vmaddz.xyzw    $vf24,$vf19,$vf24
    vmaxx.xyz      $vf24,$vf24,$vf0 # Set a negative value to 0

	Calculate the lighting effect by multiplying the local light 
	matrix by the normal line vector. If the direction of the light
	source and the normal line vector is the same, the result takes a 
	negative value. The lighting effect is, therefore, saturated with
	0.	

- Calculate the lighting effect color
    # (LTr,LTg,LTb,LTw)=[LCM](L1,L2,L3,1)
    # LCM: Light color matrix
    # LTr,LTg,LTb: Lighting effect color
    vmulax.xyzw    ACC, $vf21,$vf24
    vmadday.xyzw   ACC, $vf22,$vf24
    vmaddaz.xyzw   ACC, $vf23,$vf24
    vmaddw.xyzw    $vf24,$vf20,$vf0	

	Calculate the lighting effect color by multiplying the light color
	matrix by the lighting effect.

- Calculate the color of the vertex on the screen
    # (RR,GG,BB) = (R,G,B)*(LTr,LTg,LTb)
    # R,G,B: Color of the vertex
    # RR,GG,BB: Color of the vertex on the screen
    vmul.xyzw	$vf26,$vf24,$vf25
    # [0..255] Saturation
    vmaxx.xyz	$vf26,$vf26,$vf0
    lui		$2,0x437f
    ctc2	$2,$vi21
    vnop
    vnop
    vminii.xyz	$vf26,$vf26,I
    vftoi0.xyzw	$vf26,$vf26

	Calculate the color of the vertex on the screen by multiplying the
	lighting effect color by the color of the vertex. The resulting
	value may be saturated because its range is restricted from 0 to 
	255. Further, it is converted into the fixed point value for the 
	GIF packet.

- Calculate ST
    # (S,T,Q) = (s,t,1)/w
    vmulq.xyz	$vf28,$vf27,Q

	Multiply the value of 1/w kept in the transparent transformation
	for a perspective correction.

- Save the value of STQ, RGBA and XYZ
    sqc2	$vf28,0x0(%0)		#store STQ 
    addi	%0,0x10
    sqc2	$vf26,0x0(%0)		#store RGBA 
    addi	%0,0x10
    sqc2	vf13,0x0(%0)		#store XYZ 
    addi	%0,0x10
    #
    addi	%3,-1
    addi	%2,0x10
    addi	%4,0x10
    addi	%5,0x10
    addi	%8,0x10
    bne		$0,%3,_rotTPNCC_loop

	Save the value of STQ, RGBA and XYZ into the packet in accordance 
	with the order set in the GIFtag. Note that if transferring with
	GIF, the order is important since the value of Q in the RGBAQ 
	instruction is the value saved in a internal register immediately
 	before the ST instruction.
	The main loop of the sceVu0RotTransPersNClipCol() is finished in
	this process. Repeat the processing after the reading of the 
	vertex data until reaching the specified number of the vertex with
	incrementing each address. 
	
