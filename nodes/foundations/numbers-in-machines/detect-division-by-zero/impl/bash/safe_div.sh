#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/detect-division-by-zero/.

# Bash traps an integer division by zero as an error, and the error stops the
# arithmetic expansion. Test the divisor first, so the function can return a
# status instead.
INT64_MIN=$((-9223372036854775807 - 1))

# safe_div A B
# Prints A / B rounded toward zero, or prints nothing and returns status 1
# when B is 0 or the quotient does not fit in 64 bits.
safe_div() {
  local a=$1 b=$2
  if ((b == 0)) || ((a == INT64_MIN && b == -1)); then
    return 1
  fi
  echo $((a / b))
}
