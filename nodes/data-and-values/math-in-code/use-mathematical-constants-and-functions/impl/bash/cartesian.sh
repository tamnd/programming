#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-mathematical-constants-and-functions/.

# to_cartesian R DEGREES
# Prints "x y" with 6 decimals. Bash arithmetic has only integers, so awk
# does the floating-point work. A value that rounds to zero prints as 0.
to_cartesian() {
  awk -v r="$1" -v d="$2" 'BEGIN {
    pi = atan2(0, -1)
    t = d * pi / 180
    x = r * cos(t); y = r * sin(t)
    if (x > -5e-7 && x < 5e-7) x = 0
    if (y > -5e-7 && y < 5e-7) y = 0
    printf "%.6f %.6f\n", x, y
  }'
}
