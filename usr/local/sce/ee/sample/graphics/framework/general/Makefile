
SHELL       = /bin/sh

TOP         = /usr/local/sce/ee
LIBDIR      = $(TOP)/lib
INCDIR      = $(TOP)/include

TARGET      = main
OBJS        = crt0.o $(TARGET).o \
		gmain.o \
		micro.o camera.o packbuf.o vu1frame.o hier.o anim.o eval.o framebuf.o \
		logo.o

LCFILE      = $(LIBDIR)/app.cmd
LIBS        = $(LIBDIR)/libgraph.a \
              $(LIBDIR)/libdev.a \
              $(LIBDIR)/libdma.a \
              $(LIBDIR)/libpad.a \
              $(LIBDIR)/libpkt.a \
	      $(LIBDIR)/libvu0.a

PREFIX      = ee
AS          = $(PREFIX)-gcc
CC          = $(PREFIX)-gcc
LD          = $(PREFIX)-gcc
DVPASM      = $(PREFIX)-dvp-as
OBJDUMP     = $(PREFIX)-objdump
RUN         = dsedb -r run
RM          = /bin/rm -f

#CFLAGS      = -O2 -Wall -Werror -Wa,-al -fno-common
#CXXFLAGS    = -O2 -Wall -Werror -Wa,-al -fno-exceptions -fno-common
CFLAGS      = -g -Wall -Wa,-al -fno-common
CXXFLAGS    = -O2 -Wall -Wa,-al -fno-exceptions -fno-common
ASFLAGS     = -c -xassembler-with-cpp -Wa,-al
DVPASMFLAGS = -g 
LDFLAGS     = -Wl,-Map,$(TARGET).map -nostartfiles -L$(LIBDIR) -lm
TMPFLAGS    =

.SUFFIXES: .c .s .cc .dsm

all: $(TARGET).elf

$(TARGET).elf: $(OBJS) $(LIBS) Makefile
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
	$(RM) *.o *.map *.lst core *.dis *.elf *~

anim.o: defines.h vu1frame.h hier.h anim.h eval.h
camera.o: defines.h camera.h
framebuf.o: defines.h framebuf.h
gmain.o: defines.h packbuf.h vu1frame.h camera.h hier.h anim.h micro.h
gmain.o: gmain.h
hier.o: defines.h vu1frame.h hier.h
main.o: defines.h gmain.h
packbuf.o: defines.h packbuf.h
vu1frame.o: defines.h packbuf.h vu1frame.h framebuf.h hier.h
micro.o: vu1base.vsm vu1baseo.vsm vu1cull.vsm vu1cullo.vsm
