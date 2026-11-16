// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

#ifndef SECONDS_H
#define SECONDS_H

// The names of an enum are integer constant expressions, so one constant
// can use the others. A static const variable cannot do that in C before
// C23.
enum {
    SECONDS_PER_MINUTE = 60,
    MINUTES_PER_HOUR = 60,
    HOURS_PER_DAY = 24,
    SECONDS_PER_DAY = SECONDS_PER_MINUTE * MINUTES_PER_HOUR * HOURS_PER_DAY,
};

// Returns the number of seconds in days days of 24 hours.
long long seconds_in(long long days);

#endif
