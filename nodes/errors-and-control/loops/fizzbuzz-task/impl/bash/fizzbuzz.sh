#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/fizzbuzz-task/.

# fizzbuzz N
# Prints FizzBuzz, Fizz, Buzz or the number for each number from 1 to N, one
# on each line.
fizzbuzz() {
  local n=$1 i
  for ((i = 1; i <= n; i++)); do
    # Test 15 first: a multiple of 15 is also a multiple of 3 and of 5.
    if ((i % 15 == 0)); then
      printf '%s\n' FizzBuzz
    elif ((i % 3 == 0)); then
      printf '%s\n' Fizz
    elif ((i % 5 == 0)); then
      printf '%s\n' Buzz
    else
      printf '%s\n' "$i"
    fi
  done
}
