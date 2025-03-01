[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The Sample Program for Semaphores 

< Description of the Sample >

This sample program is to control thread operations using a VSync 
interrupt and a semaphore.


< Thread Operations >

main priority 0:
    semamain, sema1, sema2 and sema3 are created.
    PrintThread1, PrintThread2 and PrintThread3 are created and activated
    then they are put in the Ready state.  Subsequently, a VSyncStart interrupt
    is enabled and the state is shifted to the Wait state with the WaitSema
    function.  
    
PrintThread1 priority 1:
    After the semaphore sema 1 is obtained, the operation of outputting the
    thread ID to the debugging station is repeated.

PrintThread2 priority1:
    After the semaphore sema 2 is obtained, the operation of outputting the
    thread ID to the debugging station is repeated.

PrintThread3 priority1:
    After the semaphore sema 3 is obtained, the operation of outputting the
    thread ID to the debugging station is repeated.


< Interrupt >

The semaphore sema 1, sema 2 and sema 3 are returned every time 
the interrupt occurs.


< Semaphores >

Corresponding relationships
main         -- semamain    Terminates main 
PrintThread1 -- sema1       Operates PrintThread 1
PrintThread2 -- sema2       Operates PrintThread 2
PrintThread3 -- sema3       Operates PrintThread 3


< Operation of the Program >

After the PrintThread 1, 2 and 3 are terminated properly, the priority
of the main thread is lowered than that of the PrintThread 1, 2 and 3.
The main thread goes into the Wait state after the VSyncStart interrupt is
enabled.

The VSyncStart interrupt occurs after the main thread goes into the Wait
state.  Every time the interrupt occurs, the semaphore sema 1, sema 2 and
sema 3 are returned.

The PrintThread 1, 2 and 3 that are activated from the main thread, go 
into the Running state and is changed to a wait state with the WaitSema
function.  The semaphore sema 1, sema 2 and sema 3 are returned every time 
the VSyncStart interrupt occurs.  When the threads exit from the interrupt
handler, the PointThread 1, 2 and 3 are registered to the thread ready queue
then executed in the order that they go into the ready state since these
threads are in the same level of priority.

After this operation is repeated four times and before the ExitDeleteThread
function is called by the PrintThread3 that goes into the Running state last, 
the semamain semaphore of the main thread is released.  By doing this, the 
operation returns to the main thread that has lower priority after the 
termination of the PrintThread 3 then the program is terminated properly.


< File >

	semaphore.c

< Activating the Program >

	% make		: Compiles the program
	% make run	: Executess the program

	After the compilation, the program is executable with the following.

	% dsedb
	> run semaphore.elf

< Using the Controller >

	None
