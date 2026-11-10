#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/parse-and-format-a-date/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/to_utc.sh"
}

converts() { # converts TIMESTAMP WANT
  run --separate-stderr to_utc "$1"
  [ "$status" -eq 0 ]
  [ "$output" = "$2" ]
}

rejects() { # rejects TIMESTAMP
  run --separate-stderr to_utc "$1"
  [ "$status" -eq 1 ]
  [ -z "$output" ]
  [ -n "$stderr" ]
}

@test "utc" { converts "2026-11-10T12:00:00Z" "2026-11-10T12:00:00.000Z"; }
@test "positive-offset" { converts "2026-11-10T19:30:00+07:00" "2026-11-10T12:30:00.000Z"; }
@test "negative-offset" { converts "2026-11-10T07:00:00-05:00" "2026-11-10T12:00:00.000Z"; }
@test "back-a-day" { converts "2026-11-11T01:00:00+07:00" "2026-11-10T18:00:00.000Z"; }
@test "forward-a-day" { converts "2026-11-10T22:00:00-05:00" "2026-11-11T03:00:00.000Z"; }
@test "year-end" { converts "2027-01-01T00:30:00+01:00" "2026-12-31T23:30:00.000Z"; }
@test "leap-day" { converts "2024-02-29T23:00:00-02:00" "2024-03-01T01:00:00.000Z"; }
@test "half-hour-offset" { converts "2026-11-10T17:30:00+05:30" "2026-11-10T12:00:00.000Z"; }
@test "quarter-hour-offset" { converts "2026-11-10T17:45:00+05:45" "2026-11-10T12:00:00.000Z"; }
@test "unknown-offset" { converts "2026-11-10T12:00:00-00:00" "2026-11-10T12:00:00.000Z"; }
@test "fraction-one-digit" { converts "2026-11-10T12:00:00.5Z" "2026-11-10T12:00:00.500Z"; }
@test "fraction-three-digits" { converts "2026-11-10T12:00:00.123+01:00" "2026-11-10T11:00:00.123Z"; }
@test "before-epoch" { converts "1970-01-01T00:30:00+01:00" "1969-12-31T23:30:00.000Z"; }
@test "before-epoch-fraction" { converts "1969-12-31T23:59:59.999Z" "1969-12-31T23:59:59.999Z"; }
@test "largest-offset" { converts "2026-11-10T00:00:00+14:00" "2026-11-09T10:00:00.000Z"; }
@test "smallest-offset" { converts "2026-11-10T00:00:00-12:00" "2026-11-10T12:00:00.000Z"; }
@test "lower-case" { converts "2026-11-10t12:00:00z" "2026-11-10T12:00:00.000Z"; }
@test "year-one" { converts "0001-01-01T00:00:00Z" "0001-01-01T00:00:00.000Z"; }
@test "year-9999" { converts "9999-12-31T23:59:59.999Z" "9999-12-31T23:59:59.999Z"; }
@test "no-offset" { rejects "2026-11-10T12:00:00"; }
@test "date-only" { rejects "2026-11-10"; }
@test "space-separator" { rejects "2026-11-10 12:00:00Z"; }
@test "no-seconds" { rejects "2026-11-10T12:00Z"; }
@test "offset-no-colon" { rejects "2026-11-10T12:00:00+0700"; }
@test "offset-hours-only" { rejects "2026-11-10T12:00:00+07"; }
@test "not-a-leap-year" { rejects "2026-02-29T12:00:00Z"; }
@test "april-31" { rejects "2026-04-31T12:00:00Z"; }
@test "month-13" { rejects "2026-13-01T12:00:00Z"; }
@test "day-zero" { rejects "2026-11-00T12:00:00Z"; }
@test "hour-24" { rejects "2026-11-10T24:00:00Z"; }
@test "minute-60" { rejects "2026-11-10T12:60:00Z"; }
@test "leap-second" { rejects "2016-12-31T23:59:60Z"; }
@test "offset-hour-24" { rejects "2026-11-10T12:00:00+24:00"; }
@test "offset-minute-60" { rejects "2026-11-10T12:00:00+07:60"; }
@test "comma-fraction" { rejects "2026-11-10T12:00:00,5Z"; }
@test "trailing-space" { rejects "2026-11-10T12:00:00Z "; }
@test "empty" { rejects ""; }
