[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The Sample Program for Voice Output

<Description of the Sample>
	
	This is a sample program for libsd allowing to operate the SPU2 
	voice output on the IOP.  Batch processing, DMA and IRQ call back
	can be tested optionally.
	
	The VAG file format is the same as of which the current PlayStation.
	Please use the current tool for cretaion.

	Extra care should be taken in selecting core in SPU2.
	Be noted that the output of core 0 passes the main volume of core 1.
	(i.e. In case where the main volume of core 1 is zero, the sound of 
	 core 0 is not produced)
	
	If BATCH_MODE is set to 1, the operation formerly performed in the
	stand-alone API is processed in batch processing.  Writing in the 
	memory of the EE is also tested therefore, activate dsedb and dump
	memory.
	
	If IRQ_CB_TEST is set to 1, the IRQ callback is enabled and when 
	the address 0x1000 from the top (IRQ_ADDR_OFST) in VAG is accessed,
	the message "interrupt detected" is output to the console.
		
	If DMA_CB_TEST is set to 1, the DMA transfer completion callback 
	is enabled and the message "interrupt detected" is output to the
	console on completion of the VAG file transfer.

<File>
	init_bat.h
	main.c	
	piano.vag

<Activating the Program>
	% make		:Compiles the program
	% make run	:Executes the program

	The operation is normal if eight scales per octave is 
	produced in a piano tone.
	
<Using the Controller>
	None

<Note>
	None
