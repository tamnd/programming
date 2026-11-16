#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

# fibonacci N
# Prints F(N) for 0 <= N <= 92. Bash arithmetic does not check for overflow.
# Returns 1 if N is negative.
fibonacci() {
  local n=$1 a=0 b=1 next i
  if ((n < 0)); then
    return 1
  fi
  if ((n == 0)); then
    printf '0\n'
    return 0
  fi
  for ((i = 1; i < n; i++)); do
    next=$((a + b))
    a=$b
    b=$next
  done
  printf '%s\n' "$b"
}
