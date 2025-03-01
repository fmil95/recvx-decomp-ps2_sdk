[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved


The Sample Program for the Communication Mode via i.LINK (IEEE1394) 

< Description of the Sample >

	This sample program enables two PlayStation 2 consoles to 
	send/receive automatically generated data via a protocol of SCE's 
	own, i.LINK socket, and the monitor screen to display the results.
   
	This sample corresponds to the one in the EE and is functional in 
	a pair.  Please refer to the readme file of the sample for EE 
	(ee/sample/ilink/readme_e.txt) for details.
	
< File >
	ilsample.c	RPC server

        IOP modules automatically loaded
	ilink.irx (/usr/local/sce/iop/modules)
		 	i.LINK basic driver
	ilsock.irx (/usr/local/sce/iop/modules)
			i.LINK socket driver

<Activating the Program >
	Refer to the readme file of the sample for EE.

< Using the Controller >
	None

< Note >
	None
