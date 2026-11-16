#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/declare-a-constant/.

# readonly marks the names, so a later assignment or unset fails.
readonly SECONDS_PER_MINUTE=60 MINUTES_PER_HOUR=60 HOURS_PER_DAY=24
readonly SECONDS_PER_DAY=$((SECONDS_PER_MINUTE * MINUTES_PER_HOUR * HOURS_PER_DAY))

# seconds_in days: print the number of seconds in days days of 24 hours.
seconds_in() {
  printf '%d\n' "$(($1 * SECONDS_PER_DAY))"
}
