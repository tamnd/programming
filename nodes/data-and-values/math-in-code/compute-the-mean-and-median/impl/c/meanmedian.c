// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

#include "meanmedian.h"

#include <stdlib.h>
#include <string.h>

static int compare(const void *a, const void *b) {
    long long x = *(const long long *)a;
    long long y = *(const long long *)b;
    return (x > y) - (x < y);
}

bool mean_and_median(const long long *items, size_t n, double *mean, double *median) {
    if (n == 0) {
        return false;
    }
    long long *s = malloc(n * sizeof *s);
    if (s == NULL) {
        return false;
    }
    memcpy(s, items, n * sizeof *s);
    qsort(s, n, sizeof *s, compare);
    double sum = 0;
    for (size_t i = 0; i < n; i++) {
        sum += (double)items[i];
    }
    size_t mid = n / 2;
    *mean = sum / (double)n;
    *median = n % 2 == 1 ? (double)s[mid] : ((double)s[mid - 1] + (double)s[mid]) / 2;
    free(s);
    return true;
}
