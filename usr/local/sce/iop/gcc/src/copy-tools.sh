#!/bin/sh

topdir=/usr/local/sce/
distdir=/usr/local/sce/iop/gcc/

# copying iop tools

cp -p $topdir/bin/iopfixup     $distdir/mipsel-scei-elfl/bin/iopfixup
cp -p $topdir/bin/ioplib*          $distdir/mipsel-scei-elfl/bin
cp -p $topdir/iop/gcc/src/iopspecs $distdir/lib/gcc-lib/mipsel-scei-elfl/specs

cp -pr $topdir/iop/install/include/*   $distdir/mipsel-scei-elfl/include
cp -pr $topdir/iop/install/lib/*       $distdir/mipsel-scei-elfl/lib
cp -p  $topdir/iop/gcc/src/crt0.o.elf  $distdir/mipsel-scei-elfl/lib/crt0.o
