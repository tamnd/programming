#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

# permutations ITEM...
# Prints each order of the items on one line, with the items separated by
# spaces, in the lexicographic order of the positions. With no arguments, it
# prints one empty line, the one empty order.
permutations() {
  _permutations_visit "" "$@"
}

# _permutations_visit PREFIX ITEM...
# Prints PREFIX followed by each order of the items.
_permutations_visit() {
  # local keeps the variables of each call apart in the recursion.
  local prefix=$1 i
  shift
  if (($# == 0)); then
    printf '%s\n' "$prefix"
    return
  fi
  local -a rest=("$@")
  for ((i = 0; i < ${#rest[@]}; i++)); do
    _permutations_visit "${prefix:+$prefix }${rest[i]}" "${rest[@]:0:i}" "${rest[@]:i+1}"
  done
}
