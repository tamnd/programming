#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-list-contains-a-value/.

# contains VALUE ITEM...
# Returns 0 if one of the items is equal to VALUE as a string, and 1 if not.
contains() {
  local value=$1 item
  shift
  for item in "$@"; do
    # The quotes make the right side a string, not a pattern.
    if [[ $item == "$value" ]]; then
      return 0
    fi
  done
  return 1
}
