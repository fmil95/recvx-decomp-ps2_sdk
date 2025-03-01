[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved


The Sample Program for the Communication Mode via i.LINK (IEEE1394) 

< Description of the Sample >

	This sample program enables two PlayStation 2 consoles to 
	bidirectionally send/receive automatically generated data via a 
	protocol of SCE's own, i.LINK socket, and the monitor screen to 
	display the results.
  
	This sample corresponds to the one in the IOP and is functional 
	in a pair.  The program of the EE automatically loads the module 
	of the IOP, therefore, please build the sample of the IOP 
	(iop/sample/ilink) in advance.

	- Connection
	As the GUID (1394 Global Unique ID) of the other PlayStation 2 is 
	not known initially, the data is broadcasted while the receiving 
	thread monitors the socket and checks the source GUID of the 
	received packet.  If the GUID belongs to other nodes, the 
	PlayStation 2 registers it as a partner and makes a connection to 
	it.  For further communication, the PlayStation 2 sends the packet
	only to the registered nodes.  This sample program is not based on
	the assumption that the i.LINK is disconnected during the 
	communication.

	- Display
	This sample program displays whether the checksum attached to 
	the received data is correct.  Since data is asynchronously 
	received, the blank line is displayed when there is no data to be
	received.  If there is an excess of data, it is stored in FIFO, 
	and the data is retrieved in order of receipt.

	- Complementary Note on the Sample
	Data is transferred asynchronously/synchronously by 
	enabling/disabling "#define SEND_NO_WAIT" at the start of eemain.c.
  	The only difference between synchronous and asynchronous modes is 
	that the former uses SIF_RPCM_NOWAIT when performing RPC with 
	socketee.c, whereas the latter does not.
	Data is always received asynchronously.
 
	- i.LINK Socket Driver
	Please broadcast data via i.LINK socket only when it is absolutely
	necessary, since it puts a heavy load on the IOP and the number of
	the recipients of the data increases in proportion to the number of
	the nodes existing on 1394 bus.
	
	492 bytes are transmissible or receivable per packet as the 
	maximum payload (the user can use freely) when using the i.LINK 
	socket.  The data transfer rate of this sample program is 
	492 x 7 x 60=206640 bytes/sec., since it transfers/receives 7 
	packets per VSync.

	If a retrieval of a received packet from the i.LINK socket driver 
	via sceILsocketRecv or other functions takes more time than the 
	arrival of a packet, the FIFO in the driver overflows. 
	The size of this FIFO is set to 7 packets as default.
	Packets exceeding this value are not stored in the FIFO and are 
	disposed of.

< File >
	eemain.c        Main Program
	socketee.c      RPC client

< Activating the Program >
	Connect two PlayStation 2 consoles via the i.LINK cable for S400.
	(Sony VMC-IL4415, 4435 etc.)

	% make  	: Compiles the program of IOP (iop/sample/ilink)

	Move to the sample directory of EE (ee/sample/ilink)
	% make		: Compiles the program of EE

	After the compilation, activates dsedb on both of PlayStation 2 
	consoles.

	% dsedb
	> run eemain.elf

< Using the Controller >
	None

< Screen Display >
	Node ID	  : Own node ID 
	Src	  : Own GUID (64 bits)
	Dest	  : Partner's GUID (64 bits)
	Status	  : Link status of own console 
			Single:  Disconnected 
			Connect: Connected
	Elapsed	  : Elapsed time since the start of program execution 
		    (Time: Minute: Second: Frame)
	nRemHDPkt : Count of removals of half done packets (*) from 
		    the FIFO created by the application
		    (* If abnormal continuity is detected when receiving 
		     packets in 1 VSync, they are removed from the FIFO.)
	nRetryCnt : Count of data transfers retried due to an error in 
		    synchronous transfer mode 
		    Not displayed in asynchronous transfer mode.
	TxFrm	  : Transfer frame number (second: 1/60 sec.)
	nRx	  : Number of packets remaining in the receiving FIFO
	RxFrm	  : Receive frame number (second: 1/60 sec.)
	SumN...   : Checksum result of the Nth packet 

< Note >
	None
