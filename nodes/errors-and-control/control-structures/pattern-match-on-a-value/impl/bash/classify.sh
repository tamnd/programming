#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/pattern-match-on-a-value/.

# classify X Y
# Prints the place of the point (X, Y): origin, x-axis, y-axis, diagonal or other.
classify() {
  # case matches the string "X,Y" against glob patterns, in order.
  case "$1,$2" in
    0,0)
      echo origin
      ;;
    *,0)
      echo x-axis
      ;;
    0,*)
      echo y-axis
      ;;
    *)
      # A glob cannot compare X with Y, so an if does the work of a guard.
      if (($1 == $2)); then
        echo diagonal
      else
        echo other
      fi
      ;;
  esac
}
