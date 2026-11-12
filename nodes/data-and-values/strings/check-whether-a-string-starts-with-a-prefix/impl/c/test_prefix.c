// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

#include "prefix.h"

#include <stdbool.h>
#include <stdio.h>

static int failures = 0;

static void check(const char *name, bool got, bool want) {
    if (got != want) {
        fprintf(stderr, "case %s: got %d, want %d\n", name, got, want);
        failures++;
    }
}

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
int main(void) {
    check("yes", starts_with("Hello, world", "Hello"), true);
    check("no", starts_with("Hello, world", "world"), false);
    check("whole", starts_with("abc", "abc"), true);
    check("empty-prefix", starts_with("abc", ""), true);
    check("empty-s", starts_with("", "a"), false);
    check("both-empty", starts_with("", ""), true);
    check("longer-prefix", starts_with("ab", "abc"), false);
    check("case-sensitive", starts_with("Hello", "hello"), false);
    check("non-ascii", starts_with("caf\xc3\xa9 au lait", "caf\xc3\xa9"), true);
    check("cjk", starts_with("日本語", "日本"), true);
    check("astral", starts_with("\xf0\x9f\x99\x82\xf0\x9f\x99\x83", "\xf0\x9f\x99\x82"), true);
    check("astral-no", starts_with("\xf0\x9f\x99\x82", "\xf0\x9f\x99\x83"), false);
    check("combining", starts_with("cafe\xcc\x81", "cafe"), true);
    check("precomposed", starts_with("caf\xc3\xa9", "cafe"), false);
    check("ends-yes", ends_with("report.txt", ".txt"), true);
    check("ends-no", ends_with("report.txt", ".csv"), false);
    check("ends-empty", ends_with("abc", ""), true);
    check("ends-longer", ends_with("txt", "a.txt"), false);
    check("ends-case", ends_with("A.TXT", ".txt"), false);
    check("ends-astral", ends_with("a\xf0\x9f\x99\x82", "\xf0\x9f\x99\x82"), true);
    check("ends-combining", ends_with("cafe\xcc\x81", "e"), false);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
