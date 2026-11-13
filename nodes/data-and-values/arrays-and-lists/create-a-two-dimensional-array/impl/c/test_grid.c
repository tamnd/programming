// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

#include "grid.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int failures = 0;

static void check(const char *name, size_t rows, size_t cols, int value) {
    int *g = grid(rows, cols, value);
    int ok = g != NULL;
    for (size_t i = 0; ok && i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            if (g[i * cols + j] != value) {
                ok = 0;
            }
        }
    }
    if (ok && rows >= 2 && cols >= 1) {
        g[0] = value ^ 1;
        ok = g[cols] == value;
    }
    if (!ok) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
    free(g);
}

int main(void) {
    check("two-by-three", 2, 3, 0);
    check("three-by-one", 3, 1, 7);
    check("no-rows", 0, 3, 1);
    check("no-cols", 2, 0, 1);
    check("one-cell", 1, 1, 5);
    check("one-row", 1, 4, 2);
    check("one-col", 4, 1, 2);
    check("negative", 2, 2, -3);
    check("min-int", 2, 2, INT_MIN);
    check("max-int", 1, 2, INT_MAX);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
