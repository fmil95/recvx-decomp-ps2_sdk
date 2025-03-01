[SCEI CONFIDENTIAL DOCUMENT]

"PlayStation 2" Programmer Tool Runtime Library  Release 2.0

                  Copyright (C) 1999 by Sony Computer Entertainment Inc.

                                                     All Rights Reserved


[Program to Convert 3D Icon Files between ASCII and Binary Forms]

This tool converts binary 3D icon files to ASCII files.  It can also 
convert the converted ASCII files back to binary files.  Therefore, 
files can be binary-coded after being output in ASCII form by using 
a tool such as the 3D authoring tool.   

iconconv [-b] <input> <output>

The binary icon file specified in <input> is output to <output>.
With -b, the ASCII file specified in <input> is binary-coded and output 
to <output>. 

Texture cannot be ASCII-/binary-coded at this stage, so edit the 
ASCII-coded file, convert it to a binary file, and add texture to 
the file as follows:  

iconconv -b ascii.ico binary.ico
cat binary.ico tex.raw > icon.ico



