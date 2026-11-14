// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/do-set-operations/.

#include "compare_sets.h"

#include <stdlib.h>
#include <string.h>

static int by_strcmp(const void *x, const void *y) {
    return strcmp(*(const char *const *)x, *(const char *const *)y);
}

void compare_sets(const char **a, size_t na, const char **b, size_t nb, struct set_ops *r) {
    // qsort needs a valid pointer even for 0 items, so skip it for short arrays.
    if (na > 1) {
        qsort(a, na, sizeof a[0], by_strcmp);
    }
    if (nb > 1) {
        qsort(b, nb, sizeof b[0], by_strcmp);
    }
    r->n_either = r->n_both = r->n_only_a = r->n_exactly_one = 0;
    r->a_in_b = true;
    // Walk the two sorted arrays together, like the merge step of merge sort.
    size_t i = 0;
    size_t j = 0;
    while (i < na || j < nb) {
        int c;
        if (i == na) {
            c = 1;
        } else if (j == nb) {
            c = -1;
        } else {
            c = strcmp(a[i], b[j]);
        }
        const char *word = c <= 0 ? a[i] : b[j];
        r->either[r->n_either++] = word;
        if (c == 0) {
            r->both[r->n_both++] = word;
        } else {
            r->exactly_one[r->n_exactly_one++] = word;
            if (c < 0) {
                r->only_a[r->n_only_a++] = word;
                r->a_in_b = false;
            }
        }
        // Skip the word and its repeats in both arrays.
        while (i < na && strcmp(a[i], word) == 0) {
            i++;
        }
        while (j < nb && strcmp(b[j], word) == 0) {
            j++;
        }
    }
}
