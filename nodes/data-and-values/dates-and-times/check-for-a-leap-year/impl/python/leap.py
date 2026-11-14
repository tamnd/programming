# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-for-a-leap-year/.

import calendar


def is_leap_year(year: int) -> bool:
    """Return True for a leap year of the proleptic Gregorian calendar."""
    return year % 4 == 0 and (year % 100 != 0 or year % 400 == 0)


def is_leap_year_library(year: int) -> bool:
    """Return the same result with the standard library."""
    return calendar.isleap(year)
