#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/rotate-a-list/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/rotate_left.sh"
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

@test "by-two" { check rotate_left 2 1 2 3 4 5 -- 3 4 5 1 2; }
@test "by-one" { check rotate_left 1 1 2 3 -- 2 3 1; }
@test "zero" { check rotate_left 0 1 2 3 -- 1 2 3; }
@test "full-turn" { check rotate_left 3 1 2 3 -- 1 2 3; }
@test "more-than-length" { check rotate_left 4 1 2 3 -- 2 3 1; }
@test "much-larger" { check rotate_left 2000000000 1 2 3 -- 3 1 2; }
@test "empty" { check rotate_left 5 --; }
@test "empty-zero" { check rotate_left 0 --; }
@test "single" { check rotate_left 3 7 -- 7; }
@test "duplicates" { check rotate_left 1 1 1 2 -- 1 2 1; }
@test "negative-items" { check rotate_left 1 -1 -2 -3 -- -2 -3 -1; }
