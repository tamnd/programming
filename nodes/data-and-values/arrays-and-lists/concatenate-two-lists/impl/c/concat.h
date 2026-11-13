// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-two-lists/.

#ifndef CONCAT_H
#define CONCAT_H

#include <stddef.h>

// concat returns a new array with the n items of a, then the m items of b.
// The caller must free it. It returns NULL when the allocation fails.
int *concat(const int *a, size_t n, const int *b, size_t m);

#endif
