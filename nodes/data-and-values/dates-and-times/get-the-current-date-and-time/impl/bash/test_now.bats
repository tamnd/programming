#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-current-date-and-time/.

setup() {
  source "$BATS_TEST_DIRNAME/now.sh"
}

@test "epoch" { run utc_timestamp 0; [ "$status" -eq 0 ]; [ "$output" = "1970-01-01T00:00:00.000Z" ]; }
@test "billennium" { run utc_timestamp 1000000000000000000; [ "$status" -eq 0 ]; [ "$output" = "2001-09-09T01:46:40.000Z" ]; }
@test "with-milliseconds" { run utc_timestamp 1710054000123456789; [ "$status" -eq 0 ]; [ "$output" = "2024-03-10T07:00:00.123Z" ]; }
@test "leap-day" { run utc_timestamp 1709208000000000000; [ "$status" -eq 0 ]; [ "$output" = "2024-02-29T12:00:00.000Z" ]; }
@test "truncate-not-round" { run utc_timestamp 999999; [ "$status" -eq 0 ]; [ "$output" = "1970-01-01T00:00:00.000Z" ]; }
@test "one-ns-before-epoch" { run utc_timestamp -1; [ "$status" -eq 0 ]; [ "$output" = "1969-12-31T23:59:59.999Z" ]; }
@test "one-ms-before-epoch" { run utc_timestamp -1000000; [ "$status" -eq 0 ]; [ "$output" = "1969-12-31T23:59:59.999Z" ]; }
@test "just-over-one-ms-before-epoch" { run utc_timestamp -1000001; [ "$status" -eq 0 ]; [ "$output" = "1969-12-31T23:59:59.998Z" ]; }
@test "last-ms-of-a-year" { run utc_timestamp 1704067199999999999; [ "$status" -eq 0 ]; [ "$output" = "2023-12-31T23:59:59.999Z" ]; }
@test "year-2000-is-leap" { run utc_timestamp 951782400000000000; [ "$status" -eq 0 ]; [ "$output" = "2000-02-29T00:00:00.000Z" ]; }
@test "year-2100-is-not-leap" { run utc_timestamp 4107542399999999999; [ "$status" -eq 0 ]; [ "$output" = "2100-02-28T23:59:59.999Z" ]; }
@test "after-int32-seconds" { run utc_timestamp 2147483648000000000; [ "$status" -eq 0 ]; [ "$output" = "2038-01-19T03:14:08.000Z" ]; }
@test "int64-max" { run utc_timestamp 9223372036854775807; [ "$status" -eq 0 ]; [ "$output" = "2262-04-11T23:47:16.854Z" ]; }
@test "int64-min" { run utc_timestamp -9223372036854775808; [ "$status" -eq 0 ]; [ "$output" = "1677-09-21T00:12:43.145Z" ]; }

# fake_clock prints one fixed reading and logs the call. now_utc runs the clock
# in a command substitution, so the log is a file.
fake_clock() {
  echo clock >>"$BATS_TEST_TMPDIR/calls"
  echo 1710054000123456789
}

@test "now_utc with a fake clock" {
  run now_utc fake_clock
  [ "$status" -eq 0 ]
  [ "$output" = "2024-03-10T07:00:00.123Z" ]
  [ "$(<"$BATS_TEST_TMPDIR/calls")" = "clock" ]
}

@test "now_utc with the real clock" {
  run now_utc
  [ "$status" -eq 0 ]
  [[ "$output" =~ ^[0-9]{4}-[0-9]{2}-[0-9]{2}T[0-9]{2}:[0-9]{2}:[0-9]{2}\.[0-9]{3}Z$ ]]
  [ "${output:0:4}" -ge 2024 ]
}

@test "system_ns prints whole nanoseconds after 2024" {
  run system_ns
  [ "$status" -eq 0 ]
  [[ "$output" =~ ^[0-9]+000$ ]]
  ((output >= 1704067200000000000))
}
