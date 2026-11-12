// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/replace-a-substring/.

#include "replace.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, char *got, const char *want) {
    if (got == NULL || strcmp(got, want) != 0) {
        fprintf(stderr, "case %s: got [%s], want [%s]\n", name, got ? got : "NULL", want);
        failures++;
    }
    free(got);
}

// U+0301 is COMBINING ACUTE ACCENT, and U+00E9 is the precomposed é.
int main(void) {
    check("all", replace("a-b-c", "-", "+"), "a+b+c");
    check("word", replace("Hello, world", "world", "there"), "Hello, there");
    check("none", replace("abc", "x", "y"), "abc");
    check("empty-s", replace("", "a", "b"), "");
    check("delete", replace("a-b-c", "-", ""), "abc");
    check("new-contains-old", replace("aXa", "X", "XXX"), "aXXXa");
    check("overlap", replace("aaaa", "aa", "b"), "bb");
    check("overlap-odd", replace("aaa", "aa", "b"), "ba");
    check("case-sensitive", replace("Hello hello", "hello", "bye"), "Hello bye");
    check("non-ascii", replace("naïve caf\xc3\xa9", "\xc3\xa9", "e"), "naïve cafe");
    check("astral", replace("a\xf0\x9f\x99\x82" "b\xf0\x9f\x99\x82", "\xf0\x9f\x99\x82", "!"), "a!b!");
    check("combining", replace("cafe\xcc\x81", "e", "E"), "cafE\xcc\x81");
    check("precomposed", replace("caf\xc3\xa9", "e", "E"), "caf\xc3\xa9");
    check("dot", replace("a.b.c", ".", "-"), "a-b-c");
    check("dollar", replace("price", "price", "$&!"), "$&!");
    check("ampersand", replace("a-b", "-", "&"), "a&b");
    check("backslash", replace("a/b", "/", "\\"), "a\\b");
    check("first-only", replace_first("a-b-c", "-", "+"), "a+b-c");
    check("first-none", replace_first("abc", "x", "y"), "abc");
    check("first-overlap", replace_first("aaa", "aa", "b"), "ba");
    check("first-astral", replace_first("\xf0\x9f\x99\x82\xf0\x9f\x99\x82", "\xf0\x9f\x99\x82", "x"), "x\xf0\x9f\x99\x82");
    check("first-dollar", replace_first("a.b", ".", "$&"), "a$&b");
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
