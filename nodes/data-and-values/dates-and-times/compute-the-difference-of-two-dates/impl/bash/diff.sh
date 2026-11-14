#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

# days_between START_YEAR START_MONTH START_DAY END_YEAR END_MONTH END_DAY
# Prints the number of days from the start date to the end date. It needs GNU
# date. The -u option reads both dates as midnight UTC, so every day has
# 86400 seconds and the division is exact.
days_between() {
  local start end
  start=$(date -u -d "$(printf '%04d-%02d-%02d' "$1" "$2" "$3")" +%s) || return 1
  end=$(date -u -d "$(printf '%04d-%02d-%02d' "$4" "$5" "$6")" +%s) || return 1
  echo $(( (end - start) / 86400 ))
}
