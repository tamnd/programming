// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

#include "compare_sets.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

// S makes a modifiable array of words and gives its length. An empty list is
// NULL, 0, because C has no empty array.
#define S(...) (const char *[]){__VA_ARGS__}, sizeof((const char *[]){__VA_ARGS__}) / sizeof(const char *)

enum { CAP = 16 };

// append adds the k words of xs, joined by commas, and then sep to buf.
static void append(char *buf, size_t size, const char *const *xs, size_t k, const char *sep) {
    for (size_t i = 0; i < k; i++) {
        size_t len = strlen(buf);
        snprintf(buf + len, size - len, "%s%s", i > 0 ? "," : "", xs[i]);
    }
    size_t len = strlen(buf);
    snprintf(buf + len, size - len, "%s", sep);
}

// check compares the results, joined as union|intersection|difference|symmetric|subset, with want.
static void check(const char *name, const char **a, size_t na, const char **b, size_t nb, const char *want) {
    const char *either[CAP], *both[CAP], *only_a[CAP], *exactly_one[CAP];
    struct set_ops r = {.either = either, .both = both, .only_a = only_a, .exactly_one = exactly_one};
    compare_sets(a, na, b, nb, &r);
    char got[256] = "";
    append(got, sizeof got, either, r.n_either, "|");
    append(got, sizeof got, both, r.n_both, "|");
    append(got, sizeof got, only_a, r.n_only_a, "|");
    append(got, sizeof got, exactly_one, r.n_exactly_one, r.a_in_b ? "|1" : "|0");
    if (strcmp(got, want) != 0) {
        fprintf(stderr, "case %s failed: got %s, want %s\n", name, got, want);
        failures++;
    }
}

int main(void) {
    check("colors", S("red", "blue", "green"), S("blue", "yellow"), "blue,green,red,yellow|blue|green,red|green,red,yellow|0");
    check("subset", S("b"), S("a", "b", "c"), "a,b,c|b||a,c|1");
    check("both-empty", NULL, 0, NULL, 0, "||||1");
    check("empty-a", NULL, 0, S("x"), "x|||x|1");
    check("empty-b", S("x"), NULL, 0, "x||x|x|0");
    check("equal", S("a", "b"), S("b", "a"), "a,b|a,b|||1");
    check("disjoint", S("a"), S("b"), "a,b||a|a,b|0");
    check("duplicates", S("a", "a", "b"), S("b", "b"), "a,b|b|a|a|0");
    check("case", S("a"), S("A"), "A,a||a|A,a|0");
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
