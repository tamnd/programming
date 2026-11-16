// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

#include "levenshtein.h"

#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    const char *a;
    const char *b;
    int want;
};

static const struct test_case cases[] = {
    {"kitten-sitting", "kitten", "sitting", 3},
    {"rosettacode", "rosettacode", "raisethysword", 8},
    {"flaw-lawn", "flaw", "lawn", 2},
    {"both-empty", "", "", 0},
    {"empty-left", "", "abc", 3},
    {"empty-right", "abc", "", 3},
    {"equal", "same", "same", 0},
    {"one-substitution", "a", "b", 1},
    {"transposition", "ab", "ba", 2},
    {"case", "Apple", "apple", 1},
    {"disjoint", "abc", "xyz", 3},
    {"prefix", "abc", "abcdef", 3},
    {"intention", "intention", "execution", 5},
    {"space", "a b", "ab", 1},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        int got = levenshtein(c->a, c->b);
        int back = levenshtein(c->b, c->a);
        if (got != c->want || back != c->want) {
            printf("FAIL %s: got %d and %d\n", c->name, got, back);
            failures++;
        }
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
