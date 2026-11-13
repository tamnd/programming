// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

#include "contains.h"

#include <stdbool.h>
#include <stddef.h>

bool contains(const int *items, size_t n, int value) {
    for (size_t i = 0; i < n; i++) {
        if (items[i] == value) {
            return true;
        }
    }
    return false;
}
