#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

# sum_and_product ITEM...
# Prints "sum product". Bash arithmetic uses 64-bit integers with no check
# for overflow.
sum_and_product() {
  local sum=0 product=1 x
  for x in "$@"; do
    sum=$((sum + x))
    product=$((product * x))
  done
  printf '%s %s\n' "$sum" "$product"
}
