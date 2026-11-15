// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-counted-loop/.

#include "count_to.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, int n, const int *want) {
    int *out = count_to(n);
    if (out == NULL || (n > 0 && memcmp(out, want, (size_t)n * sizeof *out) != 0)) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
    free(out);
}

int main(void) {
    const int ten[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    check("ten", 10, ten);
    check("three", 3, ten);
    check("zero", 0, ten);
    check("one", 1, ten);
    check("two", 2, ten);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
