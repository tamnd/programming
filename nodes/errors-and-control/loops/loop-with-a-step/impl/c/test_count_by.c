// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

#include "count_by.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, int start, int stop, int step, const int *want, size_t n) {
    size_t count = 0;
    int *out = count_by(start, stop, step, &count);
    if (out == NULL || count != n || (n > 0 && memcmp(out, want, n * sizeof *out) != 0)) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
    free(out);
}

#define CHECK(name, start, stop, step, ...)                       \
    do {                                                          \
        const int w[] = {__VA_ARGS__};                            \
        check(name, start, stop, step, w, sizeof w / sizeof w[0]); \
    } while (0)

int main(void) {
    CHECK("by-two", 2, 8, 2, 2, 4, 6, 8);
    CHECK("down", 10, 0, -1, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
    CHECK("uneven", 1, 10, 3, 1, 4, 7, 10);
    CHECK("past-stop", 0, 9, 4, 0, 4, 8);
    CHECK("down-by-three", 10, 1, -3, 10, 7, 4, 1);
    check("empty-up", 5, 1, 1, NULL, 0);
    check("empty-down", 1, 5, -1, NULL, 0);
    CHECK("single", 3, 3, 7, 3);
    CHECK("negative", -3, 3, 2, -3, -1, 1, 3);
    CHECK("limit-up", 2147483640, INT_MAX, 5, 2147483640, 2147483645);
    CHECK("limit-down", -2147483640, INT_MIN, -5, -2147483640, -2147483645);
    CHECK("big-step", 0, 10, INT_MAX, 0);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
