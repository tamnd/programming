#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/compute-the-combinations-of-a-list/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/combinations.sh"
}

check() { # check WANT K ITEM...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(combinations "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "two-of-four" { check "1 2\n1 3\n1 4\n2 3\n2 4\n3 4\n" 2 1 2 3 4; }
@test "three-of-four" { check "1 2 3\n1 2 4\n1 3 4\n2 3 4\n" 3 1 2 3 4; }
@test "zero-k" { check "\n" 0 1 2; }
@test "empty" { check "\n" 0; }
@test "k-too-large" { check "" 3 1 2; }
@test "all" { check "1 2 3\n" 3 1 2 3; }
@test "duplicates" { check "1 1\n1 2\n1 2\n" 2 1 1 2; }
@test "unsorted" { check "3 1\n3 2\n1 2\n" 2 3 1 2; }
@test "extremes" { check "-2147483648\n2147483647\n" 1 -2147483648 2147483647; }
