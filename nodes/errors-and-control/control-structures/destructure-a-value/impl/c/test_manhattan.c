// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/destructure-a-value/.

#include "manhattan.h"

#include <stdio.h>

static int failures = 0;

static void check(const char *name, const int p[2], const int q[2], int want) {
    if (manhattan(p, q) != want) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
}

int main(void) {
    check("first", (const int[]){1, 2}, (const int[]){4, 6}, 7);
    check("on-a-line", (const int[]){0, 0}, (const int[]){10, 0}, 10);
    check("down-left", (const int[]){5, 5}, (const int[]){2, 1}, 7);
    check("mixed-signs", (const int[]){3, -1}, (const int[]){-2, 4}, 10);
    check("same-point", (const int[]){7, 7}, (const int[]){7, 7}, 0);
    check("negative", (const int[]){-3, -4}, (const int[]){-1, -1}, 5);
    check("large", (const int[]){1000000, -1000000}, (const int[]){-1000000, 1000000}, 4000000);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
