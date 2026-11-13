#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/reduce-a-list-to-a-value/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/total.sh"
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

@test "small" { check total 1 2 3 4 -- 10; }
@test "mixed" { check total 10 -3 5 -- 12; }
@test "empty" { check total -- 0; }
@test "single" { check total 7 -- 7; }
@test "negative" { check total -1 -2 -3 -- -6; }
@test "zero-sum" { check total 5 -5 -- 0; }
@test "max" { check total 2147483647 -- 2147483647; }
@test "min" { check total -2147483648 -- -2147483648; }
@test "to-the-limit" { check total 2147483646 1 -- 2147483647; }
@test "partial-overflow" { check total 2147483647 1 -1 -- 2147483647; }
@test "partial-underflow" { check total -2147483648 -1 1 -- -2147483648; }
