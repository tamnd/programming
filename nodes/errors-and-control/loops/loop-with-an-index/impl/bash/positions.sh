#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-with-an-index/.

# positions TARGET ITEM...
# Prints the 0-based index of each item that equals TARGET, one on each line.
positions() {
  local target=$1 i
  shift
  local -a items=("$@")
  # ${!items[@]} expands to the indexes of the array.
  for i in "${!items[@]}"; do
    if [[ ${items[i]} == "$target" ]]; then
      printf '%s\n' "$i"
    fi
  done
}
