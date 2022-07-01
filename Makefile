CC = gcc
CFLAGS = -std=c99 -Wall
objects = kilo.o terminal.o editor.o

all: kilo

kilo: $(objects)
	$(CC) $(CFLAGS) $(objects) -o kilo

$(objects): dbg.h

terminal.o: terminal.h editor.h

editor.o: editor.h

clean:
	rm kilo $(objects)

.PHONY : clean