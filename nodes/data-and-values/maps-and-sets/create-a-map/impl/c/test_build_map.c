// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

#include "build_map.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty list has a dummy item in it, because C has no empty array. The
// counts give the real lengths.
#define CHECK(name, n, given, m, del, k, ...)                                  \
    do {                                                                       \
        const struct entry g[] = given;                                        \
        const char *const d[] = del;                                           \
        const struct entry want[] = {__VA_ARGS__};                             \
        struct entry out[sizeof g / sizeof g[0]];                              \
        const size_t want_k = k;                                               \
        size_t got = build_map(g, n, d, m, out);                               \
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
    CHECK("fruits", 3, L({"apple", 3}, {"pear", 5}, {"fig", 2}), 0, L(""), 3, {"apple", 3}, {"fig", 2}, {"pear", 5});
    CHECK("update-and-delete", 4, L({"a", 1}, {"b", 2}, {"a", 7}, {"c", 3}), 1, L("b"), 2, {"a", 7}, {"c", 3});
    CHECK("empty", 0, L({"", 0}), 0, L(""), 0, {NULL, 0});
    CHECK("replace", 3, L({"k", 1}, {"k", 2}, {"k", 3}), 0, L(""), 1, {"k", 3});
    CHECK("delete-absent", 1, L({"a", 1}), 1, L("z"), 1, {"a", 1});
    CHECK("delete-all", 2, L({"a", 1}, {"b", 2}), 2, L("a", "b"), 0, {NULL, 0});
    CHECK("delete-twice", 2, L({"a", 1}, {"b", 2}), 2, L("a", "a"), 1, {"b", 2});
    CHECK("zero-value", 2, L({"a", 0}, {"b", -4}), 0, L(""), 2, {"a", 0}, {"b", -4});
    CHECK("case", 4, L({"b", 1}, {"B", 2}, {"a", 3}, {"A", 4}), 0, L(""), 4, {"A", 4}, {"B", 2}, {"a", 3}, {"b", 1});
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
