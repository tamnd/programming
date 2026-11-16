// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

#ifndef MIN_MAX_H
#define MIN_MAX_H

#include <stddef.h>
#include <stdint.h>

// C has no tuple type. A small struct carries the two values.
struct min_max {
    int32_t min;
    int32_t max;
};

// min_max returns the smallest and the largest of the n numbers. n must be at
// least 1.
struct min_max min_max(const int32_t *numbers, size_t n);

#endif
