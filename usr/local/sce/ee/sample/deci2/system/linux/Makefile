
TARGET = hsys
CC = gcc
CFLAGS = -O2

all: $(TARGET)

$(TARGET): main.o
	gcc -o $@ $^

clean:
	rm -f $(TARGET) *.o core
