// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-formatted-output/.

#ifndef ROW_H
#define ROW_H

#include <stddef.h>

// Writes one line of a table into out: name in 10 columns on the left, qty
// in 5 on the right and price with 2 decimals in 8 on the right. Returns the
// length of the full line, as snprintf does. If the result is size or more,
// the line was cut to fit the buffer.
int format_row(char *out, size_t size, const char *name, long long qty, double price);

#endif
