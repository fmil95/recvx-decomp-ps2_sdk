
TARGET = mmouse
CC = gcc
#CFLAGS = -I/usr/X11R6/include -O2
CFLAGS = -O2
LDFLAGS = -L /usr/X11R6/lib -lm -lX11

all: $(TARGET)

$(TARGET): child.o list.o elf.o dbgp.o netmp.o dcmp.o main.o
	gcc -o $@ $(LDFLAGS) $^

run: $(TARGET) ../main.elf
	$(TARGET) ../main.elf

clean:
	rm -f $(TARGET) *.o core

