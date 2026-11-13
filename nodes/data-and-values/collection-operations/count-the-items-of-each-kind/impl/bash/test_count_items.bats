#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/count-the-items-of-each-kind/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/count_items.sh"
}

check() { # check WANT GIVEN...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(count_items "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "mixed" { check "1 2\n2 1\n3 3\n" 3 1 3 2 1 3; }
@test "same" { check "5 3\n" 5 5 5; }
@test "empty" { check ""; }
@test "single" { check "7 1\n" 7; }
@test "negative" { check "-1 2\n0 1\n2 1\n" -1 2 -1 0; }
@test "distinct" { check "2 1\n4 1\n9 1\n" 4 2 9; }
@test "order" { check "8 1\n9 2\n" 9 8 9; }
@test "zero" { check "0 2\n" 0 0; }
@test "extremes" { check "-2147483648 1\n2147483647 2\n" 2147483647 -2147483648 2147483647; }
