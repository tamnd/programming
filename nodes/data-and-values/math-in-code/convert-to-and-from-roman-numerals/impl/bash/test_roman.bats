#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/convert-to-and-from-roman-numerals/.

setup() {
  source "$BATS_TEST_DIRNAME/roman.sh"
}

@test "year-1990" { run to_roman 1990; [ "$output" = 'MCMXC' ]; run from_roman 'MCMXC'; [ "$output" = "1990" ]; }
@test "year-2008" { run to_roman 2008; [ "$output" = 'MMVIII' ]; run from_roman 'MMVIII'; [ "$output" = "2008" ]; }
@test "each-symbol-once" { run to_roman 1666; [ "$output" = 'MDCLXVI' ]; run from_roman 'MDCLXVI'; [ "$output" = "1666" ]; }
@test "one" { run to_roman 1; [ "$output" = 'I' ]; run from_roman 'I'; [ "$output" = "1" ]; }
@test "four" { run to_roman 4; [ "$output" = 'IV' ]; run from_roman 'IV'; [ "$output" = "4" ]; }
@test "nine" { run to_roman 9; [ "$output" = 'IX' ]; run from_roman 'IX'; [ "$output" = "9" ]; }
@test "forty-nine" { run to_roman 49; [ "$output" = 'XLIX' ]; run from_roman 'XLIX'; [ "$output" = "49" ]; }
@test "ninety-nine" { run to_roman 99; [ "$output" = 'XCIX' ]; run from_roman 'XCIX'; [ "$output" = "99" ]; }
@test "four-hundred-forty-four" { run to_roman 444; [ "$output" = 'CDXLIV' ]; run from_roman 'CDXLIV'; [ "$output" = "444" ]; }
@test "year-1994" { run to_roman 1994; [ "$output" = 'MCMXCIV' ]; run from_roman 'MCMXCIV'; [ "$output" = "1994" ]; }
@test "longest" { run to_roman 3888; [ "$output" = 'MMMDCCCLXXXVIII' ]; run from_roman 'MMMDCCCLXXXVIII'; [ "$output" = "3888" ]; }
@test "largest" { run to_roman 3999; [ "$output" = 'MMMCMXCIX' ]; run from_roman 'MMMCMXCIX'; [ "$output" = "3999" ]; }

@test "round-trip" {
  # A sample of the numbers 1 to 3999, because bats is slow with thousands of subshells.
  local n s
  for ((n = 1; n <= 3999; n += 37)); do
    s=$(to_roman "$n")
    [ "$(from_roman "$s")" = "$n" ]
  done
}

@test "invalid" {
  local s
  for s in "" IIII IC VX MMMM iv ABC IIV XM; do
    run from_roman "$s"
    [ "$status" -eq 1 ]
  done
}

@test "out-of-range" {
  run to_roman 0
  [ "$status" -eq 1 ]
  run to_roman -1
  [ "$status" -eq 1 ]
  run to_roman 4000
  [ "$status" -eq 1 ]
}
