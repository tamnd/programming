#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/concatenate-two-lists/.

# concat NAME_A NAME_B
# Prints the items of the array NAME_A, then the items of the array NAME_B,
# one on each line. The names are passed, because a function cannot take an
# array as one argument.
concat() {
  local -n _concat_a=$1 _concat_b=$2
  # The quotes keep each item one word.
  local -a _concat_c=("${_concat_a[@]}" "${_concat_b[@]}")
  if ((${#_concat_c[@]} > 0)); then printf '%s\n' "${_concat_c[@]}"; fi
}
