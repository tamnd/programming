#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-permutations-of-a-list/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/permutations.sh"
}

check() { # check WANT ITEM...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(permutations "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "three" { check "1 2 3\n1 3 2\n2 1 3\n2 3 1\n3 1 2\n3 2 1\n" 1 2 3; }
@test "two" { check "5 6\n6 5\n" 5 6; }
@test "empty" { check "\n"; }
@test "single" { check "7\n" 7; }
@test "duplicates" { check "1 1 2\n1 2 1\n1 1 2\n1 2 1\n2 1 1\n2 1 1\n" 1 1 2; }
@test "unsorted" { check "3 1 2\n3 2 1\n1 3 2\n1 2 3\n2 3 1\n2 1 3\n" 3 1 2; }
@test "extremes" { check "-2147483648 2147483647\n2147483647 -2147483648\n" -2147483648 2147483647; }
