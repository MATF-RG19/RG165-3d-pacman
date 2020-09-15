PROGRAM = RG165-3d-pacman
CC      = gcc
CFLAGS  = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL -lm

$(PROGRAM): main.o level.o image.o
	$(CC) $(LDFLAGS) -o $(PROGRAM) main.o level.o image.o $(LDLIBS)

level.o : level.c 
	$(CC) -o $@ $< -c $(LDLIBS)

image.o: image.c 
	$(CC) -c image.c
	
.PHONY: clean dist

clean:
	-rm *.o $(PROGRAM)

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)
