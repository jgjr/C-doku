#include "sudoku.h"

#ifndef INTERFACE_FUNCTIONS_H_
#define INTERFACE_FUNCTIONS_H_

void print_help_message();

void print_divider_row();

void print_grid(Grid* grid);

void make_move(int direction_x, int direction_y, Position* position);

void add_number(Grid* grid, int c, Position* position);

void print_message(char* message, Position* position);

void editable(Grid* grid);

void lock(Grid* grid);

#endif
