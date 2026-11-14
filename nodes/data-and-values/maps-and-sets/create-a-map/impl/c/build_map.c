// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-map/.

#include "build_map.h"

#include <string.h>

// find returns the index of key in the sorted entries map[0 .. k), or the
// index where key goes. *found tells which.
static size_t find(const struct entry *map, size_t k, const char *key, int *found) {
    size_t lo = 0;
    size_t hi = k;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        int c = strcmp(map[mid].key, key);
        if (c == 0) {
            *found = 1;
            return mid;
        }
        if (c < 0) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    *found = 0;
    return lo;
}

size_t build_map(const struct entry *pairs, size_t n, const char *const *drop, size_t m, struct entry *out) {
    // The map is a sorted array. C has no map type in its standard library.
    size_t k = 0;
    for (size_t i = 0; i < n; i++) {
        int found;
        size_t at = find(out, k, pairs[i].key, &found);
        if (found) {
            out[at].value = pairs[i].value;
            continue;
        }
        memmove(&out[at + 1], &out[at], (k - at) * sizeof out[0]);
        out[at] = pairs[i];
        k++;
    }
    for (size_t i = 0; i < m; i++) {
        int found;
        size_t at = find(out, k, drop[i], &found);
        if (found) {
            memmove(&out[at], &out[at + 1], (k - at - 1) * sizeof out[0]);
            k--;
        }
    }
    return k;
}
