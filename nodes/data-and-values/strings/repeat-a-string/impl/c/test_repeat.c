// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

#include "repeat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

// check frees got, which is a new string from repeat.
static void check(const char *name, char *got, const char *want) {
    if (got == NULL || strcmp(got, want) != 0) {
        fprintf(stderr, "case %s: got %s, want %s\n", name, got ? got : "NULL", want);
        failures++;
    }
    free(got);
}

// The case many has the 200 chars of 100 copies of ab in one literal.
int main(void) {
    check("word", repeat("ab", 3), "ababab");
    check("one", repeat("abc", 1), "abc");
    check("dash", repeat("-", 5), "-----");
    check("zero", repeat("abc", 0), "");
    check("empty", repeat("", 5), "");
    check("empty-zero", repeat("", 0), "");
    check("many", repeat("ab", 100), "abababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababababab");
    check("astral", repeat("\xf0\x9f\x99\x82", 3), "\xf0\x9f\x99\x82\xf0\x9f\x99\x82\xf0\x9f\x99\x82");
    check("combining", repeat("e\xcc\x81", 2), "e\xcc\x81" "e\xcc\x81");
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
