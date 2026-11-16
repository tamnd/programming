#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/format-a-number-with-fixed-decimals/.

setup() {
  source "$BATS_TEST_DIRNAME/fixed.sh"
}

@test "pi-two" {
  run fixed 3.14159 2
  [ "$output" = '3.14' ]
}

@test "pad-zeros" {
  run fixed 1 3
  [ "$output" = '1.000' ]
}

@test "round-up" {
  run fixed 1234.5678 2
  [ "$output" = '1234.57' ]
}

@test "zero-digits" {
  run fixed 123.456 0
  [ "$output" = '123' ]
}

@test "float-error" {
  run fixed 0.30000000000000004 2
  [ "$output" = '0.30' ]
}

@test "negative" {
  run fixed -1.234 1
  [ "$output" = '-1.2' ]
}

@test "rounds-to-zero" {
  run fixed 0.0049 2
  [ "$output" = '0.00' ]
}

@test "stored-below-2-675" {
  run fixed 2.675 2
  [ "$output" = '2.67' ]
}

@test "stored-below-1-005" {
  run fixed 1.005 2
  [ "$output" = '1.00' ]
}

@test "large" {
  run fixed 1e+20 2
  [ "$output" = '100000000000000000000.00' ]
}

@test "many-digits" {
  run fixed 0.1 20
  [ "$output" = '0.10000000000000000555' ]
}

@test "integer-value" {
  run fixed 42 2
  [ "$output" = '42.00' ]
}
