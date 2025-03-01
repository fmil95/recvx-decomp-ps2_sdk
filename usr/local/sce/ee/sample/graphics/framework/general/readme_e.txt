[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved
                                                               Aug. 2000

Graphics Framework general sample 

< Description of the Sample >
    
    This sample is aimed to structure an integrated environment having 
    the basic concept and various functions of the integrated graphics 
    environment in sample/graphics/framwork/.

    This program will be updated in the future with enhancement of the 
    structure in sample/graphics/framwork/.

    For overview of the Graphics Framework, refer to the over_e.txt
    file.  


< Files >

    *.h		    : Prototype declaration header

    anim.c          : Functions for animation 
    camera.c        : Functions to set camera parameters
    eval.c          : Interpolation calculation functions 

    framebuf.c      : Functions to manage frame buffer area in texture
    gmain.c	    : Functions to initialize, read, and update data
    hier.c          : Functions to process hierarchical structures and 
		      shared polygons
    main.c          : Main program
    packbuf.c       : Functions to secure a buffer for packets
    vu1frame.c      : Functions to manage model data in a database

    micro.dsm       : File to manage label names in microcode
    vu1base.vsm     : VU1 microcode for 3 parallel light sources
    vu1baseo.vsm    : VU1 microcode (optimized) for 3 parallel light sources
    vu1cull.vsm     : VU1 microcode for 3 parallel light sources and culling
    vu1cullo.vsm    : VU1 microcode (optimized) for 3 parallel light sources 
		      and culling
    dino.s          : Raw format data
    logo.s          : Raw format data
    APEWALK39.s     : Sample data(Hierarchy, Animation)

    dino.hmd        : HMD data
    logo.hmd        : HMD data
    APEWALK39.hmd   : LightWave3D tutorial HMD data

    Model data and/or scene data in this directory are created exporting
    LightWave 3D 5.6 data produced by NewTek Corporation, U.S.A. with Animation
    Saver version 1.8 of D-Storm, Inc.


< Activating the Program >

    % make          : Compiles the program
    % make run      : Executes the program

    After the compilation, the program is also executable with the 
    following:

    % dsedb
    > run main.elf


< Using the Controller >

    Directional button Up/Down 	  : Camera rotation around the x-axis
    Directional button Right/Left : Camera rotation around the y-axis
    L1L2 button   		  : Camera rotation around the z-axis
    Triangle/Cross button	  : Up-and-down movement of camera
				    (direction of the y-axis)
    Square/Circle button	  : Right-and-left movement of camera
				    (direction of the x-axis)
    R1R2 button:		  : Back-and-forth movement of camera
				    (direction of the z-axis)
    START button		  : VU1 microcode switching
    SELECT button		  : Resets the camera


< Overview >

The description of the process is as follows;

 g_init();
				----- Initialization -----
				1, GS double buffer
				2, DMA packet buffer
				3, Camera parameter
				4, VU1 microcode manager

 g_read_model();
				----- Read Data  -----
				1, Initializes the data
				2, Reads the data 
				3, Generates the data buffer
				4, Generates DMA packet
				5, Checks buffer overflow 

 g_main();
				----- Main loop -----
				1, Reads pad information
				2, Calculates preprocessing (animation,
				   hierarchy,share)
				3, Creates data flowchart
				4, DMA transfer
				5, Status display
				6, Returns to 1

 g_finish();
				----- Termination process -----
				1, Clears DMA packet buffer
				2, Clears data buffer
				3, Initializes variables
				4, Initializes pad

< Remarks >

    For details of the data format, refer to doc/es2raw_e.txt in the 
    eS package.

				
< Restrictions and Note >

    Users are required to set the flag to switch resident or transient. 
    The default setting is flag 0, transient texture.  When setting the
    resident texture flag, refer to es2raw_e.txt and specify 1.
    Each base pointer is fixed with #define.  
    To change the setting, refer to framebuf.h.

    The matrix stack size of a hierarchical structure is limited to 30.
    To change the stack size setting, refer to hier.c.

    The rotation orders of the hierarchical structure and animation are in 
    the order of YXZ in accordance with LightWave 3D.  To change the rotation 
    order setting, refer to hier.c.  In the future, the rotation order will
    be reflected in the data.

    This does not support the tiling texture. 


  Notes on Trademarks
  ===================
  "LightWave 3D(TM)" is a trademark of NewTek Coporation, U.S.A.

