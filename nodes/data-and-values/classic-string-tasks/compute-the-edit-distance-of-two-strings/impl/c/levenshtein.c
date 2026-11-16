// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-edit-distance-of-two-strings/.

#include "levenshtein.h"

#include <stdlib.h>
#include <string.h>

static int min3(int x, int y, int z) {
    int m = x < y ? x : y;
    return m < z ? m : z;
}

int levenshtein(const char *a, const char *b) {
    size_t m = strlen(a);
    size_t n = strlen(b);
    int *prev = malloc((n + 1) * sizeof *prev);
    int *cur = malloc((n + 1) * sizeof *cur);
    if (prev == NULL || cur == NULL) {
        free(prev);
        free(cur);
        return -1;
    }
    for (size_t j = 0; j <= n; j++) {
        prev[j] = (int)j;
    }
    for (size_t i = 1; i <= m; i++) {
        cur[0] = (int)i;
        for (size_t j = 1; j <= n; j++) {
            int cost = a[i - 1] == b[j - 1] ? 0 : 1;
            cur[j] = min3(prev[j] + 1, cur[j - 1] + 1, prev[j - 1] + cost);
        }
        int *t = prev;
        prev = cur;
        cur = t;
    }
    int result = prev[n];
    free(prev);
    free(cur);
    return result;
}
