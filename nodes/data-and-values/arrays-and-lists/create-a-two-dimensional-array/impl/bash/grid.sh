#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/create-a-two-dimensional-array/.

# grid ROWS COLS VALUE
# Prints ROWS lines, one for each row, with COLS copies of VALUE separated by
# spaces. Bash arrays have one dimension, so a program keeps a grid as lines,
# as one array with the index row * cols + col, or as an associative array.
grid() {
  local rows=$1 cols=$2 value=$3 i j
  local -a row=()
  for ((j = 0; j < cols; j++)); do
    row+=("$value")
  done
  for ((i = 0; i < rows; i++)); do
    printf '%s\n' "${row[*]}"
  done
}
