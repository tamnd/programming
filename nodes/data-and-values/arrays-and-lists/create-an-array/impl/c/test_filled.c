// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-an-array/.

#include "filled.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

static int failures = 0;

static void check(const char *name, size_t n, int value) {
    int *a = filled(n, value);
    int ok = a != NULL;
    for (size_t i = 0; ok && i < n; i++) {
        ok = a[i] == value;
    }
    if (!ok) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
    free(a);
}

int main(void) {
    check("three-sevens", 3, 7);
    check("five-zeros", 5, 0);
    check("empty", 0, 7);
    check("single", 1, 42);
    check("zero", 2, 0);
    check("negative", 2, -1);
    check("max-int", 2, INT_MAX);
    check("min-int", 2, INT_MIN);
    check("ten", 10, 1);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
