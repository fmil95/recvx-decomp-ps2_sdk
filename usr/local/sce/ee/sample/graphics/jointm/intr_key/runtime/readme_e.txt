[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                    All Rights Reserved

Runtime Sample Program for the Key-Frame Matrix Animation by Interpolating
the Axis of Rotation 

< Description of the Sample >
	The sample program under intr_key allows to perform the key-frame 
	animation with the articulated matrix by interpolating the axis
	of rotation.

	This directory runtime program performs the actual key-frame 
	animation using the parameter sequence for interpolating 
	the axis of rotation.

< File >
	main.c      Main function, General display related function
	matintr.c   Key-frame animation related function
	matintr.h   Header file
	axis.c      Interpolating the axis of rotation related function
	axis.h      Header file
	aseq.c      For reading the parameter sequence data
	aseq.h      Header file
	axisseq.dat Parameter sequence data entity (created by mkdata)
	basic.vsm   VU1 Micro Program 
	dma.dsm     General data for DMA  
	torso.dsm   Model data of the torso part
	head.dsm    Model data of the head part
	arm.dsm     Model data of the arm part
	leg.dsm     Model data of the leg part

< Activating the Program >
	% make		:Compiles the program
	% make run	:Executes the program

	After the compilation, the program is also executable with the 
	following.

	% dsedb
	> run main.elf

< Using the Controller >
	Triangle, Circle,
	Cross, Square, R1, R2	: Changes the camera position 
	start			: Replays 

< Note >
	The parameter sequence for interpolating the axis of rotation can 
	be created by using the sample program in the mkdata directory.	
