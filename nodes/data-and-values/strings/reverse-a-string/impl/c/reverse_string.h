// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/reverse-a-string/.

#ifndef REVERSE_STRING_H
#define REVERSE_STRING_H

#include <stddef.h>

// reverse_code_points writes the code points of the UTF-8 bytes s[0..n) in
// the opposite order to out, which has room for n bytes, and returns 0.
// Each code point keeps its bytes in their order. If s is not valid UTF-8,
// it returns -1 and sets errno to EILSEQ, and out holds no result.
int reverse_code_points(const char *s, size_t n, char *out);

#endif
