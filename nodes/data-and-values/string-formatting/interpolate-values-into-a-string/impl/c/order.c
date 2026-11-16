// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/interpolate-values-into-a-string/.

#include "order.h"

#include <stdio.h>

int describe_order(char *out, size_t size, const char *name, long long count, double price) {
    const char *ending = count == 1 ? "" : "s";
    return snprintf(out, size, "Order {%s}: %lld item%s, total %.2f", name, count, ending, (double)count * price);
}
