// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-for-a-leap-year/.

#include "leap.h"

// The % operator of C keeps the sign of the left operand, so -4 % 4 is 0
// and -1 % 4 is -1. A test for 0 is right for negative years too.
bool is_leap_year(int year) {
    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}
