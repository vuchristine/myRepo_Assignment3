CC=gcc
CFLAGS=-I. -ggdb -std=c99 -Wall
LDFLAGS= -lm
pngPlay: pngPlay.o pixutils.h pixutils.o lodepng.o

clean:
	rm *.o pngPlay
