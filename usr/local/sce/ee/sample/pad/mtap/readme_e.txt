[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
	        Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

Sample to Operate the Controller via Multi Tap

<Description of the Sample>

        This program operates controllers and obtains information by 
	using the Controller library and the Multi Tap library.
        Regarding the actuator operation, this program conforms only  
	with the DUALSHOCK controllers.

<File>
        main.c

<Activating the Program>

        % make          : Compiles the program.

        After the compilation, activates dsedb.
        % dsedb
	Executes the following;
        > run main.elf.
        

<Using the Controller>

        (When connected to the DUALSHOCK controller)
        Up directional button	: Runs the small motor.
        Down directional button	: Halts the small motor.
        Left directional button : Runs the large motor.
        Right directional button: Halts the large motor.

<Notes>
        Insert the Multi Tap to Controller port 1 (port 0) and then 
	Controllers to corresponding Controller port*-A-D.
