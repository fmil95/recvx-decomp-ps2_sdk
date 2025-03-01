[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The Sample Program for the Thread Ready Queue Rotation

< Description of the Sample >

This sample program is to enhance understanding of the thread 
ready queue operation.


< Thread Operations >

main priority 0:
    PrintThread 1, PrintThread 2 and PrintThread 3 are created and 
    activated.  They are put in the Ready state and their priorities are 
    lowered to 10.

PrintThread1 priority 1:
    The thread ID is output to the dsedb console and the rotation of the 
    thread ready queue is repeated.

PrintThread2 priority 1:
    The thread ID is output to the dsedb console and the rotation of the 
    thread ready queue is repeated.

PrintThread3 priority 1:
    The thread ID is output to the dsedb console and the rotation of the 
    thread ready queue is repeated.


< Operation of the Program  >

The PrintThread 1, 2 and 3, that are activated from the main thread, are 
in the Ready state.  Since these threads are in the same priority, they 
are registered to the thread ready queue in the activated order.

If the priority of main is lowered to 10, the PrintThread 1 goes into the 
Running state then the thread ID is output to rotate the thread ready 
queue.  The PrintThread 1 is moved to the end of the thread ready queue 
and the PrintThread 2 goes into the Running state.  The same is executed 
for the PrintThread 3.  The operation terminates after repeating this 
operation four times.  When the last PrintThread 3 is terminated, the 
control returns to the main thread and the program terminates properly.


< File >

	rotate.c

< Activating the Program >

	% make		: Compiles the program
	% make run	: Executes the program

	After the compilation, the program is executable with the following.

	% dsedb
	> run rotate.elf

< Using the Controller >

	None
