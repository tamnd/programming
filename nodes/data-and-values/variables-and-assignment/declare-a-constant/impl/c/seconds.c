// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

#include "seconds.h"

long long seconds_in(long long days) {
    // SECONDS_PER_DAY has the type int, and C converts it to long long
    // before the multiplication.
    return days * SECONDS_PER_DAY;
}
