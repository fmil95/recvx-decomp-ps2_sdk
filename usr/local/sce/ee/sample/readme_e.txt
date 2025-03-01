[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

                                                        Sample Structure
========================================================================

Sample Directory Structure
------------------------------------------------------------------------
The samples with "(*)" are newly added in this release.

sce/ee/sample/
|--advanced
|  +--anti
|      |--aa1-feather				(*)
|      |--mountain				(*)
|      |--pcrtc-blend				(*)
|      |--refmap-448				(*)
|      |--refmap-4times				(*)
|      |--refmap-noAA				(*)
|      |--refmap-onepass-AAline			(*)
|      +--texmap-onepass-AAline			(*)
|--basic3d
|  |--core
|  |--vu0
|  +--vu1
|--cdvd
|  |--smp_ee
|  +--OLD
|      +--mpgst_cd
|--deci2
|  +--mouse
|  +--system
|--device
|  +--file
|--graphics
|  |--anti
|  |--bumpmap
|  |--capture
|  |--clip_vu0
|  |--clip_vu1
|  |--clut
|  |--clut_csa
|  |--framework
|  |   +--general
|  |--jointm
|  |   +--intr_key
|  |       |--mkdata
|  |       +--runtime
|  |--mipmap
|  |--point_l
|  |--refmap
|  |--scis_vu0
|  |--scis_vu1
|  |--spot_l
|  |--tex_swap
|  +--textrans
|  |   |--4BIT
|  |   |--8BIT
|  |   +--bitconv
|--ilink
|--ipu
|  |--ezcube
|  |--ezcube3
|  +--ezmovie
|--kernel
|  |--priority					(*)
|  |--rotate					(*)
|  |--semaphore					(*)
|  |--vsync					(*)
|  +--thread
|--mc
|  |--basic
|  +--icon
|--mcx
|  +--basic					(*)
|--mpeg
|  |--ezmpeg
|  |--mpegstr
|  |   +--OLD
|  +--mpegvu1
|      +--OLD
|--pad
|  |--basic
|  |--dual2
|  |--gun
|  +--mtap
|--pc
|  +--dcache
|--pkt
|  |--balls
|  |--cube
|  +--mfifo
|--scf  					(*)
|--sif
|  |--sifcmd
|  +--sifrpc
|--sound
|  |--ezbadpcm
|  |--ezbgm
|  |--ezmidi
|  |--rblock
|  +--sqsoft
|--OLD
|  +--spu2
|      |--rautodma
|      |--rseqplay
|      |--rstream
|      +--rvoice
|--usb
|  |--usbkeybd
|  +--usbmouse
|--vu0
|  |--tballs
|  |--tdiff
|  |--tfog
|  |--tmip
|  |--tskin
|  +--tsquare
+--vu1
    |--blow
    |--bspline
    |--deform
    |--hako
    +--iga


Sample Index
------------------------------------------------------------------------
Sample codes are shown below.

advanced:
	advanced/anti/aa1-feather	AA1 (hardware antialiasing function
					of GS) Sample
	advanced/anti/mountain		Sample for Antialiasing by AA1 Line
					Redrawing
	advanced/anti/pcrtc-blend	Antialiasing by PCRTC Merge Circuit
					Sample
	advanced/anti/refmap-448	Reflection Mapping/Specular Sample
					(double resolution in V direction)
	advanced/anti/refmap-4times	Reflection Mapping/Specular Sample
					(antialiasing by sub-pixel shift)
	advanced/anti/refmap-noAA	Reflection Mapping/Specular Sample
	advanced/anti/refmap-onepass-AAline
					One Path Reflection Mapping/
					Specular Sample (with AA1 line
					antialiasing - single drawing)
	advanced/anti/texmap-onepass-AAline
					One Path Texture Mapping/Specular 
					Sample (with AA1 line antialiasing
					- single drawing)

basic3d:
	Samples provided for basic procedure comprehension to display
	3D objects
	basic3d/core	Sample written with cpu core instructions only
	                for algorithm comprehension
	basic3d/vu0	Sample in which main process is replaced with 
			VU0 macrocodes based on core sample
	basic3d/vu1	Sample in which process of core/vu0 is 
			replaced with VU1 microcodes 

cdvd:
	cdvd/smp_ee		Sample to call the CD/DVD-ROM drive command
				function
        cdvd/OLD/mpgst_cd	A sample to read the data of MPEG-2 stream
				from a CD/DVD drive and plays it back
				Note: As the function of this sample is
				      incorporated into the mpeg/mpegstr 
				      sample, it won't be updated any more.

deci2:
	deci2/mouse	Sample to download the program using DECI2
			protocol and to simulate the mouse movement on
			the host machine
	deci2/system	Sample to execute System() on the host machine
			with the DECI2 protocol

device:
        device/file     Sample to manipulate files on the host machine

graphics:
	graphics/anti		Antialias sample (sort & polygon clip)
	graphics/bumpmap	Bump mapping sample
	graphics/capture	Sample to save contents of the frame
				buffer to a file on the host machine
	graphics/clip_vu0	Clipping sample (VU0)
	graphics/clip_vu1	Clipping sample (VU1)
	graphics/clut		Sample to display 4-bit/8-bit colored
				texture
	graphics/clut_csa	Sample to controll loading 4-bit texture
				CLUT to a temporary buffer with csa/cld 
				field
	graphics/framework/general
				Sample to realize the integrated environment
				for graphics with basic concepts and various
				functions
	graphics/jointm/intr_key/mkdata
				Sample to create key frame matrix animation
				data by interpolating the axis of rotation
	graphics/jointm/intr_key/runtime
				Sample to perform key frame matrix 
				animation at runtime by interpolating the 
				axis of rotation
	graphics/mipmap		Sample to perform MIPMAP
	graphics/point_l	Sample to perform point lighting
	graphics/refmap		Sample to perform illumination calculation 
				with specularity and reflection mapping
	graphics/scis_vu0	Scissoring sample program (CORE&VU0)
	graphics/scis_vu1	Scissoring sample program (VU1)
	graphics/spot_l		Sample to demonstrate the spotlight
	graphics/tex_swap	Texture swapping sample
	graphics/textrans/4BIT	Sample to transfer the 4 bit-256x256 
				texture as the 32 bit-128x64 texture
	graphics/textrans/8BIT	Sample to transfer the 8 bit-256x256 
				texture as the 32 bit-128x128 texture
	graphics/textrans/bitconv
				Off-line sample to convert the bit order 
				of the raw image in PSMT4/8 to transferable 
				bit order in PSMCT32

ilink:
        ilink		Sample for the communication via i.LINK(IEEE1394)

ipu:
        ipu/ezcube	Sample to map texture decoded with IPU to a
			3 dimensional polygon
	ipu/ezcube3	Sample to decode more than one compressed data
	ipu/ezmovie     Sample to simply reproduce moving pictures 
			with IPU

kernel:
	kernel/priority	Sample to enhance understanding of thread priority
	kernel/rotate	Sample program for the thread ready queue rotation
	kernel/semaphore	Sample to controll thread operations using
				a VSync interrupt and a semaphore
	kernel/thread	Sample to draw objects by multi-thread
			feature
	kernel/vsync	Sample program for the VSyncStart interrupt

mc:
        mc/basic	Sample to check the basic functions of memory card
			library
	mc/icon		Tool operating on Linux system to generate 3-D icons

mcx:
	mcx/basic	Sample program to check the basic features of 
			PDA libraries

mpeg:
	mpeg/ezmpeg	Sample to reproduce MPEG-2, including MPEG-1, 
			stream with IPU
	mpeg/mpegstr	Sample to reproduce MPEG-2 file with sound for
			PlayStation (PSS file) from the hard disk 
			of the host and CD/DVD. (Sample using libsdr)
	mpeg/mpegstr/OLD
			Sample to reproduce MPEG-2 file with sound for 
			PlayStation (PSS file) from the hard disk 
			of the host and CD/DVD. (Sample using librspu2)
        mpeg/mpegvu1    A sample to perform the color conversion using the 
                        VU1 instead of the IPU. (Sample using libsdr)
        mpeg/mpegvu1/OLD
			A sample to perform the color conversion using the 
                        VU1 instead of the IPU. (Sample using librspu2)

pad
	pad/basic	Sample to display the controller information
        pad/dual2       The sample program for displaying the information
                        on pressure sensitivity of analog controller 
			(DUALSHOCK 2)
        pad/gun		Sample to obtain the coordinate information on gun 
			controller and display on the screen
        pad/mtap        A sample to operate the controller via multitap.

pc:
	pc/dcache	Sample to count D chahe miss by performance
			counter	

pkt:
        pkt/balls       Sample to draw sprite
        pkt/cube        Sample to draw polygon models
	pkt/mfifo	Sample to draw the object using MFIFO

scf:
	scf		Sample to obtain the system setting information on
			PlayStation 2

sif:
	sif/sifcmd	SIF CMD protocol sample
	sif/sifrpc	SIF RPC protocol sample	

sound:
	sound/ezadpcm	Sample to play back BGM with ADPCM data streaming 
			from the disk
	sound/ezbgm	Sample to reproduce the sound data (WAV) with 
			streaming from the disk
	sound/ezmidi	Sample to create music and sound effects with MIDI
	sound/rblock	Sample to execute the I/O block transfer 
			(formerly the AutoDMA transfer) 
	sound/sqsoft	Sample to play the MIDI by the software Synthesizer

OLD/spu2:
	(Sample using librspu2)
	spu2/rautodma	Sample to produce sound with straight PCM input 
			by AutoDMA transfer feature
	spu2/rseqplay	Sample to reproduce MIDI sequence
	spu2/rstream	Sample to reproduce voice stream
	spu2/rvoice	Sample to produce voice

usb:
	usb/usbkeybd	Sample to get data of the USB keyboard from IOP 
			to EE
	usb/usbmouse	Sample to get data of the USB mouse driver from 
			IOP to EE

vu0:
        Samples which perform geometric processing via VU0
	vu0/tballs 	Sample in which three-dimensional balls move 
			around
	vu0/tdiff  	Sample to clip polygons
	vu0/tfog   	Sample to perform point lighting and object 
			clipping  
	vu0/tmip   	Sample to perform linear interpolation of MIMe 
			and MIPMODEL
	vu0/tskin  	Sample to display multiple models by reproducing 
			1 model with a matrix. 
	vu0/tsquare	Sample of VU0 macrocode which uses an in-line 
			assembler

vu1:
        Samples which perform geometric processing via VU1 
        The dvpasm format is used for the description of the microcode. 
	vu1/blow	Sample to draw particles
	vu1/bspline	Sample to draw VU1 b-spline curved surface
	vu1/deform	Sample to perform interpolation between two 
			geometric figures
	vu1/hako	Sample to perform transparency perspective 
			conversion
	vu1/iga		Sample to perform transparency perspective 
			conversion and lighting calculation

