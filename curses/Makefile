CC=gcc
CFLAGS=-lncurses -lform
PROJECT=main
PANELS=panels
TITLE=title

all: $(PROJECT).o
	$(CC) -o $(PROJECT) $(PROJECT).c $(CFLAGS)

pan: $(PANELS).o
	$(CC) -o $(PANELS) $(PANELS).c $(CFLAGS) -lpanel

title: $(TITLE).o
	$(CC) -o $(TITLE) $(TITLE).c $(CFLAGS) -lpanel

clean:
	rm $(PROJECT).o $(PROJECT)
#ex: example.o
#	$(CC) -o ex example.c $(CFLAGS)
