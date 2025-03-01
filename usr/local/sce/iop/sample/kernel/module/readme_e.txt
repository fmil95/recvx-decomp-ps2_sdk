[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Resident Module Smaple Program


< Description of the Sample >

This sample program allows to create the resident library module. 


< File >

	mylib.c			Sample source for the resident library
	mylib.tbl		Sample for the entry table of the resident
				library
	client.c		Sample for the program using the resident 
				library

< Activating the program >

	% make						:Compiles the program
	% dsreset 0 2
	% dsistart mylib.irx; dsistart client.irx	:Executes the program

	The program is also executable with the following:
	% dsidb
	> reset 0 2 ; mstart mylib.irx; mstart client.irx
