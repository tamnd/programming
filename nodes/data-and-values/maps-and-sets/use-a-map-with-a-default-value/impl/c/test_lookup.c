// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-map-with-a-default-value/.

#include "lookup.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty array has a dummy item in it, because C has no empty array. The
// counts give the real lengths.
#define CHECK(name, n, t, m, k, fallback, ...)                                 \
    do {                                                                       \
        const struct entry table[] = t;                                        \
        struct entry before[sizeof table / sizeof table[0]];                   \
        const char *const keys[] = k;                                          \
        const int want[] = {__VA_ARGS__};                                      \
        int out[sizeof keys / sizeof keys[0]];                                 \
        memcpy(before, table, sizeof table);                                   \
        lookup(table, n, keys, m, fallback, out);                              \
        if (memcmp(out, want, (m) * sizeof(int)) != 0 ||                       \
            memcmp(before, table, sizeof table) != 0) {                        \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("prices", 2, L({"apple", 3}, {"pear", 5}), 3, L("apple", "fig", "pear"), 0, 3, 0, 5);
    CHECK("negative-fallback", 1, L({"a", 1}), 2, L("b", "a"), -1, -1, 1);
    CHECK("empty-table", 0, L({"", 0}), 2, L("a", "b"), 7, 7, 7);
    CHECK("empty-keys", 1, L({"a", 1}), 0, L(""), 0, 0);
    CHECK("zero-value", 1, L({"a", 0}), 2, L("a", "b"), 5, 0, 5);
    CHECK("repeat", 1, L({"a", 1}), 3, L("b", "b", "a"), 2, 2, 2, 1);
    CHECK("case", 1, L({"a", 1}), 1, L("A"), 0, 0);
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
