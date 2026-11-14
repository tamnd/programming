// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

#include "score_lines.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

#define L(...) {__VA_ARGS__}

// An empty map has a dummy entry in it, because C has no empty array. The
// count gives the real length.
#define CHECK(name, n, given, want)                                            \
    do {                                                                       \
        const struct entry g[] = given;                                        \
        const struct entry *sorted[sizeof g / sizeof g[0]];                    \
        char buf[256];                                                         \
        int got = score_lines(g, n, sorted, buf, sizeof buf);                  \
        if (got != (int)strlen(want) || strcmp(buf, want) != 0) {              \
            fprintf(stderr, "case %s failed\n", name);                         \
            failures++;                                                        \
        }                                                                      \
    } while (0)

int main(void) {
    CHECK("scores", 3, L({"bob", 7}, {"amy", 9}, {"cal", 4}), "amy=9\nbob=7\ncal=4\ntotal=20\n");
    CHECK("one", 1, L({"x", 1}), "x=1\ntotal=1\n");
    CHECK("empty", 0, L({"", 0}), "total=0\n");
    CHECK("case", 4, L({"b", 1}, {"B", 2}, {"a", 3}, {"A", 4}), "A=4\nB=2\na=3\nb=1\ntotal=10\n");
    CHECK("negative", 2, L({"a", -5}, {"b", 5}), "a=-5\nb=5\ntotal=0\n");
    CHECK("prefix", 3, L({"abc", 3}, {"a", 2}, {"ab", 1}), "a=2\nab=1\nabc=3\ntotal=6\n");
    CHECK("reverse", 10, L({"j", 1}, {"i", 2}, {"h", 3}, {"g", 4}, {"f", 5}, {"e", 6}, {"d", 7}, {"c", 8}, {"b", 9}, {"a", 10}), "a=10\nb=9\nc=8\nd=7\ne=6\nf=5\ng=4\nh=3\ni=2\nj=1\ntotal=55\n");
    char small[8];
    const struct entry one[] = {{"apple", 3}};
    const struct entry *sorted[1];
    if (score_lines(one, 1, sorted, small, sizeof small) != -1) {
        fprintf(stderr, "case small failed\n");
        failures++;
    }
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
