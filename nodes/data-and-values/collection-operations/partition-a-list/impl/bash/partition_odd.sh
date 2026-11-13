#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/partition-a-list/.

# partition_odd ITEM...
# Prints two lines: the odd items, then the even items, each in input order
# and separated by spaces. A line is empty if it has no items.
partition_odd() {
  local -a odd=() even=()
  local x
  for x in "$@"; do
    if ((x % 2 != 0)); then
      odd+=("$x")
    else
      even+=("$x")
    fi
  done
  printf '%s\n' "${odd[*]}" "${even[*]}"
}
