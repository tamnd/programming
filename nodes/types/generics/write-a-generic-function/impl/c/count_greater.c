// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-generic-function/.

#include "count_greater.h"

size_t count_greater(const void *items, size_t count, size_t size, const void *limit,
                     int (*cmp)(const void *, const void *)) {
    const unsigned char *p = items;
    size_t n = 0;
    for (size_t i = 0; i < count; i++) {
        if (cmp(p + i * size, limit) > 0) {
            n++;
        }
    }
    return n;
}

int compare_ints(const void *a, const void *b) {
    int x = *(const int *)a;
    int y = *(const int *)b;
    return (x > y) - (x < y);
}

size_t count_greater_int(const int *items, size_t count, int limit) {
    return count_greater(items, count, sizeof *items, &limit, compare_ints);
}
