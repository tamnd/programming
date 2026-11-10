// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

#include "reverse_string.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, const char *s, const char *want) {
    size_t n = strlen(s);
    char out[64];
    if (reverse_code_points(s, n, out) != 0 || memcmp(out, want, n) != 0 || strlen(want) != n) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
}

static void check_invalid(const char *name, const char *s) {
    char out[64];
    errno = 0;
    if (reverse_code_points(s, strlen(s), out) != -1 || errno != EILSEQ) {
        fprintf(stderr, "case %s: no EILSEQ error\n", name);
        failures++;
    }
}

int main(void) {
    check("ascii", "hello", "olleh");
    check("two-words", "abc def", "fed cba");
    check("palindrome", "racecar", "racecar");
    check("empty", "", "");
    check("single", "a", "a");
    check("two-byte", "naïve", "evïan");
    check("three-byte", "日本語", "語本日");
    check("astral", "a🙂b", "b🙂a");
    check("two-astral", "🙂😀", "😀🙂");
    check("combining-mark", "e\u0301x", "x\u0301e");
    check("flag", "🇫🇷", "🇷🇫");

    check_invalid("invalid-byte", "\xff\xfe");
    check_invalid("truncated", "caf\xc3");
    check_invalid("encoded-surrogate", "a\xed\xa0\x80" "b");
    check_invalid("overlong", "\xc0\xaf");

    if (failures != 0) return 1;
    puts("all tests passed");
    return 0;
}
