[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program for the multi-thread

<Description of the Sample>

	This program is the sample program for drawing the object using 
	the multi-thread. A semaphore is used to switch the threads.
	The threads are composed of two threads for controlling, one DMA
	kick thread and some object threads. One object thread shows one
	object.

<File>

	thread.c

<Activating the Program>

	% make		: Compiles the program.
	% make run	: Executes the program.
	
	After the compilation, the program is also executable
	with the following.

	% dsedb
	> run thread.elf

<Using the Controller>

	Circle and cross button	: Generates and deletes threads
	Triangle button		: Resets the angle of rotation with random
				  numbers
	Square button		: Initializes the angle of rotation
	R1 and R2 button	: Modifies the object interval
	Left/Right directional button : Translates the object position in 
					X-axis
	Up/Down directional button    : Translates the object position in 
					Y-axis
	L1 and L2 button	: Translates the object position in Z axis
	start button			: Pause
	select + Triangle button	: Initializes (random numbers)
	select + Square button		: Initializes (alignment)
	select + R1 button		: Sets the object interval to 0
	select + R2 button		: Initializes the object interval
	select + Left/Right button	: Initializes the object position 
					  in X-axis
	select + Up/Down button		: Initializes the object position 
					  in Y-axis	
	select + L1 and L2 button	: Initializes the object position 
					  in Z-axis

[Semaphore]

    packet_sema: 
	It indicates whether data can be added to the packet or not.
	(Actually, it can operate without the semaphore)
    send_signal: 
	It indicates that the addition to the packet has been finished
	and DMA transfer is enabled.
    rotate_signal: 
	It indicates that the processing has been finished and the control
	can be transferred to another object.

[Various thread operations]

Object thread: 
If it can acquire packet_sema, it adds the data to the packet. After
returning packet_sema, it sends rotate_signal.

Control thread: 
If the control is transferred, it sends rotate_signal after sending kick_
signal. When the control is transferred, which indicates that the object
processing on the whole has been finished.

DMA kick thread:
It prepares the packet and waits for kick_signal after registering 
packet_sema. When kick_signal is transferred, it postprocesses the packet
and DMA transfers.

Object rotation thread:
It waits for rotate_signal. When the signal is transferred, it rotates the 
subject of control of the thread in priority 2.

[Thread configuration]	

     priority
	0	Object rotation thread
	1	DMA kick thread
	2	Object thread
		Object thread
		: 
		Control thread

