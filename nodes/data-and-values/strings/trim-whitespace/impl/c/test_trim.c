// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/trim-whitespace/.

#include "trim.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

// check frees got, which is a new string from trim or trim_start.
static void check(const char *name, char *got, const char *want) {
    if (got == NULL || strcmp(got, want) != 0) {
        fprintf(stderr, "case %s: got %s, want %s\n", name, got ? got : "NULL", want);
        failures++;
    }
    free(got);
}

// The white space is the 6 ASCII characters: space, tab, line feed, vertical tab, form feed and carriage return. U+00A0, U+3000, U+0085 and U+FEFF stay.
int main(void) {
    check("both-ends", trim("  hello  "), "hello");
    check("tab-newline", trim("\thello world\n"), "hello world");
    check("none", trim("hello"), "hello");
    check("empty", trim(""), "");
    check("all-space", trim(" \t\n\r "), "");
    check("vt-ff", trim("\v\fx\f\v"), "x");
    check("other-control", trim("\x01x\x01"), "\x01x\x01");
    check("no-break-space", trim("\xc2\xa0x\xc2\xa0"), "\xc2\xa0x\xc2\xa0");
    check("ideographic-space", trim("\xe3\x80\x80x\xe3\x80\x80"), "\xe3\x80\x80x\xe3\x80\x80");
    check("next-line", trim("\xc2\x85x\xc2\x85"), "\xc2\x85x\xc2\x85");
    check("bom", trim("\xef\xbb\xbfx"), "\xef\xbb\xbfx");
    check("astral", trim(" \xf0\x9f\x99\x82 "), "\xf0\x9f\x99\x82");
    check("combining", trim(" \xcc\x81x "), "\xcc\x81x");
    check("start-both-ends", trim_start("  hello  "), "hello  ");
    check("start-tab-newline", trim_start("\n\tx"), "x");
    check("start-empty", trim_start(""), "");
    check("start-all-space", trim_start("   "), "");
    check("start-no-break-space", trim_start("\xc2\xa0 x"), "\xc2\xa0 x");
    check("start-end-only", trim_start("x \t"), "x \t");
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
