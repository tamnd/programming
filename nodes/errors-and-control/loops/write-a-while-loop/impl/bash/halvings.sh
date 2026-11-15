#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-while-loop/.

# halvings N
# Prints N, N / 2, N / 4, ... while the value is greater than 0, one on each line.
halvings() {
  local n=$1
  while ((n > 0)); do
    printf '%s\n' "$n"
    # Shell arithmetic has only integers, so / drops the remainder.
    ((n /= 2))
  done
}
