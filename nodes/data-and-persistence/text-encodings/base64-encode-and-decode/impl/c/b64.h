// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/base64-encode-and-decode/.

#ifndef B64_H
#define B64_H

#include <stddef.h>

// B64_STANDARD is RFC 4648 section 4, padded with '='. B64_URL is section 5,
// with no padding.
enum b64_alphabet { B64_STANDARD, B64_URL };

enum b64_status { B64_OK, B64_INVALID, B64_NO_MEMORY };

// Returns the Base64 text of the len bytes at data as a new string, or NULL
// when there is no memory. The caller frees the string.
char *b64_encode(const unsigned char *data, size_t len, enum b64_alphabet alphabet);

// Decodes text, which must be exactly what b64_encode gives for some bytes.
// On B64_OK, *data is a new buffer of *len bytes that the caller frees. On an
// error, *data and *len do not change.
enum b64_status b64_decode(const char *text, enum b64_alphabet alphabet, unsigned char **data,
                           size_t *len);

#endif
