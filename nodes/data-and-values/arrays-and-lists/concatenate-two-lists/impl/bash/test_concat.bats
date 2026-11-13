#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/concatenate-two-lists/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/concat.sh"
}

lines() { # lines ITEM...: prints each item on a line, and nothing for no items
  if (($# > 0)); then printf '%s\n' "$@"; fi
}

check() { # check "A ITEMS" "B ITEMS" -- WANT...
  local -a left right
  read -r -a left <<<"$1"
  read -r -a right <<<"$2"
  shift 3
  local got want
  # The x keeps the trailing newlines, which $(...) removes.
  got=$(concat left right; printf x)
  want=$(lines "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "two-pairs" { check "1 2" "3 4" -- 1 2 3 4; }
@test "uneven" { check "5" "6 7 8" -- 5 6 7 8; }
@test "both-empty" { check "" "" --; }
@test "left-empty" { check "" "1 2" -- 1 2; }
@test "right-empty" { check "1 2" "" -- 1 2; }
@test "duplicates" { check "1 1" "1" -- 1 1 1; }
@test "order" { check "3 1" "2" -- 3 1 2; }
@test "limit" { check "2147483647" "-2147483648" -- 2147483647 -2147483648; }
@test "spaces" { local -a x=("New York" Paris) y=(Rome); got=$(concat x y; printf x); [ "$got" = $'New York\nParis\nRome\nx' ]; }
