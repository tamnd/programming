// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-the-characters-of-a-string/.

#include "characters.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

// check compares the n strings of got with want, and then frees got.
static void check(const char *name, const char *s, const char *const *want, size_t n) {
    size_t k = 0;
    char **got = characters(s, &k);
    if (got == NULL || k != n) {
        fprintf(stderr, "case %s: got %zu characters, want %zu\n", name, k, n);
        failures++;
    } else {
        for (size_t i = 0; i < n; i++) {
            if (strcmp(got[i], want[i]) != 0) {
                fprintf(stderr, "case %s: character %zu is wrong\n", name, i);
                failures++;
                break;
            }
        }
    }
    free_characters(got, k);
}

// A character is a code point. U+1F642 is one character, but two UTF-16 code units and four UTF-8 bytes. A combining mark, a ZWJ and each regional indicator is a character of its own.
int main(void) {
    check("ascii", "abc", (const char *const[]){"a", "b", "c"}, 3);
    check("word", "hello", (const char *const[]){"h", "e", "l", "l", "o"}, 5);
    check("space", "a b", (const char *const[]){"a", " ", "b"}, 3);
    check("empty", "", (const char *const[]){NULL}, 0);
    check("single", "x", (const char *const[]){"x"}, 1);
    check("non-ascii", "h\xc3\xa9", (const char *const[]){"h", "\xc3\xa9"}, 2);
    check("cjk", "\xe6\x97\xa5\xe6\x9c\xac", (const char *const[]){"\xe6\x97\xa5", "\xe6\x9c\xac"}, 2);
    check("astral", "a\xf0\x9f\x99\x82" "b", (const char *const[]){"a", "\xf0\x9f\x99\x82", "b"}, 3);
    check("combining", "e\xcc\x81", (const char *const[]){"e", "\xcc\x81"}, 2);
    check("regional-indicator", "\xf0\x9f\x87\xba\xf0\x9f\x87\xb8", (const char *const[]){"\xf0\x9f\x87\xba", "\xf0\x9f\x87\xb8"}, 2);
    check("zwj", "\xf0\x9f\x91\xa9\xe2\x80\x8d\xf0\x9f\x92\xbb", (const char *const[]){"\xf0\x9f\x91\xa9", "\xe2\x80\x8d", "\xf0\x9f\x92\xbb"}, 3);
    check("control", "a\tb\nc", (const char *const[]){"a", "\t", "b", "\n", "c"}, 5);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
