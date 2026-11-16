#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-fibonacci-numbers/.

setup() {
  source "$BATS_TEST_DIRNAME/fibonacci.sh"
}

@test "ten" { run fibonacci 10; [ "$output" = "55" ]; }
@test "twenty" { run fibonacci 20; [ "$output" = "6765" ]; }
@test "thirty" { run fibonacci 30; [ "$output" = "832040" ]; }
@test "zero" { run fibonacci 0; [ "$output" = "0" ]; }
@test "one" { run fibonacci 1; [ "$output" = "1" ]; }
@test "two" { run fibonacci 2; [ "$output" = "1" ]; }
@test "largest-32-bit" { run fibonacci 46; [ "$output" = "1836311903" ]; }
@test "beyond-32-bit" { run fibonacci 47; [ "$output" = "2971215073" ]; }
@test "binet-fails" { run fibonacci 71; [ "$output" = "308061521170129" ]; }
@test "largest-64-bit" { run fibonacci 92; [ "$output" = "7540113804746346429" ]; }

@test "negative-fails" { run fibonacci -1; [ "$status" -eq 1 ]; }
