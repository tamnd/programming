// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

#include "toggle.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty array has a dummy item in it, because C has no empty array. The
// counts give the real lengths.
#define CHECK(name, cnt, ws, kk, ...)                                          \
    do {                                                                       \
        const char *const words[] = ws;                                        \
        const char *const want[] = {__VA_ARGS__};                              \
        const char *out[sizeof words / sizeof words[0]];                       \
        const size_t want_k = kk;                                              \
        size_t got = toggle(words, cnt, out);                                  \
        int ok = got == want_k;                                                \
        for (size_t i = 0; ok && i < want_k; i++) {                            \
            ok = strcmp(out[i], want[i]) == 0;                                 \
        }                                                                      \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("colors", 4, L("red", "blue", "red", "green"), 2, "blue", "green");
    CHECK("fresh", 2, L("b", "a"), 2, "a", "b");
    CHECK("empty", 0, L(""), 0, "");
    CHECK("two-times", 2, L("a", "a"), 0, "");
    CHECK("three-times", 3, L("a", "a", "a"), 1, "a");
    CHECK("case", 2, L("a", "A"), 2, "A", "a");
    CHECK("prefix", 2, L("ab", "a"), 2, "a", "ab");
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
