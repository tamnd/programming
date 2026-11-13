#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-an-array/.

# filled N VALUE
# Prints VALUE N times, one on each line. A Bash array holds strings, so the
# lines stand for the items of the array.
filled() {
  local -a a=()
  local i
  for ((i = 0; i < $1; i++)); do
    a+=("$2")
  done
  if ((${#a[@]} > 0)); then printf '%s\n' "${a[@]}"; fi
}
