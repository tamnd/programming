// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-cartesian-product/.

#ifndef CARTESIAN_PRODUCT_H
#define CARTESIAN_PRODUCT_H

#include <stddef.h>
#include <stdint.h>

// An int_list is len items at items. items can be NULL when len is 0.
struct int_list {
    const int32_t *items;
    size_t len;
};

// cartesian_product writes each tuple that takes one item from each of the n
// lists to out, in nested-loop order. Tuple r is at out[r * n] to
// out[r * n + n - 1]. It stores the number of tuples in *count and returns 0,
// or it returns -1 when out has room for fewer than count * n items. Then it
// writes nothing.
int cartesian_product(const struct int_list *lists, size_t n, int32_t *out, size_t cap, size_t *count);

#endif
