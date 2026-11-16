// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-fixed-decimals/.

#ifndef FIXED_H
#define FIXED_H

#include <stddef.h>

// Writes x with exactly digits digits after the decimal point into out.
// Returns the length of the full result, as snprintf does. If the result is
// size or more, the text was cut to fit the buffer. The decimal point comes
// from the locale, and the C locale gives a dot.
int fixed(char *out, size_t size, double x, int digits);

#endif
