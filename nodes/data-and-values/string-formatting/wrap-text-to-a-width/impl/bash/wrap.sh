#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/wrap-text-to-a-width/.

# wrap text width: print the lines of text, each at most width characters
# long, with the greedy method. A word longer than width is a line of its
# own. read -d '' reads the whole text, newlines included, and returns 1 at
# the end of the input.
wrap() {
  local width=$2 line='' word
  local -a words
  read -r -d '' -a words <<<"$1" || true
  for word in "${words[@]}"; do
    if [[ -z $line ]]; then
      line=$word
    elif ((${#line} + 1 + ${#word} <= width)); then
      line+=" $word"
    else
      printf '%s\n' "$line"
      line=$word
    fi
  done
  if [[ -n $line ]]; then
    printf '%s\n' "$line"
  fi
}
