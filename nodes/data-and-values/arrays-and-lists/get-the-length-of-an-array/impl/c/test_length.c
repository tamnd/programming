// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-length-of-an-array/.

#include "length.h"

#include <stddef.h>
#include <stdio.h>

static int failures = 0;

static void check(const char *name, const char *const *items, size_t want) {
    if (length(items) != want) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
}

// Each list ends with NULL. The compound literal keeps each case on one line.
#define CHECK(name, want, ...) check(name, (const char *const[]){__VA_ARGS__}, want)

int main(void) {
    CHECK("three", 3, "a", "b", "c", NULL);
    CHECK("two-words", 2, "apple", "banana", NULL);
    CHECK("empty", 0, NULL);
    CHECK("single", 1, "x", NULL);
    CHECK("empty-strings", 2, "", "", NULL);
    CHECK("non-ascii", 2, "é", "日本語", NULL);
    CHECK("duplicates", 3, "a", "a", "a", NULL);
    CHECK("ten", 10, "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", NULL);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
