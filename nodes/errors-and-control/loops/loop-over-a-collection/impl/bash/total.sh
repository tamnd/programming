#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-over-a-collection/.

# total ITEM...
# Prints the sum of the items. Bash arithmetic uses 64-bit integers.
total() {
  local s=0 x
  for x in "$@"; do
    ((s += x))
  done
  printf '%s\n' "$s"
}
