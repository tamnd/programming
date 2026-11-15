// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

#include "zip_join.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int failures = 0;

static void check(const char *name, const char *const *lower, size_t nl, const char *const *upper, size_t nu,
                  const int *digits, size_t nd, const char *const *want, size_t nw) {
    char *out[8];
    size_t n = zip_join(lower, nl, upper, nu, digits, nd, out);
    int ok = n == nw;
    for (size_t i = 0; ok && i < n; i++) {
        ok = strcmp(out[i], want[i]) == 0;
    }
    if (n != (size_t)-1) {
        for (size_t i = 0; i < n; i++) {
            free(out[i]);
        }
    }
    if (!ok) {
        fprintf(stderr, "case %s failed\n", name);
        failures++;
    }
}

#define LEN(a) (sizeof(a) / sizeof((a)[0]))

int main(void) {
    const char *abc[] = {"a", "b", "c"}, *ABC[] = {"A", "B", "C"};
    const int d123[] = {1, 2, 3};
    const char *w_rosetta[] = {"aA1", "bB2", "cC3"};
    check("rosetta", abc, 3, ABC, 3, d123, 3, w_rosetta, LEN(w_rosetta));
    const char *xy[] = {"x", "y"}, *XY[] = {"X", "Y"};
    const int d1020[] = {10, 20};
    const char *w_words[] = {"xX10", "yY20"};
    check("words", xy, 2, XY, 2, d1020, 2, w_words, LEN(w_words));
    const char *aA1[] = {"aA1"};
    check("shorter-first", abc, 1, ABC, 2, d123, 3, aA1, 1);
    check("shorter-last", abc, 3, ABC, 3, d123, 1, aA1, 1);
    check("one-empty", abc, 2, NULL, 0, d123, 2, NULL, 0);
    check("all-empty", NULL, 0, NULL, 0, NULL, 0, NULL, 0);
    const int neg[] = {-5};
    const char *w_neg[] = {"aA-5"};
    check("negative", abc, 1, ABC, 1, neg, 1, w_neg, 1);
    const char *es_l[] = {"", "b"}, *es_u[] = {"A", ""};
    const char *w_es[] = {"A1", "b2"};
    check("empty-strings", es_l, 2, es_u, 2, d123, 2, w_es, LEN(w_es));
    if (failures > 0) {
        return 1;
    }
    printf("all cases pass\n");
    return 0;
}
