#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-a-list/.

# sort_ints ITEM...
# Prints the integer arguments in ascending order, one on each line.
sort_ints() {
  if (($# == 0)); then
    return 0
  fi
  # -n compares the numbers. Without it, sort compares text, and 10 comes
  # before 9.
  printf '%s\n' "$@" | LC_ALL=C sort -n
}
