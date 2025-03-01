[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program for downloading the program and simulating the mouse
on the host with DECI2 protocol

<Description of the Sample>
	This sample program shows how to download the program to
	the target and run it. And then the program on the host continues
	to send the mouse event to the target program. The target program
	simulates the pseudo-behaviour of the mouse by getting the
	mouse event with DECI2 protocol.

<File>
	main.c        : Main program in the target side
	vu0.c         : Matrix operation functions
	pdriver.c     : Driver program in the sample protocol
	pdriver.h     : Driver header in the sample protocol
	linux/main.c  : Main program in the host side
	linux/main.h  : Header file
	linux/child.c : child process program
	linux/deci2.h : DECI2 protocol header
	linux/dbgp.h  : DBGP protocol header
	linux/dbgp.c  : DBGP program
	linux/dcmp.h  : DCMP protocol header
	linux/dcmp.c  : DCMP program
	linux/elf.c   : ELF analysis routines
	linux/list.h  : list operation header
	linux/list.c  : list operation routines
	linux/netmp.h : NETMP protocol header
	linux/netmp.c : NETMP program

<Activating the Program>
	$ make		: Compiles the program
	$ make run	: Executes the program
	
	Or, After the compilation, 
	$ linux/mmouse main.elf

<Note>
	In this sample protocol, the protocol number is defined as 0xe000.
	The packet format preceded by DECI2 is as follows.
	This sample needs X-Window is started.

       3                   2                   1                  
     1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               x                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               y                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                               b                               |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

	x 
	  x coordinate

	y
	  y coordinate

	b
	  button information

	The outline of the processing is as follows.

	For the host side:
	1. Establishes the connection with dsnetm on TCP/IP level.
	2. Establishes the connection on deci2 level using the CONNECT
	   message of netmp protocol.
	3. resets the target.
	4. Waits for the establishment of the connection to EE.
	5. Downloads the program
	6. Waits the new protocol to be opened by the target program.
	7. Sends the mouse information when the mouse event occures.

	For the target side
	1. Registers the protocol driver.
	2. Displays the mouse cursor, and update it by getting the mouse
	   information from the host.
