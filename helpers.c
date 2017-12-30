#include <stdlib.h>
#include <stdbool.h>
#include "sudoku.h"

int random_in_range(int max) {
    unsigned long
        num_bins = (unsigned long) max + 1,
        num_rand = (unsigned long) RAND_MAX + 1,
        bin_size = num_rand / num_bins,
        defect   = num_rand % num_bins;
    long x;
    do {
        x = rand();
    }
    while (num_rand - defect <= (unsigned long)x);
    return (int) x / bin_size;
}


bool group_has_duplicates(Group group) {
    for (int j = 0; j < group.count - 1; j++) {
        for (int k = j + 1; k < group.count; k++) {
            if (group.values[j] == group.values[k] && group.values[j] != 0)
                return true;
        }
    }
    return false;
}
