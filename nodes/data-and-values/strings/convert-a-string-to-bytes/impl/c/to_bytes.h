// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.
#ifndef TO_BYTES_H
#define TO_BYTES_H

#include <stddef.h>

// to_bytes returns a new array of the *n bytes of the UTF-8 string s, without
// the terminating NUL, or NULL if there is no memory. Free it with free.
unsigned char *to_bytes(const char *s, size_t *n);

#endif
