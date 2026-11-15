// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-nested-loops/.

#include "find_in_grid.h"

#include <stdio.h>

static int failures = 0;

static void check(const char *name, const int *const *rows, const size_t *lens, size_t n, int target, int row,
                  int col) {
    cell got = find_in_grid(rows, lens, n, target);
    if (got.row != row || got.col != col) {
        fprintf(stderr, "case %s failed: got (%d, %d)\n", name, got.row, got.col);
        failures++;
    }
}

int main(void) {
    static const int m0[] = {1, 2, 3}, m1[] = {4, 5, 6}, m2[] = {7, 8, 9};
    const int *middle[] = {m0, m1, m2};
    const size_t three3[] = {3, 3, 3};
    check("middle", middle, three3, 3, 5, 1, 1);
    static const int t0[] = {3, 8, 12}, t1[] = {5, 20, 20}, t2[] = {20, 1, 4};
    const int *twenty[] = {t0, t1, t2};
    check("first-twenty", twenty, three3, 3, 20, 1, 1);
    static const int f0[] = {9, 1}, f1[] = {2, 3};
    const int *first[] = {f0, f1};
    const size_t two2[] = {2, 2};
    check("first-cell", first, two2, 2, 9, 0, 0);
    static const int l0[] = {1, 2}, l1[] = {3, 9};
    const int *last[] = {l0, l1};
    check("last-cell", last, two2, 2, 9, 1, 1);
    static const int r0[] = {1, 2, 9}, r1[] = {9, 3, 4};
    const int *rowmajor[] = {r0, r1};
    const size_t three2[] = {3, 3};
    check("row-major", rowmajor, three2, 2, 9, 0, 2);
    static const int s0[] = {1, 2}, s1[] = {3, 4};
    const int *missing[] = {s0, s1};
    check("missing", missing, two2, 2, 9, -1, -1);
    check("empty-grid", NULL, NULL, 0, 1, -1, -1);
    static const int e2[] = {5};
    const int *emptyrows[] = {NULL, NULL, e2};
    const size_t elens[] = {0, 0, 1};
    check("empty-rows", emptyrows, elens, 3, 5, 2, 0);
    static const int g0[] = {1}, g1[] = {2, 3, 4}, g2[] = {5, 6};
    const int *ragged[] = {g0, g1, g2};
    const size_t glens[] = {1, 3, 2};
    check("ragged", ragged, glens, 3, 6, 2, 1);
    static const int n0[] = {-1, -2}, n1[] = {-3, -4};
    const int *negative[] = {n0, n1};
    check("negative", negative, two2, 2, -3, 1, 0);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
