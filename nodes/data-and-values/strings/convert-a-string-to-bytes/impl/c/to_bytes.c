// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/convert-a-string-to-bytes/.

#include "to_bytes.h"

#include <stdlib.h>
#include <string.h>

// A C string is already a sequence of bytes, so the function copies them as
// unsigned char. strlen counts bytes, not code points.
unsigned char *to_bytes(const char *s, size_t *n) {
    size_t len = strlen(s);
    // malloc(0) can return NULL, so the array has at least 1 byte.
    unsigned char *out = malloc(len > 0 ? len : 1);
    *n = 0;
    if (out == NULL) {
        return NULL;
    }
    memcpy(out, s, len);
    *n = len;
    return out;
}
