CC=gcc
CFLAGS=-lncurses
PROJECT=main

all: $(PROJECT).o
	$(CC) -o $(PROJECT) $(PROJECT).c disassembler/8080disasm.c window.c  $(CFLAGS)
clean:
	rm $(PROJECT).o $(PROJECT)
#ex: example.o
#	$(CC) -o ex example.c $(CFLAGS)
