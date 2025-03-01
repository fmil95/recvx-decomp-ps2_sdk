[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

The Sample Program to Obtain the System Setting on the PlayStation 2

< Description of the Sample >

This sample program is for obtaining the system setting information on the
PlayStation 2 and to output it to the console.  The time stamp used in 
this sample is obtained from system settings.

System settings are not implemented on the DTL-T10000.  Therefore,
the setting file (T10000.scf) is provided to make the DTL-T10000 specific
settings with the sceScfT10kSetConfig function.  The setting file 
T10000.scf has the following format.


#DTL-T10000 System Configuration File T10000.scf
# 
[SPDIF]         ON              # ON OFF
[ASPECT]        FULL            # 4:3 FULL 16:9  
[LANGUAGE]      English         # Japanese English French
				  Spanish German Italian
				  Dutch Portuguese
[TIMEZONE]      0               # from -720 to 780
[SUMMERTIME]    OFF             # ON OFF   
[TIMENOTATION]  24HOUR          # 24HOUR 12HOUR  
[DATENOTATION]  YYYY/MM/DD      # YYYY/MM/DD MM/DD/YYYY DD/MM/YYYY

Characters followed by # are comments.
Characters enclosed in square brackets are setting items.
Give some space between the setting items and contents.
The comments describe what can be specified.

When it is unable to open this file, obtain the DTL-T10000 default 
settings that are described in the overview of the libscf library.


< Definition of Constants >
For a TV screen output in the PAL system, define PAL.


< Files >
	T10000.scf
	scf.c

< Activating the Program >

	% make		: Compiles the program
	% make run	: Executes the program

	After the compilation, the program is executable with the following.

	% dsedb
	> run scf.elf

< Using the Controller >

	None
