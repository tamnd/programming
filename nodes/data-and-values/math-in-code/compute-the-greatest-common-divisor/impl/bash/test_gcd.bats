#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-greatest-common-divisor/.

setup() {
  source "$BATS_TEST_DIRNAME/gcd.sh"
}

@test "common-factor" { run gcd_and_lcm 12 18; [ "$output" = "6 36" ]; run gcd_and_lcm 18 12; [ "$output" = "6 36" ]; }
@test "coprime" { run gcd_and_lcm 17 5; [ "$output" = "1 85" ]; run gcd_and_lcm 5 17; [ "$output" = "1 85" ]; }
@test "euclid-example" { run gcd_and_lcm 48 180; [ "$output" = "12 720" ]; run gcd_and_lcm 180 48; [ "$output" = "12 720" ]; }
@test "zero-and-number" { run gcd_and_lcm 0 7; [ "$output" = "7 0" ]; run gcd_and_lcm 7 0; [ "$output" = "7 0" ]; }
@test "both-zero" { run gcd_and_lcm 0 0; [ "$output" = "0 0" ]; run gcd_and_lcm 0 0; [ "$output" = "0 0" ]; }
@test "negative" { run gcd_and_lcm -12 18; [ "$output" = "6 36" ]; run gcd_and_lcm 18 -12; [ "$output" = "6 36" ]; }
@test "both-negative" { run gcd_and_lcm -4 -6; [ "$output" = "2 12" ]; run gcd_and_lcm -6 -4; [ "$output" = "2 12" ]; }
@test "equal" { run gcd_and_lcm 9 9; [ "$output" = "9 9" ]; run gcd_and_lcm 9 9; [ "$output" = "9 9" ]; }
@test "one" { run gcd_and_lcm 1 1000000007; [ "$output" = "1 1000000007" ]; run gcd_and_lcm 1000000007 1; [ "$output" = "1 1000000007" ]; }
@test "multiple" { run gcd_and_lcm 5 25; [ "$output" = "5 25" ]; run gcd_and_lcm 25 5; [ "$output" = "5 25" ]; }
@test "fibonacci" { run gcd_and_lcm 832040 514229; [ "$output" = "1 427859097160" ]; run gcd_and_lcm 514229 832040; [ "$output" = "1 427859097160" ]; }
@test "big-lcm" { run gcd_and_lcm 3000000000000000000 6000000000000000000; [ "$output" = "3000000000000000000 6000000000000000000" ]; run gcd_and_lcm 6000000000000000000 3000000000000000000; [ "$output" = "3000000000000000000 6000000000000000000" ]; }
@test "largest" { run gcd_and_lcm 9223372036854775807 -9223372036854775807; [ "$output" = "9223372036854775807 9223372036854775807" ]; run gcd_and_lcm -9223372036854775807 9223372036854775807; [ "$output" = "9223372036854775807 9223372036854775807" ]; }
