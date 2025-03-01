#
#  IOP test program Makefile
#
#  $Id: makefile,v 1.9 2000/03/02 01:46:43 xokano Exp $
#
ifeq ($(wildcard PathDefs),)
PathDefs:
	iop-path-setup > PathDefs || (rm -f PathDefs ; exit 1)
	$(MAKE) all
else
include PathDefs
endif

INCDIR  = -I/usr/local/sce/common/include

#----------- rules --------------
%.irx: %.o
	$(LINK.o) $^ -L../../../lib -L./ $(LOADLIBES) $(LDLIBS) \
	-ilb=libsd.ilb \
	-ilb=modssyn.ilb -ilb=modmidi.ilb -o $@

#----------- customize section --------------
CFLAGS   = $(INCDIR) -Wall -G 0 
ASFLAGS  = $(INCDIR)
LDLIBS   = 
PROGNAME = main.irx

all: $(PROGNAME)

clean:
	rm -f *.o $(PROGNAME) *.map

run: all
	dsidb -nr "bload demo1.mid.sq 100000;mstart ../../../modules/modmidi.irx;mstart ../../../modules/modssyn.irx;mstart $(PROGNAME)"
