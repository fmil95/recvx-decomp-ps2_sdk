[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

SIF CMD Protocol Sample Program  

< Description of the Sample >
	This sample program for the SIF CMD protocol allows to perform  
	the following in sequence.

	1. Initialization of the SIF CMD 
	2. Registration of the buffer and command function 
	3. Calling the command function of the other end 
	4. Displaying the result of the command function


< File >
	main.c

< Activating the Program >
	% dsreset 0 0
	% make		:Compiles the program

	After compilation, the program activates dsedb

	% dsedb

	Opens another window subsequently, and activates dsidb from
	the IOP sample(iop/sample/sif/sifcmd).

	Executes the following in the EE sample window: 

	> run main.elf


	Executes the following in the IOP sample window:

	> mstart iopmain.irx

	test0 = 10 test1 = 20

	The above is displayed if the program has completed successfuly.


< Using the Controller >
	None

< Note >
	
