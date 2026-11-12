// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-a-substring/.

#include "substring.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, const char *s, size_t start, size_t end, const char *want) {
    char *got = substring(s, start, end);
    if (got == NULL || strcmp(got, want) != 0) {
        fprintf(stderr, "case %s: got [%s], want [%s]\n", name, got ? got : "NULL", want);
        failures++;
    }
    free(got);
}

// The strings are UTF-8 bytes: C3 AF is U+00EF, C3 A9 is U+00E9, CC 81 is
// U+0301 COMBINING ACUTE ACCENT, and F0 9F 99 82 and F0 9F 99 83 are U+1F642
// and U+1F643.
int main(void) {
    check("middle", "Hello, world", 7, 12, "world");
    check("prefix", "Hello, world", 0, 5, "Hello");
    check("one-char", "abc", 1, 2, "b");
    check("empty", "", 0, 0, "");
    check("whole", "abc", 0, 3, "abc");
    check("empty-range", "abc", 1, 1, "");
    check("at-end", "abc", 3, 3, "");
    check("non-ascii", "na\xc3\xafve caf\xc3\xa9", 2, 5, "\xc3\xafve");
    check("cjk", "日本語テキスト", 3, 7, "テキスト");
    check("astral", "a\xf0\x9f\x99\x82" "b\xf0\x9f\x99\x83" "c", 1, 4, "\xf0\x9f\x99\x82" "b\xf0\x9f\x99\x83");
    check("astral-only", "\xf0\x9f\x99\x82\xf0\x9f\x99\x83", 1, 2, "\xf0\x9f\x99\x83");
    check("combining", "e\xcc\x81t\xc3\xa9", 0, 1, "e");
    check("combining-mark", "e\xcc\x81t\xc3\xa9", 1, 3, "\xcc\x81t");
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
