// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/group-items-by-a-key/.

#include "group_by_length.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty list has "" in it, because C has no empty array. The counts give the
// real lengths. ORDER is the words in the order of sorted.
#define CHECK(name, n, given, order, m, ...)                                   \
    do {                                                                       \
        const char *const g[] = given;                                         \
        const char *const o[] = order;                                         \
        const struct word_group w[] = {__VA_ARGS__};                           \
        const char *sorted[sizeof g / sizeof g[0]];                            \
        struct word_group out[sizeof g / sizeof g[0]];                         \
        const size_t want_n = m;                                               \
        const size_t have_n = n;                                               \
        size_t k = group_by_length(g, have_n, sorted, out);                    \
        int ok = k == want_n;                                                  \
        for (size_t i = 0; ok && i < have_n; i++) {                            \
            ok = strcmp(sorted[i], o[i]) == 0;                                 \
        }                                                                      \
        for (size_t i = 0; ok && i < want_n; i++) {                            \
            ok = out[i].len == w[i].len && out[i].start == w[i].start &&       \
                 out[i].count == w[i].count;                                   \
        }                                                                      \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("fruits", 6, L("apple", "fig", "kiwi", "pear", "plum", "date"), L("fig", "kiwi", "pear", "plum", "date", "apple"), 3, {3, 0, 1}, {4, 1, 4}, {5, 5, 1});
    CHECK("letters", 3, L("a", "bb", "c"), L("a", "c", "bb"), 2, {1, 0, 2}, {2, 2, 1});
    CHECK("empty", 0, L(""), L(""), 0, {0, 0, 0});
    CHECK("single", 1, L("x"), L("x"), 1, {1, 0, 1});
    CHECK("same-length", 3, L("cat", "dog", "cow"), L("cat", "dog", "cow"), 1, {3, 0, 3});
    CHECK("order", 3, L("ccc", "a", "bb"), L("a", "bb", "ccc"), 3, {1, 0, 1}, {2, 1, 1}, {3, 2, 1});
    CHECK("duplicates", 3, L("ab", "ab", "c"), L("c", "ab", "ab"), 2, {1, 0, 1}, {2, 1, 2});
    CHECK("long", 3, L("abcdefghij", "abcdefghi", "a"), L("a", "abcdefghi", "abcdefghij"), 3, {1, 0, 1}, {9, 1, 1}, {10, 2, 1});
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
