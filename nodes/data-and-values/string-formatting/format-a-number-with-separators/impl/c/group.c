// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-separators/.

#include "group.h"

#include <stdio.h>
#include <string.h>

void group_thousands(char *out, long long n) {
    char digits[24];
    snprintf(digits, sizeof digits, "%lld", n);
    const char *d = digits;
    if (*d == '-') {
        *out++ = *d++;
    }
    size_t len = strlen(d);
    for (size_t i = 0; i < len; i++) {
        if (i > 0 && (len - i) % 3 == 0) {
            *out++ = ',';
        }
        *out++ = d[i];
    }
    *out = '\0';
}
