[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved


Confirmation of basic function of the MEMORY CARD library

<Description of the Sample>

	This program formats the MEMORY CARD, and generates a directory 
	list and "icon.sys" using the MEMORY CARD library.

	To access the MEMORY CARD conneted via Multi Tap, define  
        #define UseMultiTap in the source code to 1.

<File>
	main.c
	slime1.ico
	slime2.ico
	slime3.ico

<Activating the Program>

        % make          : Compiles the program.

	After the compilation, execute dsedb as follows;
        % dsedb
        > run main.elf 

<Using the Controller>

	Main Menu:
	Directional button: Selects the mark in the menu items.
	START button: Executes the marked menu.

	1. get dir
	Up/Down directional button: Moves the mark of the entry.
	L1/L2 button: Scrolls the entry list when the number of entries
	is 16 or more.		
	START button: Enters into the subdirectory only when the directory
	entry is marked.
	Circle button: Selects the marked entries.
	Selected entry names are displayed just below the menu.
	The menu selected here becomes a subject of 3. file status 
	described below.
	Cross button: Deletes the marked entries.
	SELECT button: Closed the "get dir" menu. The menu is also closed 
	when an error occurs.
			
	2. mkdir + icon : Makes the directory whose title name is a entry 
	name, generates icon.sys, and copies the icon file. At least this 
	operation is necessary to save a game data. The game data is sotred
	in the directory generated here. The file name is optional.

	Right/Left directional button: Moves the cursor.
	Up/Down directional button: Modifies an entry name.
	Cross button: Backspace.
	Square button: Changes the characters on the cursor to 0.
	Triangle button: Changes the characters on the cursor to A.
	Circle button: Changes the characters on the cursor to a.
	R1 button: Inserts spaces.
	START button: Performs the initialization of the directory with the 
	set title name.
	SELECT button: Cancels a directory generation.	

	3. file status : Displays and modifies the file status selected in
	the 1. get dir menu.

	L1/L2 button: Moves the mark of the items.
	Right/Left directional button: Moves the cursor in each item.
	Up/Down directional button: Modifies values in each item.
	START button: Reflects the modification to a file.
	SELECT button: Cancels the modification.
	Attribute: File attribute.
		1 ... sceMcFileAttrPS1		PSI data
		p ... sceMcFileAttrPDAExec	PDA application
		d ... SCE_STM_D			Directory
		c ... SCE_STM_C			Copy disabled
		x ... SCE_STM_X			Executable
		w ... SCE_STM_W			Writable
		r ... SCE_STM_R			Readable
	file size: It displays the file size only when d part in the 
	attribute is x.

	create: File generation date
	
	modify: File modification date
	
	4. format: Formats the MEMORY CARD.
	Circle button: Starts to format
	Other buttons: Returns to the main menu without formatting.

	5. un-format: Unformats the MEMORY CARD.
	Circle button: Starts to unformat. Once unformatted,
	remove the MEMORY CARD for a time.
	Other buttons: Returns to the main menu without formatting.

	6. rename: Changes the file (directory) name.
	Right/Left directional button: Moves the cursor.
	Up/Down directional button: Modifies a file (directory) name.
	Cross button: Backspace.
	Square button: Changes the characters on the cursor to 0.
	Triangle button: Changes the characters on the cursor to A.
	Circle button: Changes the characters on the cursor to a.
	R1 button: Inserts spaces.
	START button: Reflects the modification to the file (directory) 
		      name.
	SELECT button: Cancels the modification.	
 
<Note>
	None
