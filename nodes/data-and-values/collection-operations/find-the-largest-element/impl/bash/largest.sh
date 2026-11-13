#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-the-largest-element/.

# largest ITEM...
# Prints the largest of the integer arguments. Give 1 argument or more.
largest() {
  local best=$1 x
  for x in "$@"; do
    # (( )) compares numbers. [[ $x > $best ]] would compare text.
    if ((x > best)); then
      best=$x
    fi
  done
  printf '%s\n' "$best"
}
