CC = gcc
CFLAGS = -std=c99 -Wall
objects = main.o terminal.o editor.o
target = min

all: $(target)

$(target): $(objects)
	$(CC) $(CFLAGS) $(objects) -o $@

$(objects): dbg.h

terminal.o: terminal.h editor.h

editor.o: editor.h

clean:
	rm $(target) $(objects)

.PHONY : clean