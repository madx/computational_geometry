include /home/madx/dev/L3/geometrie/Helium-0.30/.config
CC = gcc
CFLAGS = -Wall -ansi -pedantic -lm
GTKFLAGS = $(shell pkg-config --libs --cflags gtk+-2.0)
ifeq (${DEBUG},1)
	CFLAGS += -ggdb
endif
SRC = src/polygon.c
OBJ = ${SRC:.c=.o}
TSRC = test/polygon.c
TOBJ = ${TSRC:.c=.o}

.PHONY: all tests clean env

all: gui

tests: ${OBJ} ${TOBJ}

gui: ${OBJ}
	@echo CC src/main.c "->" bin/polytool
	$(CC) $(CFLAGS) $(GTKFLAGS) -o bin/polytool $^ src/ui.c src/main.c

env:
	@echo $(HE_CFLAGS) $(HE_LIBS)

clean:
	rm -f *.o src/*.o test/*.o

%.o: %.c
	@echo CC $< "->" $@
	@$(CC) -c $(CFLAGS) -o $@ $<
