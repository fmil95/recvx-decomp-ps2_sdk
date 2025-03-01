[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved


A sample program for calling the stream function of the IOP

< Description of the Sample >
	This is the test program of the stream function of libcdvd.
	The following is performed in the order presented.

	1. Initializing a CD/DVD-ROM Drive
	2. Calling the stream function
	3. Playing the ADPCM

< File >
	main.c

< Activating the program >
	% dsreset 0 0
	% make		: Compiles the program

	Load the media on which a sample data "PS_MONO.VB" is written 
	into a CD/DVD-ROM Drive.

	Open two windows.
	Activate dsidb on the window.
	Subsequently run a program by the following procedure
	and execute the program on the other window.
	Executes
	% make run 

	sample end.


	The above is displayed on a dsidb window and the ADPCM
	is played if the program has completed successfuly.


< Using the Controller >
	None

< Note >	
	None
