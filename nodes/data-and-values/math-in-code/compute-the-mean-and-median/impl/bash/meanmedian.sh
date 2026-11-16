#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-mean-and-median/.

# mean_and_median ITEM...
# Prints "mean median". Bash arithmetic has only integers, so sort sorts the
# items and awk computes the results as floating-point numbers. Returns 1 if
# there are no items.
mean_and_median() {
  if [ "$#" -eq 0 ]; then
    return 1
  fi
  printf '%s\n' "$@" | sort -n | awk '
    { s[NR] = $1; sum += $1 }
    END {
      mid = int((NR + 1) / 2)
      median = NR % 2 == 1 ? s[mid] : (s[mid] + s[mid + 1]) / 2
      printf "%.17g %.17g\n", sum / NR, median
    }'
}
