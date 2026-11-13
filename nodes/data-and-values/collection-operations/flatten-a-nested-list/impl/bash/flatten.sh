#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/flatten-a-nested-list/.

# flatten LIST...
# Each argument is one inner list, with the items separated by spaces. Prints
# the items of all the lists, one on each line, in order.
flatten() {
  local list
  local -a items out=()
  for list in "$@"; do
    read -r -a items <<< "$list"
    out+=("${items[@]}")
  done
  if ((${#out[@]} > 0)); then
    printf '%s\n' "${out[@]}"
  fi
}
