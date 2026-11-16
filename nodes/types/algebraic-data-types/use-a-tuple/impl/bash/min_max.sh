#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-tuple/.

# min_max NUMBER...
# Prints "MIN MAX" on one line. Bash has no tuple type, so a function returns
# several values as words of its output, and the caller reads them back.
min_max() {
  local lo=$1 hi=$1 n
  for n in "$@"; do
    ((n < lo)) && lo=$n
    ((n > hi)) && hi=$n
  done
  printf '%s %s\n' "$lo" "$hi"
}
