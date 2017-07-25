CC=gcc
#CFLAGS=-lSDL
PROJECT=main

all: $(PROJECT).o
	$(CC) -o $(PROJECT) $(PROJECT).c #$(CFLAGS)
clean:
	rm $(PROJECT).o $(PROJECT) 
#ex: example.o
#	$(CC) -o ex example.c $(CFLAGS)
