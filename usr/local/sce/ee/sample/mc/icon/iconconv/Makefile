
SHELL       = /bin/sh


TARGET      = iconconv
OBJS        = iconconv.o error.o parse.o

AS          = gcc
CC          = gcc
LD          = gcc
RM          = /bin/rm -f

CFLAGS      = -O2 -Wall -Werror 
CXXFLAGS    = -O2 -Wall -Werror
LDFLAGS     = -lm
TMPFLAGS    =

.SUFFIXES: .c .s .cc .dsm

all: $(TARGET)

$(TARGET): $(OBJS) $(LIBS)
	$(LD) -o $@  $(LCFILE) $(OBJS) $(LIBS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $*.o > $*.lst

clean:
	$(RM) *.o *~ *.lst core  $(TARGET)
