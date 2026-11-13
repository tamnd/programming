#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/search-a-sorted-list/.

# index_of TARGET ITEM...
# Prints the first index of TARGET in the sorted integer items, or -1.
index_of() {
  local target=$1
  shift
  local -a items=("$@")
  local lo=0 hi=${#items[@]} mid
  while ((lo < hi)); do
    mid=$((lo + (hi - lo) / 2))
    # (( )) compares numbers. [[ a < b ]] would compare text.
    if ((items[mid] < target)); then
      lo=$((mid + 1))
    else
      hi=$mid
    fi
  done
  if ((lo < ${#items[@]})) && ((items[lo] == target)); then
    printf '%s\n' "$lo"
  else
    printf '%s\n' -1
  fi
}
