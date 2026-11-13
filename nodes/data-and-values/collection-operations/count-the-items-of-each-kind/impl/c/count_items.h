// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

#ifndef COUNT_ITEMS_H
#define COUNT_ITEMS_H

#include <stddef.h>

struct item_count {
    int item;
    size_t count;
};

// count_items writes one pair for each distinct item of items[0..n) to out, in
// ascending order of the item. out must have room for n pairs. It returns the
// number of pairs, or 0 if n is 0. It does not change items.
size_t count_items(const int *items, size_t n, struct item_count *out);

#endif
