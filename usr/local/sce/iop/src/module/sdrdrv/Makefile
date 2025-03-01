ifeq ($(wildcard PathDefs),)
PathDefs:
	iop-path-setup > PathDefs || (rm -f PathDefs ; exit 1)
endif
include PathDefs

CFLAGS  = -I../../../include -I../../../../common/include -I. -Wall -G0 
ASFLAGS  = -I../../../include -G0 -I../../../../common/include 

#----------- customize section --------------
PROGNAME = sdrdrv

OBJS     = sdd_main.o sdd_com.o sdd_cb.o 
#ILIBS	= -ilb=$(TOP)lib/iop.ilb 
ILIBS	=

INIT_OBJ = sdd_com.o

#----------- rules --------------
all:	$(PROGNAME).irx

install: all
	cp $(PROGNAME).irx ../../../modules

clean:
	rm -f *.o $(PROGNAME).irx *.map

$(PROGNAME).irx: $(OBJS)
	$(LINK.o) -s -o $@ \
		$(OBJS) -L../../../lib -L../sd -L./  -ilb=libsd.ilb $(ILIBS)

