// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

#include "compare.h"

#include <stdbool.h>
#include <stdio.h>

static int failures = 0;

static void check(const char *name, int got, int want) {
    if (got != want) {
        fprintf(stderr, "case %s: got %d, want %d\n", name, got, want);
        failures++;
    }
}

// U+FF61 is HALFWIDTH IDEOGRAPHIC FULL STOP. It is before U+1F642 in code point order, but its UTF-16 code unit is after the high surrogate of U+1F642.
int main(void) {
    check("less", compare("apple", "banana"), -1);
    check("equal", compare("apple", "apple"), 0);
    check("greater", compare("pear", "apple"), 1);
    check("both-empty", compare("", ""), 0);
    check("empty-first", compare("", "a"), -1);
    check("prefix", compare("app", "apple"), -1);
    check("prefix-longer", compare("apple", "app"), 1);
    check("upper-before-lower", compare("Zebra", "apple"), -1);
    check("lower-after-upper", compare("a", "A"), 1);
    check("digits", compare("10", "9"), -1);
    check("non-ascii", compare("\xc3\xa9", "z"), 1);
    check("astral-after-bmp", compare("\xef\xbd\xa1", "\xf0\x9f\x99\x82"), -1);
    check("astral", compare("\xf0\x9f\x99\x82", "\xf0\x9f\x99\x83"), -1);
    check("combining", compare("cafe\xcc\x81", "caf\xc3\xa9"), -1);
    check("eq-same", equals("abc", "abc"), true);
    check("eq-case", equals("abc", "ABC"), false);
    check("eq-empty", equals("", ""), true);
    check("eq-combining", equals("cafe\xcc\x81", "caf\xc3\xa9"), false);
    check("eq-astral", equals("\xf0\x9f\x99\x82", "\xf0\x9f\x99\x82"), true);
    check("eq-trailing-space", equals("abc", "abc "), false);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
