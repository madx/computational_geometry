include /home/madx/dev/L3/geometrie/Helium-0.30/.config
CC = gcc
CFLAGS = -Wall -ansi -pedantic -lm
GTKFLAGS = $(shell pkg-config --libs --cflags gtk+-2.0)
ifeq (${DEBUG},1)
	CFLAGS += -g
endif
SRC = src/polygon.c
OBJ = ${SRC:.c=.o}

.PHONY: all clean env

all: ${OBJ}

polytest: ${OBJ}
	@echo CC src/tests/polygon.c "->" dist/polygon_test
	@$(CC) $(CFLAGS) -o dist/polygon_test $^ src/tests/polygon.c src/tests/base.c

gui: ${OBJ}
	@echo CC src/main.c "->" bin/polygones
	$(CC) $(CFLAGS) $(GTKFLAGS) -o bin/polygones $^ src/ui.c src/main.c

env:
	@echo $(HE_CFLAGS) $(HE_LIBS)

clean:
	rm -f *.o src/*.o

%.o: %.c
	@echo CC $< "->" $@
	@$(CC) -c $(CFLAGS) -o $@ $<


# projet_algo: ${OBJ}
# 	@echo CC projet_algo.c
# 	$(CC) $(CFLAGS) -o bin/projalgo $^ projet_algo.c
# 
# gtkpa: ${OBJ} src/ui.o
# 	@echo CC gtkpa.c
# 	$(CC) $(CFLAGS) $(GTKFLAGS) -o bin/gtkpa $^ gtkpa.c
# 
# 
# dist:
# 	mkdir -p $(DISTDIR)
# 	cp -r projet_algo.c README.projalgo src/ tests/ Makefile $(DISTDIR)
# 	tar cvzf $(DISTDIR).tar.gz $(DISTDIR)
# 	rm -rf $(DISTDIR)
# 
# src/ui.o: src/ui.c
# 	@echo CC $< "->" $@
# 	- $(CC) -c $(CFLAGS) $(GTKFLAGS) -o $@ $< 
# 	@echo
# 
# %.o: %.c
# 	@echo CC $< "->" $@
# 	- $(CC) -c $(CFLAGS) -o $@ $<
# 	@echo
