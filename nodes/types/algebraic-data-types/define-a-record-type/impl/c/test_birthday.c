// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-record-type/.

#include "birthday.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty array has a dummy item in it, because C has no empty array. The
// count gives the real length.
#define CHECK(label, cnt, in, who, ...)                                        \
    do {                                                                       \
        const struct person people[] = in;                                     \
        const struct person want[] = {__VA_ARGS__};                            \
        struct person before[sizeof people / sizeof people[0]];                \
        struct person out[sizeof people / sizeof people[0]];                   \
        const size_t n = cnt;                                                  \
        memcpy(before, people, sizeof people);                                 \
        birthday(people, n, who, out);                                         \
        int ok = 1;                                                            \
        for (size_t i = 0; ok && i < n; i++) {                                 \
            ok = strcmp(out[i].name, want[i].name) == 0 &&                     \
                 out[i].age == want[i].age &&                                  \
                 people[i].age == before[i].age;                               \
        }                                                                      \
        if (!ok) {                                                             \
            fprintf(stderr, "case %s failed\n", label);                        \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("first", 2, L({"Ada", 36}, {"Alan", 41}), "Ada", {"Ada", 37}, {"Alan", 41});
    CHECK("second", 2, L({"Ada", 36}, {"Alan", 41}), "Alan", {"Ada", 36}, {"Alan", 42});
    CHECK("empty", 0, L({"", 0}), "Ada", {"", 0});
    CHECK("not-found", 1, L({"Ada", 36}), "Grace", {"Ada", 36});
    CHECK("duplicate-name", 3, L({"Ada", 36}, {"Alan", 41}, {"Ada", 9}), "Ada", {"Ada", 37}, {"Alan", 41}, {"Ada", 10});
    CHECK("zero-age", 1, L({"Baby", 0}), "Baby", {"Baby", 1});
    CHECK("non-ascii", 2, L({"Zo\xc3\xab", 20}, {"Zoe", 30}), "Zo\xc3\xab", {"Zo\xc3\xab", 21}, {"Zoe", 30});
    CHECK("case", 1, L({"ada", 36}), "Ada", {"ada", 36});
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
