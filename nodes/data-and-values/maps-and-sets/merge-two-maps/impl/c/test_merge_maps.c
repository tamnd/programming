// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

#include "merge_maps.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty map has a dummy entry in it, because C has no empty array. The
// counts give the real lengths.
#define CHECK(name, nb, b, no, o, k, ...)                                      \
    do {                                                                       \
        const struct entry base[] = b;                                         \
        const struct entry over[] = o;                                         \
        const struct entry want[] = {__VA_ARGS__};                             \
        struct entry out[sizeof base / sizeof base[0] + sizeof over / sizeof over[0]]; \
        const size_t want_k = k;                                               \
        size_t got = merge_maps(base, nb, over, no, out);                      \
        int ok = got == want_k;                                                \
        for (size_t i = 0; ok && i < want_k; i++) {                            \
            ok = strcmp(out[i].key, want[i].key) == 0 &&                       \
                 out[i].value == want[i].value;                                \
        }                                                                      \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("prices", 2, L({"apple", 3}, {"pear", 5}), 2, L({"fig", 2}, {"pear", 6}), 3, {"apple", 3}, {"fig", 2}, {"pear", 6});
    CHECK("disjoint", 1, L({"a", 1}), 1, L({"b", 2}), 2, {"a", 1}, {"b", 2});
    CHECK("empty-base", 0, L({"", 0}), 1, L({"a", 1}), 1, {"a", 1});
    CHECK("empty-override", 1, L({"a", 1}), 0, L({"", 0}), 1, {"a", 1});
    CHECK("both-empty", 0, L({"", 0}), 0, L({"", 0}), 0, {NULL, 0});
    CHECK("same-keys", 2, L({"a", 1}, {"b", 2}), 2, L({"a", 9}, {"b", 8}), 2, {"a", 9}, {"b", 8});
    CHECK("zero-wins", 1, L({"a", 5}), 1, L({"a", 0}), 1, {"a", 0});
    CHECK("case", 1, L({"a", 1}), 1, L({"A", 2}), 2, {"A", 2}, {"a", 1});
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
