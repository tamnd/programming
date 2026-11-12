// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-is-empty/.

#include "empty.h"

#include <string.h>

bool is_empty(const char *s) {
    // The first byte is the null character only in the empty string.
    return s[0] == '\0';
}

bool is_blank(const char *s) {
    // strspn gives the length of the first part of s that has only these bytes.
    return s[strspn(s, " \t\n\v\f\r")] == '\0';
}
