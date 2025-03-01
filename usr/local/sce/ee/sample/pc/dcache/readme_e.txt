[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program for counting D Cache miss with the Performance Counter

<Description of the Sample>

	This program is the sample program for counting D Cache miss with
	the Performance Counter. It has four examples.

	In the first example, it sets the "Load completed " event to 
	counter 0 and "D Cache miss" event to counter 1, both of which 
	measure 1q instruction toward a cache area. D cache miss is 
	generated in the first access, but not generated in the following 
	three accesses since one line of the cache is 64-byte.

	In the second example, above measurements are executed toward a 
	non-cache area.
	
	In the third and forth examples, above two measurements are 
	executed by setting the counter 0 event to "CPU cycle".

<File>
	main.c        : Main program

<Activating the Program>
	$ make		: Compiles the program.
	$ make run	: Executes the program.
		
	After the compilation, the program is also executable with the 
	following.

	$ dsedb
	> run main.elf

<Note>
	None
