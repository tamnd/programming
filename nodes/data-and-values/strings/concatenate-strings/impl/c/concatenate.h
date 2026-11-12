// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/concatenate-strings/.

#ifndef CONCATENATE_H
#define CONCATENATE_H

#include <stdint.h>

// concat returns a new null-terminated string that holds the bytes of a
// followed by the bytes of b, or NULL if there is no memory. The caller frees
// the result.
char *concat(const char *a, const char *b);

// concat_number returns a new null-terminated string that holds s followed by
// the decimal form of n, or NULL if there is no memory. The caller frees the
// result.
char *concat_number(const char *s, int32_t n);

#endif
