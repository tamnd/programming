#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/rotate-a-list/.

# rotate_left K ITEM...
# Prints the items, one on each line, with the first K items moved to the end.
rotate_left() {
  local k=$1
  shift
  local -a items=("$@")
  local n=${#items[@]}
  if ((n == 0)); then
    return 0
  fi
  local s=$((k % n))
  printf '%s\n' "${items[@]:s}" "${items[@]:0:s}"
}
