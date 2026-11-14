#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

# count_points X,Y...
# Prints one line "X Y COUNT" for each distinct point, in ascending order of
# X, then Y.
count_points() {
  # Bash has no record type, so the key is the text "X,Y". The comma keeps
  # 1,23 and 12,3 apart. The keys compare as text, so the points must be in
  # one form: 1,2 and 01,2 would be two keys.
  local -A counts=()
  local p
  for p in "$@"; do
    counts[$p]=$(( ${counts[$p]-0} + 1 ))
  done
  for p in "${!counts[@]}"; do
    printf '%s %s %s\n' "${p%%,*}" "${p#*,}" "${counts[$p]}"
  done | LC_ALL=C sort -k1,1n -k2,2n
}
