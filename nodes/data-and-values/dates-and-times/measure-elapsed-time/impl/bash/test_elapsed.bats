#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/measure-elapsed-time/.

setup() {
  source "$BATS_TEST_DIRNAME/elapsed.sh"
}

@test "one-and-a-half-ms" { run elapsed_ms 1000000000 1001500000; [ "$status" -eq 0 ]; [ "$output" = "1.500 ms" ]; }
@test "seconds" { run elapsed_ms 0 2345678901; [ "$status" -eq 0 ]; [ "$output" = "2345.678 ms" ]; }
@test "under-one-ms" { run elapsed_ms 50000 750000; [ "$status" -eq 0 ]; [ "$output" = "0.700 ms" ]; }
@test "zero" { run elapsed_ms 123456789 123456789; [ "$status" -eq 0 ]; [ "$output" = "0.000 ms" ]; }
@test "under-one-microsecond" { run elapsed_ms 0 999; [ "$status" -eq 0 ]; [ "$output" = "0.000 ms" ]; }
@test "truncate-not-round" { run elapsed_ms 0 1999999; [ "$status" -eq 0 ]; [ "$output" = "1.999 ms" ]; }
@test "leading-zero-in-fraction" { run elapsed_ms 0 1005000; [ "$status" -eq 0 ]; [ "$output" = "1.005 ms" ]; }
@test "exact-ms" { run elapsed_ms 0 7000000; [ "$status" -eq 0 ]; [ "$output" = "7.000 ms" ]; }
@test "one-hour" { run elapsed_ms 0 3600000000000; [ "$status" -eq 0 ]; [ "$output" = "3600000.000 ms" ]; }
@test "negative-readings" { run elapsed_ms -5000000 -3500000; [ "$status" -eq 0 ]; [ "$output" = "1.500 ms" ]; }
@test "across-zero" { run elapsed_ms -1000000 500000; [ "$status" -eq 0 ]; [ "$output" = "1.500 ms" ]; }
@test "beyond-2-pow-53" { run elapsed_ms 9007199254740993 9007199255741992; [ "$status" -eq 0 ]; [ "$output" = "1.000 ms" ]; }
@test "near-int64-max" { run elapsed_ms 9223372035854775807 9223372036854775807; [ "$status" -eq 0 ]; [ "$output" = "1000.000 ms" ]; }
@test "near-int64-min" { run elapsed_ms -9223372036854775808 -9223372035854775808; [ "$status" -eq 0 ]; [ "$output" = "1000.000 ms" ]; }

# fake_clock prints the next line of $BATS_TEST_TMPDIR/readings. measure runs
# the clock in a subshell, so the position is kept in a file.
fake_clock() {
  local n
  n=$(<"$BATS_TEST_TMPDIR/next")
  echo "$((n + 1))" >"$BATS_TEST_TMPDIR/next"
  echo clock >>"$BATS_TEST_TMPDIR/calls"
  sed -n "$((n + 1))p" "$BATS_TEST_TMPDIR/readings"
}

work() {
  echo work >>"$BATS_TEST_TMPDIR/calls"
}

@test "measure with a fake clock" {
  printf '%s\n' 5000000 6500000 >"$BATS_TEST_TMPDIR/readings"
  echo 0 >"$BATS_TEST_TMPDIR/next"
  run measure fake_clock work
  [ "$status" -eq 0 ]
  [ "$output" = 1500000 ]
  [ "$(paste -sd ' ' "$BATS_TEST_TMPDIR/calls")" = "clock work clock" ]
}

@test "measure with the monotonic clock" {
  run measure monotonic_ns work
  [ "$status" -eq 0 ]
  ((output >= 0))
  [ "$(<"$BATS_TEST_TMPDIR/calls")" = work ]
}

@test "measure returns the status of the command" {
  run measure monotonic_ns false
  [ "$status" -eq 1 ]
}
