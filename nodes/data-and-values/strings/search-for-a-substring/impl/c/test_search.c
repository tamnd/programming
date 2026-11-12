// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-for-a-substring/.

#include "search.h"

#include <stdio.h>

static int failures = 0;

static void check(const char *name, long got, long want) {
    if (got != want) {
        fprintf(stderr, "case %s: got %ld, want %ld\n", name, got, want);
        failures++;
    }
}

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
int main(void) {
    check("found", find("Hello, world", "world"), 7);
    check("first-of-two", find("abcabc", "bc"), 1);
    check("not-found", find("abc", "d"), -1);
    check("empty-sub", find("abc", ""), 0);
    check("empty-s", find("", "a"), -1);
    check("both-empty", find("", ""), 0);
    check("longer-sub", find("ab", "abc"), -1);
    check("overlap", find("aaaa", "aa"), 0);
    check("case-sensitive", find("Hello", "hello"), -1);
    check("non-ascii", find("naïve caf\xc3\xa9", "caf\xc3\xa9"), 6);
    check("cjk", find("日本語テキスト", "テキ"), 3);
    check("astral", find("a\xf0\x9f\x99\x82" "b\xf0\x9f\x99\x82" "c", "b"), 2);
    check("astral-sub", find("x\xf0\x9f\x99\x82y", "\xf0\x9f\x99\x82y"), 1);
    check("combining", find("cafe\xcc\x81", "e"), 3);
    check("precomposed", find("cafe\xcc\x81", "\xc3\xa9"), -1);
    check("last-of-two", find_last("abcabc", "bc"), 4);
    check("last-not-found", find_last("abc", "d"), -1);
    check("last-empty-sub", find_last("abc", ""), 3);
    check("last-empty-s", find_last("", ""), 0);
    check("last-overlap", find_last("aaaa", "aa"), 2);
    check("last-astral", find_last("\xf0\x9f\x99\x82" "a\xf0\x9f\x99\x82" "a", "a"), 3);
    check("last-non-ascii", find_last("caf\xc3\xa9 caf\xc3\xa9", "\xc3\xa9"), 8);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
