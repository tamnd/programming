#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/add-days-to-a-date/.

setup() {
  source "$BATS_TEST_DIRNAME/add.sh"
}

@test "one-day" { run add_to_date 2024 3 10 0 0 1; [ "$status" -eq 0 ]; [ "$output" = "2024-03-11" ]; }
@test "one-month" { run add_to_date 2024 1 15 0 1 0; [ "$status" -eq 0 ]; [ "$output" = "2024-02-15" ]; }
@test "one-year" { run add_to_date 2023 6 1 1 0 0; [ "$status" -eq 0 ]; [ "$output" = "2024-06-01" ]; }
@test "day-into-next-year" { run add_to_date 2023 12 31 0 0 1; [ "$status" -eq 0 ]; [ "$output" = "2024-01-01" ]; }
@test "nothing-to-add" { run add_to_date 2024 2 29 0 0 0; [ "$status" -eq 0 ]; [ "$output" = "2024-02-29" ]; }
@test "month-end-to-leap-february" { run add_to_date 2024 1 31 0 1 0; [ "$status" -eq 0 ]; [ "$output" = "2024-02-29" ]; }
@test "month-end-to-short-february" { run add_to_date 2023 1 31 0 1 0; [ "$status" -eq 0 ]; [ "$output" = "2023-02-28" ]; }
@test "month-end-to-30-day-month" { run add_to_date 2024 3 31 0 1 0; [ "$status" -eq 0 ]; [ "$output" = "2024-04-30" ]; }
@test "back-from-month-end" { run add_to_date 2024 5 31 0 -1 0; [ "$status" -eq 0 ]; [ "$output" = "2024-04-30" ]; }
@test "back-to-leap-february" { run add_to_date 2024 3 31 0 -1 0; [ "$status" -eq 0 ]; [ "$output" = "2024-02-29" ]; }
@test "leap-day-plus-one-year" { run add_to_date 2024 2 29 1 0 0; [ "$status" -eq 0 ]; [ "$output" = "2025-02-28" ]; }
@test "leap-day-plus-four-years" { run add_to_date 2024 2 29 4 0 0; [ "$status" -eq 0 ]; [ "$output" = "2028-02-29" ]; }
@test "year-and-month-at-once" { run add_to_date 2024 2 29 1 1 0; [ "$status" -eq 0 ]; [ "$output" = "2025-03-29" ]; }
@test "months-before-days" { run add_to_date 2024 1 31 0 1 1; [ "$status" -eq 0 ]; [ "$output" = "2024-03-01" ]; }
@test "month-into-next-year" { run add_to_date 2023 12 15 0 1 0; [ "$status" -eq 0 ]; [ "$output" = "2024-01-15" ]; }
@test "thirteen-months-back" { run add_to_date 2024 1 15 0 -13 0; [ "$status" -eq 0 ]; [ "$output" = "2022-12-15" ]; }
@test "day-back-into-last-year" { run add_to_date 2024 1 1 0 0 -1; [ "$status" -eq 0 ]; [ "$output" = "2023-12-31" ]; }
@test "year-1900-is-not-leap" { run add_to_date 1900 2 28 0 0 1; [ "$status" -eq 0 ]; [ "$output" = "1900-03-01" ]; }
@test "year-2100-is-not-leap" { run add_to_date 2096 2 29 4 0 0; [ "$status" -eq 0 ]; [ "$output" = "2100-02-28" ]; }
@test "many-days" { run add_to_date 1970 1 1 0 0 2932896; [ "$status" -eq 0 ]; [ "$output" = "9999-12-31" ]; }
@test "many-days-back" { run add_to_date 9999 12 31 0 0 -3652058; [ "$status" -eq 0 ]; [ "$output" = "0001-01-01" ]; }
@test "many-months" { run add_to_date 1 1 31 0 119987 0; [ "$status" -eq 0 ]; [ "$output" = "9999-12-31" ]; }

@test "days_in_month" {
  [ "$(days_in_month 2024 2)" = 29 ]
  [ "$(days_in_month 1900 2)" = 28 ]
  [ "$(days_in_month 2000 2)" = 29 ]
  [ "$(days_in_month 2024 4)" = 30 ]
}

@test "GNU date alone moves past the end of the month" {
  run date -u -d '2024-01-31 +1 month' +%F
  [ "$output" = "2024-03-02" ]
}
