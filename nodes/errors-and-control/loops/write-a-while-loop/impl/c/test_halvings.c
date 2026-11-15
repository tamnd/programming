// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-while-loop/.

#include "halvings.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, int n, const int *want, size_t count) {
    int out[32];
    size_t got = halvings(n, out);
    if (got != count || (count > 0 && memcmp(out, want, count * sizeof *out) != 0)) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
}

#define CHECK(name, n, ...)                                \
    do {                                                   \
        const int w[] = {__VA_ARGS__};                     \
        check(name, n, w, sizeof w / sizeof w[0]);         \
    } while (0)

int main(void) {
    CHECK("rosetta", 1024, 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1);
    CHECK("ten", 10, 10, 5, 2, 1);
    check("zero", 0, NULL, 0);
    CHECK("one", 1, 1);
    CHECK("odd", 7, 7, 3, 1);
    CHECK("limit", 2147483647, 2147483647, 1073741823, 536870911, 268435455, 134217727, 67108863, 33554431, 16777215, 8388607, 4194303, 2097151, 1048575, 524287, 262143, 131071, 65535, 32767, 16383, 8191, 4095, 2047, 1023, 511, 255, 127, 63, 31, 15, 7, 3, 1);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
