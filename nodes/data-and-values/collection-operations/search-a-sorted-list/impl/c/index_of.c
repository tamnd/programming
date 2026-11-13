// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/search-a-sorted-list/.

#include "index_of.h"

long index_of(const int *items, size_t n, int target) {
    // bsearch can return any of the equal items, so this is a search for the
    // first index lo with items[lo] >= target.
    size_t lo = 0;
    size_t hi = n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (items[mid] < target) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return lo < n && items[lo] == target ? (long)lo : -1;
}
