// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/find-the-length-of-a-string/.

#ifndef STRING_LENGTH_H
#define STRING_LENGTH_H

#include <stddef.h>

// lengths writes three lengths of the valid UTF-8 bytes s[0..n) to out: the
// number of bytes, the number of UTF-16 code units and the number of code
// points. The bytes can include a null character.
void lengths(const char *s, size_t n, size_t out[3]);

#endif
