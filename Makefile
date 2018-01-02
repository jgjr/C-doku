.POSIX:
CC = gcc
CFLAGS = -std=c99 -Wall -pedantic -Werror -Wextra -O3
LDLIBS = -lncurses

obj = c-doku.o helpers.o interface.o sudoku.o
testobj = helpers.o sudoku.o tests.o

all: c-doku

play: c-doku
	./c-doku

check: tests
	./tests

c-doku: $(obj)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(obj) $(LDLIBS)

tests: $(testobj)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $(testobj) -lcmocka

c-doku.o: c-doku.c sudoku.h interface.h
helpers.o: helpers.c sudoku.h
interface.o: interface.c sudoku.h
sudoku.o: sudoku.c helpers.h sudoku.h
tests.o: tests.c helpers.h sudoku.h

clean:
	rm -f c-doku tests c-doku.x $(obj) $(testobj)
