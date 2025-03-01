[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
		Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                   All Rights Reserved

The Sample Program to Check Basic Features of PDA Libraries 


< Description of the Sample >

	This sample program is to see how to use the features of PDA 
	libraries and the reaction of the libraries when functions are 
	called.

	When accessing to a memory card that is connected via a multitap, 
	specify 1 to #define UseMultiTap in the source codes.


< File >
	main.c


< Activating the Program >

        % make          : Compiles the program

        After compilation, execute the following to activate dsedb.

        % dsedb
        > run main.elf
  

< Using the Controller>

        Main menu: 
	Directional button ... Selects the icon of menu items.
	START button ... Executes the selected icon in the menu.

	1. get dir... Displays a list of the file (a libmc.a feature).
	Up/Down directional button ... Moves the entry icon.
	L1/L2 button ... Scrolls the entry list if the entry number is 16 
			 or greater.
	SELECT button ... Exits a get dir menu.  When an error occurs, 
			  the program exits the get dir menu.


	2. pda info ... Obtains/specifies the information that can be 
			handled with the sceMcxGetInfo() and sceMcxSetInfo()
			functions.

	Up, down, right and left directional buttons ... Moves the selected
							 item.
	L1/L2/R1/R2 button ... Changes the value of the selected item.
	START button ... Incorporates the changes to PDA (day-of-week is 
			 automatically counted).
	SELECT button ... Cancels the changes and returns to the main menu.
	Running PDA application ... Displays the running application number.
				    Specifies the application number you 
				    want to execute.
	Speaker ... Displays the availability of a speaker or sets a speaker
		    is enabled.
	Infrared ... Displays the availability of infrared data 
		     communications or sets the infrared data communications
		     are enabled.
	Flash write ... Displays the availability of the flash write from 
			PDA applications or sets the flash write from PDA
			applications is enabled.
	Mode indicator on ... Displays the availability of mode indicator
			      (LED) or sets the mode indicator is enabled.
	Serial No ... Displays the PDA serial number.
	Date ... Displays/Sets the content of the real time clock.


	3. disp trans ... Switches on/off the display of indicating that
			  the PDA file is being transferred.
	Up, down, right and left directional buttons ... Moves the 
							 selected item.
	L1/L2/R1/R2 button ... Changes the value of the selected item.
	START button ... Starts to display when [start] is selected.
			 When [stop] is selected, the display is 
			 forcefully terminated.
	SELECT button ... Resets settings.
	dir ... Specifies the transfer direction (no change in the boot 
		application).
	time ... When [stop] is not executed, specifies the period of 
		 time until display is terminated.
 	4. read dev ... Executes the device entry read.
	Up, down, right and left directional buttons ... Moves the 
							 selected item.
	L1/L2/R1/R2 button ... Changes the value of the selected item.
	START button ... Executes the settings.
	SELECT button ... Resets settings.
	device No. ... Specifies the device number to call.
	param size ... Specifies the number of byte in the fixed length
		       parameter.
	data size ... Specifies the number of byte in the variable 
		      length data.

	With the two memory dumps, one memory dump in the upper phase is 
	the contents of fixed length parameters and the other memory dump 
	in the lower phase is the contents of variable length of data.


	5. write dev ... Executes the device entry write.
	Up, down, right and left directional buttons ... Moves the selected
							 item.
	L1/L2/R1/R2 button ... Changes the value of the selected item.
	START button ... Executes the settings.
	SELECT button ... Resets settings.
	Eks button ... Clears the contents of the variable length data
	device No. ... Specifies the device number to call.
	param size ... Specifies the number of byte in the fixed length
		       parameter.
	data size ... Specifies the number of byte in the variable length
		      data.

	With the two memory dumps, one memory dump in the upper phase is
	the contents of fixed length parameters and the other memory dump
	in the lower phase is the contents of variable length of data.

	The buffer of variable length of data are used commonly with 
	4.read dev and 6.edit mem.  Execution results of the case 4 and 6 
	can be used in this menu.  To clear the setting, press the eks 
	button.
	

	6. edit mem ... Displays/modifies the memory contents of PDA.
	Up, down, right and left directional buttons ... Moves the 
							 selected item.
	L1/L2/R1/R2 button ... Changes the value of the selected item.
			       (When the current offset is selected, 
				contents of other address  can be viewed
				without entering offset.)
	START button ... Transfers the contents of modification to the PDA.
	SELECT button ... Cancels the modification.
	current offset ... Displays the top address of the currently 
			   displayed memory dump.
	offset = ... Displayed when entered to this menu or the SELECT
		     button is pressed, to change the top address of 
		     memory dump.

	Depending on the address that makes an access, it is possible that
	the bus error occurs in the PDA or the written data cannot be read
	in the format as is.

	7. edit UIFS ... Displays/modifies the user interface status.
	Up, down, right and left directional buttons ... Moves the 
							 selected item.
	L1/L2/R1/R2 button ... Changes the value of the selected item.
	START button ... Transfers the contents of modification to the PDA
	SELECT button ... Cancels the modification.

	Alarm ... Displays/Sets the alarm time and the alarm on/off status.
	key lock ... Displays/Sets the key lock status.
	Volume ... Displays/Sets the speaker volume.
	Area code ... Displays the area code of PDA.
	Rtc set ... Displays/Sets the flag to indicate the real time clock
		    setting have completed.
	Font addr ... Displays the address that stores fonts used for the 
		      LCD display.

	8. Mode indicator ... Switches on/off the mode indicator(LED).
	Left/Right directional button ... Selects on or off
	START button ... Transfers the contents of modification to the PDA
	SELECT button ... Cancels the modification

< Notes >
        None
