// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/merge-two-maps/.

#include "merge_maps.h"

#include <string.h>

size_t merge_maps(const struct entry *base, size_t nb, const struct entry *over, size_t no, struct entry *out) {
    // Both arrays are in key order, so one pass over both, like the merge step
    // of a merge sort, gives the result in key order.
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    while (i < nb || j < no) {
        int c;
        if (i == nb) {
            c = 1;
        } else if (j == no) {
            c = -1;
        } else {
            c = strcmp(base[i].key, over[j].key);
        }
        if (c < 0) {
            out[k++] = base[i++];
        } else if (c > 0) {
            out[k++] = over[j++];
        } else {
            // The same key in both: take the entry of over, and skip both.
            out[k++] = over[j++];
            i++;
        }
    }
    return k;
}
