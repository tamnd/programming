#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-over-nested-loops/.

# find_in_grid TARGET ROW...
# Each ROW is one string of numbers split by spaces. Prints the row and the
# column of the first cell that equals TARGET, or "-1 -1".
find_in_grid() {
  local target=$1 row_text r=0 c x found="-1 -1"
  shift
  local -a cells
  for row_text in "$@"; do
    read -r -a cells <<<"$row_text"
    c=0
    for x in "${cells[@]}"; do
      if ((x == target)); then
        found="$r $c"
        # break 2 leaves both loops. break would leave only the inner one.
        break 2
      fi
      ((c += 1))
    done
    ((r += 1))
  done
  printf '%s\n' "$found"
}
