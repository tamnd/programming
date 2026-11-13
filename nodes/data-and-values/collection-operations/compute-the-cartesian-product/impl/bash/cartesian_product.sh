#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-cartesian-product/.

# cartesian_product LIST...
# Each argument is one list, with the items separated by spaces. Prints each
# tuple that takes one item from each list on one line, with the items
# separated by spaces, in nested-loop order. With no arguments, it prints one
# empty line, the one empty tuple.
cartesian_product() {
  local list t x
  local -a items next tuples=("")
  for list in "$@"; do
    read -r -a items <<< "$list"
    next=()
    for t in "${tuples[@]}"; do
      for x in "${items[@]}"; do
        next+=("${t:+$t }$x")
      done
    done
    tuples=("${next[@]}")
  done
  if ((${#tuples[@]} > 0)); then
    printf '%s\n' "${tuples[@]}"
  fi
}
