// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-an-index/.

#include "positions.h"

#include <stdio.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, const char *const *items, size_t n, const char *target,
                  const size_t *want, size_t wn) {
    size_t out[8];
    size_t k = positions(items, n, target, out);
    if (k != wn || (k > 0 && memcmp(out, want, k * sizeof *out) != 0)) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
}

#define LEN(a) (sizeof(a) / sizeof((a)[0]))

int main(void) {
    const char *two[] = {"a", "b", "a", "c"};
    const size_t two_w[] = {0, 2};
    check("two-matches", two, LEN(two), "a", two_w, LEN(two_w));
    const char *words[] = {"red", "green", "blue", "green"};
    const size_t words_w[] = {1, 3};
    check("words", words, LEN(words), "green", words_w, LEN(words_w));
    check("empty", NULL, 0, "a", NULL, 0);
    const char *none[] = {"a", "b"};
    check("none", none, LEN(none), "z", NULL, 0);
    const char *all[] = {"x", "x", "x"};
    const size_t all_w[] = {0, 1, 2};
    check("all", all, LEN(all), "x", all_w, LEN(all_w));
    const char *last[] = {"a", "b", "c"};
    const size_t last_w[] = {2};
    check("last", last, LEN(last), "c", last_w, LEN(last_w));
    const char *cs[] = {"A", "a"};
    const size_t cs_w[] = {1};
    check("case", cs, LEN(cs), "a", cs_w, LEN(cs_w));
    const char *es[] = {"", "a", ""};
    const size_t es_w[] = {0, 2};
    check("empty-string", es, LEN(es), "", es_w, LEN(es_w));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
