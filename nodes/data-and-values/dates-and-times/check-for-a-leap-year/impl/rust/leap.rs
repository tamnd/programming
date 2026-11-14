// SPDX-License-Identifier: MIT
// Part of https://programming.tamnd.com/check-for-a-leap-year/.

/// Returns true for a leap year of the proleptic Gregorian calendar.
pub fn is_leap_year(year: i64) -> bool {
    year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)
}
