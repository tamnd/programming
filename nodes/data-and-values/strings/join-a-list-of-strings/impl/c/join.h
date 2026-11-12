// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/join-a-list-of-strings/.

#ifndef JOIN_H
#define JOIN_H

#include <stddef.h>
#include <stdint.h>

// join returns the n items with sep between each two, in a new string that
// the caller frees. It returns NULL if the allocation fails.
char *join(const char *const *items, size_t n, const char *sep);

// join_numbers returns the decimal form of each of the n items, with sep
// between each two, in a new string that the caller frees.
char *join_numbers(const int32_t *items, size_t n, const char *sep);

#endif
