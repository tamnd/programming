#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-the-letters-of-a-text/.

# count_letters text: print the counts of the letters a to z in text, one on
# each line, in the order of the alphabet. Upper case letters count as lower
# case letters.
count_letters() {
  local text=${1,,} c i
  local -a counts=(0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)
  for ((i = 0; i < ${#text}; i++)); do
    c=${text:i:1}
    if [[ $c == [abcdefghijklmnopqrstuvwxyz] ]]; then
      ((++counts[$(printf '%d' "'$c") - 97]))
    fi
  done
  printf '%s\n' "${counts[@]}"
}
