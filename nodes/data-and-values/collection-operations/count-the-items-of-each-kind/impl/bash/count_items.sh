#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

# count_items ITEM...
# Prints one line "ITEM COUNT" for each distinct item, in ascending order of
# the item. It prints nothing if there are no items.
count_items() {
  # -A makes an associative array. Without it, a key such as -1 is an index
  # that counts back from the end of the array.
  local -A counts=()
  local x
  for x in "$@"; do
    counts[$x]=$((${counts[$x]:-0} + 1))
  done
  # The order of the keys of an associative array is not specified.
  for x in "${!counts[@]}"; do
    printf '%s %s\n' "$x" "${counts[$x]}"
  done | sort -n
}
