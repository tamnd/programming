// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/pattern-match-on-a-value/.

#include "classify.h"

const char *classify(int x, int y) {
    // C has no pattern matching, so an if chain tests the cases in order.
    if (x == 0 && y == 0) {
        return "origin";
    }
    if (y == 0) {
        return "x-axis";
    }
    if (x == 0) {
        return "y-axis";
    }
    if (x == y) {
        return "diagonal";
    }
    return "other";
}
