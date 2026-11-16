// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/interpolate-values-into-a-string/.

#ifndef ORDER_H
#define ORDER_H

#include <stddef.h>

// Writes "Order {name}: count item(s), total count*price" into out. Returns
// the length of the full text, as snprintf does. If the result is size or
// more, the text was cut to fit the buffer.
int describe_order(char *out, size_t size, const char *name, long long count, double price);

#endif
