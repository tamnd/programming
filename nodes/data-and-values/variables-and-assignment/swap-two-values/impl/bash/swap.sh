#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/swap-two-values/.

# swap_items i j item...: print the items, separated by spaces, with the
# items at the indexes i and j swapped. Bash has no parallel assignment, so
# the swap keeps one value in a temporary variable.
swap_items() {
  local i=$1 j=$2
  shift 2
  local -a xs=("$@")
  local tmp=${xs[i]}
  xs[i]=${xs[j]}
  xs[j]=$tmp
  printf '%s\n' "${xs[*]}"
}
