// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/format-a-number-with-fixed-decimals/.

#include "fixed.h"

#include <stdio.h>

int fixed(char *out, size_t size, double x, int digits) {
    return snprintf(out, size, "%.*f", digits, x);
}
