// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

#ifndef COMBINATIONS_H
#define COMBINATIONS_H

#include <stddef.h>
#include <stdint.h>

// COMBINATIONS_MAX is the largest n.
#define COMBINATIONS_MAX 32

// combinations writes each choice of k of the n items to out, in the
// lexicographic order of the positions. Choice r is at out[r * k] to
// out[r * k + k - 1]. It stores the number of choices, C(n, k), in *count and
// returns 0. k = 0 gives one empty choice, and k > n gives no choices. It
// returns -1 when n > COMBINATIONS_MAX, when C(n, k) does not fit in a
// size_t, or when out has room for fewer than C(n, k) * k items. Then it
// writes nothing.
int combinations(const int32_t *items, size_t n, size_t k, int32_t *out, size_t cap, size_t *count);

#endif
