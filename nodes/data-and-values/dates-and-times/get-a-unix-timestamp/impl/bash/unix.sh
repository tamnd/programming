#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-a-unix-timestamp/.

# unix_timestamp YEAR MONTH DAY HOUR MINUTE SECOND OFFSET_MINUTES
# Prints the seconds since 1970-01-01T00:00:00Z of a local time with the given
# UTC offset. It needs GNU date, which reads a date with an offset after -d.
unix_timestamp() {
  local year=$1 month=$2 day=$3 hour=$4 minute=$5 second=$6 offset=$7
  local sign=+
  if (( offset < 0 )); then
    sign=-
    offset=$(( -offset ))
  fi
  local text
  printf -v text '%04d-%02d-%02dT%02d:%02d:%02d%s%02d:%02d' \
    "$year" "$month" "$day" "$hour" "$minute" "$second" "$sign" $(( offset / 60 )) $(( offset % 60 ))
  date -u -d "$text" +%s
}

# now_unix_seconds prints the current Unix time in whole seconds.
now_unix_seconds() {
  printf '%s\n' "$EPOCHSECONDS"
}

# now_unix_millis prints the current Unix time in whole milliseconds.
now_unix_millis() {
  local micros=${EPOCHREALTIME//[!0-9]/}
  printf '%s\n' $(( micros / 1000 ))
}
