#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/do-complex-number-arithmetic/.

setup() {
  source "$BATS_TEST_DIRNAME/complexarith.sh"
}

# close GOT WANT: each has 8 numbers, separated by spaces.
close() {
  awk -v g="$1" -v w="$2" 'BEGIN {
    n = split(g, x, " "); m = split(w, y, " ")
    if (n != 8 || m != 8) exit 1
    for (i = 1; i <= 8; i++) {
      e = x[i] - y[i]; if (e < 0) e = -e
      t = y[i] < 0 ? -y[i] : y[i]; t = t * 1e-12; if (t < 1e-9) t = 1e-9
      if (e > t) exit 1
    }
  }'
}

@test "textbook" { run complex_arithmetic 3 4 1 -2; [ "$status" -eq 0 ]; close "$output" "4 2 11 -2 -1 2 5 0.9272952180016122"; }
@test "rosetta" { run complex_arithmetic 1 1 3.14159 1.25; [ "$status" -eq 0 ]; close "$output" "4.14159 2.25 1.8915899999999999 4.39159 0.38414593243590134 0.16546321590504276 1.4142135623730951 0.7853981633974483"; }
@test "real-only" { run complex_arithmetic 6 0 3 0; [ "$status" -eq 0 ]; close "$output" "9 0 18 0 2 0 6 0"; }
@test "imaginary-unit" { run complex_arithmetic 0 1 0 1; [ "$status" -eq 0 ]; close "$output" "0 2 -1 0 1 0 1 1.5707963267948966"; }
@test "negative-real" { run complex_arithmetic -2 0 1 0; [ "$status" -eq 0 ]; close "$output" "-1 0 -2 0 -2 0 2 3.141592653589793"; }
@test "zero" { run complex_arithmetic 0 0 2 -3; [ "$status" -eq 0 ]; close "$output" "2 -3 0 0 0 0 0 0"; }
@test "negative-phase" { run complex_arithmetic 1 -1 -1 -1; [ "$status" -eq 0 ]; close "$output" "0 -2 -2 0 0 1 1.4142135623730951 -0.7853981633974483"; }
@test "imaginary-divisor" { run complex_arithmetic 2 4 0 2; [ "$status" -eq 0 ]; close "$output" "2 6 -8 4 2 -1 4.47213595499958 1.1071487177940904"; }
@test "large-divisor" { run complex_arithmetic 1e+150 1e+150 1e+155 1e+155; [ "$status" -eq 0 ]; close "$output" "1.00001e+155 1.00001e+155 0 2e+305 9.999999999999999e-06 0 1.414213562373095e+150 0.7853981633974483"; }

@test "divide-by-zero" { run complex_arithmetic 1 1 0 0; [ "$status" -eq 1 ]; }
