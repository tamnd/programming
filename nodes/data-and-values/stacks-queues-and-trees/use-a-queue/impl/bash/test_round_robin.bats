#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-queue/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/round_robin.sh"
}

check() { # check WANT SLICE WORK...
  local want=$1
  shift
  run finish_order "$@"
  [ "$status" -eq 0 ]
  [ "$output" = "$want" ] || { printf 'got:  %s\nwant: %s\n' "$output" "$want"; return 1; }
}

@test "example" { check '1 2 0' 1 3 1 2; }
@test "slice-two" { check '1 3 0 2' 2 4 1 3 2; }
@test "empty" { check '' 3; }
@test "one" { check '0' 2 7; }
@test "big-slice" { check '0 1 2' 10 3 1 2; }
@test "slice-one" { check '2 0 1' 1 2 2 1; }
@test "equal-work" { check '0 1 2' 1 2 2 2; }
@test "long-job-first" { check '1 2 0' 3 10 1 1; }
