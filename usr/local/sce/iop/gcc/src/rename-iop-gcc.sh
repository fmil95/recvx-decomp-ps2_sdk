#!/bin/sh

cd /usr/local/sce/iop/gcc/bin

mv mipsel-scei-elfl-addr2line	iop-elf-addr2line
mv mipsel-scei-elfl-ar		iop-elf-ar
mv mipsel-scei-elfl-as		iop-elf-as
mv mipsel-scei-elfl-c++filt	iop-elf-c++filt
mv mipsel-scei-elfl-gasp	iop-elf-gasp
mv mipsel-scei-elfl-gcc		iop-elf-gcc
mv mipsel-scei-elfl-ld		iop-elf-ld
mv mipsel-scei-elfl-nm		iop-elf-nm
mv mipsel-scei-elfl-objcopy	iop-elf-objcopy
mv mipsel-scei-elfl-objdump	iop-elf-objdump
mv mipsel-scei-elfl-ranlib	iop-elf-ranlib
mv mipsel-scei-elfl-size	iop-elf-size
mv mipsel-scei-elfl-strings	iop-elf-strings
mv mipsel-scei-elfl-strip	iop-elf-strip

ln iop-elf-addr2line	iop-addr2line
ln iop-elf-ar		iop-ar
ln iop-elf-as		iop-as
ln iop-elf-c++filt	iop-c++filt
ln iop-elf-gasp		iop-gasp
ln iop-elf-gcc		iop-gcc
ln iop-elf-ld		iop-ld
ln iop-elf-nm		iop-nm
ln iop-elf-objcopy	iop-objcopy
ln iop-elf-objdump	iop-objdump
ln iop-elf-ranlib	iop-ranlib
ln iop-elf-size		iop-size
ln iop-elf-strings	iop-strings
ln iop-elf-strip	iop-strip
