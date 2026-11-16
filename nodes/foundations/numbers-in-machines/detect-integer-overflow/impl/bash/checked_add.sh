#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/detect-integer-overflow/.

# Bash arithmetic uses 64-bit signed integers with no check for overflow, so
# test the operands before the addition.
INT64_MAX=9223372036854775807
INT64_MIN=$((-9223372036854775807 - 1))

# checked_add A B
# Prints A + B, or prints nothing and returns status 1 when the sum does not
# fit in 64 bits.
checked_add() {
  local a=$1 b=$2
  if ((b > 0 && a > INT64_MAX - b)) || ((b < 0 && a < INT64_MIN - b)); then
    return 1
  fi
  echo $((a + b))
}
