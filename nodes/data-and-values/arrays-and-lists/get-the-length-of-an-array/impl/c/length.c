// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/get-the-length-of-an-array/.

#include "length.h"

#include <stddef.h>

size_t length(const char *const *items) {
    size_t n = 0;
    // An empty string is not a null pointer, so it counts as an item.
    while (items[n] != NULL) {
        n++;
    }
    return n;
}
