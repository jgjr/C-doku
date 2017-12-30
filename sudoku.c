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
    for (int grid_i = 0; grid_i < 81; grid_i++) {
        grid->values[grid_i] = 0;
        grid->blanks[grid_i] = grid_i;
    } 
    grid->blank_count = 81;
}


Grid* clone_grid(Grid source_grid) {
    Grid* dest_grid = new_blank_grid();
    for (int grid_i = 0; grid_i < 81; grid_i++) {
        dest_grid->values[grid_i] = source_grid.values[grid_i];
        dest_grid->blanks[grid_i] = source_grid.blanks[grid_i];
    } 
    dest_grid->blank_count = source_grid.blank_count;
    return dest_grid;
}


void replace_grid(Grid source_grid, Grid* dest_grid) {
    for (int grid_i = 0; grid_i < 81; grid_i++) {
        dest_grid->values[grid_i] = source_grid.values[grid_i];
        dest_grid->blanks[grid_i] = source_grid.blanks[grid_i];
    } 
    dest_grid->blank_count = source_grid.blank_count;
}


bool check_rows(Grid grid) {
    for (int row_start = 0; row_start < 81; row_start += 9) {
        Group row = {.values = {0}, .count = 0};
        for (int row_item = row_start; row_item < row_start + 9; row_item++) {
            row.values[row.count] = grid.values[row_item];
            row.count++;
        }
        if (group_has_duplicates(row) == true)
            return false;
    }
    return true;
}


bool check_cols(Grid grid) {
    for (int col_start = 0; col_start < 9; col_start++) {
        Group col = {.values = {0}, .count = 0};
        for (int col_item = col_start; col_item < 81; col_item += 9) {
            col.values[col.count] = grid.values[col_item];
            col.count++;
        }
        if (group_has_duplicates(col) == true)
            return false;
    }
    return true;
}


bool check_boxes(Grid grid) {
    Group box = {.values = {0}, .count = 0};
    for (int box_i = 0; box_i < 27; box_i++) {
        int box_x = ((box_i / 9) * 3) + (box_i % 3);
        for (int box_j = 0; box_j < 3; box_j++) {
            Position pos = {.x = box_x, .y = ((box_i % 9) / 3) * 3 + box_j};
            box.values[box.count] = get_grid_value(grid, pos);
            box.count++;
            if(box.count == 9) {
                if (group_has_duplicates(box) == true) {
                    return false;
                } else {
                    memset(box.values, 0, 9);
                    box.count = 0;
                }
            }
        }
    }
    return true;
}


bool is_grid_valid(Grid grid) {
    if (check_rows(grid) == false || check_cols(grid) == false || check_boxes(grid) == false) {
        return false;
    } else {
        return true;
    }
}


bool is_grid_complete(Grid grid) {
    set_blanks(&grid);
    if(grid.blank_count > 0) {
        return false;        
    } else {
        return true;
    }
}


int find_random_valid_entry(Grid grid, int square) {
    Grid* trial_grid = clone_grid(grid);
    int tried[9] = {0};
    int tried_count = 0;
    int ret = 0;
    while(tried_count <= 8) {
        int rand_int = random_in_range(8);
        if (tried[rand_int] == 0) {
            tried[rand_int] = 1;
            tried_count++;
            trial_grid->values[square] = rand_int + 1;
            if (is_grid_valid(*trial_grid) == true) {
                ret = rand_int + 1;
                break;
            }
        }
    }
    free(trial_grid);
    return ret;
}


int find_incremented_valid_entry(Grid grid, int square, int start) {
    Grid* trial_grid = clone_grid(grid);
    int ret = 0;
    int val = start + 1;
    while(val <= 9) {
        trial_grid->values[square] = val;
        if (is_grid_valid(*trial_grid) == true) {
            ret = val;
            break;
        } else {
            val++;
        }
    }
    free(trial_grid);
    return ret;
}


Grid* new_complete_grid() {
    Grid* grid = new_blank_grid();
    int i = 0;
    int last_backtrack = 0;
    while(i < 81) {
        int entry = find_random_valid_entry(*grid, i);
        if (entry) {
            grid->values[i] = entry;
            i++;
        } else {
            i = (i / 9) * 9;
            if(i == last_backtrack && i >= 9)
                i -= 9;
            for (int grid_i = i; grid_i < 81; grid_i++)
                grid->values[grid_i] = 0;
            last_backtrack = i;
        }
    }
    grid->blank_count = 0;
    return grid;
}


Grid* new_game(int filled) {
    Grid* grid = new_complete_grid();
    while(grid->blank_count < 81 - filled) {
        int random_to_remove = random_in_range(80);
        if (grid->values[random_to_remove]) {
            int removed = grid->values[random_to_remove];
            grid->values[random_to_remove] = 0;
            grid->blank_count++;
            if (is_new_grid_valid(*grid) == false) {
                grid->values[random_to_remove] = removed;
                grid->blank_count--;
            }
        }
    }
    return grid;
}


bool solve_game(Grid* grid) {
    if (is_grid_valid(*grid) == false)
        return false;
    set_blanks(grid);
    first_solution(grid, 0);
    return true;
}


bool first_solution(Grid* grid, int current) {
    if (current >= grid->blank_count)
        return true;
    int current_blank = grid->blanks[current];
    grid->values[current_blank] = find_incremented_valid_entry(*grid, current_blank, grid->values[current_blank]);
    if (grid->values[current_blank] != 0) {
        first_solution(grid, current + 1);
    } else if(current > 0) {
        first_solution(grid, current - 1);
    }
}


void set_blanks(Grid* grid) {
    grid->blank_count = 0;
    for (int grid_i = 0; grid_i < 81; grid_i++) {
        if (grid->values[grid_i] == 0) {
            grid->blanks[grid->blank_count] = grid_i;
            grid->blank_count++;
        }
    }    
}


bool is_new_grid_valid(Grid grid) {
    Grid* trial_grid = clone_grid(grid);
    set_blanks(trial_grid);
    int num = num_solutions(trial_grid, 0, 0);
    free(trial_grid);
    if (num == 1) {
        return true;
    } else {
        return false;
    }
}


int num_solutions(Grid* grid, int current, int solutions) {
    if (solutions > 1)
        return solutions;
    if (current >= grid->blank_count) {
        solutions++;
        solutions += num_solutions(grid, current - 1, solutions);
        return solutions;
    }
    int current_blank = grid->blanks[current];
    grid->values[current_blank] = find_incremented_valid_entry(*grid, current_blank, grid->values[current_blank]);
    if (grid->values[current_blank]) {
        num_solutions(grid, current + 1, solutions);
    } else if(current > 0) {
        num_solutions(grid, current - 1, solutions);
    }
}


int get_grid_value(Grid grid, Position pos) {
    return grid.values[position_on_grid(pos)];
}


int position_on_grid(Position pos) {
    return (pos.y * 9) + pos.x;
}
