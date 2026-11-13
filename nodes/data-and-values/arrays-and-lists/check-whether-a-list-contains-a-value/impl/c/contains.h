// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

#ifndef CONTAINS_H
#define CONTAINS_H

#include <stdbool.h>
#include <stddef.h>

// contains returns true if one of the n items is equal to value.
bool contains(const int *items, size_t n, int value);

#endif
