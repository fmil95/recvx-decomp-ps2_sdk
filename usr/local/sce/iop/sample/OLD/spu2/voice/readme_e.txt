[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 1.6
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The Sample Program for the Voice Pronunciation

<Description of the Sample>

	The program is for the SPU2 voice pronunciation on the IOP using  
	the interim sound library, libspu2. Tests for DMA and IRQ 
	callback are possible optionally.

	The VAG file formats are the same as those of current PlayStation.
	Uses the current tool for generating.

	The usage of API is quite similar to that of current PlayStation,
	but the core has to be selected in SPU2. Note that the output of 
	core 0 goes through the main volume of core 1, that is, if the 
	main volume of core 1 is 0, the tone of core 0 is not pronounced 
	either.

	If IRQ_CB_TEST is set to 1, the IRQ callback is enabled and when 
	the address of 0x1000 from the first (IRQ_ADDR_OFST) in VAG is 
	accessed, the message "IRQ interrupt detected" is output in the
	console.

	If DMA_CB_TEST is set to 1, the DMA transfer completion callback 
	is enabled and when the transfer of the VAG file is completed, 
	the message "DMA interrupt detected" is output in the console.

<File>
	main.c	
	piano.vag

<Activating the Program>
	% make		:Compiles the program.
	% make run	:Executes the program.

	The program is normal if eight notes in one octave can be 
	pronounced on Piano.

<Using the Controller>
	None

<Note>
	None
