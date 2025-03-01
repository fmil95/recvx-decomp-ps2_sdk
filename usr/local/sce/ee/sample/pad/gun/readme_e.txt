[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
            Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                               All Rights Reserved

Sample Program for the Gun Controller 

< Description of the Sample >

	This is a sample program for obtaining the coordinate information
	on the gun controller (SCPH-00034) and displays it on a screen.

< Files >
        main.c
        sprt.c
        sprt.h
        pstypes.h

< Activating the Program >

	% make		: Compiles the program

	After compilation, activates dsedb.

	Executes the following.
	% dsedb
	> run main.elf


< Note >
        When using this program via a multitap (for PlayStation 2), 
	insert it into the controller port A.

