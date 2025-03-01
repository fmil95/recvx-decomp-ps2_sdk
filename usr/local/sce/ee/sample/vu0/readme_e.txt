[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

VU0 Sample Programs

<Description of the Samples>
VU0 sample programs are as shown below.

vu0/tballs :	Sample in which three-dimensional balls move around
vu0/tfog   :	Sample to perform point lighting and object clipping
vu0/tmip   :	Sample to perform linear interpolation of MIMe and 
		MIPMODEL
vu0/tskin  :	Sample to display multiple models by reproducing 
		1 model with a matrix. 
vu0/tsquare:	Sample of VU0 macrocode which uses an in-line assembler
vu0/tdiff  :	Sample to clip polygons

<File>
vu0/tballs:	object.c
		object.h
		objfast.c
		objfastA.c
		objnorm.c
		objsimpl.c
		sce_pp3.c
		tballs.c
		util.c
vu0/tdiff:
		object.c
		object.h
		objfast.c
		objfastA.c
		objnorm.c
		objpart.c
		objsimpl.c
		sce_pp3.c
		tdiff.c
		util.c

vu0/tfog:
		object.c
		object.h
		objfast.c
		objfastA.c
		objnorm.c
		objpart.c
		objsimpl.c
		sce_pp3.c
		tfog.c
		util.c

vu0/tmip:
		object.c
		object.h
		objfast.c
		objfastA.c
		objnorm.c
		objpart.c
		objsimpl.c
		sce_pp3.c
		tmip.c
		util.c

vu0/tskin:
		object.c
		object.h
		objfast.c
		objfastA.c
		objnorm.c
		objpart.c
		objsimpl.c
		sce_pp3.c
		tskin.c
		util.c

vu0/tsquare:
		object.c
		object.h
		objfast.c
		objfastA.c
		objnorm.c
		objpart.c
		objsimpl.c
		sce_pp3.c
		tsquare.c
		util.c

<Activating the Program>
	% make		: Compiles the program.
	% make run	: Executes the program.

	After the compilation, the programs are also executable 
	with the following.
	% dsedb
	> run *.elf
 
<Using the Controller>
	Not defined.

<Note>
	None.





