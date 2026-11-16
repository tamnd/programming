// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-integer-arithmetic/.

#include "arith.h"

#include <stdio.h>

static int failures = 0;

static void check(const char *label, int64_t a, int64_t b, int n, const int64_t want[6]) {
    int64_t got[6];
    arith(a, b, n, got);
    for (int i = 0; i < 6; i++) {
        if (got[i] != want[i]) {
            fprintf(stderr, "case %s failed at %d\n", label, i);
            failures++;
            return;
        }
    }
}

#define CHECK(label, a, b, n, ...) check(label, a, b, n, (const int64_t[6]){__VA_ARGS__})

int main(void) {
    CHECK("small", 7, 2, 3, 9, 5, 14, 3, 1, 343);
    CHECK("exact-division", 20, 5, 2, 25, 15, 100, 4, 0, 400);
    CHECK("negative-dividend", -7, 2, 3, -5, -9, -14, -3, -1, -343);
    CHECK("negative-divisor", 7, -2, 2, 5, 9, -14, -3, 1, 49);
    CHECK("both-negative", -7, -2, 1, -9, -5, 14, 3, -1, -7);
    CHECK("zero-dividend", 0, 5, 0, 5, -5, 0, 0, 0, 1);
    CHECK("zero-power", 5, 3, 0, 8, 2, 15, 1, 2, 1);
    CHECK("divisor-larger", 3, 7, 1, 10, -4, 21, 0, 3, 3);
    CHECK("largest", 100000, 100000, 3, 200000, 0, 10000000000, 1, 0, 1000000000000000);
    CHECK("smallest", -100000, 1, 3, -99999, -100001, -100000, -100000, 0, -1000000000000000);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
