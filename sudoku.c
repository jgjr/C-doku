#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "helpers.h"
#include "sudoku.h"


Grid* new_blank_grid() {
    Grid* grid = malloc(sizeof(Grid));
    clear_grid(grid);
    return grid;
}


void clear_grid(Grid* grid) {
    for (int grid_i = 0; grid_i < GRID_SIZE; grid_i++) {
        grid->values[grid_i] = 0;
        grid->blanks[grid_i] = grid_i;
    } 
    grid->blank_count = GRID_SIZE;
}


void replace_grid(Grid* source_grid, Grid* dest_grid) {
    for (int grid_i = 0; grid_i < GRID_SIZE; grid_i++) {
        dest_grid->values[grid_i] = source_grid->values[grid_i];
        dest_grid->blanks[grid_i] = source_grid->blanks[grid_i];
    } 
    dest_grid->blank_count = source_grid->blank_count;
}


bool check_rows(Grid* grid) {
    for (int row_start = 0; row_start < GRID_SIZE; row_start += GROUP_SIZE) {
        Group row = {.values = {0}, .count = 0};
        for (int row_item = row_start; row_item < row_start + GROUP_SIZE; row_item++) {
            row.values[row.count] = grid->values[row_item];
            row.count++;
        }
        if (group_has_duplicates(row) == true)
            return false;
    }
    return true;
}


bool check_cols(Grid* grid) {
    for (int col_start = 0; col_start < GROUP_SIZE; col_start++) {
        Group col = {.values = {0}, .count = 0};
        for (int col_item = col_start; col_item < GRID_SIZE; col_item += GROUP_SIZE) {
            col.values[col.count] = grid->values[col_item];
            col.count++;
        }
        if (group_has_duplicates(col) == true)
            return false;
    }
    return true;
}


bool check_boxes(Grid* grid) {
    Group box = {.values = {0}, .count = 0};
    for (int box_i = 0; box_i < 27; box_i++) {
        int box_x = ((box_i / GROUP_SIZE) * 3) + (box_i % 3);
        for (int box_j = 0; box_j < 3; box_j++) {
            Position pos = {.x = box_x, .y = ((box_i % GROUP_SIZE) / 3) * 3 + box_j};
            box.values[box.count] = get_grid_value(grid, pos);
            box.count++;
            if(box.count == GROUP_SIZE) {
                if (group_has_duplicates(box) == true) {
                    return false;
                } else {
                    memset(box.values, 0, GROUP_SIZE * sizeof(int));
                    box.count = 0;
                }
            }
        }
    }
    return true;
}


bool is_grid_valid(Grid* grid) {
    if (check_rows(grid) == false || check_cols(grid) == false || check_boxes(grid) == false) {
        return false;
    } else {
        return true;
    }
}


bool is_grid_complete(Grid* grid) {
    set_blanks(grid);
    if(grid->blank_count > 0) {
        return false;        
    } else {
        return true;
    }
}


int find_random_valid_entry(Grid* grid, int square) {
    int tried[GROUP_SIZE] = {0};
    int tried_count = 0;
    while(tried_count <= 8) {
        int rand_int = random_in_range(8);
        if (tried[rand_int] == 0) {
            tried[rand_int] = 1;
            tried_count++;
            grid->values[square] = rand_int + 1;
            if (is_grid_valid(grid) == true) {
                return rand_int + 1;
            }
        }
    }
    grid->values[square] = 0;
    return 0;
}


int find_incremented_valid_entry(Grid* grid, int square, int start) {
    int val = start + 1;
    while(val <= GROUP_SIZE) {
        grid->values[square] = val;
        if (is_grid_valid(grid) == true) {
            return val;
        } else {
            val++;
        }
    }
    grid->values[square] = 0;
    return 0;
}


void new_complete_grid(Grid* grid) {
    clear_grid(grid);
    int i = 0;
    int last_backtrack = 0;
    while(i < GRID_SIZE) {
        int entry = find_random_valid_entry(grid, i);
        if (entry) {
            i++;
        } else {
            i = (i / GROUP_SIZE) * GROUP_SIZE;
            if(i == last_backtrack && i >= GROUP_SIZE)
                i -= GROUP_SIZE;
            for (int grid_i = i; grid_i < GRID_SIZE; grid_i++)
                grid->values[grid_i] = 0;
            last_backtrack = i;
        }
    }
    grid->blank_count = 0;
}


void new_game(Grid* grid, int filled) {
    new_complete_grid(grid);
    while(grid->blank_count < GRID_SIZE - filled) {
        int random_to_remove = random_in_range(80);
        if (grid->values[random_to_remove]) {
            int removed = grid->values[random_to_remove];
            grid->values[random_to_remove] = 0;
            grid->blank_count++;
            if (single_solution(*grid) == false) {
                grid->values[random_to_remove] = removed;
                grid->blank_count--;
            }
        }
    }
}


void solve_game(Grid* grid) {
    set_blanks(grid);
    find_solution(grid, 0);
}


bool find_solution(Grid* grid, int current_blank) {
    if (current_blank >= grid->blank_count)
        return true;
    int current_square = grid->blanks[current_blank];
    int entry = find_incremented_valid_entry(grid, current_square, grid->values[current_square]);
    if (entry != 0) {
        return find_solution(grid, current_blank + 1);
    } else if(entry == 0 && current_blank > 0) {
        return find_solution(grid, current_blank - 1);
    } else {
        return false;
    }
}


void set_blanks(Grid* grid) {
    grid->blank_count = 0;
    for (int grid_i = 0; grid_i < GRID_SIZE; grid_i++) {
        if (grid->values[grid_i] == 0) {
            grid->blanks[grid->blank_count] = grid_i;
            grid->blank_count++;
        }
    }    
}


bool single_solution(Grid grid) {
    solve_game(&grid);
    grid.values[grid.blanks[grid.blank_count - 1]] = 0;
    bool retry = find_solution(&grid, grid.blank_count - 2);
    if (retry == false) {
        return true;
    } else {
        return false;
    }
}


int get_grid_value(Grid* grid, Position pos) {
    return grid->values[position_on_grid(pos)];
}


int position_on_grid(Position pos) {
    return (pos.y * GROUP_SIZE) + pos.x;
}
