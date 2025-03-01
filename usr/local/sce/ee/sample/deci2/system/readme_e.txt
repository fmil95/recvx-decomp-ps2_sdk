[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The sample program for executing the system() function on the host with 
DECI2 protocol

<Description of the Sample>
	This program is the sample program for executing the system()
	function on the host from a target program with DECI2 protocol 
	and for displaying the result.

<File>
	main.c        : Main program in the target side
	pdriver.c     : Driver program in the sample protocol
	pdriver.h     : Driver header in the sample protocol
	linux/main.c  : Main program in the host side
	linux/deci2.h : DECI2 protocol header
	linux/dcmp.h  : DCMP protocol header
	linux/netmp.h : NETMP protocol header

<Activating the Program>
	$ make : Compiles the program
	
	After the compilation, 
	executes $ cd linux and $ ./hsys -d targetname (or IP address)
	Opens the other window

	$make run
		or
	$ dsedb
	> run main.elf

<Note>
	In this sample protocol, the protocol number is defined as 0xe000.
	The packet format preceded by DECI2 is as follows.

       3                   2                   1                  
     1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    |                            Result                             |
    +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
                                                          Data ...  |
                                                       -+-+-+-+-+-+-+

	Result 
	  Set 0 when required the command execution from the target
	  program and the return value of system() when replying to
	  the host.	

	Data
	  It is null-terminated strings.
	  Set command strings when required the command execution from the
	  target program and the standard output result of the command 
	  executed in system() when replying to the host.

	The outline of the processing is as follows.

	For the host side:
	1. Establishes the connection with dsnetm on TCP/IP level.
	2. Establishes the connection on deci2 level using the CONNECT
	   message of netmp protocol.
	3. Waits for the request from the target to come infinitely.
	4. When requested the command execution from the target, executes
	   the command and replies the result to the target after saving 
	   it in a temporary file.

	For the target side
	1. Registers the protocol driver.
	2. Sends the command to the host.
	3. Waits for the result of the command to return infinitely.
	4. Displays the result when the result returns.

