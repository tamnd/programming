// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/iterate-over-a-map/.

#include "score_lines.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// by_key compares two items of the array of pointers. qsort gives the
// comparison a pointer to each item, so each argument points to a pointer.
static int by_key(const void *a, const void *b) {
    const struct entry *x = *(const struct entry *const *)a;
    const struct entry *y = *(const struct entry *const *)b;
    return strcmp(x->key, y->key);
}

int score_lines(const struct entry *map, size_t n, const struct entry **sorted, char *buf, size_t cap) {
    for (size_t i = 0; i < n; i++) {
        sorted[i] = &map[i];
    }
    if (n > 0) {
        qsort(sorted, n, sizeof sorted[0], by_key);
    }
    size_t len = 0;
    int total = 0;
    for (size_t i = 0; i <= n; i++) {
        int w;
        if (i < n) {
            w = snprintf(buf + len, cap - len, "%s=%d\n", sorted[i]->key, sorted[i]->value);
            total += sorted[i]->value;
        } else {
            w = snprintf(buf + len, cap - len, "total=%d\n", total);
        }
        if (w < 0 || (size_t)w >= cap - len) {
            return -1;
        }
        len += (size_t)w;
    }
    return (int)len;
}
