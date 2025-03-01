
SHELL       = /bin/sh

TOP         = /usr/local/sce/ee
LIBDIR      = $(TOP)/lib
INCDIR      = $(TOP)/../common/include -I$(TOP)/include

TARGET      = scf
OBJS        = crt0.o \
              $(TARGET).o \
	    
LCFILE      = $(LIBDIR)/app.cmd
LIBS        = $(LIBDIR)/libgraph.a \
              $(LIBDIR)/libcdvd.a \
	      $(LIBDIR)/libdev.a \
	      $(LIBDIR)/libpkt.a \
	      $(LIBDIR)/libdma.a \
              $(LIBDIR)/libkernl.a \
	      $(LIBDIR)/libscf.a

PREFIX      = ee
AS          = $(PREFIX)-gcc
CC          = $(PREFIX)-gcc
LD          = $(PREFIX)-gcc
DVPASM      = $(PREFIX)-dvp-as
OBJDUMP     = $(PREFIX)-objdump
RUN         = dsedb -r run
RM          = /bin/rm -f

CFLAGS      = -O2 -Wall -Wa,-al -fno-common -Werror
CXXFLAGS    = -O2 -Wall -Wa,-al -fno-exceptions -fno-common
ASFLAGS     = -c -xassembler-with-cpp -Wa,-al
DVPASMFLAGS = -g 
LDFLAGS     = -Wl,-Map,$(TARGET).map -nostartfiles -L$(LIBDIR) -lm
TMPFLAGS    =

.SUFFIXES: .c .s .cc .dsm

all: $(TARGET).elf

$(TARGET).elf: $(OBJS) $(LIBS)
	$(LD) -o $@ -T $(LCFILE) $(OBJS) $(LIBS) $(LDFLAGS)

crt0.o: $(LIBDIR)/crt0.s
	$(AS) $(ASFLAGS) $(TMPFLAGS) -o $@ $< > $*.lst

.s.o:
	$(AS) $(ASFLAGS) $(TMPFLAGS) -I$(INCDIR) -o $@ $< > $*.lst

.dsm.o:
	$(DVPASM) $(DVPASMFLAGS) -I$(INCDIR) -o $@ $< > $*.lst

.c.o:
	$(CC) $(CFLAGS) $(TMPFLAGS) -I$(INCDIR) -c $< -o $*.o > $*.lst

.cc.o:
	$(CC) $(CXXFLAGS) $(TMPFLAGS) -I$(INCDIR) -c $< -o $*.o > $*.lst

run: $(TARGET).elf
	$(RUN) $(TARGET).elf

clean:
	$(RM) *.o *.map *.lst core *.dis *.elf