[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved


The Sample Program for the VSyncStart Interrupt 


< Description of the Sample >

This sample program is to enhance understanding of threads and the
VSyncStart interrupt handler operations.


< Thread Operations >

main priority 0:
    ReadThread is activated and it is synchronized with the VSyncStart
    interrupt to output character strings to the dsedb console.

ReadThread priority 1:
    The main thread reads files only when it is waiting for the MainSema 
    semaphore.  When the main thread finishes reading files, the 
    ReadThread releases the ReadEnd semaphore and deletes the own thread 
    so that the main function is completed.  


< Interrupt >

The MainSema semaphore is released with every VSyncStart.


< Semaphores >

main       -- MainSema    	Sets the main thread in the Running state 
			  	every interrupt 
ReadThread -- ReadEndSema 	Indicates the end of a file-read 


< Operation of the Program >

Threre are two threads that are main and ReadThread.  Every time the 
VSyncStart interrupt handler operates, the main thread starts operation.
Therfore, the lower priority is assigned to the ReadThread than the main 
thread.


Handler     V          V          V          V
main        |-------   |-------   |-------   |
ReadThread  |       ---|       ---|       -- |

To confirm whether the ReadThread operation has completed, check the
PollingSema function in the main thread.


There are two ways of setting the main thread in the Wait state;

    WaitSema
    SleepThread

Both methods are effective, however, the semaphore is used in this
sample as it is preferable from the point of flexibility.

In the VSyncStart interrupt handler, the main thread is set to the Running
state by calling the iSignalSema function.

Before making the VSyncStart interrupt enabled, set the main thread 
in the Wait state.  This is to avoid the main thread remains in the 
Running state although it should be changed to the Wait state when the
iSignalSema function is called in the handler.

When a read-file is complete, the ReadThread releases the ReadEnd 
semaphore to terminate the main thread.  At the same time, if the
VSyncStart interrupt occurs before the ExitDeleteThread is called, 
control is returned to the main thread and the main thread operation
ends.  To terminate the ReadThread, lowers the priority of ReadThread
than the main thread.


< File 

	vsync.c

< Activating the Program >

	% make		: Compiles the program 
	% make run	: Executes the program

	After the compilation, the program is executable with the following.

	% dsedb
	> run vsync.elf

< Using the Controller >

	None
