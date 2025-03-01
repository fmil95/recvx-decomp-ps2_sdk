/* SCEI CONFIDENTIAL
 "PlayStation 2" Programmer Tool Runtime Library  Release 2.0
 */
/*
 *                      Emotion Engine Library
 *                          Version 0.10
 *                           Shift-JIS
 *
 *      Copyright (C) 1998-1999 Sony Computer Entertainment Inc.
 *                        All Rights Reserved.
 *
 *                       libkernel - app.cmd
 *                        kernel libraly
 *
 *       Version        Date            Design      Log
 *  --------------------------------------------------------------------
 *      0.11            May.10.1999     horikawa    
 */

_stack = -1;
_stack_size = 0x00100000;
_i_stack_size = 0x00002000;
_heap_size = -1;

GROUP(-lc -lkernl -lgcc)
ENTRY(ENTRYPOINT)
SECTIONS {
	.indata		0x00100000: { *(.indata) }
	.text		0x00200000: {
		crt0.o(.text)
		*(.text)
	}
	.reginfo		  : { KEEP(*(.reginfo)) }
	.data		ALIGN(128): { *(.data) }
	.rodata		ALIGN(128): { *(.rodata) }
	.rdata		ALIGN(128): { *(.rdata) }
	.gcc_except_table ALIGN(128): { *(.gcc_except_table) }
	_gp = ALIGN(128) + 0x7ff0;
	.lit8       	ALIGN(128): { *(.lit8) }
	.lit4       	ALIGN(128): { *(.lit4) }
	.sdata		ALIGN(128): { *(.sdata) }
	.sbss		ALIGN(128): { _fbss = .; *(.sbss) *(.scommon) }
        .stack          ALIGN(128)(NOLOAD): {
                /*
		 _stack = .;
                 . += _stack_size;
                 _stack_btm = .;
		*/
                _i_stack = .;
                . += _i_stack_size;
                _i_stack_btm = .;
        }
	.bss		ALIGN(128): { *(.bss) }
	.mfifo          ALIGN(0x40000): { *(.mfifo) }
	end = .;
	_end = .;
	.spad		0x70000000: {
		 crt0.o(.spad)
		 *(.spad)
	}
}
