### Makefile

SHELL       = /bin/sh

TOP         = /usr/local/sce/ee
BINDIR      = /usr/local/sce/ee/gcc/bin
LIBDIR      = $(TOP)/lib
INCDIR      = $(TOP)/include
KERNEL      = $(TOP)/lib/kernel.elf

TARGET      = main
OBJS        = crt0.o \
		texload.o \
		$(TARGET).o \
		devinit.o \
		work.o \
		misc_packet.o \
		gterrain.o \
		gterrain_packet.o \
		random.o \
		clip.o \
		matrix_util.o \
		view_util.o \
		dma_util.o


LCFILE      = $(LIBDIR)/app.cmd
#LIBS	    = $(LIBDIR)/libkernel.a
LIBS        = $(LIBDIR)/libgraph.a \
               $(LIBDIR)/libdma.a \
               $(LIBDIR)/libdev.a \
               $(LIBDIR)/libpad.a \
               $(LIBDIR)/libpkt.a

PREFIX      = ee
AS          = $(BINDIR)/$(PREFIX)-gcc
CC          = $(BINDIR)/$(PREFIX)-gcc
LD          = $(BINDIR)/$(PREFIX)-gcc
DVPASM      = $(BINDIR)/$(PREFIX)-dvp-as
OBJDUMP     = $(BINDIR)/$(PREFIX)-objdump -mmips:5900
ERUN        = dsedb -r run
RM          = /bin/rm -f

CFLAGS      = -O2 -Werror -Wall -Wa,-al -fno-common -fshort-double
#CFLAGS      = -O2 -Wa,-al -fno-common -fshort-double -fno-strict-aliasing -fno-strength-reduce
#CFLAGS      = -O2 -Wa,-al -fno-common -fshort-double -fno-strength-reduce
#CFLAGS      = -O -Wa,-al -fno-common -fshort-double
CXXFLAGS    = -O2 -Wa,-al -fno-exceptions -fno-common
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
	$(ERUN) $(TARGET).elf

dump: $(TARGET).elf $(OBJS)
	(for i in *.o; do $(OBJDUMP) -d $$i > `basename $$i .o`.dis; done)
	$(OBJDUMP) -D $(TARGET).elf > $(TARGET).elf.dis

clean:
	$(RM) *.o *.map *.lst core *.dis *.elf
