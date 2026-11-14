#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/format-a-duration/.

# format_duration SECONDS
# Prints the duration as HH:MM:SS. The hours do not wrap at 24. Bash
# arithmetic uses 64-bit integers.
format_duration() {
  local s=$1 sign=
  if ((s < 0)); then
    sign=-
    s=$((-s))
  fi
  printf '%s%02d:%02d:%02d\n' "$sign" "$((s / 3600))" "$((s % 3600 / 60))" "$((s % 60))"
}
