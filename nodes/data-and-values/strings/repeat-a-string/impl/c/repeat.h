// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

#ifndef REPEAT_H
#define REPEAT_H

#include <stddef.h>

// repeat returns a new string with n copies of s, or NULL if there is no
// memory or the length overflows. The caller frees the result.
char *repeat(const char *s, size_t n);

#endif
