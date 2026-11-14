#!/usr/bin/env bats
# SPDX-License-Identifier: MIT
# Part of https://programming.tamnd.com/use-a-custom-key-in-a-map/.

bats_require_minimum_version 1.5.0

setup() {
  source "$BATS_TEST_DIRNAME/count_points.sh"
}

check() { # check WANT ARG...
  local want got
  # %b turns each \n in WANT into a newline. The x keeps the trailing
  # newlines, which $(...) removes.
  want=$(printf '%b' "$1"; printf x)
  shift
  got=$(count_points "$@"; printf x)
  [ "$got" = "$want" ] || { printf 'got:  %q\nwant: %q\n' "$got" "$want"; return 1; }
}

@test "clicks" { check "1 2 2\n3 4 1\n" 1,2 3,4 1,2; }
@test "swap" { check "1 2 1\n2 1 1\n" 2,1 1,2; }
@test "empty" { check ""; }
@test "one" { check "0 0 1\n" 0,0; }
@test "negative" { check "-1 0 2\n0 -1 1\n" -1,0 0,-1 -1,0; }
@test "joined-digits" { check "1 23 1\n12 3 1\n" 1,23 12,3; }
@test "all-same" { check "5 5 3\n" 5,5 5,5 5,5; }
