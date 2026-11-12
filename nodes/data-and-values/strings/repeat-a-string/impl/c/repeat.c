// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/repeat-a-string/.

#include "repeat.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

char *repeat(const char *s, size_t n) {
    size_t len = strlen(s);
    // len * n + 1 must not overflow size_t.
    if (len != 0 && n > (SIZE_MAX - 1) / len) {
        return NULL;
    }
    char *out = malloc(len * n + 1);
    if (out == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < n; i++) {
        memcpy(out + i * len, s, len);
    }
    out[len * n] = '\0';
    return out;
}
