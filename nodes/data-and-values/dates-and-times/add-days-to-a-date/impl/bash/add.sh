#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/add-days-to-a-date/.

# days_in_month YEAR MONTH prints the number of days in the month.
days_in_month() {
  local year=$1 month=$2
  case $month in
    2)
      if (( year % 4 == 0 && (year % 100 != 0 || year % 400 == 0) )); then echo 29; else echo 28; fi
      ;;
    4 | 6 | 9 | 11) echo 30 ;;
    *) echo 31 ;;
  esac
}

# add_to_date YEAR MONTH DAY YEARS MONTHS DAYS
# Adds years and months at once, then days, and prints the date as YYYY-MM-DD.
# A day after the end of the new month becomes its last day. The months use
# shell arithmetic, because GNU date moves such a day into the next month.
# The days use UTC seconds, so no time zone change can move the result.
add_to_date() {
  local year=$1 month=$2 day=$3 years=$4 months=$5 days=$6
  local total=$(( year * 12 + month - 1 + years * 12 + months ))
  local new_year=$(( total / 12 )) new_month=$(( total % 12 + 1 ))
  local last
  last=$(days_in_month "$new_year" "$new_month")
  (( day > last )) && day=$last
  local start
  start=$(date -u -d "$(printf '%04d-%02d-%02d' "$new_year" "$new_month" "$day")" +%s) || return 1
  date -u -d "@$(( start + days * 86400 ))" +%F
}
