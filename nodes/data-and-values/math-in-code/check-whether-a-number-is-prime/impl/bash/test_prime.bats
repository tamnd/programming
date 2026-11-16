#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-whether-a-number-is-prime/.

setup() {
  source "$BATS_TEST_DIRNAME/prime.sh"
}

@test "small-prime" { run is_prime 17; [ "$status" -eq 0 ]; }
@test "small-composite" { run is_prime 15; [ "$status" -eq 1 ]; }
@test "even" { run is_prime 100; [ "$status" -eq 1 ]; }
@test "odd-composite" { run is_prime 91; [ "$status" -eq 1 ]; }
@test "negative" { run is_prime -7; [ "$status" -eq 1 ]; }
@test "zero" { run is_prime 0; [ "$status" -eq 1 ]; }
@test "one" { run is_prime 1; [ "$status" -eq 1 ]; }
@test "two" { run is_prime 2; [ "$status" -eq 0 ]; }
@test "three" { run is_prime 3; [ "$status" -eq 0 ]; }
@test "square-of-prime" { run is_prime 25; [ "$status" -eq 1 ]; }
@test "large-square" { run is_prime 4293001441; [ "$status" -eq 1 ]; }
@test "carmichael" { run is_prime 561; [ "$status" -eq 1 ]; }
@test "large-prime" { run is_prime 1000000007; [ "$status" -eq 0 ]; }
@test "mersenne-prime" { run is_prime 2147483647; [ "$status" -eq 0 ]; }
@test "large-composite" { run is_prime 2147483649; [ "$status" -eq 1 ]; }
