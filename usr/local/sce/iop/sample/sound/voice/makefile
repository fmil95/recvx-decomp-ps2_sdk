#
#  IOP test program Makefile
#
#  $Id: makefile,v 1.9 2000/03/02 01:46:45 xokano Exp $
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
	-ilb=libsd.ilb -o $@

#----------- customize section --------------
CFLAGS   = $(INCDIR) -Wall -G 0 
ASFLAGS  = $(INCDIR)
LDLIBS   = 
PROGNAME = main.irx

all: $(PROGNAME)

clean:
	rm -f *.o $(PROGNAME) *.map

run: all
	dsifilesv -c " dsreset 0 2; dsistart -nf ../../../modules/libsd.irx; dsistart -nf $(PROGNAME);"

