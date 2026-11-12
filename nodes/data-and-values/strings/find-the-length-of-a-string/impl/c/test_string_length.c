// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

#include "string_length.h"

#include <stdio.h>

static int failures = 0;

static void check(const char *name, const char *s, size_t n, size_t units, size_t code_points) {
    size_t out[3];
    lengths(s, n, out);
    if (out[0] != n || out[1] != units || out[2] != code_points) {
        fprintf(stderr, "case %s: got %zu %zu %zu\n", name, out[0], out[1], out[2]);
        failures++;
    }
}

// The array of a string literal also holds the terminating null character, so
// the number of bytes of the text is sizeof minus 1. strlen stops at the first
// null character, so it is wrong for the nul case.
#define CHECK(name, lit, units, code_points) check(name, lit, sizeof(lit) - 1, units, code_points)

int main(void) {
    CHECK("ascii", "hello", 5, 5);
    CHECK("words", "Hello, world", 12, 12);
    CHECK("two-byte", "naïve", 5, 5);
    CHECK("three-byte", "日本語", 3, 3);
    CHECK("cyrillic", "Привет", 6, 6);
    CHECK("empty", "", 0, 0);
    CHECK("single", "a", 1, 1);
    CHECK("euro-sign", "€", 1, 1);
    CHECK("astral", "🙂", 2, 1);
    CHECK("mixed", "a🙂b", 4, 3);
    // U+0301 COMBINING ACUTE ACCENT is CC 81 in UTF-8.
    CHECK("combining-mark", "e\xcc\x81", 2, 2);
    CHECK("precomposed", "\xc3\xa9", 1, 1);
    CHECK("flag", "🇫🇷", 4, 2);
    // U+200D ZERO WIDTH JOINER is E2 80 8D in UTF-8.
    CHECK("zwj-family", "👨\xe2\x80\x8d👩\xe2\x80\x8d👧", 8, 5);
    CHECK("nul", "a\0b", 3, 3);
    if (failures != 0) {
        return 1;
    }
    puts("ok");
    return 0;
}
