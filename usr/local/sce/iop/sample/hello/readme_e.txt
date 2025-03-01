[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The Sample Program for the hello print 

< Description of the Sample >

 Subsequent to displaying "hello !\n", the arguments which were passed to
 the program are appeared one by one.

< File >

	thread.c

< Activating the Program >

	% make							  :Compiling 
	% dsreset 0 2 ; dsistart hello.irx   arbitrary arguments  :Executing 

	Also executable with the following. 
	% dsidb
	> reset 0 2 ; mstart hello 	     arbitrary arguments
