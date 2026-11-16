#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/print-formatted-output/.

setup() {
  source "$BATS_TEST_DIRNAME/row.sh"
}

@test "apple" {
  run format_row 'apple' 3 1.5
  [ "$output" = 'apple     |    3|    1.50' ]
}

@test "widget" {
  run format_row 'widget' 12 19.99
  [ "$output" = 'widget    |   12|   19.99' ]
}

@test "laptop" {
  run format_row 'laptop' 1 1234.5
  [ "$output" = 'laptop    |    1| 1234.50' ]
}

@test "empty-name" {
  run format_row '' 0 0
  [ "$output" = '          |    0|    0.00' ]
}

@test "exact-widths" {
  run format_row 'abcdefghij' 99999 99999.99
  [ "$output" = 'abcdefghij|99999|99999.99' ]
}

@test "long-name" {
  run format_row 'watermelonade' 5 2
  [ "$output" = 'watermelonade|    5|    2.00' ]
}

@test "wide-quantity" {
  run format_row 'pen' 123456 0.5
  [ "$output" = 'pen       |123456|    0.50' ]
}

@test "wide-price" {
  run format_row 'car' 1 123456.75
  [ "$output" = 'car       |    1|123456.75' ]
}

@test "negative-values" {
  run format_row 'refund' -2 -3.5
  [ "$output" = 'refund    |   -2|   -3.50' ]
}

@test "rounds-up" {
  run format_row 'nail' 1000 0.006
  [ "$output" = 'nail      | 1000|    0.01' ]
}

@test "rounds-down" {
  run format_row 'bolt' 7 2.344
  [ "$output" = 'bolt      |    7|    2.34' ]
}

@test "name-with-spaces" {
  run format_row 'blue pen' 4 0.99
  [ "$output" = 'blue pen  |    4|    0.99' ]
}

@test "name-with-percent" {
  run format_row '100% {x}' 1 1
  [ "$output" = '100% {x}  |    1|    1.00' ]
}
