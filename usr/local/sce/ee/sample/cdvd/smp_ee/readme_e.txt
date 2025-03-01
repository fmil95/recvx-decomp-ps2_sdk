[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The call sample of the CD/DVD-ROM Drive command function  

< Description of the Sample >
	This is a test program of the command function of the CD/DVD-ROM.

	1. Initialization of the CD/DVD-ROM Drive
	2. Call of the command function
	3. Displaying the result of the command function
	4. Exchange of media

	Processes 1 to 4 are repeated in the above order.

< File >
	main.c

< Activating the Program >
	% dsreset 0 0
	% make		: Compiles the program

	After compilation, set the CD of PlayStation on CD/DVD-ROM Drive 
	and activate dsedb.

	% dsedb

	Execute the following.

	> run main.elf

	Exchange media when a message requesting an exchange of media 
	is indicated.

< Using the Controller >
	None

< Note >
	
