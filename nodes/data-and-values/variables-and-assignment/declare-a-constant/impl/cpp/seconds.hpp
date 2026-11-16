// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/declare-a-constant/.

#ifndef SECONDS_HPP
#define SECONDS_HPP

// constexpr asks the compiler to compute each value, and static_assert
// checks a value at compile time.
inline constexpr long long seconds_per_minute = 60;
inline constexpr long long minutes_per_hour = 60;
inline constexpr long long hours_per_day = 24;
inline constexpr long long seconds_per_day = seconds_per_minute * minutes_per_hour * hours_per_day;

static_assert(seconds_per_day == 86400);

// Returns the number of seconds in days days of 24 hours.
constexpr long long seconds_in(long long days) {
    return days * seconds_per_day;
}

#endif
