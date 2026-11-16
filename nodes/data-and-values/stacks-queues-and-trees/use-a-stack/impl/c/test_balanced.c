// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-stack/.

#include "balanced.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *label, const char *text, bool want) {
    if (balanced(text) != want) {
        fprintf(stderr, "case %s failed\n", label);
        failures++;
    }
}

int main(void) {
    static char deep[2001];
    memset(deep, '(', 1000);
    memset(deep + 1000, ')', 1000);
    deep[2000] = '\0';
    check("pair", "()", true);
    check("nested", "{[()()]}", true);
    check("code", "f(a[0], {b: 1})", true);
    check("crossed", "([)]", false);
    check("empty", "", true);
    check("no-brackets", "abc", true);
    check("unclosed", "((", false);
    check("extra-close", "())", false);
    check("close-first", ")(", false);
    check("wrong-kind", "(]", false);
    check("deep", deep, true);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
