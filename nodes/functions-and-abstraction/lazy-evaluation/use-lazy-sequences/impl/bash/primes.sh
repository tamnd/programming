#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-lazy-sequences/.

# is_prime K
# Returns 0 if K is a prime number, by trial division.
is_prime() {
  local k=$1 d
  (( k < 2 )) && return 1
  for ((d = 2; d * d <= k; d++)); do
    (( k % d == 0 )) && return 1
  done
  return 0
}

# naturals START
# Prints START, START + 1, ... without end. It stops when its output closes.
naturals() {
  local i
  for ((i = $1; ; i++)); do
    printf '%d\n' "$i" || return 0
  done
}

# primes
# Prints the prime numbers without end. It reads one number at a time from
# naturals, so it does only the work that the reader asks for.
primes() {
  local k
  naturals 2 | while read -r k; do
    is_prime "$k" || continue
    printf '%d\n' "$k" || break
  done
}

# first_primes N
# Prints the first N prime numbers, one on each line.
first_primes() {
  (( $1 > 0 )) || return 0
  primes | head -n "$1"
}
