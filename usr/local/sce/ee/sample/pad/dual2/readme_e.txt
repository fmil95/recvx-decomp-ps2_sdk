[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
		Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

The Sample Program for Displaying the Information on Pressure Sensitivity

<Description of the Sample>

	This sample program is for operation to control DUALSHOCK2 and obtain 
	the information on pressure sensitivity with the controller library.  

<File>
        main.c
	 sprt.c
        sprt.h
        pstypes.h

<Activating the Program>

	% make		:Compiles the program

	after compilation, activates dsedb
	% dsedb
	> run main.elf
	

<Using the Controller>

	By pressing the arbitrary buttons except select and start, the 
	corespondent gauge increases or decreases depending on how strong 
	it's pressed.   
	  

<Note>
	Insert the controller into the Controller port 1(port 0).
	This program does not support the connection via the Multi Tap (SCPH-1070).
