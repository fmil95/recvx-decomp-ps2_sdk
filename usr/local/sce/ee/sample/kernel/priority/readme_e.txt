[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The Sample Program for the Priority of Threads


< Description of the Sample >

This sample program is to enhance understanding of the priority of threads.

< Thread Operations>

main priority 0:

    ReadThread and PrintThread are created and activated.  Their 
    priorities are lowered to 10 to become the Ready state.
    
ReadThread priority 1:
    The thread ID is output to the dsedb console, and a file read is 
    continued.

PrintThread priority 2:
    The thread ID is output to the dsedb console.

< Semaphore >

ReadEndSema -- This semaphore indicates the termination of the ReadThread 
	       file read.

< Operation of the Program >

The ReadThread and PrintThread, that are activated from the main thread, 
are in the Ready state.


If the priority of the main thread is lowered than that of the ReadThread 
and PrintThread, the ReadThread that is in a higher priority goes into the
Running state.  If the ReadThread is changed to the Wait state the 
PrintThread, that is in a lower priority, goes into the Running state.


When the ReadThread finishes reading the file, it releases the ReadEndSema
and terminates the PrintThread.  After the termination of the ReadThread, 
the main thread operation ends.


< File >

	priority.c

< Activating the Program >

	% make		: Compiles the program
	% make run	: Executes the program

	After the compilation, the program is executable with the following.

	% dsedb
	> run priority.elf

< Using the Controller >

	None

