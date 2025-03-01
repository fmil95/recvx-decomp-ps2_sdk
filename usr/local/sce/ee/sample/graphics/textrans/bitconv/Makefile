
SHELL       = /bin/sh


TARGET      = bitconv
OBJS        = $(TARGET).o 

AS          = g++
CC          = g++
LD          = gcc
RM          = /bin/rm -f

CFLAGS      = -O2 -Wall -Werror 
CXXFLAGS    = -O2 -Wall -Werror
LDFLAGS     = -lm
TMPFLAGS    =

.SUFFIXES: .c .s .cc .dsm .cpp

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(LD) -o $@  $(LCFILE) $(OBJS) $(LIBS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $*.o > $*.lst

.cpp.o:
	$(CC) $(CFLAGS) -c $< -o $*.o > $*.lst

clean:
	$(RM) *.o *~ *.lst core  $(TARGET)
