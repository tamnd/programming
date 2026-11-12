// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

#ifndef COMPARE_H
#define COMPARE_H

#include <stdbool.h>

// compare returns -1, 0 or 1 for the code point order of the UTF-8 strings a
// and b.
int compare(const char *a, const char *b);

// equals reports whether a and b have the same bytes.
bool equals(const char *a, const char *b);

#endif
