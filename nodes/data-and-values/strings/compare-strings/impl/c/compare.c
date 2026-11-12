// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/compare-strings/.

#include "compare.h"

#include <string.h>

int compare(const char *a, const char *b) {
    // strcmp compares the bytes as unsigned char, and the order of UTF-8 bytes
    // is the order of the code points. The sign is all that strcmp promises.
    int r = strcmp(a, b);
    return (r > 0) - (r < 0);
}

bool equals(const char *a, const char *b) {
    return strcmp(a, b) == 0;
}
