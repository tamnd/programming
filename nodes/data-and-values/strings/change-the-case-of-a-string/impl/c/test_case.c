// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/change-the-case-of-a-string/.

#include "case.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

// check frees got, which is a new string from to_upper or to_lower.
static void check(const char *name, char *got, const char *want) {
    if (got == NULL || strcmp(got, want) != 0) {
        fprintf(stderr, "case %s: got %s, want %s\n", name, got ? got : "NULL", want);
        failures++;
    }
    free(got);
}

// Only the ASCII letters change. U+0131 is the dotless i, U+0130 is the capital I with a dot, U+212A is the KELVIN SIGN, and U+FF41 is the fullwidth a.
int main(void) {
    check("word", to_upper("hello"), "HELLO");
    check("mixed", to_upper("Hello, World!"), "HELLO, WORLD!");
    check("already", to_upper("ABC"), "ABC");
    check("empty", to_upper(""), "");
    check("digits-punct", to_upper("a1-b2_c3"), "A1-B2_C3");
    check("non-ascii", to_upper("caf\xc3\xa9"), "CAF\xc3\xa9");
    check("sharp-s", to_upper("stra\xc3\x9f" "e"), "STRA\xc3\x9f" "E");
    check("dotless-i", to_upper("\xc4\xb1 i"), "\xc4\xb1 I");
    check("greek", to_upper("\xcf\x83"), "\xcf\x83");
    check("fullwidth", to_upper("\xef\xbd\x81"), "\xef\xbd\x81");
    check("astral", to_upper("a\xf0\x9f\x99\x82" "b"), "A\xf0\x9f\x99\x82" "B");
    check("combining", to_upper("cafe\xcc\x81"), "CAFE\xcc\x81");
    check("lower-word", to_lower("HELLO"), "hello");
    check("lower-mixed", to_lower("Hello, World!"), "hello, world!");
    check("lower-empty", to_lower(""), "");
    check("lower-dotted-i", to_lower("\xc4\xb0"), "\xc4\xb0");
    check("lower-sigma", to_lower("\xce\xa3\xce\x91\xce\xa3"), "\xce\xa3\xce\x91\xce\xa3");
    check("lower-kelvin", to_lower("\xe2\x84\xaa"), "\xe2\x84\xaa");
    check("lower-astral", to_lower("A\xf0\x9f\x99\x82" "B"), "a\xf0\x9f\x99\x82" "b");
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
