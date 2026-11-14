// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/create-a-set/.

#include "toggle.h"

#include <string.h>

// find returns the index of word in set (k items), or the index where word
// must go if set does not have it. *found tells which.
static size_t find(const char *const *set, size_t k, const char *word, int *found) {
    size_t lo = 0;
    size_t hi = k;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        int c = strcmp(set[mid], word);
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

size_t toggle(const char *const *words, size_t n, const char **out) {
    // C has no set type. A sorted array is a set: a binary search tests
    // membership, and memmove opens or closes a gap.
    size_t k = 0;
    for (size_t i = 0; i < n; i++) {
        int found;
        size_t at = find(out, k, words[i], &found);
        if (found) {
            memmove(&out[at], &out[at + 1], (k - at - 1) * sizeof out[0]);
            k--;
        } else {
            memmove(&out[at + 1], &out[at], (k - at) * sizeof out[0]);
            out[at] = words[i];
            k++;
        }
    }
    return k;
}
