[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
		Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

The Sample Program for Displaying the Controller Information

<Description of the Sample>

	This sample program is for a series of controller operations
	such as	obtaining the button information or setting the 
	actuator etc. with the controller library.  However, the
	actuator controll only supports DUALSHOCK.  

<File>
        main.c

<Activating the Program>

	% make		:Compiles the program

	after compilation, activates dsedb
	% dsedb
	> run main.elf
	

<Using the Controller>

	(when connected to DUALSHOCK)
	Directional button up   :	Rotates the small motor 
	Directional button down :	Halts the small motor 
	Directional button left	:	Rotates the large motor
	Directional button right:	Halts the large mortor

<Note>
	Insert the controller into the Controller port 1(port 0).
	This program does not support the Multi Tap (SCPH-1070).
