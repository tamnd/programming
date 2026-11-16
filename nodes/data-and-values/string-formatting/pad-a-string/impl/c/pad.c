// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pad-a-string/.

#include "pad.h"

#include <string.h>

void pad(const char *s, size_t width, char fill, char *left, char *right) {
    size_t len = strlen(s);
    size_t extra = width > len ? width - len : 0;
    memset(left, fill, extra);
    memcpy(left + extra, s, len + 1);
    memcpy(right, s, len);
    memset(right + len, fill, extra);
    right[len + extra] = '\0';
}
