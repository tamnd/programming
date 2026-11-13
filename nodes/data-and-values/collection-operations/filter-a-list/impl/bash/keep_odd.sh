#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/filter-a-list/.

# keep_odd ITEM...
# Prints the odd integer arguments, one on each line, in their order.
keep_odd() {
  local x
  for x in "$@"; do
    # Use != 0, not == 1: the remainder of a negative item can be negative.
    if ((x % 2 != 0)); then
      printf '%s\n' "$x"
    fi
  done
}
