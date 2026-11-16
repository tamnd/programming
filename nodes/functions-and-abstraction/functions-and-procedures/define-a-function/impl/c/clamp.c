// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/define-a-function/.

#include "clamp.h"

int clamp(int value, int low, int high) {
    if (value < low) {
        return low;
    }
    if (value > high) {
        return high;
    }
    return value;
}
