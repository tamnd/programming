// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

#include "prefix.h"

#include <string.h>

bool starts_with(const char *s, const char *prefix) {
    return strncmp(s, prefix, strlen(prefix)) == 0;
}

bool ends_with(const char *s, const char *suffix) {
    size_t n = strlen(s);
    size_t m = strlen(suffix);
    // A suffix longer than s cannot match, and the check keeps s + n - m in s.
    return m <= n && memcmp(s + n - m, suffix, m) == 0;
}
