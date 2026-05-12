CC		= gcc
CFLAGS	= -Iinclude -std=gnu23 -Wall -O2
# LIBS	= lib/libSDL3.a
TARGET	= bin/geomist

all:	$(TARGET)

$(TARGET):	obj/main.o obj/display.o obj/demo.o
	@mkdir -p bin
	$(CC) -lm -lSDL3 -o $@ $^ $(LIBS)

obj/%.o:	src/%.c
	@mkdir -p obj
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	rm -rf bin/*
	rm -rf obj/*
