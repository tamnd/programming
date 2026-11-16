#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-integer-arithmetic/.

# arith A B N
# Prints A + B, A - B, A * B, A / B (rounded toward zero), A % B (with the
# sign of A) and A to the power N, separated by spaces. B must not be 0.
# Bash arithmetic has the operator ** for a power; ^ is the bitwise
# exclusive or.
arith() {
  local a=$1 b=$2 n=$3
  echo "$((a + b)) $((a - b)) $((a * b)) $((a / b)) $((a % b)) $((a ** n))"
}
