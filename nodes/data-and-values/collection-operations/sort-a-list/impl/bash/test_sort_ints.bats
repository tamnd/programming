#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/sort-a-list/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/sort_ints.sh"
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

@test "small" { check sort_ints 3 1 2 -- 1 2 3; }
@test "five" { check sort_ints 5 4 1 3 2 -- 1 2 3 4 5; }
@test "empty" { check sort_ints --; }
@test "single" { check sort_ints 1 -- 1; }
@test "sorted" { check sort_ints 1 2 3 -- 1 2 3; }
@test "reversed" { check sort_ints 9 8 7 -- 7 8 9; }
@test "duplicates" { check sort_ints 2 1 2 1 -- 1 1 2 2; }
@test "negative" { check sort_ints -1 -10 5 -- -10 -1 5; }
@test "digits" { check sort_ints 10 9 100 1 -- 1 9 10 100; }
@test "extremes" { check sort_ints 2147483647 -2147483648 0 -- -2147483648 0 2147483647; }
