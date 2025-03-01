[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

Phoneme Data for Software Syntesizer (Evaluation version)

<Description of the Sample>

	This is a sample program for the phoneme data(SSB) for the CSL
	Software Synthesizer (libssyn).  This is an evaluation version
	and will be refined in future.

	Tone array is GM compatible with a SC88 style apart from the
	specification method for the drum part.  With the GM, the 
	channel #10 acts as the drum part.  On top of that, the drum part
	can be specified exclusively in the GS.  There is no concept of
	drum part in the CSL Software Synthesizer and the drum set is
	allocated to #120 of BankChange(MSB).  Therefore, set BankChange
        (MSB)120 to the channel which should be used as a drum part.

	SMF2SQ has a feature which converts any part of  BankChange(MSB) 
	to the arbitrary value.  However, it does not add the BankChange
        (MSB) specificatoin to a truck, therefore, the conversion is not 
        taken place if the BankChange is not included.
	
	The SSD editor is planned to be embedded in the next releasing
	tool (JAM) which allows to create the drum part individually.  
	Since the exsisting data can be edited, it will be possible to
	delete an unnecessary data in order to compact the data. 

	This phoneme data is free for use as long as it is used for the
	PS2 title otherwise it is prohibited.

<File>
	sce.ssb

<Note>
	None
