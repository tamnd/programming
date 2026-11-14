#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-difference-of-two-dates/.

setup() {
  source "$BATS_TEST_DIRNAME/diff.sh"
}

@test "same-day" { run days_between 2024 3 10 2024 3 10; [ "$status" -eq 0 ]; [ "$output" = 0 ]; }
@test "next-day" { run days_between 2024 3 10 2024 3 11; [ "$status" -eq 0 ]; [ "$output" = 1 ]; }
@test "common-year" { run days_between 2023 1 1 2024 1 1; [ "$status" -eq 0 ]; [ "$output" = 365 ]; }
@test "leap-year" { run days_between 2024 1 1 2025 1 1; [ "$status" -eq 0 ]; [ "$output" = 366 ]; }
@test "end-before-start" { run days_between 2024 3 11 2024 3 10; [ "$status" -eq 0 ]; [ "$output" = -1 ]; }
@test "across-a-dst-change" { run days_between 2024 3 9 2024 3 11; [ "$status" -eq 0 ]; [ "$output" = 2 ]; }
@test "across-leap-day" { run days_between 2024 2 28 2024 3 1; [ "$status" -eq 0 ]; [ "$output" = 2 ]; }
@test "month-end-to-next-month" { run days_between 2024 1 31 2024 3 1; [ "$status" -eq 0 ]; [ "$output" = 30 ]; }
@test "year-1900-is-not-leap" { run days_between 1900 1 1 1901 1 1; [ "$status" -eq 0 ]; [ "$output" = 365 ]; }
@test "year-2000-is-leap" { run days_between 2000 1 1 2001 1 1; [ "$status" -eq 0 ]; [ "$output" = 366 ]; }
@test "400-years" { run days_between 2000 1 1 2400 1 1; [ "$status" -eq 0 ]; [ "$output" = 146097 ]; }
@test "across-the-epoch" { run days_between 1969 12 31 1970 1 1; [ "$status" -eq 0 ]; [ "$output" = 1 ]; }
@test "gregorian-reform-gap" { run days_between 1582 10 4 1582 10 15; [ "$status" -eq 0 ]; [ "$output" = 11 ]; }
@test "more-than-292-years" { run days_between 1700 1 1 2300 1 1; [ "$status" -eq 0 ]; [ "$output" = 219145 ]; }
@test "whole-range" { run days_between 1 1 1 9999 12 31; [ "$status" -eq 0 ]; [ "$output" = 3652058 ]; }
@test "whole-range-backward" { run days_between 9999 12 31 1 1 1; [ "$status" -eq 0 ]; [ "$output" = -3652058 ]; }
@test "year-99" { run days_between 99 12 31 100 1 1; [ "$status" -eq 0 ]; [ "$output" = 1 ]; }
