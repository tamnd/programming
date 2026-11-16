#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-switch-or-match/.

# days_in_month MONTH
# Prints the number of days in a month of a common year, or 0 for a bad month.
days_in_month() {
  case $1 in
    2)
      printf '%s\n' 28
      ;;
    4 | 6 | 9 | 11)
      printf '%s\n' 30
      ;;
    1 | 3 | 5 | 7 | 8 | 10 | 12)
      printf '%s\n' 31
      ;;
    *)
      printf '%s\n' 0
      ;;
  esac
}
