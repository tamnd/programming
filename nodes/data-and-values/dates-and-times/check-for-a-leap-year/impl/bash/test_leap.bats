#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/check-for-a-leap-year/.

setup() {
  source "$BATS_TEST_DIRNAME/leap.sh"
}

@test "divisible-by-4" { run is_leap_year 2024; [ "$status" -eq 0 ]; }
@test "not-divisible-by-4" { run is_leap_year 2023; [ "$status" -eq 1 ]; }
@test "century" { run is_leap_year 1900; [ "$status" -eq 1 ]; }
@test "divisible-by-400" { run is_leap_year 2000; [ "$status" -eq 0 ]; }
@test "next-century" { run is_leap_year 2100; [ "$status" -eq 1 ]; }
@test "year-1600" { run is_leap_year 1600; [ "$status" -eq 0 ]; }
@test "year-1700" { run is_leap_year 1700; [ "$status" -eq 1 ]; }
@test "year-2400" { run is_leap_year 2400; [ "$status" -eq 0 ]; }
@test "year-1" { run is_leap_year 1; [ "$status" -eq 1 ]; }
@test "year-4" { run is_leap_year 4; [ "$status" -eq 0 ]; }
@test "year-100" { run is_leap_year 100; [ "$status" -eq 1 ]; }
@test "year-1500-julian-leap" { run is_leap_year 1500; [ "$status" -eq 1 ]; }
@test "year-1582" { run is_leap_year 1582; [ "$status" -eq 1 ]; }
@test "year-9999" { run is_leap_year 9999; [ "$status" -eq 1 ]; }
@test "year-zero" { run is_leap_year 0; [ "$status" -eq 0 ]; }
@test "year-minus-1" { run is_leap_year -1; [ "$status" -eq 1 ]; }
@test "year-minus-4" { run is_leap_year -4; [ "$status" -eq 0 ]; }
@test "year-minus-100" { run is_leap_year -100; [ "$status" -eq 1 ]; }
@test "year-minus-400" { run is_leap_year -400; [ "$status" -eq 0 ]; }
