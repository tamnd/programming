// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-queue/.

#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// finish_order writes the n job numbers to order, in the order in which
// round-robin finishes them. work has n values. order must have room for n
// values. It returns false if it cannot get memory.
bool finish_order(const int32_t *work, size_t n, int32_t slice, size_t *order);

#endif
