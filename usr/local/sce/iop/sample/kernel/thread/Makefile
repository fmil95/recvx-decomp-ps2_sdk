#
#  IOP thread sample program Makefile
#
#  $Id: Makefile,v 1.8 2000/08/02 14:54:36 tei Exp $
#

#----------- customize section --------------
CFLAGS   = -Wall
ASFLAGS  = 
LOADLIBES=

PROG1 = createth.irx
PROG2 = sleepth.irx
PROG3 = eventth.irx
PROG4 = startthargs.irx

all:	$(PROG1) $(PROG2) $(PROG3) $(PROG4)

clean:
	rm -f *.o $(PROG1) $(PROG2) $(PROG3) $(PROG4) *.map

#----------- rules --------------
%.irx: %.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

-include PathDefs
PathDefs:
	iop-path-setup > PathDefs || (rm -f PathDefs ; exit 1)
