
SHELL       = /bin/sh

TOP         = /usr/local/sce/ee
LIBDIR      = $(TOP)/lib
INCDIR1      = $(TOP)/include
INCDIR2      = $(TOP)/../common/include

TARGET      = main
OBJS        = crt0.o \
		main.o \
		read.o \
		readbuf.o \
		audiodec.o \
		vibuf.o \
		videodec.o \
		vobuf.o \
		disp.o \
		util.o \
		strfile.o

DATAFILE    = ../../../../data/movie/sample.pss

LCFILE      = $(LIBDIR)/app.cmd
LIBS        = $(LIBDIR)/libgraph.a \
              $(LIBDIR)/libdma.a \
              $(LIBDIR)/libdev.a \
              $(LIBDIR)/libpkt.a \
	      $(LIBDIR)/libvu0.a \
	      $(LIBDIR)/libmpeg.a \
	      $(LIBDIR)/libipu.a \

LIBOPT	  = -lgraph -ldma -ldev -lpkt -lvu0 -lpad \
	-lrspu2 -lmpeg -lipu -lcdvd -lm

PREFIX      = ee
AS          = $(PREFIX)-gcc
CC          = $(PREFIX)-gcc
LD          = $(PREFIX)-gcc
DVPASM      = $(PREFIX)-dvp-as
OBJDUMP     = $(PREFIX)-objdump
RUN         = dsedb -r run
RM          = /bin/rm -f

#CFLAGS      = -O2 -Wall -Wa,-al -g
CFLAGS      = -O2 -Wall -Werror -Wa,-al
CXXFLAGS    = -O2 -Wall -Werror -Wa,-al -fno-exceptions -fno-common
ASFLAGS     = -c -xassembler-with-cpp -Wa,-al
DVPASMFLAGS = -g 
LDFLAGS     = -Wl,-Map,$(TARGET).map -nostartfiles -L$(LIBDIR) $(LIBOPT)
TMPFLAGS    =

.SUFFIXES: .c .s .cc .dsm

all: $(TARGET).elf

$(TARGET).elf: $(OBJS) $(LIBS)
	$(LD) -o $@ -T $(LCFILE) $(OBJS) $(LDFLAGS)

crt0.o: $(LIBDIR)/crt0.s
	$(AS) $(ASFLAGS) $(TMPFLAGS) -o $@ $< > $*.lst

.s.o:
	$(AS) $(ASFLAGS) $(TMPFLAGS) -I$(INCDIR1) -I$(INCDIR2) -o $@ $< > $*.lst

.dsm.o:
	$(DVPASM) $(DVPASMFLAGS) -I$(INCDIR1) -I$(INCDIR2) -o $@ $< > $*.lst

.c.o:
	$(CC) $(CFLAGS) $(TMPFLAGS) -I$(INCDIR1) -I$(INCDIR2) -c $< -o $*.o > $*.lst

.cc.o:
	$(CC) $(CXXFLAGS) $(TMPFLAGS) -I$(INCDIR1) -I$(INCDIR2) -c $< -o $*.o > $*.lst

run: $(TARGET).elf
	$(RUN) $(TARGET).elf $(DATAFILE)

noaudio: $(TARGET).elf
	$(RUN) $(TARGET).elf -noaudio $(DATAFILE)

help: $(TARGET).elf
	$(RUN) $(TARGET).elf -help $(DATAFILE)

clean:
	$(RM) *.o *.map *.lst core *.dis *.elf

########
compare:
	${MAKE} "CFLAGS=${CFLAGS} -DCOMPARE_ON"

g++:
	${MAKE} "CC=ee-g++" "LD=ee-g++"


