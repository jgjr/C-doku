#include "sudoku.h"

#ifndef INTERFACE_FUNCTIONS_H_
#define INTERFACE_FUNCTIONS_H_

void print_divider_row();

void print_grid(Grid* grid);

int* get_cursor_position(Position* position);

void position_cursor(Position* position);

void make_move(int direction_x, int direction_y, Position* position);

void add_number(Grid* grid, int c, Position* position);

void print_message(char* message, Position* position);

#endif
