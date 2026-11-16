// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

#ifndef ROMAN_H
#define ROMAN_H

#include <stddef.h>

// The size of a buffer for the longest numeral, MMMDCCCLXXXVIII, and the
// null byte.
#define ROMAN_SIZE 16

// Writes the Roman numeral of n into out. Returns 0, or -1 if n is not in
// 1 to 3999 or size is less than ROMAN_SIZE.
int to_roman(int n, char *out, size_t size);

// Returns the value of a Roman numeral in the standard form, or -1.
int from_roman(const char *s);

#endif
