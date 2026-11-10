#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/parse-and-format-a-date/.

# The date command differs between systems: GNU date has -d, and BSD date has
# -j and -f. So this file parses the text with [[ =~ ]] and counts days with
# the integer arithmetic of the shell, which is the same on every system.

# The date-time of RFC 3339 section 5.6, with a fraction of 1 to 3 digits.
# The groups: 1 year, 2 month, 3 day, 4 hour, 5 minute, 6 second, 7 fraction,
# 8 Z or offset, 9 offset sign, 10 offset hours, 11 offset minutes.
TO_UTC_RFC3339='^([0-9]{4})-([0-9]{2})-([0-9]{2})[Tt]([01][0-9]|2[0-3]):([0-5][0-9]):([0-5][0-9])(\.[0-9]{1,3})?([Zz]|([+-])([01][0-9]|2[0-3]):([0-5][0-9]))$'

# _days_from_civil YEAR MONTH DAY
# Sets _days to the days from 1970-01-01 to the date (Howard Hinnant). A year
# starts on 1 March here, so the leap day is the last day. The shell rounds a
# division toward zero, so the year must be 1 or more.
_days_from_civil() {
  local y=$1 m=$2 d=$3 era yoe doy doe
  ((y -= m <= 2, era = y / 400, yoe = y - era * 400))
  ((doy = (153 * (m > 2 ? m - 3 : m + 9) + 2) / 5 + d - 1))
  ((doe = yoe * 365 + yoe / 4 - yoe / 100 + doy))
  ((_days = era * 146097 + doe - 719468))
}

# _civil_from_days DAYS
# Sets _year, _month and _day to the date DAYS days after 1970-01-01.
_civil_from_days() {
  local z=$(($1 + 719468)) era doe yoe doy mp
  ((era = z / 146097, doe = z - era * 146097))
  ((yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365))
  ((doy = doe - (365 * yoe + yoe / 4 - yoe / 100), mp = (5 * doy + 2) / 153))
  ((_day = doy - (153 * mp + 2) / 5 + 1, _month = mp < 10 ? mp + 3 : mp - 9))
  ((_year = yoe + era * 400 + (_month <= 2)))
}

# to_utc TIMESTAMP
# Prints the instant of an RFC 3339 timestamp in UTC, as
# YYYY-MM-DDTHH:MM:SS.sssZ. Prints an error and returns 1 when TIMESTAMP is
# not an RFC 3339 date-time with an offset, or when its date is not in the
# calendar.
to_utc() {
  local ts=$1 year month day hour minute second fraction offset=0 max total
  local _days _year _month _day
  if ! [[ $ts =~ $TO_UTC_RFC3339 ]]; then
    printf 'to_utc: not an RFC 3339 date-time: "%s"\n' "$ts" >&2
    return 1
  fi
  # 10# reads "08" as eight. With no base, a leading 0 means octal.
  year=$((10#${BASH_REMATCH[1]})) month=$((10#${BASH_REMATCH[2]}))
  day=$((10#${BASH_REMATCH[3]})) hour=$((10#${BASH_REMATCH[4]}))
  minute=$((10#${BASH_REMATCH[5]})) second=$((10#${BASH_REMATCH[6]}))
  # The fraction: 1 to 3 digits, padded to milliseconds.
  fraction=${BASH_REMATCH[7]#.}000
  fraction=$((10#${fraction:0:3}))
  if [[ -n ${BASH_REMATCH[9]} ]]; then
    offset=$((10#${BASH_REMATCH[10]} * 3600 + 10#${BASH_REMATCH[11]} * 60))
    if [[ ${BASH_REMATCH[9]} == - ]]; then offset=$((-offset)); fi
  fi
  case $month in
    1 | 3 | 5 | 7 | 8 | 10 | 12) max=31 ;;
    4 | 6 | 9 | 11) max=30 ;;
    2) max=$((year % 4 == 0 && (year % 100 != 0 || year % 400 == 0) ? 29 : 28)) ;;
    *) max=0 ;;
  esac
  if ((day < 1 || day > max)); then
    printf 'to_utc: not a date in the calendar: "%s"\n' "$ts" >&2
    return 1
  fi
  _days_from_civil "$year" "$month" "$day"
  ((total = (_days * 86400 + hour * 3600 + minute * 60 + second - offset) * 1000 + fraction))
  # Round the days down, also for an instant before 1970.
  ((_days = total / 86400000, total %= 86400000))
  if ((total < 0)); then ((total += 86400000, _days -= 1)); fi
  _civil_from_days "$_days"
  printf '%04d-%02d-%02dT%02d:%02d:%02d.%03dZ\n' "$_year" "$_month" "$_day" \
    $((total / 3600000)) $((total / 60000 % 60)) $((total / 1000 % 60)) $((total % 1000))
}
