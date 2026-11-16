#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compose-functions/.

inc() { echo $(($1 + 1)); }
dec() { echo $(($1 - 1)); }
double() { echo $((2 * $1)); }
negate() { echo $((0 - $1)); }

# compose_all X NAME...
# Calls the functions that the names name, from the last to the first, on
# X, and prints the result. A Bash function is not a value, so the helper
# takes the names and the value, and it cannot return a new function.
compose_all() {
  local x=$1
  shift
  local -a fs=("$@")
  local i
  for ((i = ${#fs[@]} - 1; i >= 0; i--)); do
    x=$("${fs[i]}" "$x")
  done
  echo "$x"
}
