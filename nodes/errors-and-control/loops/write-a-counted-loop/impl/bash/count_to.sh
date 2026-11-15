#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-counted-loop/.

# count_to N
# Prints the numbers 1 to N, one on each line.
count_to() {
  local i
  # {1..$1} would not work: brace expansion runs before $1 expands.
  for ((i = 1; i <= $1; i++)); do
    printf '%s\n' "$i"
  done
}
