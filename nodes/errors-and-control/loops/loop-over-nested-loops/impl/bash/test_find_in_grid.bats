#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/loop-over-nested-loops/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/find_in_grid.sh"
}

check() { # check WANT TARGET ROW...
  local want=$1
  shift
  local got
  got=$(find_in_grid "$@"; printf x)
  [ "$got" = "$want"$'\n'x ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "middle" { check "1 1" 5 "1 2 3" "4 5 6" "7 8 9"; }
@test "first-twenty" { check "1 1" 20 "3 8 12" "5 20 20" "20 1 4"; }
@test "first-cell" { check "0 0" 9 "9 1" "2 3"; }
@test "last-cell" { check "1 1" 9 "1 2" "3 9"; }
@test "row-major" { check "0 2" 9 "1 2 9" "9 3 4"; }
@test "missing" { check "-1 -1" 9 "1 2" "3 4"; }
@test "empty-grid" { check "-1 -1" 1; }
@test "empty-rows" { check "2 0" 5 "" "" "5"; }
@test "ragged" { check "2 1" 6 "1" "2 3 4" "5 6"; }
@test "negative" { check "1 0" -3 "-1 -2" "-3 -4"; }
