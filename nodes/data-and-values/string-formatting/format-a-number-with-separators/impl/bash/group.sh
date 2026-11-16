#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/format-a-number-with-separators/.

# group_thousands n: print n with a comma between each group of 3 digits.
# Each pass of the loop puts one comma before the last 3 digits that have
# no comma yet. The number stays text, so the shell does no arithmetic.
group_thousands() {
  local s=$1 sign=''
  if [[ $s == -* ]]; then
    sign=-
    s=${s#-}
  fi
  while [[ $s =~ ^([0-9]+)([0-9]{3})(,.*)?$ ]]; do
    s="${BASH_REMATCH[1]},${BASH_REMATCH[2]}${BASH_REMATCH[3]}"
  done
  printf '%s\n' "$sign$s"
}
