[SCEI CONFIDENTIAL DOCUMENT]
"PlayStation 2" Programmer Tool Runtime Library  Release 2.0
                  Copyright (C) 2000 by Sony Computer Entertainment Inc.
                                                     All Rights Reserved

                                                        Sample Structure
========================================================================

Sample Directory Structure
------------------------------------------------------------------------
The samples with "(*)" is newly added in this release.

sce/iop/sample/
|--cdvd
|   |--smp_iop
|   |--stmadpcm
|   |--stmread
|   +--stmspcm
|--hello
|--ilink
|--kernel
|   |--module
|   +--thread
|--sif
|   |--sifcmd
|   +--sifrpc
|--sound
|   |--ezadpcm
|   |--ezbgm
|   |--ezmidi
|   |--sqhard
|   |--sqsoft
|   +--voice
|--OLD
|   |--spu2
|       |--autodma
|       |--seqplay
|       |--stream
|       +--voice
+--usb
    |--usbdesc			(*)
    |--usbkeybd
    +--usbmouse


Sample Index
------------------------------------------------------------------------
Sample codes are shown below.

cdvd:
	cdvd/smp_iop	Sample to call the CD/DVD-ROM Drive command
			function on the IOP.
        cdvd/stmadpcm	Sample to reproduce the ADPCM streaming
        cdvd/stmread	Sample to read the file using the stream 
                        functions from a CD/DVD drive.
        cdvd/stmspcm	Sample to reproduce the straight PCM streaming

hello:
	hello		Sample to display "hello !" 

ilink:
	ilink		Sample for the communication via i.LINK(IEEE1394)

kernel:
	kernel/module   Sample to show how to create a resident library
			module
	kernel/thread	Sample to generate a thread and boot-up, 
			Operate a thread priority, Synchronize threads

sif:
	sif/sifcmd	SIF CMD protocol sample
	sif/sifrpc	SIF RPC protocol sample

sound:
	sound/ezadpcm	Sample to play back BGM with ADPCM data streaming 
			from the disk
	sound/ezbgm	Sample to reproduce the sound data (WAV) with 
			streaming from the disk
	sound/ezmidi	Sample to create music and sound effects with MIDI
	sound/sqhard	Sample to play the MIDI by the hardware synthesizer
	sound/sqsoft	Sample to play the MIDI by the software synthesizer
	sound/voice	Sample to play the voice

OLD/spu2:
	(Sample using libspu2)
	spu2/autodma	Sample to reproduce the sound by the straight PCM 
			input using the	AutoDMA transfer function of the
			Interim sound library (libspu2)
	spu2/seqplay	Sample to reproduce the MIDI sequence on the IOP
			using the Interim sound library(libspu2, 
			libsnd2)
	spu2/stream	Sample to reproduce the voice stream by the SPU2 
			on the IOP using the Interim sound library
			(libspu2)
	spu2/voice	Sample to reproduce the SPU2 voice on the IOP using
			the Interim sound library(libspu2)

usb:
	usb/usbdesc	Sample to dump the static descriptor of the USB 
			device.
	usb/usbkeybd	Sample driver of the USB keyboard.
	usb/usbmouse	Sample presenting how to use USB driver (USBD)


Preliminary Arrangements for Sample Compilation   
------------------------------------------------
None
