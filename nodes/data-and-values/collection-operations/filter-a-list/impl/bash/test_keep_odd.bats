#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/filter-a-list/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/keep_odd.sh"
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

@test "mixed" { check keep_odd 1 2 3 4 5 -- 1 3 5; }
@test "tens" { check keep_odd 10 15 20 25 -- 15 25; }
@test "empty" { check keep_odd --; }
@test "none-odd" { check keep_odd 2 4 6 --; }
@test "all-odd" { check keep_odd 1 3 5 -- 1 3 5; }
@test "negative" { check keep_odd -3 -2 -1 0 -- -3 -1; }
@test "zero" { check keep_odd 0 --; }
@test "order" { check keep_odd 9 1 7 -- 9 1 7; }
@test "extremes" { check keep_odd -2147483648 2147483647 -- 2147483647; }
@test "duplicates" { check keep_odd 3 3 2 3 -- 3 3 3; }
