### Makefile

SHELL       = /bin/sh

TOP         = /usr/local/sce/ee
BINDIR      = /usr/local/sce/ee/gcc/bin
LIBDIR      = $(TOP)/lib
INCDIR      = $(TOP)/include
#BINDIR      = /usr/local/sce_release1.1.0/bin
DSMDIR      = /usr/local/sce_release1.3.0/bin

TARGET      = main
OBJS        = crt0.o \
		$(TARGET).o vu1mpg.o matrix.o packet.o dma.o fractalobj.o gscntrl.o

LCFILE      = $(LIBDIR)/app.cmd
LIBS        = $(LIBDIR)/libgraph.a \
              $(LIBDIR)/libdma.a \
              $(LIBDIR)/libdev.a \
              $(LIBDIR)/libpkt.a \
              $(LIBDIR)/libpad.a \
              $(LIBDIR)/libvu0.a

PREFIX      = ee
AS          = $(BINDIR)/$(PREFIX)-gcc
CC          = $(BINDIR)/$(PREFIX)-gcc
LD          = $(BINDIR)/$(PREFIX)-gcc
#DVPASM      = $(BINDIR)/$(PREFIX)-dvp-as
#OBJDUMP     = $(BINDIR)/$(PREFIX)-objdump -mmips:5900
DVPASM      = $(PREFIX)-dvp-as
OBJDUMP     = $(PREFIX)-objdump

#RUN         = dsedb -r run
#RUN         = $(DSMDIR)/dsedb -d iravati -r run
RUN         = dsedb -r run
RM          = /bin/rm -f


#CFLAGS      = -O2 -Wall -Wa,-al -fno-common
#CFLAGS      = -O2 -Wa,-al -fno-common -fno-strict-aliasing -fno-strength-reduce
#CFLAGS      = -O2 -Wall -Wa,-al -fno-common -fshort-double -fno-strict-aliasing -fno-strength-reduce
CFLAGS      = -O2 -Wall -Werror -Wa,-al -fno-common -fshort-double -fno-strict-aliasing -fno-strength-reduce
#CFLAGS      = -O2 -Wa,-al -fno-common -fno-strict-aliasing
#CFLAGS      = -O2 -Wall -Wa,-al -fno-common -fshort-double
CXXFLAGS    = -O2 -Wa -Werror,-al -fno-exceptions -fno-common
#CXXFLAGS    = -O2 -Wall -Werror -Wa,-al -fno-exceptions -fno-common 
#ASFLAGS     = -c -xassembler-with-cpp -Wa,-al
#DVPASMFLAGS = -g 
#LDFLAGS     = -Wl,-Map,$(TARGET).map -nostartfiles -L$(LIBDIR) -lm
#TMPFLAGS    =
ASFLAGS     = -c -xassembler-with-cpp -Wa,-al
DVPASMFLAGS = -g 
LDFLAGS     = -Wl,-Map,$(TARGET).map -nostartfiles -L$(LIBDIR) -lm
TMPFLAGS    =

.SUFFIXES: .c .s .cc .dsm

all: $(TARGET).elf #kernel.elf 

kernel.elf: $(KERNEL)
	rm -f kernel.elf
	ln -s $(KERNEL) kernel.elf

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

dump: $(TARGET).elf $(OBJS)
	(for i in *.o; do $(OBJDUMP) -d $$i > `basename $$i .o`.dis; done)
	$(OBJDUMP) -D $(TARGET).elf > $(TARGET).elf.dis

clean:
	$(RM) *.o *.map *.lst core *.dis *.elf
