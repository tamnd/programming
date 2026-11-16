// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pad-a-string/.

#include "pad.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name;
    const char *s;
    size_t width;
    char fill;
    const char *left;
    const char *right;
};

static const struct test_case cases[] = {
    {"zeros", "7", 3, '0', "007", "700"},
    {"spaces", "abc", 6, ' ', "   abc", "abc   "},
    {"stars", "abc", 6, '*', "***abc", "abc***"},
    {"empty-string", "", 3, '-', "---", "---"},
    {"width-zero", "abc", 0, ' ', "abc", "abc"},
    {"exact-width", "abcd", 4, '.', "abcd", "abcd"},
    {"shorter-width", "hello", 3, ' ', "hello", "hello"},
    {"negative-number-text", "-42", 6, '0', "000-42", "-42000"},
    {"fill-same-as-text", "aa", 5, 'a', "aaaaa", "aaaaa"},
    {"one-char-wide", "x", 1, '_', "x", "x"},
    {"spaces-inside", "a b", 7, '.', "....a b", "a b...."},
};

int main(void) {
    int failures = 0;
    char left[64];
    char right[64];
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        pad(c->s, c->width, c->fill, left, right);
        if (strcmp(left, c->left) != 0 || strcmp(right, c->right) != 0) {
            printf("FAIL %s: got \"%s\" \"%s\"\n", c->name, left, right);
            failures++;
        }
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
