CC = gcc
CFLAGS = -Wall -ansi -pedantic -lm
GTKFLAGS = $(shell pkg-config --libs --cflags gtk+-2.0)
ifeq (${DEBUG},1)
	CFLAGS += -ggdb
endif
SRC = src/polygon.c
OBJ = ${SRC:.c=.o}
USRC = src/ui.c src/callbacks.c
UOBJ = ${USRC:.c=.o}
TSRC = test/polygon.c
TOBJ = ${TSRC:.c=.o}

.PHONY: all tests clean env

all: gui

tests: ${OBJ} ${TOBJ}

gui: ${OBJ} ${UOBJ}
	@echo Make binary "->" bin/polytool
	@$(CC) $(CFLAGS) $(GTKFLAGS) -o bin/polytool $^ 

clean:
	rm -f *.o src/*.o test/*.o

src/ui.o: src/ui.c
	@echo CC $< "->" $@
	@$(CC) -c $(CFLAGS) $(GTKFLAGS) -o $@ $<

src/callbacks.o: src/callbacks.c
	@echo CC $< "->" $@
	@$(CC) -c $(CFLAGS) $(GTKFLAGS) -o $@ $<

src/polygon.o: src/polygon.c
	@echo CC $< "->" $@
	@$(CC) -c $(CFLAGS) -o $@ $<
