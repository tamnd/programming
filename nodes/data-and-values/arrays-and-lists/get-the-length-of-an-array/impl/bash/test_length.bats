#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/get-the-length-of-an-array/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/length.sh"
}

check() { # check WANT ITEM...
  local want=$1 got
  shift
  got=$(length "$@")
  [ "$got" = "$want" ] || { printf 'got:  %s\nwant: %s\n' "$got" "$want"; return 1; }
}

@test "three" { check 3 "a" "b" "c"; }
@test "two-words" { check 2 "apple" "banana"; }
@test "empty" { check 0; }
@test "single" { check 1 "x"; }
@test "empty-strings" { check 2 "" ""; }
@test "non-ascii" { check 2 "é" "日本語"; }
@test "duplicates" { check 3 "a" "a" "a"; }
@test "ten" { check 10 "0" "1" "2" "3" "4" "5" "6" "7" "8" "9"; }
