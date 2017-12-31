#include <stdlib.h>
#include <curses.h>
#include "sudoku.h"

void print_grid(Grid* grid) {
    erase();
    printw("+-----+-----+-----+\n");
    for (int row_i = 0; row_i < GROUP_SIZE; row_i++) {
        printw("|");
        for (int column_i = 0; column_i < GROUP_SIZE; column_i++) {
            int grid_position = (row_i * GROUP_SIZE) + column_i;
            if (grid->values[grid_position] == 0)
                printw(" ");
            else
                printw("%d", grid->values[grid_position]);
            if ((column_i + 1) % 3 == 0)
                printw("|");
            else
                printw(" ");
        }
        printw("\n");
        if((row_i + 1) % 3 == 0)
            printw("+-----+-----+-----+\n");
    } 
}


int* get_cursor_position(Position* position) {
    int* cursor = malloc(sizeof(int) * 2);
    cursor[1] = (position->x * 2) + 1;
    cursor[0] = position->y + 1 + (position->y / 3);
    return cursor;
}


void position_cursor(Position* position) {
    int* cursor_position = get_cursor_position(position);
    move(cursor_position[0], cursor_position[1]);
    refresh();
    free(cursor_position);
}


void make_move(int direction_x, int direction_y, Position* position) {
    if (position->x + direction_x >= 0 &&
        position->x + direction_x <= GROUP_SIZE - 1 &&
        position->y + direction_y >= 0  &&
        position->y + direction_y <= GROUP_SIZE - 1)
    {
        position->x += direction_x;
        position->y += direction_y;
        position_cursor(position);
    }
}


void add_number(Grid* grid, int c, Position* position) {
    int grid_position = (position->y * GROUP_SIZE) + position->x;
    grid->values[grid_position] = c;
    print_grid(grid);
    make_move(0, 0, position);
}


void print_message(char* message, Position* position) {
    move(14, 0);
    clrtoeol();
    printw("%s", message);
    make_move(0, 0, position);
}
