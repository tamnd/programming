// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pattern-match-on-a-value/.

#include "classify.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, int x, int y, const char *want) {
    if (strcmp(classify(x, y), want) != 0) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
}

int main(void) {
    check("x-axis", 5, 0, "x-axis");
    check("y-axis", 0, -3, "y-axis");
    check("diagonal", 4, 4, "diagonal");
    check("other", 2, 7, "other");
    check("origin", 0, 0, "origin");
    check("negative-diagonal", -2, -2, "diagonal");
    check("anti-diagonal", 3, -3, "other");
    check("negative-x-axis", -7, 0, "x-axis");
    check("one-one", 1, 1, "diagonal");
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
