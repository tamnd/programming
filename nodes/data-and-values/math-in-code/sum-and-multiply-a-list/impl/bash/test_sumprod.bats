#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sum-and-multiply-a-list/.

setup() {
  source "$BATS_TEST_DIRNAME/sumprod.sh"
}

@test "small" { run sum_and_product 1 2 3 4; [ "$output" = "10 24" ]; }
@test "mixed" { run sum_and_product 3 -2 5; [ "$output" = "6 -30" ]; }
@test "empty" { run sum_and_product ; [ "$output" = "0 1" ]; }
@test "single" { run sum_and_product 7; [ "$output" = "7 7" ]; }
@test "with-zero" { run sum_and_product 5 0 9; [ "$output" = "14 0" ]; }
@test "two-negatives" { run sum_and_product -2 -3; [ "$output" = "-5 6" ]; }
@test "three-negatives" { run sum_and_product -1 -2 -3; [ "$output" = "-6 -6" ]; }
@test "ones" { run sum_and_product 1 1 1; [ "$output" = "3 1" ]; }
@test "large-product" { run sum_and_product 3037000499 3037000499; [ "$output" = "6074000998 9223372030926249001" ]; }
@test "to-the-limit" { run sum_and_product 9223372036854775806 1; [ "$output" = "9223372036854775807 9223372036854775806" ]; }
@test "beyond-double" { run sum_and_product 9007199254740993; [ "$output" = "9007199254740993 9007199254740993" ]; }
@test "min" { run sum_and_product -9223372036854775808; [ "$output" = "-9223372036854775808 -9223372036854775808" ]; }

@test "wraps-around" { run sum_and_product 4294967296 4294967296; [ "$output" = "8589934592 0" ]; }
