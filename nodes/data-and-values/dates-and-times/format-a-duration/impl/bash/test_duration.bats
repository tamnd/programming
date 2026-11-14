#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/format-a-duration/.

setup() {
  source "$BATS_TEST_DIRNAME/duration.sh"
}

@test "zero" { run format_duration 0; [ "$status" -eq 0 ]; [ "$output" = "00:00:00" ]; }
@test "seconds-only" { run format_duration 59; [ "$status" -eq 0 ]; [ "$output" = "00:00:59" ]; }
@test "one-minute" { run format_duration 60; [ "$status" -eq 0 ]; [ "$output" = "00:01:00" ]; }
@test "one-hour" { run format_duration 3600; [ "$status" -eq 0 ]; [ "$output" = "01:00:00" ]; }
@test "hours-minutes-seconds" { run format_duration 3723; [ "$status" -eq 0 ]; [ "$output" = "01:02:03" ]; }
@test "just-under-an-hour" { run format_duration 3599; [ "$status" -eq 0 ]; [ "$output" = "00:59:59" ]; }
@test "just-under-a-day" { run format_duration 86399; [ "$status" -eq 0 ]; [ "$output" = "23:59:59" ]; }
@test "one-day" { run format_duration 86400; [ "$status" -eq 0 ]; [ "$output" = "24:00:00" ]; }
@test "more-than-a-day" { run format_duration 90061; [ "$status" -eq 0 ]; [ "$output" = "25:01:01" ]; }
@test "hundred-hours" { run format_duration 360000; [ "$status" -eq 0 ]; [ "$output" = "100:00:00" ]; }
@test "one-week" { run format_duration 604800; [ "$status" -eq 0 ]; [ "$output" = "168:00:00" ]; }
@test "one-year" { run format_duration 31536000; [ "$status" -eq 0 ]; [ "$output" = "8760:00:00" ]; }
@test "minus-one-second" { run format_duration -1; [ "$status" -eq 0 ]; [ "$output" = "-00:00:01" ]; }
@test "negative-minute" { run format_duration -61; [ "$status" -eq 0 ]; [ "$output" = "-00:01:01" ]; }
@test "negative-hours" { run format_duration -3723; [ "$status" -eq 0 ]; [ "$output" = "-01:02:03" ]; }
@test "negative-day" { run format_duration -90061; [ "$status" -eq 0 ]; [ "$output" = "-25:01:01" ]; }
@test "beyond-go-duration" { run format_duration 10000000000; [ "$status" -eq 0 ]; [ "$output" = "2777777:46:40" ]; }
@test "largest" { run format_duration 1000000000000000; [ "$status" -eq 0 ]; [ "$output" = "277777777777:46:40" ]; }
@test "most-negative" { run format_duration -1000000000000000; [ "$status" -eq 0 ]; [ "$output" = "-277777777777:46:40" ]; }
