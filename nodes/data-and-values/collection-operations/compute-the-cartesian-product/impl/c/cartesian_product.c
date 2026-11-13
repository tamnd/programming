// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

#include "cartesian_product.h"

#include <stdint.h>

int cartesian_product(const struct int_list *lists, size_t n, int32_t *out, size_t cap, size_t *count) {
    // The product of no lists is one empty tuple.
    size_t total = 1;
    for (size_t k = 0; k < n; k++) {
        if (lists[k].len == 0) {
            *count = 0;
            return 0;
        }
    }
    for (size_t k = 0; k < n; k++) {
        // total * len could wrap around, so compare with SIZE_MAX / len.
        if (total > SIZE_MAX / lists[k].len) {
            return -1;
        }
        total *= lists[k].len;
    }
    if (n > 0 && total > cap / n) {
        return -1;
    }
    for (size_t r = 0; r < total; r++) {
        // The digits of r, with the last list as the lowest digit, give the
        // index of the item in each list.
        size_t rest = r;
        for (size_t k = n; k-- > 0;) {
            out[r * n + k] = lists[k].items[rest % lists[k].len];
            rest /= lists[k].len;
        }
    }
    *count = total;
    return 0;
}
