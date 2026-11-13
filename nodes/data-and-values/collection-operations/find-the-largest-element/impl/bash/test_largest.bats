#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/find-the-largest-element/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/largest.sh"
}

lines() { # lines ITEM...: prints each item on a line, and nothing for no items
  if (($# > 0)); then printf '%s\n' "$@"; fi
}

check() { # check FUNCTION GIVEN... -- WANT...
  local fn=$1 got want
  shift
  local -a given=()
  while [ "$1" != -- ]; do given+=("$1"); shift; done
  shift
  # The x keeps the trailing newlines, which $(...) removes.
  got=$("$fn" "${given[@]}"; printf x)
  want=$(lines "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "digits" { check largest 3 1 4 1 5 -- 5; }
@test "middle" { check largest 10 20 15 -- 20; }
@test "single" { check largest 7 -- 7; }
@test "all-negative" { check largest -3 -1 -2 -- -1; }
@test "first" { check largest 9 2 3 -- 9; }
@test "last" { check largest 1 2 9 -- 9; }
@test "duplicates" { check largest 5 9 9 1 -- 9; }
@test "all-same" { check largest 4 4 4 -- 4; }
@test "zero" { check largest -5 0 -1 -- 0; }
@test "extremes" { check largest -2147483648 2147483647 0 -- 2147483647; }
@test "min-only" { check largest -2147483648 -- -2147483648; }
