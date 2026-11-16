#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/declare-a-variable/.

setup() {
  source "$BATS_TEST_DIRNAME/total.sh"
}

@test "one-two-three" {
  run total 1 2 3
  [ "$status" -eq 0 ]
  [ "$output" = "6" ]
}

@test "four-values" {
  run total 10 20 30 40
  [ "$status" -eq 0 ]
  [ "$output" = "100" ]
}

@test "mixed-signs" {
  run total 5 -2 7
  [ "$status" -eq 0 ]
  [ "$output" = "10" ]
}

@test "empty" {
  run total 
  [ "$status" -eq 0 ]
  [ "$output" = "0" ]
}

@test "single" {
  run total 42
  [ "$status" -eq 0 ]
  [ "$output" = "42" ]
}

@test "all-negative" {
  run total -1 -2 -3
  [ "$status" -eq 0 ]
  [ "$output" = "-6" ]
}

@test "zeros" {
  run total 0 0 0
  [ "$status" -eq 0 ]
  [ "$output" = "0" ]
}

@test "cancel-out" {
  run total 7 -7
  [ "$status" -eq 0 ]
  [ "$output" = "0" ]
}

@test "past-32-bits" {
  run total 4000000000 5000000000
  [ "$status" -eq 0 ]
  [ "$output" = "9000000000" ]
}

@test "eight-values" {
  run total 1 1 1 1 1 1 1 1
  [ "$status" -eq 0 ]
  [ "$output" = "8" ]
}
