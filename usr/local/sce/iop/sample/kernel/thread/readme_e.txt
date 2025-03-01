[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The Sample Program for the Multithread 

< Description of the Sample >

These are basic samples for generating and activating threads, 
operating a thread priority and synchronizing threads and so on.


< File >

	createth.c		Basic sample for generating and
				activating threads
	sleepth.c		Sample for SleepThread()/WakeupThread() 
	eventth.c		Sample for SetEventFlag()/WaitEventFlag() 
	startthargs.c		Sample for activating threads

< Activating the Program >

	% make					:Compiles the program

    createth execution

	Firstly, execute the following on another window
	% dsicons 

	subsequently execute the below

	% dsreset 0 2
	% dsistart createth.irx

	State of the six threads activation is displayed on the dsicons
	executed window.


	Also executable with the following
	% dsidb
	> reset 0 2 ; mstart createth.irx


    sleepth execution

	Firstly, prepare two windows and execute the below 
	in each window
	% dsicons 0
	and
	% dsicons 1-9

	subsequently, execute the below in the third window
	% dsreset 0 2
	% dsistart sleepth.irx

	The prompt '0..5 or a..f or A..F > ' appears on the dsicons 0 executed
	window.  At this time, the command input thread in an input-waiting
	status with the prompt appeared and another six threads are activated. 
	The input-waiting status thread with the prompt appeared are in higher 
	priority than the rest of six threads but since they are in the WAIT 
	status, each of those six threads acquires the execution right and 
	displays the start message.  
	
	Enter numeric characters 0 to 5 and hit the RETURN key to perform
	WakeupThread execution of the corresponding thread.
	
	Enter characters a to f and hit the RETURN key to perform 
	WakeupThread execution of the corresponding threads #0 to #5 twice.
	
	Enter characters A to F and hit the RETURN key to perform WakeupThread
	execution of the corresponding threads #0 to #5 four times.
	
	Also executable with the following
	% dsidb
	> reset 0 2 ; mstart sleepth.irx


	If the following are activated in each window, the individual
	thread's status can be displayed every TTY which were opened by
	each thread. 
	
	% dsicons 1
	% dsicons 2
	% dsicons 3
	% dsicons 4
	% dsicons 5


    eventth execution

	Firstly, prepare two windows and execute the below 
	in each window
	% dsicons 0
	and
	% dsicons 1-9
	

	subsequently, execute the below in the third window
	% dsreset 0 2
	% dsistart eventth.irx

	The prompt '0..5,a > ' appears on the dsicons 0 executed window.
	At this time, the command input thread in an input-waiting status 
	with the prompt appeared and another six threads are activated. 
	The input-waiting status thread with the prompt appeared are in higher 
	priority than the rest of six threads but since they are in the WAIT 
	status, each of those six threads acquires the execution right and 
	displays the start message.  

	Enter numeric characters 0 to 5 and hit the RETURN key to flag the
	corresponding bit of the event flag. 

	Enter a and hit the RETURN key to flag all bits of the event flag.

	Also executable with the following
	% dsidb
	> reset 0 2 ; mstart eventth.irx

	If the following are activated in each window, the individual
	thread's status can be displayed every TTY which were opened by
	each thread. 
	% dsicons 1
	% dsicons 2
	% dsicons 3
	% dsicons 4
	% dsicons 5


    startthargs execution

	Firstly, execute the following on another window 
	% dsicons 
	
	subsequently, execute the below

	% dsreset 0 2
	% dsistart startthargs.irx

	State of the thread's starting argument is displayed on the dsicons
	executed window.

	Also executable with the following
	% dsidb
	> reset 0 2 ; mstart startthargs.irx
