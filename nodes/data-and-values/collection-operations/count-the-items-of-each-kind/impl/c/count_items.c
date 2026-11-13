// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

#include "count_items.h"

#include <stdlib.h>
#include <string.h>

// cmp_int compares two ints. It does not subtract them, because the
// subtraction can overflow.
static int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

size_t count_items(const int *items, size_t n, struct item_count *out) {
    if (n == 0) {
        return 0;
    }
    // qsort sorts in place, so sort a copy.
    int *s = malloc(n * sizeof *s);
    if (s == NULL) {
        return 0;
    }
    memcpy(s, items, n * sizeof *s);
    qsort(s, n, sizeof *s, cmp_int);
    size_t k = 0;
    for (size_t i = 0; i < n; i++) {
        if (k > 0 && out[k - 1].item == s[i]) {
            out[k - 1].count++;
        } else {
            out[k].item = s[i];
            out[k].count = 1;
            k++;
        }
    }
    free(s);
    return k;
}
