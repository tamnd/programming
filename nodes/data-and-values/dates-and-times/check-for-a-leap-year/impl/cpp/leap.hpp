// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-for-a-leap-year/.

#ifndef IS_LEAP_YEAR_HPP
#define IS_LEAP_YEAR_HPP

#include <chrono>

// Returns true for a leap year of the proleptic Gregorian calendar.
// std::chrono::year holds the years -32767 to 32767.
inline bool is_leap_year(int y) {
    return std::chrono::year{y}.is_leap();
}

#endif
