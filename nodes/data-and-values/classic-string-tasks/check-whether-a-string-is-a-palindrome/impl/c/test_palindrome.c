// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-a-palindrome/.

#include "palindrome.h"

#include <stdio.h>
#include <stdlib.h>

struct test_case {
    const char *name;
    const char *text;
    bool want;
};

static const struct test_case cases[] = {
    {"racecar", "racecar", true},
    {"panama", "A man, a plan, a canal: Panama", true},
    {"hello", "hello", false},
    {"empty", "", true},
    {"only-punctuation", "?! ...", true},
    {"one-letter", "a", true},
    {"two-letters", "ab", false},
    {"mixed-case", "Aa", true},
    {"nixon", "No 'x' in Nixon", true},
    {"cat-i-saw", "Was it a car or a cat I saw?", true},
    {"digits", "12321", true},
    {"digits-not", "123", false},
    {"zero-p", "0P", false},
    {"race-a-car", "race a car", false},
    {"even-near", "abca", false},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        if (is_palindrome(cases[i].text) != cases[i].want) {
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
