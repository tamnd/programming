#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/flatten-a-nested-list/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/flatten.sh"
}

check() { # check WANT LIST...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(flatten "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "three" { check "1\n2\n3\n4\n5\n6\n" "1 2" "3" "4 5 6"; }
@test "two" { check "7\n8\n9\n" "7" "8 9"; }
@test "empty" { check ""; }
@test "empty-inner" { check "1\n" "" "1" ""; }
@test "all-empty" { check "" "" ""; }
@test "single" { check "5\n" "5"; }
@test "one-list" { check "1\n2\n3\n" "1 2 3"; }
@test "negative" { check "-1\n-2\n3\n" "-1 -2" "3"; }
@test "extremes" { check "-2147483648\n2147483647\n0\n" "-2147483648" "2147483647 0"; }
