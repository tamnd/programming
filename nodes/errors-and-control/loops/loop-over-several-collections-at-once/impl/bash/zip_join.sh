#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-over-several-collections-at-once/.

# zip_join LOWER UPPER DIGITS
# Takes the names of three arrays. For each index below the smallest length,
# prints the three items joined, one string on each line.
zip_join() {
  # Namerefs read the arrays of the caller. The long names avoid a clash with
  # the names that the caller passes.
  local -n zip_join_l=$1 zip_join_u=$2 zip_join_d=$3
  local n=${#zip_join_l[@]} i
  ((${#zip_join_u[@]} < n)) && n=${#zip_join_u[@]}
  ((${#zip_join_d[@]} < n)) && n=${#zip_join_d[@]}
  for ((i = 0; i < n; i++)); do
    printf '%s\n' "${zip_join_l[i]}${zip_join_u[i]}${zip_join_d[i]}"
  done
}
