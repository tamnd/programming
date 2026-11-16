// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compute-the-mean-and-median/.

#ifndef MEAN_AND_MEDIAN_H
#define MEAN_AND_MEDIAN_H

#include <stdbool.h>
#include <stddef.h>

// Sets *mean and *median for the n items and returns true. Returns false
// if n is 0 or if the copy cannot be allocated. The items do not change.
bool mean_and_median(const long long *items, size_t n, double *mean, double *median);

#endif
