// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/remove-characters-from-a-string/.

#include "remove.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

// check frees got, which is a new string from remove_chars.
static void check(const char *name, char *got, const char *want) {
    if (got == NULL || strcmp(got, want) != 0) {
        fprintf(stderr, "case %s: got %s, want %s\n", name, got ? got : "NULL", want);
        failures++;
    }
    free(got);
}

// A character is a code point. U+1F642 and U+1F643 have the same high surrogate in UTF-16, and U+00E0 and U+00E9 have the same first byte in UTF-8.
int main(void) {
    check("one-char", remove_chars("banana", "a"), "bnn");
    check("set", remove_chars("hello, world", "lo"), "he, wrd");
    check("punct", remove_chars("a-b_c.d", "-_."), "abcd");
    check("empty", remove_chars("", "abc"), "");
    check("empty-set", remove_chars("hello", ""), "hello");
    check("none-found", remove_chars("hello", "xyz"), "hello");
    check("all", remove_chars("aaa", "a"), "");
    check("adjacent", remove_chars("aab", "a"), "b");
    check("astral", remove_chars("a\xf0\x9f\x99\x82" "b\xf0\x9f\x99\x82", "\xf0\x9f\x99\x82"), "ab");
    check("astral-other", remove_chars("\xf0\x9f\x99\x82\xf0\x9f\x99\x83", "\xf0\x9f\x99\x83"), "\xf0\x9f\x99\x82");
    check("non-ascii", remove_chars("\xc3\xa0\xc3\xa9", "\xc3\xa9"), "\xc3\xa0");
    check("combining-mark", remove_chars("e\xcc\x81x", "\xcc\x81"), "ex");
    check("combining-base", remove_chars("e\xcc\x81", "e"), "\xcc\x81");
    check("composed", remove_chars("e\xcc\x81", "\xc3\xa9"), "e\xcc\x81");
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
