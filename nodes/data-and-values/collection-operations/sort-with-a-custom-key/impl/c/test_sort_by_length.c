// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/sort-with-a-custom-key/.

#include "sort_by_length.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty list has "" in it, because C has no empty array. N gives the real
// length.
#define CHECK(name, n, given, sorted)                                          \
    do {                                                                       \
        const char *g[] = given;                                               \
        const char *const want[] = sorted;                                     \
        const size_t have_n = n;                                               \
        sort_by_length(g, have_n);                                             \
        int ok = 1;                                                            \
        for (size_t i = 0; ok && i < have_n; i++) {                            \
            ok = strcmp(g[i], want[i]) == 0;                                   \
        }                                                                      \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("fruits", 4, L("pear", "fig", "apple", "kiwi"), L("fig", "kiwi", "pear", "apple"));
    CHECK("three", 3, L("bb", "a", "ccc"), L("a", "bb", "ccc"));
    CHECK("empty", 0, L(""), L(""));
    CHECK("single", 1, L("x"), L("x"));
    CHECK("same-length", 3, L("dog", "cat", "ant"), L("ant", "cat", "dog"));
    CHECK("duplicates", 3, L("b", "a", "b"), L("a", "b", "b"));
    CHECK("sorted", 3, L("a", "bb", "ccc"), L("a", "bb", "ccc"));
    CHECK("reverse", 3, L("ccc", "bb", "a"), L("a", "bb", "ccc"));
    CHECK("long", 3, L("abcdefghij", "abcdefghi", "z"), L("z", "abcdefghi", "abcdefghij"));
    CHECK("prefix", 4, L("ab", "a", "abc", "b"), L("a", "b", "ab", "abc"));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
