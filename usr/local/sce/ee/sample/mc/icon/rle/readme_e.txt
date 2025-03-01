[SCEI CONFIDENTIAL DOCUMENT]

"PlayStation 2" Programmer Tool Runtime Library  Release 2.0

                  Copyright (C) 1999 by Sony Computer Entertainment Inc.

                                                     All Rights Reserved


[Texture Compression Program for 3D Icons in MEMORY CARD]

3D icon file textures to be saved in the MEMORY CARD can be run-length 
coded.  This program performs run-length coding.

Usage : rle [-x] <input> <output>

The file specified in <input> is compressed and output to <output>.
With -x, the file specified in <input> is decompressed and output to 
<output>.

Since textures used for icons are in PSMCT16, a data format in 2-byte 
units, they are most efficiently compressed by applying run-length 
coding in 2-byte units.
Some images cannot be compressed and might become bigger in size.  In 
such cases, do not compress the images, but use the textures.

As an example, a texture and an object can be merged into an icon file 
according to the following procedure.

1. Create an icon file aaa.ico, which consists of only model and animation 
   sections without a texture.  Set the parameters for model section 
   attributes (TEX=1, RLE=1).
2. Prepare a texture as raw data in PSMCT16 format (tex.raw).
3. Compress the data with rle tex.raw tex.rle.
4. Merge the above data into one file with a command such as 
   cat aaa.ico tex.rle > ICON.ico.


