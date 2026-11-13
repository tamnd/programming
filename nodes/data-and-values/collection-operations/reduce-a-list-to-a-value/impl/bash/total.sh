#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

# total ITEM...
# Prints the sum of the integer arguments. The sum of no arguments is 0.
total() {
  local acc=0 x
  for x in "$@"; do
    acc=$((acc + x))
  done
  printf '%s\n' "$acc"
}
