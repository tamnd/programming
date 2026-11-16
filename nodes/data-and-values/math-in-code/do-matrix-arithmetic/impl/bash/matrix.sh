#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-matrix-arithmetic/.

# A matrix is a string of rows separated by ";", with the numbers of a row
# separated by spaces: "1 2;3 4" is a 2 x 2 matrix.

# matrix_multiply A B
# Prints the product A times B in the same form. Exits with 1 if the number
# of columns of A is not the number of rows of B.
matrix_multiply() {
  local -a ra rb row
  local n m p i j k s
  IFS=';' read -ra ra <<< "$1"
  IFS=';' read -ra rb <<< "$2"
  read -ra row <<< "${ra[0]}"; m=${#row[@]}
  read -ra row <<< "${rb[0]}"; p=${#row[@]}
  n=${#ra[@]}
  if (( m != ${#rb[@]} )); then
    echo "the sizes do not match" >&2
    return 1
  fi
  # Bash has no two-dimensional arrays, so A and B are flat, in row-major order.
  local -a a=() b=() out=()
  for (( i = 0; i < n; i++ )); do read -ra row <<< "${ra[i]}"; a+=("${row[@]}"); done
  for (( k = 0; k < m; k++ )); do read -ra row <<< "${rb[k]}"; b+=("${row[@]}"); done
  for (( i = 0; i < n; i++ )); do
    row=()
    for (( j = 0; j < p; j++ )); do
      s=0
      for (( k = 0; k < m; k++ )); do
        (( s += a[i * m + k] * b[k * p + j] ))
      done
      row+=("$s")
    done
    out+=("${row[*]}")
  done
  local IFS=';'
  echo "${out[*]}"
}

# transpose A
# Prints the transpose of A: row i of the result is column i of A.
transpose() {
  local -a ra row out=()
  local i j
  IFS=';' read -ra ra <<< "$1"
  read -ra row <<< "${ra[0]}"
  local m=${#row[@]}
  for (( j = 0; j < m; j++ )); do
    local col=()
    for (( i = 0; i < ${#ra[@]}; i++ )); do
      read -ra row <<< "${ra[i]}"
      col+=("${row[j]}")
    done
    out+=("${col[*]}")
  done
  local IFS=';'
  echo "${out[*]}"
}
