#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/map-over-a-list/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/square_all.sh"
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

@test "small" { check square_all 1 2 3 -- 1 4 9; }
@test "tens" { check square_all 5 10 -- 25 100; }
@test "empty" { check square_all --; }
@test "single" { check square_all 7 -- 49; }
@test "negative" { check square_all -3 -1 -- 9 1; }
@test "zero" { check square_all 0 -- 0; }
@test "limit" { check square_all 46340 -46340 -- 2147395600 2147395600; }
@test "order" { check square_all 3 1 2 -- 9 1 4; }
@test "duplicates" { check square_all 2 2 -- 4 4; }
