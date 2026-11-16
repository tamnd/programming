// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

#include "sums.h"

#include <stdio.h>

static int failures = 0;

static void check(const char *name, const long long *const chunks[], const size_t lengths[], size_t n,
                  const long long want[]) {
    long long got[32] = {0};
    if (!parallel_sums(chunks, lengths, n, got)) {
        fprintf(stderr, "case %s failed: a thread did not start\n", name);
        failures++;
        return;
    }
    for (size_t i = 0; i < n; i++) {
        if (got[i] != want[i]) {
            fprintf(stderr, "case %s failed at %zu: got %lld, want %lld\n", name, i, got[i], want[i]);
            failures++;
            return;
        }
    }
}

int main(void) {
    const long long g = 1000000000;
    check("two", (const long long *[]){(long long[]){1, 2, 3}, (long long[]){4, 5}}, (size_t[]){3, 2}, 2,
          (long long[]){6, 9});
    check("three", (const long long *[]){(long long[]){10}, (long long[]){20, 30}, (long long[]){-5, 5}},
          (size_t[]){1, 2, 2}, 3, (long long[]){10, 50, 0});
    check("empty", NULL, NULL, 0, NULL);
    check("empty-chunk", (const long long *[]){NULL, (long long[]){7}}, (size_t[]){0, 1}, 2, (long long[]){0, 7});
    check("single", (const long long *[]){(long long[]){42}}, (size_t[]){1}, 1, (long long[]){42});
    check("negative", (const long long *[]){(long long[]){-1, -2}, (long long[]){-3}}, (size_t[]){2, 1}, 2,
          (long long[]){-3, -3});
    check("limit", (const long long *[]){(long long[]){g, g, g}, (long long[]){-g, -g, -g}}, (size_t[]){3, 3}, 2,
          (long long[]){3 * g, -3 * g});

    long long items[32][3], want[32];
    const long long *many[32];
    size_t lengths[32];
    for (int i = 0; i < 32; i++) {
        items[i][0] = items[i][1] = items[i][2] = i;
        many[i] = items[i];
        lengths[i] = 3;
        want[i] = 3LL * i;
    }
    check("many", many, lengths, 32, want);

    if (failures == 0) {
        printf("all tests passed\n");
    }
    return failures == 0 ? 0 : 1;
}
