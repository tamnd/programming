#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/append-to-a-list/.

# insert_at NAME INDEX VALUE
# Prints the items of the array NAME with VALUE at position INDEX, one on each
# line. INDEX equal to the number of items appends. It returns 1 when INDEX is
# out of range. The name is passed, because a function cannot take an array
# as one argument.
insert_at() {
  local -n _insert_at_items=$1
  local index=$2
  if ! [[ $index =~ ^[0-9]+$ ]] || ((index > ${#_insert_at_items[@]})); then
    return 1
  fi
  # ${a[@]:i:n} takes n items from position i. The quotes keep each item one word.
  local -a _insert_at_out=("${_insert_at_items[@]:0:index}" "$3" "${_insert_at_items[@]:index}")
  printf '%s\n' "${_insert_at_out[@]}"
}
