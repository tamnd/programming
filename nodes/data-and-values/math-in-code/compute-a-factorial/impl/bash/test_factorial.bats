#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-a-factorial/.

setup() {
  source "$BATS_TEST_DIRNAME/factorial.sh"
}

@test "five" { run factorial 5; [ "$output" = "120" ]; }
@test "ten" { run factorial 10; [ "$output" = "3628800" ]; }
@test "zero" { run factorial 0; [ "$output" = "1" ]; }
@test "one" { run factorial 1; [ "$output" = "1" ]; }
@test "largest-32-bit" { run factorial 12; [ "$output" = "479001600" ]; }
@test "beyond-32-bit" { run factorial 13; [ "$output" = "6227020800" ]; }
@test "largest-64-bit" { run factorial 20; [ "$output" = "2432902008176640000" ]; }

@test "negative-fails" { run factorial -1; [ "$status" -eq 1 ]; }
