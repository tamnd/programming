#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-an-enum-with-data/.

# Bash has no enum with data. A shape is a string: the kind, then the sides,
# separated by spaces, such as "rectangle 2 5". A case statement on the kind
# plays the part of the match.

# make_shape KIND SIDE...
# Prints the shape string, or returns status 1 when the data is not a valid
# shape.
make_shape() {
  local kind=$1 s
  shift
  for s in "$@"; do
    ((s > 0)) || return 1
  done
  case "$kind:$#" in
    square:1 | rectangle:2) ;;
    triangle:3)
      (($1 < $2 + $3 && $2 < $1 + $3 && $3 < $1 + $2)) || return 1
      ;;
    *) return 1 ;;
  esac
  printf '%s\n' "$kind $*"
}

# shape_perimeter SHAPE
shape_perimeter() {
  local kind a b c
  read -r kind a b c <<<"$1"
  case "$kind" in
    square) echo $((4 * a)) ;;
    rectangle) echo $((2 * (a + b))) ;;
    triangle) echo $((a + b + c)) ;;
    *) return 1 ;;
  esac
}

# perimeter KIND SIDE...
# Prints the perimeter, or prints nothing and returns status 1.
perimeter() {
  local shape
  shape=$(make_shape "$@") || return 1
  shape_perimeter "$shape"
}
