#include <stdlib.h>
#include <curses.h>
#include "sudoku.h"

void print_help_message() {
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
}


void print_divider_row(const chtype left, const chtype middle, const chtype right) {
    addch(left);
    for (int line_i = 0; line_i < 5; line_i++) 
        addch(ACS_HLINE);
    addch(middle);
    for (int line_i = 0; line_i < 5; line_i++) 
        addch(ACS_HLINE);
    addch(middle);
    for (int line_i = 0; line_i < 5; line_i++) 
        addch(ACS_HLINE);
    addch(right);
    printw("\n");
}

void print_grid(Grid* grid) {
    erase();
    print_divider_row(ACS_ULCORNER, ACS_TTEE, ACS_URCORNER);
    for (int row_i = 0; row_i < GROUP_SIZE; row_i++) {
        addch(ACS_VLINE);
        for (int column_i = 0; column_i < GROUP_SIZE; column_i++) {
            int grid_position = (row_i * GROUP_SIZE) + column_i;
            if (grid->values[grid_position] == 0) {
                printw(" ");
            } else if (grid->square_type[grid_position] == 0) {
                attron(COLOR_PAIR(1));
                printw("%d", grid->values[grid_position]);
                attroff(COLOR_PAIR(1));
            } else if (grid->square_type[grid_position] == 2) {
                attron(COLOR_PAIR(2));
                printw("%d", grid->values[grid_position]);
                attroff(COLOR_PAIR(2));
            } else {
                printw("%d", grid->values[grid_position]);
            }
            if ((column_i + 1) % 3 == 0)
                addch(ACS_VLINE);
            else
                printw(" ");
        }
        printw("\n");
        if((row_i + 1) % 3 == 0 && row_i != 8)
            print_divider_row(ACS_LTEE, ACS_PLUS, ACS_RTEE);
    } 
    print_divider_row(ACS_LLCORNER, ACS_BTEE, ACS_LRCORNER);
}


void make_move(int direction_x, int direction_y, Position* position) {
    if (position->x + direction_x >= 0 &&
        position->x + direction_x <= GROUP_SIZE - 1 &&
        position->y + direction_y >= 0  &&
        position->y + direction_y <= GROUP_SIZE - 1)
    {
        position->x += direction_x;
        position->y += direction_y;
        move(position->y + 1 + (position->y / 3), (position->x * 2) + 1);
        refresh();
    }
}


void add_number(Grid* grid, int c, Position* position) {
    int grid_position = position_on_grid(position);
    if (grid->square_type[grid_position] == 0) {
        grid->values[grid_position] = c;
        print_grid(grid);
        make_move(0, 0, position);
    }
}


void print_message(char* message, Position* position) {
    move(14, 0);
    clrtoeol();
    printw("%s", message);
    make_move(0, 0, position);
}

void editable(Grid* grid) {
    for (int grid_i = 0; grid_i < GRID_SIZE; grid_i++) {
        grid->square_type[grid_i] = 0;
    } 
}


void lock(Grid* grid) {
    for (int grid_i = 0; grid_i < GRID_SIZE; grid_i++) {
        if (grid->values[grid_i] != 0)
            grid->square_type[grid_i] = 1;
    } 
}
