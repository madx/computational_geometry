CC = gcc
CFLAGS = -Wall -ansi -pedantic -lm
GTKFLAGS = $(shell pkg-config --libs --cflags gtk+-2.0)
ifeq (${DEBUG},1)
	CFLAGS += -ggdb
endif
SRC = src/polygon.c src/stack.c src/algorithms.c
OBJ = ${SRC:.c=.o}
USRC = src/ui.c src/callbacks.c src/algo_split.c src/algo_hull.c src/algo_rtr.c\
       src/algo_mtr.c
UOBJ = ${USRC:.c=.o}

.PHONY: all clean env

all: gui

test_polygon: ${OBJ}
	@echo Make binary "->" dist/polygon_test
	@$(CC) $(CFLAGS) -o dist/polygon_test test/polygon.c $^
	@valgrind --leak-check=full --track-origins=yes dist/polygon_test

test_stack: ${OBJ}
	@echo Make binary "->" dist/stack_test
	@$(CC) $(CFLAGS) -o dist/stack_test test/stack.c $^
	@valgrind --leak-check=full --track-origins=yes dist/stack_test

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

src/stack.o: src/stack.c
	@echo CC $< "->" $@
	@$(CC) -c $(CFLAGS) -o $@ $<

src/algorithms.o: src/algorithms.c
	@echo CC $< "->" $@
	@$(CC) -c $(CFLAGS) -o $@ $<

src/algo_split.o: src/algo_split.c
	@echo CC $< "->" $@
	@$(CC) -c $(CFLAGS) $(GTKFLAGS) -o $@ $<

src/algo_hull.o: src/algo_hull.c
	@echo CC $< "->" $@
	@$(CC) -c $(CFLAGS) $(GTKFLAGS) -o $@ $<

src/algo_mtr.o: src/algo_mtr.c
	@echo CC $< "->" $@
	@$(CC) -c $(CFLAGS) $(GTKFLAGS) -o $@ $<

src/algo_rtr.o: src/algo_rtr.c
	@echo CC $< "->" $@
	@$(CC) -c $(CFLAGS) $(GTKFLAGS) -o $@ $<
