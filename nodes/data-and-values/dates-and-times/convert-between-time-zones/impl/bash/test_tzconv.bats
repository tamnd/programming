#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/convert-between-time-zones/.

setup() {
  source "$BATS_TEST_DIRNAME/tzconv.sh"
}

@test "new-york-to-utc" { run convert_time_zone 2024 7 1 12 0 0 America/New_York UTC; [ "$status" -eq 0 ]; [ "$output" = "2024-07-01T16:00:00+00:00" ]; }
@test "utc-to-tokyo" { run convert_time_zone 2024 1 15 0 0 0 UTC Asia/Tokyo; [ "$status" -eq 0 ]; [ "$output" = "2024-01-15T09:00:00+09:00" ]; }
@test "london-to-new-york" { run convert_time_zone 2024 1 15 12 0 0 Europe/London America/New_York; [ "$status" -eq 0 ]; [ "$output" = "2024-01-15T07:00:00-05:00" ]; }
@test "date-changes" { run convert_time_zone 2024 1 15 22 0 0 America/Los_Angeles Asia/Tokyo; [ "$status" -eq 0 ]; [ "$output" = "2024-01-16T15:00:00+09:00" ]; }
@test "same-zone" { run convert_time_zone 2024 3 10 12 0 0 America/New_York America/New_York; [ "$status" -eq 0 ]; [ "$output" = "2024-03-10T12:00:00-04:00" ]; }
@test "nonexistent-time" { run convert_time_zone 2024 3 10 2 30 0 America/New_York UTC; [ "$status" -eq 0 ]; [ "$output" = "2024-03-10T07:30:00+00:00" ]; }
@test "ambiguous-time" { run convert_time_zone 2024 11 3 1 30 0 America/New_York UTC; [ "$status" -eq 0 ]; [ "$output" = "2024-11-03T05:30:00+00:00" ]; }
@test "ambiguous-time-london" { run convert_time_zone 2024 10 27 1 30 0 Europe/London UTC; [ "$status" -eq 0 ]; [ "$output" = "2024-10-27T00:30:00+00:00" ]; }
@test "dst-dates-differ" { run convert_time_zone 2024 3 20 12 0 0 America/New_York Europe/London; [ "$status" -eq 0 ]; [ "$output" = "2024-03-20T16:00:00+00:00" ]; }
@test "southern-hemisphere" { run convert_time_zone 2024 1 15 12 0 0 Australia/Sydney UTC; [ "$status" -eq 0 ]; [ "$output" = "2024-01-15T01:00:00+00:00" ]; }
@test "half-hour-offset" { run convert_time_zone 2024 1 15 12 0 0 Asia/Kolkata UTC; [ "$status" -eq 0 ]; [ "$output" = "2024-01-15T06:30:00+00:00" ]; }
@test "45-minute-offset" { run convert_time_zone 2024 1 15 12 0 0 UTC Asia/Kathmandu; [ "$status" -eq 0 ]; [ "$output" = "2024-01-15T17:45:00+05:45" ]; }
@test "half-hour-dst" { run convert_time_zone 2024 1 15 12 0 0 Australia/Lord_Howe UTC; [ "$status" -eq 0 ]; [ "$output" = "2024-01-15T01:00:00+00:00" ]; }
@test "across-the-date-line" { run convert_time_zone 2024 1 15 12 0 0 Pacific/Kiritimati Pacific/Pago_Pago; [ "$status" -eq 0 ]; [ "$output" = "2024-01-14T11:00:00-11:00" ]; }
@test "before-skipped-day" { run convert_time_zone 2011 12 29 12 0 0 Pacific/Apia UTC; [ "$status" -eq 0 ]; [ "$output" = "2011-12-29T22:00:00+00:00" ]; }
@test "skipped-day" { run convert_time_zone 2011 12 30 12 0 0 Pacific/Apia UTC; [ "$status" -eq 0 ]; [ "$output" = "2011-12-30T22:00:00+00:00" ]; }
@test "after-skipped-day" { run convert_time_zone 2011 12 31 12 0 0 Pacific/Apia UTC; [ "$status" -eq 0 ]; [ "$output" = "2011-12-30T22:00:00+00:00" ]; }
@test "old-dst-rule" { run convert_time_zone 2006 3 15 12 0 0 America/New_York UTC; [ "$status" -eq 0 ]; [ "$output" = "2006-03-15T17:00:00+00:00" ]; }
@test "posix-sign" { run convert_time_zone 2024 1 15 12 0 0 UTC Etc/GMT+5; [ "$status" -eq 0 ]; [ "$output" = "2024-01-15T07:00:00-05:00" ]; }
@test "result-before-epoch" { run convert_time_zone 1970 1 1 0 0 0 UTC America/New_York; [ "$status" -eq 0 ]; [ "$output" = "1969-12-31T19:00:00-05:00" ]; }
@test "far-future" { run convert_time_zone 2050 7 1 12 0 0 America/New_York UTC; [ "$status" -eq 0 ]; [ "$output" = "2050-07-01T16:00:00+00:00" ]; }
@test "unknown-zone" { run convert_time_zone 2024 1 1 0 0 0 Mars/Olympus UTC; [ "$status" -eq 1 ]; }
