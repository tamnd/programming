// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/rotate-a-list/.

#ifndef ROTATE_LEFT_H
#define ROTATE_LEFT_H

#include <stddef.h>

// rotate_left writes items[0..n) to out[0..n), with the first k items moved
// to the end.
void rotate_left(const int *items, size_t n, size_t k, int *out);

#endif
