#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/slice-a-list/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/slice_list.sh"
}

lines() { # lines ITEM...: prints each item on a line, and nothing for no items
  if (($# > 0)); then printf '%s\n' "$@"; fi
}

check() { # check START STOP "ITEMS" -- WANT...
  local start=$1 stop=$2
  local -a items
  read -r -a items <<<"$3"
  shift 4
  local got want
  # The x keeps the trailing newlines, which $(...) removes.
  got=$(slice_list "$start" "$stop" "${items[@]}"; printf x)
  want=$(lines "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "middle" { check 1 3 "1 2 3 4 5" -- 2 3; }
@test "prefix" { check 0 2 "1 2 3" -- 1 2; }
@test "suffix" { check 1 3 "1 2 3" -- 2 3; }
@test "empty-list" { check 0 0 "" --; }
@test "empty-slice" { check 2 2 "1 2 3" --; }
@test "whole" { check 0 3 "1 2 3" -- 1 2 3; }
@test "single" { check 0 1 "7" -- 7; }
@test "last-one" { check 2 3 "1 2 3" -- 3; }
@test "at-end" { check 3 3 "1 2 3" --; }
@test "limit" { check 0 2 "2147483647 -2147483648 0" -- 2147483647 -2147483648; }
@test "spaces" { got=$(slice_list 1 3 a "New York" Paris; printf x); [ "$got" = $'New York\nParis\nx' ]; }
@test "bad-range" { if slice_list 0 4 1 2 3 >/dev/null; then return 1; fi; if slice_list 2 1 1 2 3 >/dev/null; then return 1; fi; }
