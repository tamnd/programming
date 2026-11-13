#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

# combinations K ITEM...
# Prints each choice of K items on one line, with the items separated by
# spaces, in the lexicographic order of the positions. K = 0 prints one empty
# line, the one empty choice. K more than the number of items prints nothing.
combinations() {
  local k=$1
  shift
  _combinations_visit "$k" "" "$@"
}

# _combinations_visit K PREFIX ITEM...
# Prints PREFIX followed by each choice of K of the items.
_combinations_visit() {
  # local keeps the variables of each call apart in the recursion.
  local k=$1 prefix=$2 i
  shift 2
  if ((k == 0)); then
    printf '%s\n' "$prefix"
    return
  fi
  local -a rest=("$@")
  # Stop when too few items are left to fill the choice.
  for ((i = 0; i + k <= ${#rest[@]}; i++)); do
    _combinations_visit "$((k - 1))" "${prefix:+$prefix }${rest[i]}" "${rest[@]:i+1}"
  done
}
