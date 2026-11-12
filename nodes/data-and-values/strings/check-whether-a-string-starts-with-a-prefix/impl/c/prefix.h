// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-string-starts-with-a-prefix/.

#ifndef PREFIX_H
#define PREFIX_H

#include <stdbool.h>

// starts_with reports whether the UTF-8 string s starts with prefix.
bool starts_with(const char *s, const char *prefix);

// ends_with reports whether the UTF-8 string s ends with suffix.
bool ends_with(const char *s, const char *suffix);

#endif
