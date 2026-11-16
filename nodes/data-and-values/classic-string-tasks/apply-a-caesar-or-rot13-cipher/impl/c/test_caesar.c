// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/apply-a-caesar-or-rot13-cipher/.

#include "caesar.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct test_case {
    const char *name;
    const char *text;
    long long shift;
    const char *want;
};

static const struct test_case cases[] = {
    {"hi-2", "HI", 2, "JK"},
    {"hi-20", "HI", 20, "BC"},
    {"rot13", "Hello, World!", 13, "Uryyb, Jbeyq!"},
    {"empty", "", 3, ""},
    {"zero", "abc", 0, "abc"},
    {"full-turn", "abc", 26, "abc"},
    {"minus-one", "abc", -1, "zab"},
    {"decode", "Uryyb, Jbeyq!", -13, "Hello, World!"},
    {"above-26", "xyz", 27, "yza"},
    {"wrap", "Zz", 1, "Aa"},
    {"non-letters", "123 !? [`{@", 5, "123 !? [`{@"},
    {"non-ascii", "café ü", 1, "dbgé ü"},
    {"large", "abc", 9007199254740991LL, "fgh"},
    {"large-negative", "abc", -9007199254740991LL, "vwx"},
};

int main(void) {
    int failures = 0;
    for (size_t i = 0; i < sizeof cases / sizeof cases[0]; i++) {
        const struct test_case *c = &cases[i];
        char buf[64];
        snprintf(buf, sizeof buf, "%s", c->text);
        caesar(buf, c->shift);
        if (strcmp(buf, c->want) != 0) {
            printf("FAIL %s: got \"%s\"\n", c->name, buf);
            failures++;
        }
        caesar(buf, -c->shift);
        if (strcmp(buf, c->text) != 0) {
            printf("FAIL %s decoded: got \"%s\"\n", c->name, buf);
            failures++;
        }
    }
    if (failures != 0) {
        return EXIT_FAILURE;
    }
    printf("ok\n");
    return EXIT_SUCCESS;
}
