// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/loop-with-a-step/.

#ifndef COUNT_BY_H
#define COUNT_BY_H

#include <stddef.h>

// count_by returns a new array of start, start + step, ... while the value has
// not passed stop, and sets *count to the number of values. step must not be 0.
// The caller must free the array. It returns NULL when malloc fails.
int *count_by(int start, int stop, int step, size_t *count);

#endif
