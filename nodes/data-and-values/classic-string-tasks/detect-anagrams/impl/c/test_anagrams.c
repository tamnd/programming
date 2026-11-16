// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/detect-anagrams/.

#include "anagrams.h"

#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    const char *a;
    const char *b;
    bool want;
};

static const struct test_case cases[] = {
    {"listen-silent", "listen", "silent", true},
    {"dormitory", "Dormitory", "dirty room", true},
    {"hello-world", "hello", "world", false},
    {"both-empty", "", "", true},
    {"one-empty", "a", "", false},
    {"same-set-other-counts", "aab", "abb", false},
    {"extra-letter", "abc", "abcd", false},
    {"same-length-other-letters", "abc", "abd", false},
    {"case", "Listen", "SILENT", true},
    {"same-word", "stop", "stop", true},
    {"gentleman", "a gentleman", "elegant man", true},
    {"they-see", "The eyes!", "They see.", true},
    {"punctuation-only", "!?", "...", true},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        if (are_anagrams(c->a, c->b) != c->want || are_anagrams(c->b, c->a) != c->want) {
            printf("FAIL %s\n", c->name);
            failures++;
        }
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
