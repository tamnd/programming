// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/run-tasks-on-a-thread-pool/.

#ifndef POOL_H
#define POOL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define POOL_MAX_WORKERS 8

// Runs one job for each of the n numbers on a pool of 1 to POOL_MAX_WORKERS threads and writes
// the step count of numbers[i] to out[i]. Returns false if workers is out of range or no thread starts.
bool steps_on_pool(size_t workers, const int32_t numbers[], size_t n, int32_t out[]);

#endif
