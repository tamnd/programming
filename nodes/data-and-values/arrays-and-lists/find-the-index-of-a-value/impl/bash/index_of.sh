#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-the-index-of-a-value/.

# index_of VALUE ITEM...
# Prints the first index of VALUE among the items, counted from 0, and
# returns 0. Prints nothing and returns 1 if VALUE is not there. The index is
# printed, not returned, because a return status is only 0 to 255.
index_of() {
  local value=$1 i=0 item
  shift
  for item in "$@"; do
    if [[ $item == "$value" ]]; then
      printf '%s\n' "$i"
      return 0
    fi
    i=$((i + 1))
  done
  return 1
}
