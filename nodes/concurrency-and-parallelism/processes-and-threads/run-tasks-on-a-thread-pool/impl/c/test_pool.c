// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

#include "pool.h"

#include <stdio.h>

static int failures = 0;

static int32_t reference_steps(int64_t n) {
    int32_t steps = 0;
    while (n != 1) {
        n = n % 2 == 0 ? n / 2 : 3 * n + 1;
        steps++;
    }
    return steps;
}

static void check(const char *name, size_t workers, const int32_t numbers[], size_t n, const int32_t want[]) {
    int32_t got[200] = {0};
    if (!steps_on_pool(workers, numbers, n, got)) {
        fprintf(stderr, "case %s failed: the pool did not start\n", name);
        failures++;
        return;
    }
    for (size_t i = 0; i < n; i++) {
        if (got[i] != want[i]) {
            fprintf(stderr, "case %s failed at %zu: got %d, want %d\n", name, i, got[i], want[i]);
            failures++;
            return;
        }
    }
}

int main(void) {
    check("four-jobs", 2, (int32_t[]){1, 2, 3, 6}, 4, (int32_t[]){0, 1, 7, 8});
    check("long-runs", 3, (int32_t[]){27, 97, 871}, 3, (int32_t[]){111, 118, 178});
    check("empty", 2, NULL, 0, NULL);
    check("one-worker", 1, (int32_t[]){7, 8, 9}, 3, (int32_t[]){16, 3, 19});
    check("more-workers-than-jobs", 8, (int32_t[]){5, 6}, 2, (int32_t[]){5, 8});
    check("duplicates", 3, (int32_t[]){27, 27, 27, 1}, 4, (int32_t[]){111, 111, 111, 0});
    check("wide-intermediate", 2, (int32_t[]){113383, 837799}, 2, (int32_t[]){247, 524});
    int32_t many[200], want[200];
    for (int i = 0; i < 200; i++) {
        many[i] = i + 1;
        want[i] = reference_steps(i + 1);
    }
    check("many", 4, many, 200, want);
    if (steps_on_pool(0, many, 200, want) || steps_on_pool(POOL_MAX_WORKERS + 1, many, 200, want)) {
        fprintf(stderr, "case bad-workers failed: want false\n");
        failures++;
    }
    if (failures == 0) {
        printf("all tests passed\n");
    }
    return failures == 0 ? 0 : 1;
}
