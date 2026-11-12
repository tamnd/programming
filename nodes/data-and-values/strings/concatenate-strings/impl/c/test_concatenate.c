// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

#include "concatenate.h"

#include <stdint.h>
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

int main(void) {
    check("hello-world", concat("Hello, ", "world"), "Hello, world");
    check("two-words", concat("good", "bye"), "goodbye");
    check("keep-spaces", concat("a ", " b"), "a  b");
    check("empty-both", concat("", ""), "");
    check("empty-left", concat("", "abc"), "abc");
    check("empty-right", concat("abc", ""), "abc");
    check("non-ascii", concat("naï", "ve"), "naïve");
    check("astral", concat("a", "🙂"), "a🙂");
    // U+0301 COMBINING ACUTE ACCENT is CC 81 in UTF-8.
    check("combining-mark", concat("e", "\xcc\x81"), "e\xcc\x81");
    check("flag-halves", concat("🇫", "🇷"), "🇫🇷");
    check("digit-strings", concat("1", "2"), "12");

    check("positive", concat_number("item", 3), "item3");
    check("zero", concat_number("n=", 0), "n=0");
    check("negative", concat_number("x", -5), "x-5");
    check("empty-prefix", concat_number("", 42), "42");
    check("int32-max", concat_number("max=", INT32_MAX), "max=2147483647");
    check("int32-min", concat_number("min=", INT32_MIN), "min=-2147483648");
    if (failures != 0) {
        return 1;
    }
    puts("ok");
    return 0;
}
