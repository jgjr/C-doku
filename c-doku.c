#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <ctype.h>
#include <time.h>
#include <string.h>
#include "sudoku.h"
#include "interface.h"

bool save_grid(Grid* grid, char* grid_file_path) {
    FILE *grid_file = fopen(grid_file_path, "w");
    if (grid_file == NULL) {
        printw("Error saving grid!\n");
        return false;
    }
    for (int grid_i = 0; grid_i < 81; grid_i++) {
        fprintf(grid_file, "%d", grid->values[grid_i]);
    } 
    fclose(grid_file);
    return true;
}


bool open_grid(Grid* grid, char* grid_file_path) {
    FILE *grid_file = fopen(grid_file_path, "r");
    if (grid_file == NULL) {
        return false;
    }
    for (int grid_i = 0; grid_i < 81; grid_i++) {
        fscanf(grid_file, "%1d", &grid->values[grid_i]);
    } 
    fclose(grid_file);
    return true;
}


typedef struct Grids {
   Grid* grid;
   Grid* undo_grid;
   Grid* redo_grid;
   int* undo_level;
} Grids;


void store_undo(Grids grids) {
    *grids.undo_level = 0;
    replace_grid(grids.grid, grids.undo_grid);
}


void undo(Grids grids) {
    if (*grids.undo_level >= 0) {
        *grids.undo_level = -1;
        replace_grid(grids.grid, grids.redo_grid);
        replace_grid(grids.undo_grid, grids.grid);
    }
}


void redo(Grids grids) {
    if (*grids.undo_level <= 0) {
        *grids.undo_level = 1;
        replace_grid(grids.grid, grids.undo_grid);
        replace_grid(grids.redo_grid, grids.grid);
    }
}

int main(int argc, char *argv[]){


    /* Display a help message if the -h option is specified */
    if (argc > 1 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        printf("\nUsage:\n");
        printf("c-doku [-h/--help] - displays this help message\n");
        printf("c-doku - opens saved game, or new medium grid if no file is present\n");
        printf("c-soku [easy/medium/hard] - opens a new game with desired difficulty level\n");
        printf("\nUseful keybindings:\n");
        printf("n - new game\n");
        printf("s - solve game\n");
        printf("x - clear grid\n");
        printf("v - validate grid\n");
        printf("c - check if grid is complete\n");
        printf("For complete list see README.md\n");
        printf("\n");
        return 0;
    }


    /* Setup for ncurses and random number generation */
    srand(time(NULL));
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);


    /* Setup for the grid file */
    char* grid_file_path;
    char* grid_file_name = "/.grid.cdoku";
    grid_file_path = malloc(strlen(getenv("HOME")) + strlen(grid_file_name) + 1);
    strcpy(grid_file_path, getenv("HOME"));
    strcat(grid_file_path, grid_file_name);


    /* The key pressed by the user and its integer representation */
    char c;
    int c_int;
    int filled = 35;


    Position position = { .x = 0, .y = 0 };
    Grid* grid = new_blank_grid();
    Grid* undo_grid = new_blank_grid();
    Grid* redo_grid = new_blank_grid();
    int undo_level = 0;
    Grids grids = { .grid = grid, .undo_grid = undo_grid, .redo_grid = redo_grid, .undo_level = &undo_level };
    

    bool existing_grid = open_grid(grid, grid_file_path);
    store_undo(grids);
    print_grid(grid); 


    /* If a difficulty level has been specified */
    if(argc > 1 && strcmp(argv[1], "easy") == 0) {
        filled = 40;
    } else if(argc > 1 && strcmp(argv[1], "medium") == 0) {
        filled = 35;
    } else if(argc > 1 && strcmp(argv[1], "hard") == 0) {
        filled = 30;
    }


    if (existing_grid == false) {
        new_game(grid, filled);
    }
    print_grid(grid); 
    make_move(0, 0, &position);
    if (existing_grid == true) {
        print_message("SAVED GRID OPENED", &position);
    } else {
        print_message("NEW GAME", &position);
    }


    while((c = getch()) != 'q') {
        switch(c) {
            case 4:
            case 'h':
                make_move(-1, 0, &position);
                break;
            case 2:
            case 'j':
                make_move(0, 1, &position);
                break;
            case 3:
            case 'k':
                make_move(0, -1, &position);
                break;
            case 5:
            case 'l':
                make_move(1, 0, &position);
                break;
            case 'w':
                save_grid(grid, grid_file_path);
                print_message("SAVED", &position);
                break;
            case 'n':
                store_undo(grids);
                new_game(grid, filled);
                print_grid(grid);
                print_message("NEW GAME", &position);
                break;
            case 's':
                if(is_grid_valid(grid) == false) {
                    print_message("INVALID", &position);
                } else {
                    store_undo(grids);
                    solve_game(grid);
                    print_grid(grid);
                    print_message("SOLVED", &position);
                }
                break;
            case 'm':
                if(is_grid_valid(grid) == false) {
                    print_message("INVALID", &position);
                } else if (single_solution(*grid) == true) {
                    print_message("SINGLE SOLUTION", &position);
                } else {
                    print_message("MULTIPLE SOLUTIONS", &position);
                }
                break;
            case 'x':
                store_undo(grids);
                clear_grid(grid);
                print_grid(grid);
                make_move(0, 0, &position);
                print_message("GRID CLEARED", &position);
                break;
            case 'v':
                if(is_grid_valid(grid) == true) {
                    print_message("VALID", &position);
                } else {
                    print_message("INVALID", &position);
                }
                break;
            case 'c':
                if(is_grid_valid(grid) == false) {
                    print_message("INVALID", &position);
                } else if(is_grid_complete(grid) == 0) {
                    print_message("INCOMPLETE", &position);
                } else {
                    print_message("COMPLETE", &position);
                }
                break;
            case 'u':
                undo(grids);
                print_grid(grid);
                make_move(0, 0, &position);
                break;
            case 'r':
                redo(grids);
                print_grid(grid);
                make_move(0, 0, &position);
                break;
            case ' ':
                print_message("", &position);
                break;
            case 'd':
                store_undo(grids);
                add_number(grid, 0, &position);
                break;
            case '\t':
                if (position.x + 3 < 9) {
                    make_move(3, 0, &position);
                } else {
                    make_move(-6, 3, &position);
                }
                break;
            case '\n':
                if (position.y + 3 < 9) {
                    make_move(0, 3, &position);
                } else {
                    make_move(3, -6, &position);
                }
                break;
            case '^':
                make_move(-position.x, 0, &position);
                break;
            case '$':
                make_move(8 - position.x, 0, &position);
                break;
            case 'g':
                make_move(0, -position.y, &position);
                break;
            case 'G':
                make_move(0, 8 - position.y, &position);
                break;
            default:
                c_int = c - '0';
                if(c_int >= 0 && c_int <= 9) {
                    store_undo(grids);
                    add_number(grid, c_int, &position);
                }
                break;
        }
    }

    endwin();

    return 0;
}
