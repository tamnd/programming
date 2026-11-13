// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

#ifndef PERMUTATIONS_H
#define PERMUTATIONS_H

#include <stddef.h>
#include <stdint.h>

// PERMUTATIONS_MAX is the largest n. 12! is 479001600, which fits in a
// 32-bit size_t.
#define PERMUTATIONS_MAX 12

// permutations writes each order of the n items to out, in the lexicographic
// order of the positions. Order r is at out[r * n] to out[r * n + n - 1]. It
// stores the number of orders, n!, in *count and returns 0. It returns -1
// when n > PERMUTATIONS_MAX, or when out has room for fewer than n! * n
// items. Then it writes nothing.
int permutations(const int32_t *items, size_t n, int32_t *out, size_t cap, size_t *count);

#endif
