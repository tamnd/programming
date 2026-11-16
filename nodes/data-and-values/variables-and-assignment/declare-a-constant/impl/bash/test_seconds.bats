#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/declare-a-constant/.

setup() {
  source "$BATS_TEST_DIRNAME/seconds.sh"
}

@test "one-day" {
  run seconds_in 1
  [ "$status" -eq 0 ]
  [ "$output" = "86400" ]
}

@test "one-week" {
  run seconds_in 7
  [ "$status" -eq 0 ]
  [ "$output" = "604800" ]
}

@test "one-year" {
  run seconds_in 365
  [ "$status" -eq 0 ]
  [ "$output" = "31536000" ]
}

@test "zero" {
  run seconds_in 0
  [ "$status" -eq 0 ]
  [ "$output" = "0" ]
}

@test "two-days" {
  run seconds_in 2
  [ "$status" -eq 0 ]
  [ "$output" = "172800" ]
}

@test "leap-year" {
  run seconds_in 366
  [ "$status" -eq 0 ]
  [ "$output" = "31622400" ]
}

@test "last-32-bit-day" {
  run seconds_in 24855
  [ "$status" -eq 0 ]
  [ "$output" = "2147472000" ]
}

@test "past-32-bits" {
  run seconds_in 24856
  [ "$status" -eq 0 ]
  [ "$output" = "2147558400" ]
}

@test "million-days" {
  run seconds_in 1000000
  [ "$status" -eq 0 ]
  [ "$output" = "86400000000" ]
}

@test "seconds per day" {
  [ "$SECONDS_PER_DAY" -eq 86400 ]
}

@test "a constant gets no new value" {
  run bash -c 'source "$1"; SECONDS_PER_DAY=1' _ "$BATS_TEST_DIRNAME/seconds.sh"
  [ "$status" -ne 0 ]
}
