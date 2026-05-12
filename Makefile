CC		= gcc
GLSLC	= glslc
INC		= -Iinclude
CFLAGS	= $(INC) -std=gnu23 -Wall -O2
# LIBS	= lib/libSDL3.a
TARGET	= bin/geomist

all:	$(TARGET)

FILES	= main.c \
		  display.c \
		  demo.c \
		  shader.c \
		  graphics/mist.c

SHADER_FILES	= ellipse_mist.vert \
				  mist.frag

SRCS			= $(FILES:%=src/%)
OBJS			= $(SRCS:src/%.c=obj/%.o)
SHADER_SRCS		= $(SHADER_FILES:%=src/shader/%)
VERT_SPVS		= $(SHADER_SRCS:src/shader/%.vert=obj/vert/%.spv)
FRAG_SPVS		= $(SHADER_SRCS:src/shader/%.frag=obj/frag/%.spv)

$(TARGET):		$(OBJS)
	@mkdir -p bin
	$(CC) -lm -lSDL3 -o $@ $^ $(LIBS)

obj/shader.o:	src/shader.c $(VERT_SPVS) $(FRAG_SPVS)
	@mkdir -p src
	$(CC) -c $(CFLAGS) -o $@ $<

obj/%.o:		src/%.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) -o $@ $<

obj/vert/%.spv:	src/shader/%.vert
	@mkdir -p obj/vert
	$(GLSLC) -c -o $@ $<

obj/frag/%.spv:	src/shader/%.frag
	@mkdir -p obj/frag
	$(GLSLC) -c -o $@ $<

clean:
	rm -rf bin/*
	rm -rf obj/*
