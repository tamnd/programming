// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-singleton/.

#include "id_generator.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, int calls, long a, long b, long c) {
    const long want[3] = {a, b, c};
    long got[3];
    singleton_report(calls, got);
    if (memcmp(got, want, sizeof want) != 0) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
}

int main(void) {
    check("three-calls", 3, 2, 1, 1);
    check("ten-calls", 10, 9, 1, 1);
    check("single-call", 1, 0, 1, 1);
    check("pair", 2, 1, 1, 1);
    check("limit", 1000000, 999999, 1, 1);
    if (id_generator_instance() != id_generator_instance() || id_generator_constructions() != 1) {
        fprintf(stderr, "case one-instance failed\n");
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
