all: output

output: c-doku.c sudoku.c helpers.c interface.c
	gcc helpers.c sudoku.c interface.c c-doku.c -o c-doku -lncurses -ltinfo

debug: c-doku.c sudoku.c helpers.c interface.c
	gcc -g helpers.c sudoku.c interface.c c-doku.c -o c-doku.x -lncurses -ltinfo

test: tests.c c-doku.c sudoku.c helpers.c interface.c
	gcc helpers.c sudoku.c tests.c -o tests -lcmocka && ./tests

clean:
	 rm c-doku c-doku.x 
