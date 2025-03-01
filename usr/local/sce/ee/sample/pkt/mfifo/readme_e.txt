[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0

                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program for MFIFO

<Description of the Sample>
	This program is the sample program for drawing the object using 
	MFIFO. Double buffering and ringbuffering are executed in SPR and
	the main memory respectively by DMA transfer from SPR to the main
	memory and from the main memory to GIF.
	And MFIFO area reserves the section for MFIFO area by compiling 
	.cmd file.
<File>

	mfifo.cmd

<Activating the Program>

	% make		: Compiles the program.
	% make run	: Executes the program.

	After the compilation, the program is also executable with the
	following.

	% dsedb

	> run mfifo.elf

<Using the Controller>

	Circle/Cross button	: Increases or decreases the object.
	Triangle/Square button	: Increases or decreases the object size.
	R1 and R2 button	: Modifies the object interval.
	Left/Right directional button	: Translates the object position 
					  in X axis.
	Up/Down directional button	: Translates the object position 
					  in Y axis.
	L1 and L2		: Translates the object position in Z 
				  axis.
	start button		: Pauses.
	select + Circle/Cross button	: Initializes the number of the 
					  object.
	select + R1 button		: Sets the object interval to 0.
	select + R2 button		: Initializes the object interval.
	select + Left/Right directional button	: Initializes the object
						  position in X axis.
	select + Up/Down directional button	: Initializes the object
						  position in Y axis.
	select + L1 and L2 button	: Initializes the object position
					  in Z axis.
	select + start		: Initializes the object position in X, Y,
				  and Z axis.

<Note>
	# Notes on MFIFO

 -	The size of the Ringbuffer(MFIFO_SIZE) must be qword of n-th power
	of 2. Therefore, MFIFO_SIZE-0x10 must be set in D_RBSR. In fact,
	MFIFO_SIZE is like 0x00200000 or 0x00008000, and 0x001FFFF0 or 
	0x00007FF0 is set in D_RBSR.
	
 -	The ringbuffer area must be aligned on MFIFO_SIZE. There are
	three ways for realizing this.
	(A) Specifies the alignment when reserving the area as a global 
	    variable.
	(B) Uses the area which users are allowed to use with the kernel.
	(C) Reserves the section for the buffer area by compiling .cmd 
	    file. 
	
	(A)	char base[SIZE] __attribute__ ((aligned(SIZE)));
	As above, "aligned" is used to specify. A maximum SIZE is 32KB
	(0x8000) as a limitation of "aligned". Therefore, if using the
	ringbuffer over 32KB, uses the following method.

	(B)	char* base = (char*)0x00100000;
	As above, uses the area by specifying an available area for user
	(0x00100000 to 0x00200000) with a pointer. Users have to manage 
	the available area for user.

	(C)	.mfifo		ALIGN(0x40000): { *(.mfifo) }
	As above, adds the section in .cmd file and reserves the area by
	specifying the section like char base[0x40000] __attribute__ 
	((section(".mfifo")));.

 -	D_RBOR AND D_RBSR must be 0. It will always be 0 if above 
	conditions are satisfied. In fact, when MFIFO_SIZE is 0x00040000,
	D_RBOR and D_RBSR is like 0x00240000 and 0x0003FFF0 respectively.

 -	STR bit in the drain channel must be set to 1 after setting
	D_RBOR, D_RBSR and D_CTRL.MFD and before sending the source
	channel (DMA8).

 -	STR bit in the drain channel will be set to 0 when ENDtag is sent.

 -	Note that the registers related to MFIFO except QWC use the unit 
	of qword, but QWC uses that of byte.

 -	To judge whether the source channel can transfer to MFIFO, uses 
	the following expression.
	
	remain = (tadr==madr) ? MFIFO_SIZE

		: (tadr+MFIFO_SIZE-madr)&(MFIFO_SIZE-0x10);

	It calculates the capacity of writable area (remain). The transfer
	is enabled when remain is lager than the data amount of the source
	channel supposed to be transferred.

	# Note when using the GIF channel (path3) as the drain channel

 -	Since sceGsSwapDBuff switches DoubleBuffer via GIF, D_CTRL.MFD
	must be returned to 0, or MFIFO must be finished. According to 
	that, the contents of D8_MADR, D2_TADR and D2_CHCR etc. must be 
	reset. If using the VIF1 channel (path2), MFIFO can continue to be
	used.

	# Others

 -	Since sceGsSwapDBuff switches DoubleBuffer via GIF, if the  
	sceGsDBuff variable is on cache, the cache must be writebacked 
	with SyncDCache and so on after changing the value.

