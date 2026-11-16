#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/swap-two-values/.

setup() {
  source "$BATS_TEST_DIRNAME/swap.sh"
}

@test "first-and-last" {
  run swap_items 0 2 1 2 3
  [ "$status" -eq 0 ]
  [ "$output" = "3 2 1" ]
}

@test "middle-pair" {
  run swap_items 1 2 10 20 30 40
  [ "$status" -eq 0 ]
  [ "$output" = "10 30 20 40" ]
}

@test "two-items" {
  run swap_items 0 1 5 7
  [ "$status" -eq 0 ]
  [ "$output" = "7 5" ]
}

@test "single-item" {
  run swap_items 0 0 9
  [ "$status" -eq 0 ]
  [ "$output" = "9" ]
}

@test "same-index" {
  run swap_items 1 1 1 2 3
  [ "$status" -eq 0 ]
  [ "$output" = "1 2 3" ]
}

@test "higher-index-first" {
  run swap_items 3 0 1 2 3 4
  [ "$status" -eq 0 ]
  [ "$output" = "4 2 3 1" ]
}

@test "equal-values" {
  run swap_items 0 1 4 4
  [ "$status" -eq 0 ]
  [ "$output" = "4 4" ]
}

@test "negative-values" {
  run swap_items 0 2 -1 0 1
  [ "$status" -eq 0 ]
  [ "$output" = "1 0 -1" ]
}

@test "int32-extremes" {
  run swap_items 0 1 2147483647 -2147483648
  [ "$status" -eq 0 ]
  [ "$output" = "-2147483648 2147483647" ]
}

@test "eight-items" {
  run swap_items 2 6 1 2 3 4 5 6 7 8
  [ "$status" -eq 0 ]
  [ "$output" = "1 2 7 4 5 6 3 8" ]
}
