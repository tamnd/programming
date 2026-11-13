// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/flatten-a-nested-list/.

#include "flatten.h"

#include <string.h>

int flatten(const struct int_list *lists, size_t n, int32_t *out, size_t cap, size_t *len) {
    size_t total = 0;
    for (size_t i = 0; i < n; i++) {
        // total + lists[i].len could wrap around, so compare with cap - total.
        if (lists[i].len > cap - total) {
            return -1;
        }
        total += lists[i].len;
    }
    size_t at = 0;
    for (size_t i = 0; i < n; i++) {
        if (lists[i].len > 0) {
            memcpy(out + at, lists[i].items, lists[i].len * sizeof *out);
            at += lists[i].len;
        }
    }
    *len = total;
    return 0;
}
