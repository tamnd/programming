#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/destructure-a-value/.

# manhattan P Q
# Takes the names of two arrays (x y) and prints their Manhattan distance.
manhattan() {
  local -n manhattan_p=$1 manhattan_q=$2
  local x1 y1 x2 y2 dx dy
  # read splits a line into words and gives one word to each name.
  read -r x1 y1 <<<"${manhattan_p[*]}"
  read -r x2 y2 <<<"${manhattan_q[*]}"
  dx=$((x1 - x2))
  dy=$((y1 - y2))
  printf '%s\n' "$((${dx#-} + ${dy#-}))"
}
