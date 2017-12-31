#include <stdbool.h>

#ifndef SUDOKU_FUNCTIONS_H_
#define SUDOKU_FUNCTIONS_H_

typedef struct Position {
   int x;
   int y;
} Position;

typedef struct Movement {
   int x;
   int y;
} Movement;

typedef struct Grid {
   int values[81];
   int blanks[81];
   int blank_count;
} Grid;

typedef struct Group {
   int values[9];
   int count;
} Group;

Grid* new_blank_grid();

void clear_grid(Grid* grid);

Grid* clone_grid(Grid source_grid);

void replace_grid(Grid* source_grid, Grid* dest_grid);

bool check_rows(Grid* grid);

bool check_cols(Grid* grid);

bool check_boxes(Grid* grid);

bool is_grid_valid(Grid* grid);

bool is_grid_complete(Grid* grid);

int find_random_valid_entry(Grid* grid, int square);

int find_incremented_valid_entry(Grid* grid, int square, int start);

void new_complete_grid(Grid* grid);

void new_game(Grid* grid, int filled);

bool solve_game(Grid* grid);

bool find_solution(Grid* grid, int current_blank);

void set_blanks(Grid* grid);

bool is_new_grid_valid(Grid grid);

int num_solutions(Grid* grid, int current, int solutions);

int get_grid_value(Grid* grid, Position pos);

int position_on_grid(Position pos);

#endif
