#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

# gcd_and_lcm A B
# Prints "gcd lcm". The remainder in Bash has the sign of the dividend, so
# the loop works on the absolute values.
gcd_and_lcm() {
  local a=$1 b=$2 x y r
  x=$((a < 0 ? -a : a))
  y=$((b < 0 ? -b : b))
  while ((y != 0)); do
    r=$((x % y))
    x=$y
    y=$r
  done
  if ((a == 0 || b == 0)); then
    printf '%s 0\n' "$x"
  else
    printf '%s %s\n' "$x" "$(((a < 0 ? -a : a) / x * (b < 0 ? -b : b)))"
  fi
}
