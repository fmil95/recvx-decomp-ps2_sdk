[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Sample Reproducing Moving Pictures Simply with IPU

<Description of the Sample>
	This is a simple moving-picture reproduction sample program, 
	which uses only the IDEC command of the IPU.
	
	This program DMA-transfers the whole compressed data to the 
	IPU.  It issues 1 IDEC command every frame and executes frame 
	decoding.  The decoded data is DMA-transferred to the GS via 
	PATH3. 

	The data used in this sample is the necessary number of data 
	(macro blocks), each of which is interpreted by 1 IDEC.  
	Each frame is independent, so no difference is taken among 
	the frames. 

	Since the core is hardly used and VU is not used at all in 
	the EE, it is also easy to multiplex this program with other 
	types of processing such as polygon processing. 
	The image transferred to the frame buffer is displayed as it is 
	in this sample.  However, it is also possible to paste a moving 
	picture to a polygon as a texture by changing the program. 

<File>
        ezmovie.h
        ldimage.c
        main.c
	ez.ipu

<Activating the Program>
	% make		: Compiles the program.
	% make run	: Executes the program.
	
	After the compilation, the program is also executable 
	with the following.

	% dsedb
	> run ezmovie ez.ipu

<Using the Controller>
	Not defined.

<Note>
	The tools used in this sample to create moving-picture data 
	have not been released so far. 
