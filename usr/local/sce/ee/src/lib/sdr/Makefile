### Makefile

SHELL=/bin/sh

TOP = /usr/local/sce/ee
INCDIR = $(TOP)/include -I$(TOP)/../common/include
PUB_INCDIR = $(TOP)/include
LIB_INST_DIR = $(TOP)/lib
HDR_INST_DIR = $(TOP)/include

TARGET = libsdr
OBJS = sdr_main.o sdr_cb.o 
INST_HDRS =

PREFIX=ee
AS = $(PREFIX)-gcc
CC = $(PREFIX)-gcc
AR = $(PREFIX)-ar
RANLIB = $(PREFIX)-ranlib
DVPASM = $(PREFIX)-dvp-as
RM=/bin/rm -f
INSTALL = install


CFLAGS = -c -Wa,-al -fno-common -O2 -G0 
ASFLAGS = -c -xassembler-with-cpp -Wa,-al 
DVPASMFLAGS = -g 

INIT_OBJ = sdr_main.o

.SUFFIXES: .c .s .cc .dsm

all: $(TARGET).a $(LIB_INST_DIR)

$(TARGET).a: $(OBJS)
	$(RM) $@
	$(AR) r $@ $(OBJS)
	$(RANLIB) $@ 

.s.o:
	$(AS) $(ASFLAGS) -I$(INCDIR) -I$(PUB_INCDIR) -o $@ $< > $*.lst

.c.o:
	$(CC) $(CFLAGS) -I$(INCDIR) -I$(PUB_INCDIR) -c $< -o $*.o > $*.lst

.dsm.o:
	$(DVPASM) $(DVPASMFLAGS) -I$(INCDIR) -I$(PUB_INCDIR) -o $@ $< > $*.lst

clean:
	/bin/rm -f *.o *.lst core *.elf *.a

install: all
	$(INSTALL) -d $(LIB_INST_DIR)
	$(INSTALL) -d $(HDR_INST_DIR)
	cp $(TARGET).a $(LIB_INST_DIR)
#	cp crt0.s $(LIB_INST_DIR)
#	cp app.cmd $(LIB_INST_DIR)
#	cp $(INST_HDRS) $(HDR_INST_DIR)

