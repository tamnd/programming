// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

#ifndef ZIP_JOIN_H
#define ZIP_JOIN_H

#include <stddef.h>

// zip_join joins lower[i], upper[i] and the decimal form of digits[i] for each
// index i below the smallest of the three lengths. It writes a new string for
// each index to out, which must have room for the smallest length, and returns
// the number of strings. The caller frees each string. It returns (size_t)-1
// when malloc fails.
size_t zip_join(const char *const *lower, size_t nl, const char *const *upper, size_t nu, const int *digits,
                size_t nd, char **out);

#endif
