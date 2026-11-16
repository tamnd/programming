// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-priority-queue/.

#ifndef SERVE_ORDER_H
#define SERVE_ORDER_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// serve_order writes the n request numbers to order in serve order: the
// largest priority first, and for equal priorities, the smaller request
// number first. order must have room for n values. It returns false if it
// cannot get memory.
bool serve_order(const int32_t *priorities, size_t n, size_t *order);

#endif
