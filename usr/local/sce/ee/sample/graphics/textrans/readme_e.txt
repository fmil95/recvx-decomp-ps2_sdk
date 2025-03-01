[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0

                  Copyright (C) 1999 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved


High Speed Transmission of Texture with the PSMCT32 Format


< Description of the Sample >

        When using texture in the GS, transfer the texture and CLUT from 
	the main memoryto the frame buffer.  When the transfer from the 
	main memory to the GS is performed, the bit conversion based on a 
	specific rule is automatically performed for high speed texture 
	drawing, and the texture is stored on a frame buffer in the GS.

        For this reason, the transfer speed of the bit pattern texture for
	the PSMT4, PSMT8 and PSMCT16 are lower than that of the PSMCT32 
	even if the transfer data size(not a resolution) is the same.
	(doc/ee/word/tips/gsvpu.ppt: Refer to "Texture transfer")
        
	When swapping the drawing texture on the frame buffer, it is
	effective to improve the performance of the 4/8 bit-texture
	to be converted into the bit pattern transferable as the PSMCT32
	beforehand, transferred, and referred to as PSMT4/8.

	This sample shows the following:
	- An off-line bit conversion sample program which converts the
	  PSMT4/8 texture into the format which can be transferred to
	  the GS as the PSMCT32.
	- A comparison of performance between the transfer of PSMT4/8 
	  without conversion and that with convertion to the PSMCT32.

        For details of each bit pattern in the GS local memory, please
	refer to "Details of the GS local memory" which is available on
	the SCE-NET.   

< File >
	bitconv/ is an off-line sample program which allows to convert
	the raw-image bit location of PSMT4/8 to transferable bit location
	as the PSMCT32.
        
        bitconv <bit number of the source image (4/8)> <width of the input
	image> <height of the input image> <input image> <output image>

        < Limitations >
        - Width and height of the texture must be a power of 2.
	- If the texture is smaller than the texture page size, 
	  it occasionally fails to be converted properly.

      4bit/
        This sample enables the 4 bit-256x256 texture to transfer 
	as the 32 bit-128x64 texture.  
	By setting #define __32BIT__, the texture converted to 32 bits 
	can be transferred.
	The transfer data size is 32768 bytes.  The program transfers
	this data 100 times then displays the H-count.
	
	Transfer to TBP=6720;
        PSMT4   : 221
        PSMCT32 : 98
        PSMCT32 is transferred in a higher speed.
	In the case of the PSMT4, if the TBP is set to other than 
	the page boundary (a multiple of 16), the performance extremely
	degrades.


      8bit/
        This sample enables the 8 bit-256x256 texture to transfer 
	as the 32 bit-128x128 texture.  
	By setting #define __32BIT__, the texture converted to 32 bits 
	can be transferred.
	The transfer data size is 65536 bytes.  The program transfers
	this data 100 times then displays the H-count.

        Transfer to TBP=6720;
        PSMT8   : 205
        PSMCT32 : 178
        PSMCT32 is transferred in a higher speed.
	In the case of the PSMT8, if the TBP is set to other than 
	the page boundary (a multiple of 16), the performance extremely
	degrades.

< Activating the Program >
	% make		: Compiles the program
	% make run	: Executes the program

	Also, the program can be executed with the following:

	% dsedb
	> run main.elf

< Using the Controller >
	None

