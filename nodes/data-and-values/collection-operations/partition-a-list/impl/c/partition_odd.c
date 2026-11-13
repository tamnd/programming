// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/partition-a-list/.

#include "partition_odd.h"

void partition_odd(const int *items, size_t n, int *odd, size_t *n_odd, int *even, size_t *n_even) {
    size_t no = 0;
    size_t ne = 0;
    for (size_t i = 0; i < n; i++) {
        if (items[i] % 2 != 0) {
            odd[no++] = items[i];
        } else {
            even[ne++] = items[i];
        }
    }
    *n_odd = no;
    *n_even = ne;
}
