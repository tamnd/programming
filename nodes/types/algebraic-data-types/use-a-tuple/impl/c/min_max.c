// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/use-a-tuple/.

#include "min_max.h"

struct min_max min_max(const int32_t *numbers, size_t n) {
    struct min_max r = {numbers[0], numbers[0]};
    for (size_t i = 1; i < n; i++) {
        if (numbers[i] < r.min) {
            r.min = numbers[i];
        }
        if (numbers[i] > r.max) {
            r.max = numbers[i];
        }
    }
    return r; // a struct is returned by value
}
