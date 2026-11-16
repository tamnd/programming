// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

#include "letters.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name;
    const char *text;
    long long want[26];
};

static const struct test_case cases[] = {
    {"hello", "hello", {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {"pangram", "The quick brown fox jumps over the lazy dog", {1, 1, 1, 1, 3, 1, 1, 2, 1, 1, 1, 1, 1, 1, 4, 1, 1, 2, 1, 2, 2, 1, 1, 1, 1, 1}},
    {"mississippi", "Mississippi", {0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 1, 0, 0, 2, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0}},
    {"empty", "", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {"no-letters", "123 !?", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {"mixed-case", "AaAa", {4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {"first-and-last", "aZ", {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}},
    {"only-z", "zzz", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3}},
    {"white-space", " \t\n", {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}},
    {"sentence", "Count Each Letter, once.", {1, 0, 3, 0, 4, 0, 0, 1, 0, 0, 0, 1, 0, 2, 2, 0, 0, 1, 0, 3, 1, 0, 0, 0, 0, 0}},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        long long got[26];
        count_letters(cases[i].text, got);
        if (memcmp(got, cases[i].want, sizeof got) != 0) {
            printf("FAIL %s\n", cases[i].name);
            failures++;
        }
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
