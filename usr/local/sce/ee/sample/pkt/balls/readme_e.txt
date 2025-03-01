[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Sample of Scratch Balls

<Description of the Sample>
	This is a sample program which draws a polygon from the 
	scratchpad to the VIF1 directly via PATH2 by previously 
	creating a packet on the scratchpad and rewriting only x and 
	y fields for high-speed processing.

	The DMA transfer to the scratchpad is used when reading the 
	database for the balls as well. 

<File>
	balltex.s
	main.c

<Activating the Program>
	% make		: Compiles the program.
	% make run	: Executes the program.

	After the compilation, the program is also executable 
	with the following. 
	% dsedb
	> run main.elf

<Using the Controller>
	 Up directional button    : Increases No. of balls.
	 Down directional button  : Decreases No. of balls.

<Note>
	None.

