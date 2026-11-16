// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wrap-text-to-a-width/.

#include "wrap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name;
    const char *text;
    size_t width;
    int count;
    const char *want;
};

static const struct test_case cases[] = {
    {"fox-ten", "The quick brown fox jumps over the lazy dog", 10, 5, "The quick\nbrown fox\njumps over\nthe lazy\ndog\n"},
    {"fox-twenty", "The quick brown fox jumps over the lazy dog", 20, 3, "The quick brown fox\njumps over the lazy\ndog\n"},
    {"sentence", "Wrap text to a width with a greedy loop.", 15, 3, "Wrap text to a\nwidth with a\ngreedy loop.\n"},
    {"empty", "", 10, 0, ""},
    {"only-spaces", "   ", 5, 0, ""},
    {"fits", "short text", 20, 1, "short text\n"},
    {"exact-fit", "abcde fghij", 5, 2, "abcde\nfghij\n"},
    {"long-word", "a supercalifragilistic word", 10, 3, "a\nsupercalifragilistic\nword\n"},
    {"many-spaces", "one   two\tthree\nfour", 9, 3, "one two\nthree\nfour\n"},
    {"width-one", "a b c", 1, 3, "a\nb\nc\n"},
    {"line-exact", "ab cd ef", 5, 2, "ab cd\nef\n"},
};

int main(void) {
    int failures = 0;
    char buf[128];
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        int n = wrap(c->text, c->width, buf, sizeof buf);
        if (n != c->count || strcmp(buf, c->want) != 0) {
            printf("FAIL %s: got %d \"%s\"\n", c->name, n, buf);
            failures++;
        }
    }
    char small[8];
    if (wrap("The quick brown fox", 10, small, sizeof small) != -1) {
        printf("FAIL small buffer\n");
        failures++;
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
