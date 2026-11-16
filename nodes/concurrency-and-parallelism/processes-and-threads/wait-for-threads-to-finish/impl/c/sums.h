// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/wait-for-threads-to-finish/.

#ifndef SUMS_H
#define SUMS_H

#include <stdbool.h>
#include <stddef.h>

// Sums each of the n chunks on its own thread; chunk i has lengths[i] items.
// Waits for all threads and writes sum i to out[i]. Returns false if a thread cannot be created.
bool parallel_sums(const long long *const chunks[], const size_t lengths[], size_t n, long long out[]);

#endif
