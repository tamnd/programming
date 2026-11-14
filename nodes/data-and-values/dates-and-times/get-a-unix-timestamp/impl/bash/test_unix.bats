#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-a-unix-timestamp/.

setup() {
  source "$BATS_TEST_DIRNAME/unix.sh"
}

@test "epoch" { run unix_timestamp 1970 1 1 0 0 0 0; [ "$status" -eq 0 ]; [ "$output" = "0" ]; }
@test "billennium" { run unix_timestamp 2001 9 9 1 46 40 0; [ "$status" -eq 0 ]; [ "$output" = "1000000000" ]; }
@test "negative-offset" { run unix_timestamp 2024 3 10 2 0 0 -300; [ "$status" -eq 0 ]; [ "$output" = "1710054000" ]; }
@test "positive-offset" { run unix_timestamp 2024 3 10 16 0 0 540; [ "$status" -eq 0 ]; [ "$output" = "1710054000" ]; }
@test "one-second-before-epoch" { run unix_timestamp 1969 12 31 23 59 59 0; [ "$status" -eq 0 ]; [ "$output" = "-1" ]; }
@test "offset-moves-before-epoch" { run unix_timestamp 1970 1 1 0 0 0 60; [ "$status" -eq 0 ]; [ "$output" = "-3600" ]; }
@test "offset-moves-to-next-year" { run unix_timestamp 2023 12 31 19 0 0 -300; [ "$status" -eq 0 ]; [ "$output" = "1704067200" ]; }
@test "offset-of-45-minutes" { run unix_timestamp 2024 1 1 5 45 0 345; [ "$status" -eq 0 ]; [ "$output" = "1704067200" ]; }
@test "offset-of-minus-9-30" { run unix_timestamp 2023 12 31 14 30 0 -570; [ "$status" -eq 0 ]; [ "$output" = "1704067200" ]; }
@test "offset-plus-14" { run unix_timestamp 1970 1 1 14 0 0 840; [ "$status" -eq 0 ]; [ "$output" = "0" ]; }
@test "offset-minus-12" { run unix_timestamp 1969 12 31 12 0 0 -720; [ "$status" -eq 0 ]; [ "$output" = "0" ]; }
@test "leap-day" { run unix_timestamp 2024 2 29 0 0 0 0; [ "$status" -eq 0 ]; [ "$output" = "1709164800" ]; }
@test "year-2000-is-leap" { run unix_timestamp 2000 2 29 0 0 0 0; [ "$status" -eq 0 ]; [ "$output" = "951782400" ]; }
@test "year-1900-is-not-leap" { run unix_timestamp 1900 3 1 0 0 0 0; [ "$status" -eq 0 ]; [ "$output" = "-2203891200" ]; }
@test "int32-max" { run unix_timestamp 2038 1 19 3 14 7 0; [ "$status" -eq 0 ]; [ "$output" = "2147483647" ]; }
@test "after-int32-max" { run unix_timestamp 2038 1 19 3 14 8 0; [ "$status" -eq 0 ]; [ "$output" = "2147483648" ]; }
@test "int32-min" { run unix_timestamp 1901 12 13 20 45 52 0; [ "$status" -eq 0 ]; [ "$output" = "-2147483648" ]; }
@test "year-99-is-not-1999" { run unix_timestamp 99 12 31 23 59 59 0; [ "$status" -eq 0 ]; [ "$output" = "-59011459201" ]; }
@test "first-second-of-year-1" { run unix_timestamp 1 1 1 0 0 0 0; [ "$status" -eq 0 ]; [ "$output" = "-62135596800" ]; }
@test "last-second-of-year-9999" { run unix_timestamp 9999 12 31 23 59 59 0; [ "$status" -eq 0 ]; [ "$output" = "253402300799" ]; }

@test "back to a date" {
  run date -u -d @1710054000 +%Y-%m-%dT%H:%M:%S
  [ "$output" = "2024-03-10T07:00:00" ]
  run date -u -d @-62135596800 +%Y-%m-%dT%H:%M:%S
  [ "$output" = "0001-01-01T00:00:00" ]
}

@test "now" {
  seconds=$(now_unix_seconds)
  millis=$(now_unix_millis)
  [[ $seconds =~ ^[0-9]+$ ]]
  [[ $millis =~ ^[0-9]+$ ]]
  (( seconds >= 1704067200 ))
  diff=$(( millis / 1000 - seconds ))
  (( diff >= 0 && diff <= 1 ))
}
