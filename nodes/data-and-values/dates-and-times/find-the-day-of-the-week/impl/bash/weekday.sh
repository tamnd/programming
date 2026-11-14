#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-the-day-of-the-week/.

# day_of_week YEAR MONTH DAY
# Prints the ISO 8601 day of the week: 1 for Monday to 7 for Sunday. It needs
# GNU date. %u is the ISO number, and %w counts from 0 for Sunday. The -u
# option reads the date as midnight UTC.
day_of_week() {
  date -u -d "$(printf '%04d-%02d-%02d' "$1" "$2" "$3")" +%u
}

# iso_week YEAR MONTH DAY
# Prints the ISO 8601 week-based year and week number, such as "2025 1".
# %G is the week-based year and %V is the week number. %Y is the calendar
# year, and it is different for some days at the start and end of a year.
iso_week() {
  local out
  out=$(date -u -d "$(printf '%04d-%02d-%02d' "$1" "$2" "$3")" '+%G %V') || return 1
  local week_year=${out% *} week=${out#* }
  echo "$(( 10#$week_year )) $(( 10#$week ))"
}
