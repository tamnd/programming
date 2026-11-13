#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/slice-a-list/.

# slice_list START STOP ITEM...
# Prints the items from position START up to, but not including, position
# STOP, one on each line. It returns 1 when the range is out of bounds.
slice_list() {
  local start=$1 stop=$2
  shift 2
  local -a items=("$@")
  if ! [[ $start =~ ^[0-9]+$ && $stop =~ ^[0-9]+$ ]] || ((start > stop || stop > ${#items[@]})); then
    return 1
  fi
  # ${items[@]:offset:length} takes a length, not a stop position.
  local -a part=("${items[@]:start:stop-start}")
  if ((${#part[@]} > 0)); then printf '%s\n' "${part[@]}"; fi
}
