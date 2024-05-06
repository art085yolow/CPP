IDIR=.
CC=g++
CCFLAGS=-I$(IDIR)

ODIR=obj
LDIR=.

LIBS=-lm -lglfw -Wall -Wextra -fsanitize=address

#_DEPS = glad.h khrplatform.h
#_OBJ = chip8_unit.o chip8emu.o

#$(ODIR)/%.o: %.cpp $(CFLAGS)
#	$(CC) -c -o $@ $< $(CFLAGS)
all: chip8emu
	
chip8emu: chip8emu.o chip8_unit.o glad.o khrplatform.h
	$(CC) -o $@ chip8emu.o chip8_unit.o glad.o $(CCFLAGS) $(LIBS)

test_chip8: chip8_test.o chip8_unit.o
	$(CC) -o $@ $^

chip8_test.o: chip8_test.cpp
	$(CC) -c -o $@ $^

chip8_unit.o: chip8_unit.cpp
	$(CC) -c -o $@ $^

glad.o: glad.c
	$(CC) -c -o $@ $^

.PHONY: clean

clean:
	rm -f *.o *~ core $(IDIR)/*~
