#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/iterate-in-chunks/.

# chunks SIZE ITEM...
# Prints one line for each chunk of SIZE items, with the items separated by
# spaces. The last chunk can be shorter. SIZE must be at least 1.
chunks() {
  local size=$1 i
  shift
  local -a items=("$@")
  for ((i = 0; i < ${#items[@]}; i += size)); do
    # ${items[@]:i:size} is size items from index i, not a part of a string.
    printf '%s\n' "${items[*]:i:size}"
  done
}
