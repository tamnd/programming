#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/zip-two-lists/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/zip_pairs.sh"
}

check() { # check WANT A_ITEMS B_ITEMS
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  got=$(zip_pairs "$2" "$3"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "same-length" { check "1 4\n2 5\n3 6\n" "1 2 3" "4 5 6"; }
@test "tens" { check "10 30\n20 40\n" "10 20" "30 40"; }
@test "empty" { check "" "" ""; }
@test "first-empty" { check "" "" "1 2"; }
@test "second-empty" { check "" "1 2" ""; }
@test "first-shorter" { check "1 7\n" "1" "7 8"; }
@test "second-shorter" { check "1 9\n" "1 2 3" "9"; }
@test "negative" { check "-1 3\n-2 -4\n" "-1 -2" "3 -4"; }
@test "extremes" { check "-2147483648 2147483647\n2147483647 -2147483648\n" "-2147483648 2147483647" "2147483647 -2147483648"; }
@test "duplicates" { check "5 5\n5 5\n" "5 5" "5 5"; }
