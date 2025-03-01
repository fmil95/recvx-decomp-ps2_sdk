[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Sample Program for the Data Creation of Key-Frame Matrix Animation
by Interpolationg the Axis of Rotation

< Description of the Sample >
	The program under intr_key allows to perform the key-frame 
	animation by interpolating the axis of rotation with the 
	articulated matrix.

	The mkdata program in this directory creates the parameter 
	sequence for interpolating the axis of rotation from the original
	key-frame matrix sequence and outputs to the file.

< File >
	main.c   Main function, I/O function 
	axis.c   Interpolating the axis of rotation related function 
 	matrix.c Matrix operation function 
	matseq.c Input data (Key-frame matrix sequence)

< Activatig the Program >
	% make		:Compiles the program
	% make run	:Executes the program

	The program is also executable with the following:

	% dsedb
	> run *.elf
	
	The program has successfuly completed if the contents of the 
	output axisseq.dat file is identical to the contents of the 
	axisseq.ref. 
	
< Using the Controller >
	None

< Note >
	The output is written in the text format to the axisseq.dat file.
	The format of this data is the same as the one for the runtime
	sample program.  To output in the binary format, change the 
	writeMatrix() or writeAxis() as necessary.  

