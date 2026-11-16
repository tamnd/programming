// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/print-formatted-output/.

#include "row.h"

#include <stdio.h>

int format_row(char *out, size_t size, const char *name, long long qty, double price) {
    return snprintf(out, size, "%-10s|%5lld|%8.2f", name, qty, price);
}
