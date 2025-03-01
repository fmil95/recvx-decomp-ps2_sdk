[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

SIF RPC Protocol Sample Program

< Description of the Sample >
	This sample program is for the SIF RPC protocol.
	The server client operation is performed in the EE/IOP.
	Enabled "#define SERVER" is used as a server.

	Data is created in the client and sent to the server.
	The data received by the server is manipulated and sent back to 
	the client.  

	Server is called in some modes.

< File >
	iopmain.c

<Activating the Program >
	% dsreset 0 0
	% make		:Compiles the program

	After compilation, the program activates dsidb.

	% dsidb

	Opens another window subsequently, and activates dsedb from
	the EE sample (ee/sample/sif/sifrpc).
       
	Executes the following in the IOP sample window:

	> mstart iopmain.irx
	
	Executes the following in the EE sample window: 

	> run main.elf
	
	If the data sent by the server masks 0xff000000 and sends it
	back to the client, the program has completed successfully.

< Using the Controller >
	None

< Note >
	The program in this version is about the same as the program 
	in the EE.
