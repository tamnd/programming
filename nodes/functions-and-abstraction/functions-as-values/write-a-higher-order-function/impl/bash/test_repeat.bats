#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/write-a-higher-order-function/.

setup() {
  source "$BATS_TEST_DIRNAME/repeat.sh"
}

double() { echo $(($1 * 2)); }

@test "one-step" { run collatz_after 6 1; [ "$output" = "3" ]; }
@test "three-steps" { run collatz_after 6 3; [ "$output" = "5" ]; }
@test "ten-steps" { run collatz_after 27 10; [ "$output" = "214" ]; }
@test "zero-times" { run collatz_after 7 0; [ "$output" = "7" ]; }
@test "start-one" { run collatz_after 1 1; [ "$output" = "4" ]; }
@test "cycle" { run collatz_after 1 3; [ "$output" = "1" ]; }
@test "past-one" { run collatz_after 16 10; [ "$output" = "1" ]; }
@test "peak-27" { run collatz_after 27 77; [ "$output" = "9232" ]; }
@test "peak-9663" { run collatz_after 9663 48; [ "$output" = "27114424" ]; }
@test "limit" { run collatz_after 10000 1000; [ "$output" = "2" ]; }
@test "repeat takes another function" { run repeat double 10 1; [ "$output" = "1024" ]; }
