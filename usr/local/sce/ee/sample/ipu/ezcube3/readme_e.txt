[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program for decoding more than one compressed data

<Description of the Sample>
	This sample program is the extended version of the program in 
	/sample/ipu/ezcube. It decodes more than one compressed data
	one by one, DMA transfers to the texture buffer and shows
	them as a texture. It uses IPU efficiently by decoding the next
	data immediately after finishing decoding, with DMA ending
	interruption from IPU.

	
<File>
	main.c
	ldimage.c
	ezcube.c
	ezcube.h
	ezmovie.h
	ez.ipu
	rugby.ipu
	456.ipu
	1_6.ipu
	wada.ipu
	yama.ipu

<Activating the Program>
	% make		: Compiles the program
	% make run	: Executes the program

	After the compilation, the program is also executable with the
	following.
	
	% dsedb
	> run main.elf

<Using the Controller>
	Up/Down directional button	: Rotation around x-axis
       	Left/Right directional button   : Rotation around y-axis
 	L1,L2 button          : Rotation around z-axis 
	R1,R2 button	      : Translation of the cube around z-axis
        Triangle/Cross button : Rotation around x-axis of the camera 
        Circle/Square button  : Rotation around y-axis of the camera 
	START button          : Reset


