#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/define-a-function/.

setup() {
  source "$BATS_TEST_DIRNAME/clamp.sh"
}

@test "inside" { run clamp 5 1 10; [ "$output" = "5" ]; }
@test "below" { run clamp -3 0 10; [ "$output" = "0" ]; }
@test "above" { run clamp 42 0 10; [ "$output" = "10" ]; }
@test "at-low" { run clamp 0 0 10; [ "$output" = "0" ]; }
@test "at-high" { run clamp 10 0 10; [ "$output" = "10" ]; }
@test "equal-bounds" { run clamp 7 3 3; [ "$output" = "3" ]; }
@test "negative-range" { run clamp -50 -20 -10; [ "$output" = "-20" ]; }
@test "zero" { run clamp 0 -5 5; [ "$output" = "0" ]; }
@test "int-max" { run clamp 2147483647 -100 100; [ "$output" = "100" ]; }
@test "int-min" { run clamp -2147483648 -100 100; [ "$output" = "-100" ]; }
@test "full-range" { run clamp -2147483648 -2147483648 2147483647; [ "$output" = "-2147483648" ]; }
