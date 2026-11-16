#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-priority-queue/.

# serve_order PRIORITY...
# Prints the request numbers in serve order on one line: the largest priority
# first, and for equal priorities, the smaller request number first. Bash has
# no priority queue. When all of the requests come before the first one is
# served, a sort gives the same order: sort -n compares the priorities as
# numbers, -r makes the largest first, and the second key keeps the arrival
# order of equal priorities.
serve_order() {
  local i
  local -a order=()
  mapfile -t order < <(
    for ((i = 1; i <= $#; i++)); do
      printf '%s %s\n' "${!i}" "$((i - 1))"
    done | sort -k1,1nr -k2,2n | cut -d ' ' -f 2
  )
  printf '%s\n' "${order[*]}"
}
