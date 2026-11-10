// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/base64-encode-and-decode/.

#include "b64.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static const char STANDARD[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char URL[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

char *b64_encode(const unsigned char *data, size_t len, enum b64_alphabet alphabet) {
    const char *chars = alphabet == B64_URL ? URL : STANDARD;
    // Each group of 3 bytes gives 4 characters. Check that the size fits.
    if (len / 3 >= (SIZE_MAX - 5) / 4) {
        return NULL;
    }
    char *text = malloc(len / 3 * 4 + 5);
    if (text == NULL) {
        return NULL;
    }
    char *p = text;
    for (size_t i = 0; i < len; i += 3) {
        size_t n = len - i < 3 ? len - i : 3;
        // Put the 1 to 3 bytes in the top 24 bits of v, with zero bits after.
        uint32_t v = (uint32_t)data[i] << 16;
        if (n > 1) {
            v |= (uint32_t)data[i + 1] << 8;
        }
        if (n > 2) {
            v |= data[i + 2];
        }
        for (size_t k = 0; k < 4; k++) {
            if (k <= n) {
                *p++ = chars[(v >> (18 - 6 * k)) & 63];
            } else if (alphabet == B64_STANDARD) {
                *p++ = '=';
            }
        }
    }
    *p = '\0';
    return text;
}

enum b64_status b64_decode(const char *text, enum b64_alphabet alphabet, unsigned char **data,
                           size_t *len) {
    const char *chars = alphabet == B64_URL ? URL : STANDARD;
    size_t n = strlen(text);
    if (alphabet == B64_STANDARD) {
        if (n % 4 != 0) {
            return B64_INVALID;
        }
        // Remove at most two '=' at the end. Any other '=' is not in the
        // alphabet, so the loop below rejects it.
        for (int k = 0; k < 2 && n > 0 && text[n - 1] == '='; k++) {
            n--;
        }
    }
    if (n % 4 == 1) {
        return B64_INVALID;
    }
    unsigned char *out = malloc(n / 4 * 3 + 3);
    if (out == NULL) {
        return B64_NO_MEMORY;
    }
    size_t m = 0;
    for (size_t i = 0; i < n; i += 4) {
        size_t count = n - i < 4 ? n - i : 4;
        uint32_t v = 0;
        for (size_t k = 0; k < count; k++) {
            const char *c = memchr(chars, text[i + k], 64);
            if (c == NULL) {
                free(out);
                return B64_INVALID;
            }
            v = (v << 6) | (uint32_t)(c - chars);
        }
        // 4, 3 or 2 characters hold 3, 2 or 1 bytes and 0, 2 or 4 pad bits.
        size_t bytes = count * 6 / 8;
        size_t pad_bits = count * 6 - bytes * 8;
        if ((v & ((UINT32_C(1) << pad_bits) - 1)) != 0) {
            free(out);
            return B64_INVALID;
        }
        v >>= pad_bits;
        for (size_t k = bytes; k-- > 0;) {
            out[m++] = (unsigned char)(v >> (8 * k));
        }
    }
    *data = out;
    *len = m;
    return B64_OK;
}
