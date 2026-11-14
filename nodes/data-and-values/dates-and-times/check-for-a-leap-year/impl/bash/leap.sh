#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-for-a-leap-year/.

# is_leap_year YEAR
# Returns status 0 for a leap year of the proleptic Gregorian calendar and
# status 1 otherwise, so it can be used in an if statement.
is_leap_year() {
  local year=$1
  (( year % 4 == 0 && (year % 100 != 0 || year % 400 == 0) ))
}
