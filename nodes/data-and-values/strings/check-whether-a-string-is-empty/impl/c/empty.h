// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

#ifndef EMPTY_H
#define EMPTY_H

#include <stdbool.h>

// is_empty reports whether the string s has no characters. s must not be NULL.
bool is_empty(const char *s);

// is_blank reports whether each byte of s is ASCII white space.
bool is_blank(const char *s);

#endif
