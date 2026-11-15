// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/write-a-while-loop/.

#include "halvings.h"

#include <stddef.h>

size_t halvings(int n, int out[32]) {
    size_t count = 0;
    while (n > 0) {
        out[count++] = n;
        n /= 2;
    }
    return count;
}
