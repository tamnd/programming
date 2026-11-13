#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/zip-two-lists/.

# zip_pairs "A_ITEMS" "B_ITEMS"
# Each argument is a list of items, separated by spaces. Prints one line
# "A_ITEM B_ITEM" for each index that is in both lists, in index order.
zip_pairs() {
  local -a a b
  read -ra a <<<"$1"
  read -ra b <<<"$2"
  # ${#a[@]} is the number of items. ${#a} is the length of the first item.
  local n=${#a[@]} i
  if ((${#b[@]} < n)); then
    n=${#b[@]}
  fi
  for ((i = 0; i < n; i++)); do
    printf '%s %s\n' "${a[i]}" "${b[i]}"
  done
}
