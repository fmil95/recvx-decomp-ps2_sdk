ifeq ($(wildcard PathDefs),)
PathDefs:
	iop-path-setup > PathDefs || (rm -f PathDefs ; exit 1)
	make all
else
include PathDefs
endif

INCOPT = -I../../../../common/include -I../../../include

CFLAGS  = $(INCOPT) -I. -Wall -G0 -g
ASFLAGS = $(INCOPT) -G0
RM          = /bin/rm -f

COMPILE.s = $(CC) -xassembler-with-cpp $(ASFLAGS) $(CPPFLAGS) $(TARGET_MACH) -c

#----------- customize section --------------
PROGNAME = ezadpcm

OBJS     = entry.o command.o
#ILIBS	= -ilb=$(TOP)lib/iop.ilb 
ILIBS	=

#----------- rules --------------
all:	$(PROGNAME).irx

clean:
	rm -f *.o $(PROGNAME).irx *.obj *.map

$(PROGNAME).irx: $(OBJS)
	$(LINK.o)  -o $@ \
		$(OBJS) -L../../../lib -L./  -ilb=libsd.ilb  -ilb=cdvdman.ilb  $(ILIBS)

entry.o:	ezadpcm.h
command.o:	ezadpcm.h
