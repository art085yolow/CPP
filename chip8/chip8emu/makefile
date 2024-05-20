IDIR=.
CC=g++
CCFLAGS +=-I$(IDIR) $(LDIR) $(LIBS) 

NOPT = -O0 -g
OPT = -O3

ODIR=obj
LDIR=-L.

LIBS=-lm -lSDL2 -lSDL2main -lGL -Wall -Wextra -fsanitize=address -Wfree-nonheap-object

#_DEPS = glad.h khrplatform.h
#_OBJ = chip8_unit.o chip8emu.o

#$(ODIR)/%.o: %.cpp $(CFLAGS)
#	$(CC) -c -o $@ $< $(CFLAGS)
all: chipemu
	
chipemu: chip8emu.cpp chip8_unit.o
	$(CC) $(NOPT) $(CCFLAGS) chip8emu.cpp chip8_unit.o -o $@

test_chip8: chip8_test.o chip8_unit.o
	$(CC) -o $@ $^

chip8_test.o: chip8_test.cpp
	$(CC) -c -o $@ $^

chip8_unit.o: chip8_unit.cpp
	$(CC) -c -o $@ $^

decodechip8: decodechip8.cpp
	$(CC) -o $@ $^

.PHONY: clean
.PHONY: final

final: chip8emu.o chip8_unit.o
	$(CC) $(OPT) $(CCFLAGS) chip8emu.o chip8_unit.o -o $@

clean:
	rm -f *.o *~ core $(IDIR)/*~
