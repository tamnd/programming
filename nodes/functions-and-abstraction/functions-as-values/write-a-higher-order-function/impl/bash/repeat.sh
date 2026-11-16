#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-higher-order-function/.

# repeat F TIMES X
# Applies the function named F to X, TIMES times, and prints the result.
# A Bash function is not a value, so repeat takes the name of the function
# and cannot return a new function.
repeat() {
  local f=$1 times=$2 x=$3 i
  for ((i = 0; i < times; i++)); do
    x=$("$f" "$x")
  done
  echo "$x"
}

# collatz_step N
# Prints N / 2 for an even N and 3N + 1 for an odd N.
collatz_step() {
  local n=$1
  if ((n % 2 == 0)); then
    echo $((n / 2))
  else
    echo $((3 * n + 1))
  fi
}

# collatz_after START TIMES
# Prints the value after TIMES Collatz steps from START.
collatz_after() {
  repeat collatz_step "$2" "$1"
}
