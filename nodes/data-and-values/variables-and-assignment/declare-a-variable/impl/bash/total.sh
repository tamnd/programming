#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/declare-a-variable/.

# total value...: print the sum of the values. local declares the variable
# result in the function, and declare -i gives it the integer attribute, so
# each assignment computes an arithmetic value. Without local, the variable
# is global.
total() {
  local -i result=0
  local value
  for value in "$@"; do
    result=result+value
  done
  printf '%d\n' "$result"
}
