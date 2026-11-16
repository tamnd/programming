// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-variable/.

#include "total.h"

long long total(const long long *values, size_t n) {
    // A local variable has no start value in C, so give it one.
    long long result = 0;
    for (size_t i = 0; i < n; i++) {
        result = result + values[i];
    }
    return result;
}
