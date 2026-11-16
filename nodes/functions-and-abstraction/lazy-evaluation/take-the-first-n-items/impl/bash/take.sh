#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/take-the-first-n-items/.

# take N ITEM...
# Prints the first N items, one on each line. Fewer items give all of them.
take() {
  local n=$1
  shift
  # ${@:1:n} is n arguments from the first one; it stops at the last argument.
  (( n > 0 && $# > 0 )) && printf '%s\n' "${@:1:n}"
  return 0
}

# naturals START
# Prints START, START + 1, ... without end. It stops when its output closes:
# a write to a closed pipe kills it or makes printf fail.
naturals() {
  local i
  for ((i = $1; ; i++)); do
    printf '%d\n' "$i" || return 0
  done
}

# take_naturals START N
# Prints the first N items of the infinite sequence. head reads N lines and
# exits, and then naturals stops.
take_naturals() {
  (( $2 > 0 )) || return 0
  naturals "$1" | head -n "$2"
}
