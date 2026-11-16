#!/usr/bin/env bash
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

# complex_arithmetic A_RE A_IM B_RE B_IM
# Prints the sum, the product and the quotient of a and b, then abs(a) and
# phase(a): 8 numbers on one line. Bash arithmetic has only integers, so awk
# does the floating-point work. The division divides by the larger part of b
# first, so it cannot overflow.
complex_arithmetic() {
  awk -v a="$1" -v b="$2" -v c="$3" -v d="$4" '
  function abs(x) { return x < 0 ? -x : x }
  BEGIN {
    if (c == 0 && d == 0) { print "division by zero" > "/dev/stderr"; exit 1 }
    if (abs(c) >= abs(d)) {
      r = d / c; t = c + d * r
      qr = (a + b * r) / t; qi = (b - a * r) / t
    } else {
      r = c / d; t = c * r + d
      qr = (a * r + b) / t; qi = (b * r - a) / t
    }
    # abs(a) scales by the larger part, so the squares cannot overflow.
    m = abs(a) > abs(b) ? abs(a) : abs(b)
    len = m == 0 ? 0 : m * sqrt((a / m) ^ 2 + (b / m) ^ 2)
    printf "%.17g %.17g %.17g %.17g %.17g %.17g %.17g %.17g\n", a + c, b + d, a * c - b * d, a * d + b * c, qr, qi, len, atan2(b, a)
  }'
}
