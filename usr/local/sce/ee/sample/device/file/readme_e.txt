[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Sample to Use Files in Host Machine

<Description of the Sample>
	This is a sample program, which accesses the files in the host 
	machine.
	1. Makes a file named "copy.c", a copy of the content of 
	   the file "main.c".
	2. Makes incremental data every 4 bytes, and writes the data 
	   to the file named "incdata". The incdata is read and 
	   verified whether or not it is correct. 

<File>
	main.c

<Activating the Program>
	% make		: Compiles the program.
	% make run	: Executes the program.

	After the compilation, the program is also executable 
	with the following.
	% dsedb
	> run main.elf

	The program ends normally with the indication of "sample end".
	Whether the file copy.c and the file main.c are the same or not 
	is confirmed. 
	% diff main.c copy.c
 	
<Using the Controller>
	Not defined.

<Note>
	None.

