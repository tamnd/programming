#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/map-over-a-list/.

# square_all ITEM...
# Prints the square of each integer argument, one on each line, in order.
square_all() {
  local x
  for x in "$@"; do
    printf '%s\n' "$((x * x))"
  done
}
