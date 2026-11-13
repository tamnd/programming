#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/group-items-by-a-key/.

# group_by_length WORD...
# Prints one line "LENGTH WORD..." for each length, in ascending order of the
# length. The words of a line are in input order.
group_by_length() {
  local -A groups=()
  local w
  for w in "$@"; do
    groups[${#w}]+=" $w"
  done
  # sort -n puts the lines in ascending order of the length.
  for w in "${!groups[@]}"; do
    printf '%s%s\n' "$w" "${groups[$w]}"
  done | sort -n
}
