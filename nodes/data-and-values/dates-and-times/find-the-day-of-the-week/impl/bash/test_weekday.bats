#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-the-day-of-the-week/.

setup() {
  source "$BATS_TEST_DIRNAME/weekday.sh"
}

@test "a-sunday" { run day_of_week 2024 3 10; [ "$status" -eq 0 ]; [ "$output" = 7 ]; }
@test "a-monday" { run day_of_week 2024 3 11; [ "$status" -eq 0 ]; [ "$output" = 1 ]; }
@test "the-epoch" { run day_of_week 1970 1 1; [ "$status" -eq 0 ]; [ "$output" = 4 ]; }
@test "year-2000" { run day_of_week 2000 1 1; [ "$status" -eq 0 ]; [ "$output" = 6 ]; }
@test "a-saturday" { run day_of_week 2024 3 16; [ "$status" -eq 0 ]; [ "$output" = 6 ]; }
@test "leap-day" { run day_of_week 2024 2 29; [ "$status" -eq 0 ]; [ "$output" = 4 ]; }
@test "after-leap-day-2000" { run day_of_week 2000 3 1; [ "$status" -eq 0 ]; [ "$output" = 3 ]; }
@test "year-1900-is-not-leap" { run day_of_week 1900 3 1; [ "$status" -eq 0 ]; [ "$output" = 4 ]; }
@test "year-2100-is-not-leap" { run day_of_week 2100 3 1; [ "$status" -eq 0 ]; [ "$output" = 1 ]; }
@test "before-the-epoch" { run day_of_week 1969 12 31; [ "$status" -eq 0 ]; [ "$output" = 3 ]; }
@test "gregorian-reform" { run day_of_week 1582 10 15; [ "$status" -eq 0 ]; [ "$output" = 5 ]; }
@test "day-before-the-reform" { run day_of_week 1582 10 4; [ "$status" -eq 0 ]; [ "$output" = 1 ]; }
@test "first-day" { run day_of_week 1 1 1; [ "$status" -eq 0 ]; [ "$output" = 1 ]; }
@test "last-day" { run day_of_week 9999 12 31; [ "$status" -eq 0 ]; [ "$output" = 5 ]; }
@test "year-99" { run day_of_week 99 12 31; [ "$status" -eq 0 ]; [ "$output" = 4 ]; }
@test "iso-week-of-next-year" { run day_of_week 2024 12 30; [ "$status" -eq 0 ]; [ "$output" = 1 ]; }
@test "iso-week-of-last-year" { run day_of_week 2027 1 1; [ "$status" -eq 0 ]; [ "$output" = 5 ]; }
@test "month-end" { run day_of_week 2024 4 30; [ "$status" -eq 0 ]; [ "$output" = 2 ]; }
@test "iso week of 2024-03-10" { run iso_week 2024 3 10; [ "$status" -eq 0 ]; [ "$output" = "2024 10" ]; }
@test "iso week of 2024-12-30" { run iso_week 2024 12 30; [ "$status" -eq 0 ]; [ "$output" = "2025 1" ]; }
@test "iso week of 2027-01-01" { run iso_week 2027 1 1; [ "$status" -eq 0 ]; [ "$output" = "2026 53" ]; }
@test "iso week of 2000-01-01" { run iso_week 2000 1 1; [ "$status" -eq 0 ]; [ "$output" = "1999 52" ]; }
@test "iso week of 1969-12-31" { run iso_week 1969 12 31; [ "$status" -eq 0 ]; [ "$output" = "1970 1" ]; }
@test "iso week of 0099-12-31" { run iso_week 99 12 31; [ "$status" -eq 0 ]; [ "$output" = "99 53" ]; }
