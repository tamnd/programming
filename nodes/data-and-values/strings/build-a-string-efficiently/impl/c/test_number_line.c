// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/build-a-string-efficiently/.

#include "number_line.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, int n, const char *want) {
    char *got = number_line(n);
    if (got == NULL || strcmp(got, want) != 0) {
        fprintf(stderr, "case %s: got [%s], want [%s]\n", name, got ? got : "NULL", want);
        failures++;
    }
    free(got);
}

// check_long checks the length, the start and the end of a long result.
static void check_long(const char *name, int n, size_t size, const char *end) {
    char *got = number_line(n);
    size_t len = got ? strlen(got) : 0;
    size_t e = strlen(end);
    if (got == NULL || len != size || strncmp(got, "1,2,3,", 6) != 0 || strcmp(got + len - e, end) != 0) {
        fprintf(stderr, "case %s: got a length of %zu, want %zu\n", name, len, size);
        failures++;
    }
    free(got);
}

int main(void) {
    check("zero", 0, "");
    check("one", 1, "1");
    check("three", 3, "1,2,3");
    check("five", 5, "1,2,3,4,5");
    check("seven", 7, "1,2,3,4,5,6,7");
    check("nine", 9, "1,2,3,4,5,6,7,8,9");
    check("ten", 10, "1,2,3,4,5,6,7,8,9,10");
    check_long("hundred", 100, 291, ",99,100");
    check_long("hundred-and-one", 101, 295, ",100,101");
    check_long("thousand", 1000, 3892, ",999,1000");
    check_long("large", 100000, 588894, ",99999,100000");
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
