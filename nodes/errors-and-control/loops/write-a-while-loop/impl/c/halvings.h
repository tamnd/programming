// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-while-loop/.

#ifndef HALVINGS_H
#define HALVINGS_H

#include <stddef.h>

// halvings writes n, n / 2, n / 4, ... while the value is greater than 0 to
// out, and returns the number of values. out must have room for 32 values.
size_t halvings(int n, int out[32]);

#endif
