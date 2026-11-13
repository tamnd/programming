// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/append-to-a-list/.

#ifndef INSERT_AT_H
#define INSERT_AT_H

#include <stddef.h>

// insert_at returns a new array of n + 1 items: the n items of items with
// value at position index. index n appends. The caller must free the result.
// It returns NULL when index > n or when the allocation fails.
int *insert_at(const int *items, size_t n, size_t index, int value);

#endif
