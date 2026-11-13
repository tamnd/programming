#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/search-a-sorted-list/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/index_of.sh"
}

check() { # check FUNCTION GIVEN... -- WANT
  local fn=$1 got
  shift
  local -a given=()
  while [ "$1" != -- ]; do given+=("$1"); shift; done
  shift
  got=$("$fn" "${given[@]}")
  [ "$got" = "$1" ] || { printf 'got:  %s\nwant: %s\n' "$got" "$1"; return 1; }
}

@test "middle" { check index_of 5 1 3 5 7 9 -- 2; }
@test "first" { check index_of 1 1 3 5 7 9 -- 0; }
@test "last" { check index_of 9 1 3 5 7 9 -- 4; }
@test "missing" { check index_of 4 1 3 5 7 9 -- -1; }
@test "below" { check index_of 0 1 3 5 -- -1; }
@test "above" { check index_of 6 1 3 5 -- -1; }
@test "empty" { check index_of 1 -- -1; }
@test "single-hit" { check index_of 7 7 -- 0; }
@test "single-miss" { check index_of 8 7 -- -1; }
@test "duplicates" { check index_of 2 1 2 2 2 3 -- 1; }
@test "all-same" { check index_of 4 4 4 4 4 -- 0; }
@test "negative" { check index_of -5 -9 -5 -1 0 -- 1; }
@test "extremes" { check index_of 2147483647 -2147483648 0 2147483647 -- 2; }
@test "min-target" { check index_of -2147483648 -2147483648 0 2147483647 -- 0; }
