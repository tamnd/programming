// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-a-list/.

#include "sort_ints.h"

#include <stdlib.h>

// compare returns a negative value, 0 or a positive value. It does not use
// x - y, because the subtraction can overflow.
static int compare(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

void sort_ints(int *items, size_t n) {
    if (n > 1) {
        qsort(items, n, sizeof items[0], compare);
    }
}
