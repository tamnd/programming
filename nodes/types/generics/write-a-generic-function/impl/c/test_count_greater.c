// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "count_greater.h"

struct test_case {
    const char *name;
    int items[5];
    size_t count;
    int limit;
    size_t want;
};

static const struct test_case cases[] = {
    {"some", {34, 50, 25, 100, 65}, 5, 50, 2},
    {"all", {3, 4, 5}, 3, 2, 3},
    {"none", {3, 4, 5}, 3, 9, 0},
    {"empty", {0}, 0, 0, 0},
    {"equal-not-counted", {5, 5, 5}, 3, 5, 0},
    {"duplicates", {7, 1, 7, 7}, 4, 6, 3},
    {"negative", {-3, -1, 0, 2}, 4, -2, 3},
    {"min-limit", {INT_MIN, 0, INT_MAX}, 3, INT_MIN, 2},
    {"max-limit", {INT_MAX, 1}, 2, INT_MAX, 0},
    {"single", {1}, 1, 0, 1},
};

static int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char *const *)a, *(const char *const *)b);
}

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        size_t got = count_greater_int(c->items, c->count, c->limit);
        if (got != c->want) {
            printf("FAIL %s: got %zu, want %zu\n", c->name, got, c->want);
            failures++;
        }
    }
    const char *words[] = {"pear", "apple", "plum"};
    const char *limit = "orange";
    size_t got = count_greater(words, 3, sizeof words[0], &limit, compare_strings);
    if (got != 2) {
        printf("FAIL strings: got %zu, want 2\n", got);
        failures++;
    }
    if (failures != 0) {
        return 1;
    }
    printf("ok\n");
    return 0;
}
