#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-a-factorial/.

# factorial N
# Prints N! for 0 <= N <= 20. Bash arithmetic does not check for overflow.
# Returns 1 if N is negative.
factorial() {
  local n=$1 f=1 i
  if ((n < 0)); then
    return 1
  fi
  for ((i = 2; i <= n; i++)); do
    f=$((f * i))
  done
  printf '%s\n' "$f"
}
