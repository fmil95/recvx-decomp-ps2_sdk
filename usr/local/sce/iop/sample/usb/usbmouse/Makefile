#
#  IOP usbmouse program Makefile
#
#  $Id: Makefile,v 1.5 2000/04/21 05:28:58 xokano Exp $
#
ifeq ($(wildcard PathDefs),)
PathDefs:
	iop-path-setup > PathDefs || (rm -f PathDefs ; exit 1)
	$(MAKE) all
else
include PathDefs
endif

#----------- rules --------------
%.irx: %.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

#----------- customize section --------------
CFLAGS   =  -Wall -G0
ASFLAGS  =
LDLIBS   = -ilb=usbd.ilb
PROGNAME = usbmouse.irx
OBJS	 = $(PROGNAME:%.irx=%.o) # add here other objects

all:	$(PROGNAME)

clean:
	rm -f *.o $(PROGNAME) *.map

run:	$(PROGNAME)
	dsreset 0 0 ;\
	dsistart /usr/local/sce/iop/modules/usbd.irx ;\
	dsistart $(PROGNAME)

$(PROGNAME:%.irx=%.o) : $(PROGNAME:%.irx=%.c) # add your local include file

$(PROGNAME): $(OBJS)
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@
