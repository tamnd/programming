// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

#ifndef FLATTEN_H
#define FLATTEN_H

#include <stddef.h>
#include <stdint.h>

// An int_list is len items at items. items can be NULL when len is 0.
struct int_list {
    const int32_t *items;
    size_t len;
};

// flatten writes the items of the n lists to out, in order, and stores the
// number of items in *len. It returns 0, or -1 when out has room for fewer
// than all the items. Then it writes nothing.
int flatten(const struct int_list *lists, size_t n, int32_t *out, size_t cap, size_t *len);

#endif
