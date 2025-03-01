[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The Sample Program Calling the IOP Stream Function 

< Description of the Sample >
	This is a test program of the libcdvd stream function.
	This sample program performs the following.

	1. Initializes the CD/DVD-ROM related programs
	2. Calls the stream related functions

< File >
	main.c

<Activating the Program >
	% dsreset 0 0
	% make		: Compiles the program

	Load the "PlayStation" CD into the CD/DVD-ROM drive.
	Open two windows.  Firstly, activate dsidb in the window,
	then execute the program in the other window following the
	steps below.

	% make run	Executes the program

	If sample end. appears on the dsidb window and the program 
	file of the "PlayStation" CD is copied to the current directory, 
	the program has been successfully completed.

< Using the Controller >
	None

< Note >
	None

