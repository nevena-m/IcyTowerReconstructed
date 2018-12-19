PROGRAM = icy_tower
CC      = gcc
GDB		= -ggdb
CFLAGS  = -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglfw -lglut -lGLU -lGL -lm

C_FILES= $(wildcard src/*.c)
OBJ_FILES= $(addprefix obj/,$(notdir $(C_FILES:.c=.o)))
HEAD_FILES = $(wildcard include/*.h)

$(PROGRAM): $(OBJ_FILES)  $(HEAD_FILES)
	$(CC) $(GDB) $(LDFLAGS) $(OBJ_FILES) -o $@ $(LDLIBS)

obj/%.o: src/%.c | obj
	$(CC) $(GDB) $< -c $(CFLAGS) -o $@


.PHONY: clean dist

obj:
	mkdir obj

clean:
	-rm $(OBJ_FILES) $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)

