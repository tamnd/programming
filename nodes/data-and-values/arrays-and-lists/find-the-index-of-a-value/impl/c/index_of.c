// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-index-of-a-value/.

#include "index_of.h"

#include <stddef.h>

ptrdiff_t index_of(const int *items, size_t n, int value) {
    for (size_t i = 0; i < n; i++) {
        if (items[i] == value) {
            return (ptrdiff_t)i;
        }
    }
    return -1;
}
