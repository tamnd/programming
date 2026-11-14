#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sleep-for-a-duration/.

setup() {
  source "$BATS_TEST_DIRNAME/sleep.sh"
}

@test "one-call" { run sleep_requests 1000000 0 1000000; [ "$status" -eq 0 ]; [ "$output" = "1000000" ]; }
@test "woke-late" { run sleep_requests 1000000 0 1080000; [ "$status" -eq 0 ]; [ "$output" = "1000000" ]; }
@test "woke-early" { run sleep_requests 1000000 0 600000 1000000; [ "$status" -eq 0 ]; [ "$output" = "1000000 400000" ]; }
@test "one-second" { run sleep_requests 1000000000 5000000000 6000050000; [ "$status" -eq 0 ]; [ "$output" = "1000000000" ]; }
@test "zero" { run sleep_requests 0 0; [ "$status" -eq 0 ]; [ "$output" = "" ]; }
@test "negative" { run sleep_requests -5000000 0; [ "$status" -eq 0 ]; [ "$output" = "" ]; }
@test "most-negative" { run sleep_requests -4611686018427387904 0; [ "$status" -eq 0 ]; [ "$output" = "" ]; }
@test "several-early-wakes" { run sleep_requests 10000000 0 2000000 5000000 9999999 10000000; [ "$status" -eq 0 ]; [ "$output" = "10000000 8000000 5000000 1" ]; }
@test "no-progress" { run sleep_requests 1000000 0 0 1000000; [ "$status" -eq 0 ]; [ "$output" = "1000000 1000000" ]; }
@test "exact-deadline" { run sleep_requests 3000000 100 3000100; [ "$status" -eq 0 ]; [ "$output" = "3000000" ]; }
@test "one-ns-short" { run sleep_requests 3000000 0 2999999 3000000; [ "$status" -eq 0 ]; [ "$output" = "3000000 1" ]; }
@test "extra-readings" { run sleep_requests 1000000 0 1000000 5000000; [ "$status" -eq 0 ]; [ "$output" = "1000000" ]; }
@test "negative-origin" { run sleep_requests 2000000000 -5000000000 -4000000000 -3000000000; [ "$status" -eq 0 ]; [ "$output" = "2000000000 1000000000" ]; }
@test "origin-after-100-days" { run sleep_requests 1500000 8640000000000000 8640000001000000 8640000001500000; [ "$status" -eq 0 ]; [ "$output" = "1500000 500000" ]; }
@test "one-nanosecond" { run sleep_requests 1 0 1000000; [ "$status" -eq 0 ]; [ "$output" = "1" ]; }
@test "sub-millisecond" { run sleep_requests 1500000 0 1000000 1500000; [ "$status" -eq 0 ]; [ "$output" = "1500000 500000" ]; }
@test "one-hour" { run sleep_requests 3600000000000 0 3600000000000; [ "$status" -eq 0 ]; [ "$output" = "3600000000000" ]; }
@test "thirty-days" { run sleep_requests 2592000000000000 0 2147483647000000 2592000000000000; [ "$status" -eq 0 ]; [ "$output" = "2592000000000000 444516353000000" ]; }

@test "sleep_for_ns waits at least the duration" {
  local start=$EPOCHREALTIME
  sleep_for_ns 20000000
  local end=$EPOCHREALTIME
  # EPOCHREALTIME has six decimal places, so the digits without the point are microseconds.
  (( ${end/[.,]/} - ${start/[.,]/} >= 20000 ))
}

@test "sleep_for_ns returns at once for zero and a negative duration" {
  local start=$EPOCHREALTIME
  sleep_for_ns 0
  sleep_for_ns -5000000
  local end=$EPOCHREALTIME
  (( ${end/[.,]/} - ${start/[.,]/} < 1000000 ))
}
