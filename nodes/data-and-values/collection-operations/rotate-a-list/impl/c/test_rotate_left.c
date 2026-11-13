// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

#include "rotate_left.h"

#include <stdbool.h>
#include <stdio.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// Each list has a 0 in it when it is empty, because C has no empty array.
// The count n gives the real length.
static void check(const char *name, const int *got, const int *want, size_t n) {
    bool ok = true;
    for (size_t i = 0; ok && i < n; i++) {
        ok = got[i] == want[i];
    }
    if (!ok) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
}

#define CHECK(name, n, given, k, want)   \
    do {                                 \
        const int g[] = given;           \
        const int w[] = want;            \
        int out[sizeof g / sizeof g[0]]; \
        rotate_left(g, n, k, out);       \
        check(name, out, w, n);          \
    } while (0)

int main(void) {
    CHECK("by-two", 5, L(1, 2, 3, 4, 5), 2, L(3, 4, 5, 1, 2));
    CHECK("by-one", 3, L(1, 2, 3), 1, L(2, 3, 1));
    CHECK("zero", 3, L(1, 2, 3), 0, L(1, 2, 3));
    CHECK("full-turn", 3, L(1, 2, 3), 3, L(1, 2, 3));
    CHECK("more-than-length", 3, L(1, 2, 3), 4, L(2, 3, 1));
    CHECK("much-larger", 3, L(1, 2, 3), 2000000000, L(3, 1, 2));
    CHECK("empty", 0, L(0), 5, L(0));
    CHECK("empty-zero", 0, L(0), 0, L(0));
    CHECK("single", 1, L(7), 3, L(7));
    CHECK("duplicates", 3, L(1, 1, 2), 1, L(1, 2, 1));
    CHECK("negative-items", 3, L(-1, -2, -3), 1, L(-2, -3, -1));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
