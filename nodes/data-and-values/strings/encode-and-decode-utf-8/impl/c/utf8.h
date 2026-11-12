// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/encode-and-decode-utf-8/.
#ifndef UTF8_H
#define UTF8_H

#include <stddef.h>
#include <stdint.h>

// encode_utf8 writes the 1 to 4 UTF-8 bytes of the code point cp to out, and
// returns the number of bytes. cp must be a Unicode scalar value.
size_t encode_utf8(uint32_t cp, unsigned char out[4]);

// decode_utf8 returns the code point of the one UTF-8 sequence in the n bytes
// of b, or -1 if the bytes are not exactly one well-formed sequence.
long decode_utf8(const unsigned char *b, size_t n);

#endif
