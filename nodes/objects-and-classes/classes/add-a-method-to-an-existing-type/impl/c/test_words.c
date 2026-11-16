// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/add-a-method-to-an-existing-type/.

#include "words.h"

#include <stdio.h>

static int failures = 0;

static void check(const char *name, const char *text, int want) {
    int got = word_count(text);
    if (got != want) {
        fprintf(stderr, "case %s failed: got %d, want %d\n", name, got, want);
        failures++;
    }
}

int main(void) {
    check("three-words", "the quick fox", 3);
    check("one-word", "hello", 1);
    check("punctuation", "Hello, world! How are you?", 5);
    check("empty", "", 0);
    check("blank", " \t\n ", 0);
    check("mixed-whitespace", "  two\twords\r\nand three  ", 4);
    check("non-ascii", "na\xc3\xafve caf\xc3\xa9 \xe6\x9d\xb1\xe4\xba\xac", 3);
    if (failures == 0) {
        printf("all cases passed\n");
    }
    return failures == 0 ? 0 : 1;
}
